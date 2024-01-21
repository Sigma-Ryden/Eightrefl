#ifndef REW_PROPERTY_HPP
#define REW_PROPERTY_HPP

#include <string> // string
#include <any> // any

#include <functional> // function
#include <utility> // addressof

#include <type_traits> // is_reference_v, is_const_v, remove_reference_t

#include <Rew/Attribute.hpp>
#include <Rew/Meta.hpp>

#include <Rew/Utility.hpp>

#define CORE_PROPERTY(property_name_str, ...)                                                           \
    {                                                                                                   \
        auto __get = ::rew::utility::member_property_get_ptr<R>(&R::__VA_ARGS__);                       \
        auto __set = ::rew::utility::member_property_set_ptr<R>(&R::__VA_ARGS__);                       \
        auto __meta = ::rew::find_or_add_property(__reflection, property_name_str, __get, __set);       \
        using __traits = ::rew::meta::property_traits<decltype(__get)>;                                 \
        visitor.template property<R, typename __traits::property_type>(*__meta);                        \
    }

#define PROPERTY(...) CORE_PROPERTY(#__VA_ARGS__, __VA_ARGS__)

namespace rew
{

struct type_t;

struct property_t
{
    const std::string name;
    type_t *const type = nullptr;
    const std::function<void(std::any& context, std::any& result)> get = nullptr;
    const std::function<void(std::any& context, const std::any& value)> set = nullptr;
    const std::function<std::any(std::any& context)> ptr = nullptr;
    attribute_t<std::any> meta;
};

namespace detail
{

template <typename ReflectableType, typename PropertyGetterType>
auto property_get_impl(PropertyGetterType getter)
{
    return [getter](std::any& context, std::any& value)
    {
        value = (std::any_cast<ReflectableType*>(context)->*getter)();
    };
}

} // namespace detail

namespace handler
{

template <typename ReflectableType, typename PropertyType>
auto property_get(PropertyType ReflectableType::* property)
{
    return [property](std::any& context, std::any& result)
    {
        result = std::any_cast<ReflectableType*>(context)->*property;
    };
}

template <typename ReflectableType, typename PropertyType>
auto property_get(PropertyType (ReflectableType::* getter)(void) const)
{
    return detail::property_get_impl<ReflectableType>(getter);
}

template <typename ReflectableType, typename PropertyType>
auto property_get(PropertyType (ReflectableType::* getter)(void))
{
    return detail::property_get_impl<ReflectableType>(getter);
}

template <typename ReflectableType, typename PropertyType>
auto property_set(PropertyType ReflectableType::* property)
{
    return [property](std::any& context, const std::any& value)
    {
        std::any_cast<ReflectableType*>(context)->*property = std::any_cast<const PropertyType&>(value);
    };
}

template <typename ReflectableType, typename PropertyType>
auto property_set(void (ReflectableType::* setter)(PropertyType))
{
    return [setter](std::any& context, const std::any& value)
    {
        (std::any_cast<ReflectableType*>(context)->*setter)(std::any_cast<const PropertyType&>(value));
    };
}

} // namespace handler

namespace detail
{

template <typename ReflectableType, typename PropertyGetterType>
auto property_ptr_impl(PropertyGetterType getter)
{
    return [getter](std::any& context) -> std::any
    {
        using type = typename meta::property_traits<decltype(getter)>::property_type;
        if constexpr (std::is_reference_v<type>)
        {
            constexpr auto is_const_value = std::is_const_v<std::remove_reference_t<type>>;

            auto address = std::addressof((std::any_cast<ReflectableType*>(context)->*getter)());

            if constexpr (is_const_value) return const_cast<void*>(static_cast<const void*>(address));
            else return address;
        }
        else
        {
            return nullptr;
        }
    };
}

} // namespace detail

namespace handler
{

template <typename ReflectableType, typename PropertyType>
auto property_ptr(PropertyType ReflectableType::* property)
{
    return [property](std::any& context) -> std::any
    {
        return std::addressof(std::any_cast<ReflectableType*>(context)->*property);
    };
}

template <typename ReflectableType, typename PropertyType>
auto property_ptr(PropertyType (ReflectableType::* getter)(void) const)
{
    return detail::property_ptr_impl<ReflectableType>(getter);
}

template <typename ReflectableType, typename PropertyType>
auto property_ptr(PropertyType (ReflectableType::* getter)(void))
{
    return detail::property_ptr_impl<ReflectableType>(getter);
}

} // namespace handler

} // namespace rew

#endif // REW_PROPERTY_HPP
