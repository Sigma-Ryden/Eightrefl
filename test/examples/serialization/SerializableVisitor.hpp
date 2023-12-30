#ifndef SERIALIZABLE_VISITOR_HPP
#define SERIALIZABLE_VISITOR_HPP

#include <Rew/Core.hpp>
#include "SerializationFixture.hpp"

struct serializable_visitor_t : rew::visitor_t
{
public:
    inline static const auto save_mode = "save";
    inline static const auto load_mode = "load";

public:
    using saveload_function_t = void(*)(sf::core::IOArchive&, void*);

public:
    template <typename ReflectableType>
    void type(rew::type_t& type)
    {
        if constexpr (sf::meta::is_serializable<ReflectableType>::value)
        {
            sf::serializable<ReflectableType>();
        }
    }

    template <typename ReflectableType, typename PropertyType>
    void property(rew::property_meta_t& meta)
    {
        if constexpr (sf::meta::is_serializable<PropertyType>::value)
        {
            saveload_function_t save = [](sf::core::IOArchive& archive, void* data)
            {
                archive << *static_cast<PropertyType*>(data);
            };

            saveload_function_t load = [](sf::core::IOArchive& archive, void* data)
            {
                archive >> *static_cast<PropertyType*>(data);
            };

            meta.meta.add(save_mode, save);
            meta.meta.add(load_mode, load);
        }
    }

    template <typename ReflectableType, typename ParentReflectableType>
    void parent(rew::parent_meta_t& meta)
    {
        if constexpr (sf::meta::is_serializable<ParentReflectableType>::value)
        {
            saveload_function_t save = [](sf::core::IOArchive& archive, void* child)
            {
                archive << sf::hierarchy<ParentReflectableType>(*static_cast<ReflectableType*>(child));
            };

            saveload_function_t load = [](sf::core::IOArchive& archive, void* child)
            {
                archive >> sf::hierarchy<ParentReflectableType>(*static_cast<ReflectableType*>(child));
            };

            meta.meta.add(save_mode, save);
            meta.meta.add(load_mode, load);
        }
    }
};

void reflectable_saveload(sf::core::IOArchive& archive, void* context, std::type_index typeindex, const char* mode);

CONDITIONAL_SERIALIZATION(Save, rew::is_custom_reflectable<T>::value)
{
    reflectable_saveload(archive, std::addressof(self), typeid(T), serializable_visitor_t::save_mode);
}

CONDITIONAL_SERIALIZATION(Load, rew::is_custom_reflectable<T>::value)
{
    reflectable_saveload(archive, std::addressof(self), typeid(T), serializable_visitor_t::load_mode);
}

REFLECTABLE_VISITOR_REGISTRY(0, serializable_visitor_t)

#endif // SERIALIZABLE_VISITOR_HPP
