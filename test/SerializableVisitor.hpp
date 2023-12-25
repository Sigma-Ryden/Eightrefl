#ifndef SERIALIZABLE_VISITOR_HPP
#define SERIALIZABLE_VISITOR_HPP

#include <Rew/Core.hpp>

#define INSTANTIABLE_TYPE ::rew::Reflectable
#include "SerializationFixture.hpp"

struct serializable_visitor_t : rew::visitor_t
{
    inline static const auto save_mode_v = "save";
    inline static const auto load_mode_v = "load";

    using saveload_function_t = std::function<void(sf::core::IOArchive&, void*)>;

    template <typename ReflectableType>
    void type(rew::type_t& type)
    {
        /*
        if constexpr (sf::meta::is_registered<ReflectableType>::value)
        {
            //sf::serializable<ReflectableType>();
        }
        */
    }

    template <typename ReflectableType, typename PropertyType>
    void property(rew::property_meta_t& meta)
    {
        if constexpr (sf::meta::is_unsupported<PropertyType>::value)
        {
            saveload_function_t save = [](sf::core::IOArchive& archive, void* data)
            {
                archive << *static_cast<PropertyType*>(data);
            };

            saveload_function_t load = [](sf::core::IOArchive& archive, void* data)
            {
                archive >> *static_cast<PropertyType*>(data);
            };

            meta.meta.add(save_mode_v, save);
            meta.meta.add(load_mode_v, load);
        }
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

void saveload_impl(sf::core::IOArchive& archive, rew::Reflectable& reflectable, std::type_index typeindex, const char* mode);

template <typename T, typename enable = void>
struct is_complete : std::false_type {};

template <typename T>
struct is_complete<T, std::void_t<decltype(sizeof(T))>> : std::true_type {};

CONDITIONAL_SERIALIZATION(Save, is_complete<rew_reflection_registry_t<T>>::value)
{
    if constexpr (std::is_base_of_v<rew::Reflectable, T>)
    {
        saveload_impl(archive, self, typeid(T), serializable_visitor_t::save_mode_v);
    }
}

CONDITIONAL_SERIALIZATION(Load, is_complete<rew_reflection_registry_t<T>>::value)
{
    if constexpr (std::is_base_of_v<rew::Reflectable, T>)
    {
        saveload_impl(archive, self, typeid(T), serializable_visitor_t::load_mode_v);
    }
}

REFLECTABLE_VISITOR_REGISTRY(0, serializable_visitor_t)

#endif // SERIALIZABLE_VISITOR_HPP
