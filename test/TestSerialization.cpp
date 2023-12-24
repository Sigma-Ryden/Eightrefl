#include "RewTestingBase.hpp"
#include "TestSerialization/SerializationFixture.hpp"

struct Reflectable : sf::Instantiable {};

struct serializable_visitor_t : rew::visitor_t
{
    inline static const auto save_mode_v = "save";
    inline static const auto load_mode_v = "load";

    using saveload_function_t = std::function<void(sf::core::IOArchive &, void*)>;

    template <typename ReflectableType>
    void type(rew::type_t& type)
    {
        sf::serializable<ReflectableType>();
    }

    template <typename ReflectableType, typename PropertyType>
    void property(rew::property_meta_t& meta)
    {
        saveload_function_t save = [](sf::core::IOArchive& archive, void* data)
        {
            archive << *static_cast<PropertyType*>(data);
        };

        saveload_function_t load = [](sf::core::IOArchive& archive, void* data)
        {
            archive >> *static_cast<PropertyType *>(data);
        };

        meta.meta.add(save_mode_v, save);
        meta.meta.add(load_mode_v, load);
    }

    template <typename ReflectableType, typename ParentReflectableType>
    void parent(rew::parent_meta_t& meta)
    {
        saveload_function_t save = [](sf::core::IOArchive& archive, void* child)
        {
            archive << sf::hierarchy<ParentReflectableType>(*static_cast<ReflectableType*>(child));
        };

        saveload_function_t load = [](sf::core::IOArchive& archive, void* child)
        {
            archive >> sf::hierarchy<ParentReflectableType>(*static_cast<ReflectableType*>(child));
        };

        meta.meta.add(save_mode_v, save);
        meta.meta.add(load_mode_v, load);
    }
};

void saveload_reflectable(sf::core::IOArchive& archive, Reflectable& reflectable, std::type_index typeindex, const char* mode)
{
    using saveload_function_t = serializable_visitor_t::saveload_function_t;

    auto type = rew::registry.find(typeindex);
    auto reflectable_ptr = std::addressof(reflectable);

    for (auto& [name, parent] : type->reflection->parent.all)
    {
        auto& saveload = std::any_cast<saveload_function_t&>(*parent.meta.find(mode));
        saveload(archive, reflectable_ptr);
    }

    for (auto& [name, property] : type->reflection->property.all)
    {
        auto& saveload = std::any_cast<saveload_function_t&>(*property.meta.find(mode));
        saveload(archive, property.ptr(reflectable_ptr));
    }
}

CONDITIONAL_SERIALIZATION(Save, std::is_base_of_v<Reflectable, T>)
{
    saveload_reflectable(archive, self, typeid(T), serializable_visitor_t::save_mode_v);
}

CONDITIONAL_SERIALIZATION(Load, std::is_base_of_v<Reflectable, T>)
{
    saveload_reflectable(archive, self, typeid(T), serializable_visitor_t::load_mode_v);
}

REFLECTABLE_VISITOR_REGISTRY(1, serializable_visitor_t)

TEST_SPACE()
{

struct TBaseData : Reflectable
{
    int x = 0;
};

struct TData : TBaseData
{
    int y = 0;
};

} // TEST_SPACE

REFLECTABLE(int)
REFLECTABLE_INIT()

REFLECTABLE(TBaseData)
    PROPERTY(x)
REFLECTABLE_INIT()

REFLECTABLE(TData)
    PARENT(TBaseData)
    PROPERTY(y)
REFLECTABLE_INIT()

TEST(TestLibrary, TestSerialization)
{
    serializable_visitor_t serializable;
    for (auto& [name, type] : rew::registry.all) type->evaluate(serializable);

    static int s_x = 123;
    static int s_y = 456;

    std::vector<unsigned char> storage;
    {
        TData d;
        d.x = s_x;
        d.y = s_y;

        auto ar = sf::oarchive(storage);
        ar & d;
    }
    {
        TData d;

        auto ar = sf::iarchive(storage);
        ar & d;

        EXPECT("data", d.x == s_x && d.y == s_y);
    }
}
