#ifndef REW_PROPERTY_HPP
#define REW_PROPERTY_HPP

#include <string> // string
#include <map> // map
#include <any> // any

#include <functional> // function

#include <Rew/Attribute.hpp>
#include <Rew/Meta.hpp>

#define CORE_PROPERTY(property_get_handler, property_set_handler, ...)                                  \
    {                                                                                                   \
        using __type = decltype(property_value(&info_t::type::__VA_ARGS__));                            \
        auto __meta = reflection->property.find(#__VA_ARGS__);                                          \
        if (__meta == nullptr) __meta = &reflection->property.add(                                      \
            #__VA_ARGS__,                                                                               \
            {                                                                                           \
                #__VA_ARGS__,                                                                           \
                property_get_handler(&info_t::type::__VA_ARGS__),                                       \
                property_set_handler(&info_t::type::__VA_ARGS__)                                        \
            }                                                                                           \
        );                                                                                              \
        visitor.template property<info_t::type, __type>(*__meta);                                       \
    }

#define CORE_BUILTIN_PROPERTY(builtin_property_get_handler, builtin_property_set_handler, ...)          \
    {                                                                                                   \
        auto __meta = reflection->property.find(#__VA_ARGS__);                                          \
        if (__meta == nullptr) __meta = &reflection->property.add(                                      \
            #__VA_ARGS__,                                                                               \
            {                                                                                           \
                #__VA_ARGS__,                                                                           \
                builtin_property_get_handler<info_t::type>(),                                           \
                builtin_property_set_handler<info_t::type>()                                            \
            }                                                                                           \
        );                                                                                              \
        visitor.template property<info_t::type, info_t::type>(*__meta);                                 \
    }

#define PROPERTY(...)                                                                                   \
    CORE_PROPERTY(property_get_handler, property_set_handler, __VA_ARGS__)

#define BUILTIN_PROPERTY(...)                                                                           \
    CORE_BUILTIN_PROPERTY(builtin_property_get_handler, builtin_property_set_handler, __VA_ARGS__)

namespace rew
{

struct type_t;

struct property_meta_t
{
    // TODO:
    // add type_t and reg for primitives
    const std::string name;
    const std::function<void(std::any&, std::any&)> get = nullptr;
    const std::function<void(std::any&, const std::any&)> set = nullptr;
    type_t *const type = nullptr;
    meta_t meta;
};

using property_t = attribute_t<property_meta_t>;

template <typename ReflectableType, typename PropertyType>
PropertyType property_value(PropertyType ReflectableType::*);

template <typename ReflectableType, typename PropertyType>
PropertyType property_value(PropertyType (ReflectableType::*)(void) const);

template <typename ReflectableType, typename PropertyType>
PropertyType property_value(PropertyType (ReflectableType::*)(void));

template <typename ReflectableType, typename PropertyType>
PropertyType property_value();

template <typename ReflectableType, typename PropertyType>
auto property_get_handler(PropertyType ReflectableType::* property)
{
    return [property](std::any& self, std::any& result)
    {
        result = std::any_cast<ReflectableType*>(self)->*property;
    };
}

template <typename ReflectableType, typename PropertyType>
auto property_set_handler(PropertyType ReflectableType::* property)
{
    return [property](std::any& self, const std::any& value)
    {
        std::any_cast<ReflectableType*>(self)->*property = std::any_cast<const PropertyType&>(value);
    };
}

template <typename ReflectableType, typename PropertyGetterType>
auto property_get_handler_impl(PropertyGetterType getter)
{
    return [getter](std::any& self, std::any& value)
    {
        value = (std::any_cast<ReflectableType*>(self)->*getter)();
    };
}

template <typename ReflectableType, typename PropertyType>
auto property_get_handler(PropertyType (ReflectableType::* getter)(void) const)
{
    return property_get_handler_impl<ReflectableType>(getter);
}

template <typename ReflectableType, typename PropertyType>
auto property_get_handler(PropertyType (ReflectableType::* getter)(void))
{
    return property_get_handler_impl<ReflectableType>(getter);
}

template <typename ReflectableType, typename PropertyType>
auto property_set_handler(void (ReflectableType::* setter)(PropertyType))
{
    return [setter](std::any& self, const std::any& value)
    {
        (std::any_cast<ReflectableType*>(self)->*setter)(std::any_cast<const PropertyType&>(value));
    };
}

template <typename ReflectableType>
auto builtin_property_get_handler()
{
    return [](std::any& self, std::any& result)
    {
        result = std::any_cast<ReflectableType>(self);
    };
}

template <typename ReflectableType>
auto builtin_property_set_handler()
{
    return [](std::any& self, const std::any& result)
    {
        std::any_cast<ReflectableType&>(self) = std::any_cast<ReflectableType>(result);
    };
}

} // namespace rew

#endif // REW_PROPERTY_HPP
