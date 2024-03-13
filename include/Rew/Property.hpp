#ifndef REW_PROPERTY_HPP
#define REW_PROPERTY_HPP

#include <string> // string
#include <any> // any

#include <functional> // function
#include <memory> // addressof

#include <type_traits> // is_reference_v

#include <Rew/Attribute.hpp>
#include <Rew/Meta.hpp>

#include <Rew/Utility.hpp>

#include <Rew/Detail/Macro.hpp> // __REW_EXPAND

#define RAW_PROPERTY(name_str, name_get, name_set)                                                      \
    {                                                                                                   \
        using __access_traits = rew::meta::access_traits<R>;                                            \
        auto __get = __access_traits::getter::of(&R::__REW_EXPAND name_get);                            \
        auto __set = __access_traits::setter::of(&R::__REW_EXPAND name_set);                            \
        using __traits = rew::meta::property_traits<decltype(__get)>;                                   \
        auto __meta = rew::find_or_add_property(__reflection, name_str, __get, __set);                  \
        injection.template property<R, typename __traits::type>::type>(*__meta);                        \
    }

#define PROPERTY(...) RAW_PROPERTY(#__VA_ARGS__, (__VA_ARGS__), (__VA_ARGS__))

#define RAW_FREE_PROPERTY(name_str, name_get, name_set)                                                 \
    {                                                                                                   \
        using __access_traits = rew::meta::access_traits<>;                                             \
        auto __get = __access_traits::getter::of(&__REW_EXPAND name_get);                               \
        auto __set = __access_traits::setter::of(&__REW_EXPAND name_set);                               \
        using __traits = rew::meta::property_traits<decltype(__get)>;                                   \
        auto __meta = rew::find_or_add_property(__reflection, name_str, __get, __set);                  \
        injection.template property<R, typename __traits::type>(*__meta);                               \
    }

#define FREE_PROPERTY(...) RAW_FREE_PROPERTY(#__VA_ARGS__, (__VA_ARGS__), (__VA_ARGS__))

namespace rew
{

struct type_t;

struct property_t
{
    const std::string name;
    type_t *const type = nullptr;
    const std::function<void(const std::any& context, std::any& result)> get = nullptr;
    const std::function<void(const std::any& context, const std::any& value)> set = nullptr;
    const std::function<std::any(const std::any& context)> ptr = nullptr;
    attribute_t<std::any> meta;
};

namespace detail
{

template <typename ReflectableType, typename PropertyGetterType>
auto handler_property_get_impl(PropertyGetterType getter)
{
    return [getter](const std::any& context, std::any& value)
    {
        using type = typename meta::property_traits<PropertyGetterType>::type;

        value = forward<type>
        (
            (std::any_cast<ReflectableType*>(context)->*getter)()
        );
    };
}

} // namespace detail

template <typename ReflectableType, typename PropertyType>
auto handler_property_get(PropertyType ReflectableType::* property)
{
    return [property](const std::any& context, std::any& result)
    {
        result = forward<PropertyType>
        (
            std::any_cast<ReflectableType*>(context)->*property
        );
    };
}

template <typename ReflectableType, typename PropertyType>
auto handler_property_get(PropertyType (ReflectableType::* getter)(void) const)
{
    return detail::handler_property_get_impl<ReflectableType>(getter);
}

template <typename ReflectableType, typename PropertyType>
auto handler_property_get(PropertyType (ReflectableType::* getter)(void) const&)
{
    return detail::handler_property_get_impl<ReflectableType>(getter);
}

template <typename ReflectableType, typename PropertyType>
auto handler_property_get(PropertyType (ReflectableType::* getter)(void))
{
    return detail::handler_property_get_impl<ReflectableType>(getter);
}

template <typename ReflectableType, typename PropertyType>
auto handler_property_get(PropertyType (ReflectableType::* getter)(void) &)
{
    return detail::handler_property_get_impl<ReflectableType>(getter);
}

template <typename PropertyType>
auto handler_property_get(PropertyType* property)
{
    return [property](const std::any&, std::any& result)
    {
        result = forward<PropertyType>(*property);
    };
}

template <typename PropertyType>
auto handler_property_get(PropertyType (*getter)(void))
{
    return [getter](const std::any&, std::any& result)
    {
        result = forward<PropertyType>(getter());
    };
}

namespace detail
{

template <typename ReflectableType, typename PropertySetterType>
auto handler_property_set_impl(PropertySetterType setter)
{
    return [setter](const std::any& context, const std::any& value)
    {
        using type = typename meta::property_traits<PropertySetterType>::type;

        (std::any_cast<ReflectableType*>(context)->*setter)(forward<type>(value));
    };
}

} // namespace detail

template <typename ReflectableType, typename PropertyType>
auto handler_property_set(PropertyType ReflectableType::* property)
{
    return [property](const std::any& context, const std::any& value)
    {
        std::any_cast<ReflectableType*>(context)->*property = forward<PropertyType>(value);
    };
}

template <typename ReflectableType, typename PropertyType>
auto handler_property_set(void (ReflectableType::* setter)(PropertyType))
{
    return detail::handler_property_set_impl<ReflectableType>(setter);
}

template <typename ReflectableType, typename PropertyType>
auto handler_property_set(void (ReflectableType::* setter)(PropertyType) &)
{
    return detail::handler_property_set_impl<ReflectableType>(setter);
}

template <typename PropertyType>
auto handler_property_set(PropertyType* property)
{
    return [property](const std::any&, const std::any& value)
    {
        *property = forward<PropertyType>(value);
    };
}

template <typename PropertyType>
auto handler_property_set(void (*setter)(PropertyType))
{
    return [setter](const std::any&, const std::any& value)
    {
        setter(forward<PropertyType>(value));
    };
}

namespace detail
{

template <typename ReflectableType, typename PropertyGetterType>
auto handler_property_ptr_impl(PropertyGetterType getter)
{
    return [getter](const std::any& context) -> std::any
    {
        using type = typename meta::property_traits<PropertyGetterType>::type;

        if constexpr (std::is_reference_v<type>)
        {
            auto address = std::addressof((std::any_cast<ReflectableType*>(context)->*getter)());
            return const_cast<void*>(static_cast<const void*>(address));
        }
        else
        {
            return static_cast<void*>(nullptr);
        }
    };
}

} // namespace detail

template <typename ReflectableType, typename PropertyType>
auto handler_property_ptr(PropertyType ReflectableType::* property)
{
    return [property](const std::any& context) -> std::any
    {
        return std::addressof(std::any_cast<ReflectableType*>(context)->*property);
    };
}

template <typename ReflectableType, typename PropertyType>
auto handler_property_ptr(PropertyType (ReflectableType::* getter)(void) const)
{
    return detail::handler_property_ptr_impl<ReflectableType>(getter);
}

template <typename ReflectableType, typename PropertyType>
auto handler_property_ptr(PropertyType (ReflectableType::* getter)(void) const&)
{
    return detail::handler_property_ptr_impl<ReflectableType>(getter);
}

template <typename ReflectableType, typename PropertyType>
auto handler_property_ptr(PropertyType (ReflectableType::* getter)(void))
{
    return detail::handler_property_ptr_impl<ReflectableType>(getter);
}

template <typename ReflectableType, typename PropertyType>
auto handler_property_ptr(PropertyType (ReflectableType::* getter)(void) &)
{
    return detail::handler_property_ptr_impl<ReflectableType>(getter);
}

template <typename PropertyType>
auto handler_property_ptr(PropertyType* property)
{
    return [property](const std::any&) -> std::any
    {
        return std::addressof(property);
    };
}

template <typename PropertyType>
auto handler_property_ptr(PropertyType (*getter)(void))
{
    return [getter](const std::any&) -> std::any
    {
        if constexpr (std::is_reference_v<PropertyType>)
        {
            auto address = std::addressof(getter());
            return const_cast<void*>(static_cast<const void*>(address));
        }
        else
        {
            return static_cast<void*>(nullptr);
        }
    };
}

} // namespace rew

#endif // REW_PROPERTY_HPP
