#include "Rew.hpp"

#include <iostream>

#define println(...) std::cout << '\t' << #__VA_ARGS__ << ' ' << __VA_ARGS__ << '\n';

struct TBase
{
    std::string Data = "abcd";

    static void Boo();
};

REFLECTABLE(TBase)
    PROPERTY(Data)
    FUNCTION(Boo)
REFLECTABLE_INIT()

void TBase::Boo()
{
    println("TBase::Boo()");
}

struct TObject : TBase
{
    double Foo(int i, const std::string& s);
    int Var = 0;
};

REFLECTABLE(TObject)
    PROPERTY(Var)
    FUNCTION(Foo)
    PARENT(TBase)
    FACTORY(TObject)
    META("Hash", 5678)
REFLECTABLE_INIT()

double TObject::Foo(int i, const std::string& s)
{
    println(i);
    println(s);
    return 3.14;
}

struct register_t : rew::register_base_t
{
    template <typename PropertyType>
    void property(const rew::property_t::meta_t& meta)
    {
    }

    template <typename FunctionType>
    void function(const rew::function_t::meta_t& meta)
    {
    }

    template <class BaseClassType, class DerivedClassType>
    void parent(const rew::parent_t::meta_t& meta)
    {
    }

    template <class OtherClassType, typename... ArgumentTypes>
    void factory(const rew::factory_t::meta_t& meta)
    {
    }

    template <typename MetaType>
    void meta(const char* name, const MetaType& data)
    {
    }
};
#define SFREQUIRE(...) std::enable_if_t<__VA_ARGS__, int> = 0

struct register_base_trait
{
    inline static std::map<std::size_t, std::size_t> rtti_table;
};

template <int Key> struct register_trait
{
    using type = void;
};

#define REFLECTABLE_REGISTRY(index, ...)                                                                \
    template <> struct register_trait<index> {                                                          \
        using type = __VA_ARGS__;                                                                       \
        inline static auto _ = (register_base_trait::rtti_table[typeid(type).hash_code()] = index);     \
    };

REFLECTABLE_REGISTRY(0, rew::register_t)
REFLECTABLE_REGISTRY(1, register_t)

struct polymorphic_register_t
{
    template <class ReflectableType>
    static void call(rew::register_base_t& registry)
    {
        auto key = register_base_trait::rtti_table.at(typeid(registry).hash_code());
        try_call<ReflectableType>(registry, key);
    }

    template <class ReflectableType, std::size_t Key = 0>
    static void try_call(rew::register_base_t& registry, int key)
    {
        if constexpr (Key < 8)
        {
            using derived_register_t = typename register_trait<Key>::type;

            if constexpr (not std::is_same_v<derived_register_t, void>)
            {
                using call_t = typename rew::detail::reflection_registry_impl_t<ReflectableType>::call_t;
                if (Key == key) call_t(dynamic_cast<derived_register_t&>(registry));
            }
            else
            {
                try_call<ReflectableType, Key + 1>(registry, key);
            }
        }
        else
        {
            throw "bad registry";
        }
    }
};

int main()
{
    auto reflection = rew::reflection.find("TObject");
    std::map<std::string, std::function<void(rew::register_base_t&)>> mm = {
        {
            "TObject",
            [](rew::register_base_t& r) {
                polymorphic_register_t::call<TObject>(r);
            }
        },
        {
            "TBase",
            [](rew::register_base_t& r) {
                polymorphic_register_t::call<TBase>(r);
            }
        },
    };

    register_t r;
    mm.at("TObject")(r);

    auto factory = reflection->factory.find("TObject");
    auto object = factory->call({});

    std::any result;

    auto function = reflection->function.find("Foo");
    function->call(object, result, { 128, std::string("Tom") });

    println(std::any_cast<double>(result));

    auto property = reflection->property.find("Var");
    property->set(object, 321);
    property->get(object, result);

    println(std::any_cast<int>(result));

    println(reflection->name);

    auto meta = reflection->meta.find("Hash");
    result = *meta;
    println(std::any_cast<int>(result));

    auto parent = reflection->parent.find("TBase");
    auto base = parent->get(object);
    auto base_reflection = parent->reflection();

    for (auto& [name, meta] : base_reflection->property.all)
    {
        std::any value;
        meta.get(base, value);
    }

    auto base_function = base_reflection->function.find("Boo");
    base_function->call(nullptr, result, {});

    return 0;
}
