#ifndef REW_PROPERTY_HPP
#define REW_PROPERTY_HPP

#include <string> // string
#include <map> // map
#include <any> // any

#include <functional> // function

#include <Rew/Attribute.hpp>
#include <Rew/Meta.hpp>

#define PROPERTY(...)                                                                                   \
    visitor.template property<decltype(property_owner(&info_t::type::__VA_ARGS__)),                     \
                              decltype(property_value(&info_t::type::__VA_ARGS__))>({                   \
        #__VA_ARGS__,                                                                                   \
        property_get_handler(&info_t::type::__VA_ARGS__),                                               \
        property_set_handler(&info_t::type::__VA_ARGS__)                                                \
    });

namespace rew
{

struct property_meta_t
{
    // TODO:
    // add type_t and reg for primitives
    const std::string name;
    const std::function<void(void*, std::any&)> get = nullptr;
    const std::function<void(void*, const std::any&)> set = nullptr;
    meta_t meta;
};

using property_t = attribute_t<property_meta_t>;

template <typename ReflectableType, typename PropertyType>
ReflectableType property_owner(PropertyType ReflectableType::*);

template <typename ReflectableType, typename PropertyType>
ReflectableType property_owner(PropertyType (ReflectableType::*)(void));

template <typename ReflectableType, typename PropertyType>
ReflectableType property_owner();

template <typename ReflectableType, typename PropertyType>
PropertyType property_value(PropertyType ReflectableType::*);

template <typename ReflectableType, typename PropertyType>
PropertyType property_value(PropertyType (ReflectableType::*)(void));

template <typename ReflectableType, typename PropertyType>
PropertyType property_value();

template <typename ReflectableType, typename PropertyType>
auto property_get_handler(PropertyType ReflectableType::* property)
{
    return [property](void* self, std::any& result)
    {
        result = static_cast<ReflectableType*>(self)->*property;
    };
}

template <typename ReflectableType, typename PropertyType>
auto property_set_handler(PropertyType ReflectableType::* property)
{
    return [property](void* self, const std::any& value)
    {
        static_cast<ReflectableType*>(self)->*property = std::any_cast<const PropertyType&>(value);
    };
}

template <typename ReflectableType, typename PropertyType>
auto property_get_handler(PropertyType (ReflectableType::* getter)(void))
{
    return [getter](void* self, std::any& value)
    {
        value = (static_cast<ReflectableType*>(self)->*getter)();
    };
}

template <typename ReflectableType, typename PropertyType>
auto property_set_handler(void (ReflectableType::* setter)(PropertyType))
{
    return [setter](void* self, const std::any& value)
    {
        (static_cast<ReflectableType*>(self)->*setter)(std::any_cast<const PropertyType&>(value));
    };
}

template <typename ReflectableType, typename PropertyType>
auto property_get_handler()
{
    return [](void* self, std::any& result)
    {
        result = *static_cast<PropertyType*>(self);
    };
}

template <typename ReflectableType, typename PropertyType>
auto property_set_handler()
{
    return [](void* self, const std::any& result)
    {
        *static_cast<PropertyType*>(self) = std::any_cast<PropertyType>(result);
    };
}

} // namespace rew

#endif // REW_PROPERTY_HPP
