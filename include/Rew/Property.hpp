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

#define RAW_PROPERTY(name_str, get, set, ...)                                                           \
    {                                                                                                   \
        using __access = rew::meta::access_traits<CleanR>;                                              \
        auto [__get, __set] = __access::template property<__VA_ARGS__>::of(&CleanR::get, &CleanR::set); \
        using __traits = rew::meta::property_traits<decltype(__get)>;                                   \
        auto __meta = rew::find_or_add_property<__VA_ARGS__>(__reflection, name_str, __get, __set);     \
        injection.template property<CleanR, typename __traits::type>(*__meta);                          \
    }

#define PROPERTY(name, ...) RAW_PROPERTY(#name, name, name, __VA_ARGS__)

#define RAW_FREE_PROPERTY(name_str, get, set, ...)                                                      \
    {                                                                                                   \
        using __access = rew::meta::access_traits<>;                                                    \
        auto [__get, __set] = __access::template property<__VA_ARGS__>::of(&get, &set);                 \
        using __traits = rew::meta::property_traits<decltype(__get)>;                                   \
        auto __meta = rew::find_or_add_property<__VA_ARGS__>(__reflection, name_str, __get, __set);     \
        injection.template property<CleanR, typename __traits::type>(*__meta);                          \
    }

#define FREE_PROPERTY(name, ...) RAW_FREE_PROPERTY(#name, name, name, __VA_ARGS__)

namespace rew
{

struct type_t;

struct property_t
{
    std::string const name;
    type_t *const type = nullptr;
    std::function<void(std::any const& context, std::any& result)> const get = nullptr;
    std::function<void(std::any const& context, std::any const& value)> const set = nullptr;
    std::function<std::any(std::any const& outer_context)> const context = nullptr;
    attribute_t<std::any> meta;
};

namespace detail
{

template <typename ReflectableType, typename PropertyGetterType>
auto handler_property_get_impl(PropertyGetterType getter)
{
    return [getter](std::any const& context, std::any& value)
    {
        using type = typename meta::property_traits<PropertyGetterType>::type;

        value = utility::backward<type>
        (
            (std::any_cast<ReflectableType*>(context)->*getter)()
        );
    };
}

} // namespace detail

template <typename ReflectableType, typename PropertyType>
auto handler_property_get(PropertyType ReflectableType::* property)
{
    return [property](std::any const& context, std::any& result)
    {
        result = utility::forward<PropertyType>
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
    return [property](std::any const&, std::any& result)
    {
        result = utility::forward<PropertyType>(*property);
    };
}

template <typename PropertyType>
auto handler_property_get(PropertyType (*getter)(void))
{
    return [getter](std::any const&, std::any& result)
    {
        result = utility::backward<PropertyType>(getter());
    };
}

namespace detail
{

template <typename ReflectableType, typename PropertySetterType>
auto handler_property_set_impl(PropertySetterType setter)
{
    return [setter](std::any const& context, std::any const& value)
    {
        using type = typename meta::property_traits<PropertySetterType>::type;

        (std::any_cast<ReflectableType*>(context)->*setter)(utility::forward<type>(value));
    };
}

} // namespace detail

template <typename ReflectableType, typename PropertyType>
auto handler_property_set(PropertyType ReflectableType::* property)
{
    return [property](std::any const& context, std::any const& value)
    {
        std::any_cast<ReflectableType*>(context)->*property = utility::forward<PropertyType>(value);
    };
}

template <typename ReflectableType, typename PropertyType>
auto handler_property_set(PropertyType const ReflectableType::* property)
{
    return nullptr;
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
    return [property](std::any const&, std::any const& value)
    {
        *property = utility::forward<PropertyType>(value);
    };
}

template <typename PropertyType>
auto handler_property_set(PropertyType const* property)
{
    return nullptr;
}

template <typename PropertyType>
auto handler_property_set(void (*setter)(PropertyType))
{
    return [setter](std::any const&, std::any const& value)
    {
        setter(utility::forward<PropertyType>(value));
    };
}

template <typename ReflectableType, typename PropertyType>
auto handler_property_set(PropertyType (ReflectableType::* getter)(void) const)
{
    return nullptr;
}

template <typename ReflectableType, typename PropertyType>
auto handler_property_set(PropertyType (ReflectableType::* getter)(void) const&)
{
    return nullptr;
}

template <typename ReflectableType, typename PropertyType>
auto handler_property_set(PropertyType (ReflectableType::* getter)(void))
{
    return nullptr;
}

template <typename ReflectableType, typename PropertyType>
auto handler_property_set(PropertyType (ReflectableType::* getter)(void) &)
{
    return nullptr;
}

template <typename PropertyType>
auto handler_property_set(PropertyType (*getter)(void))
{
    return nullptr;
}

namespace detail
{

template <typename ReflectableType, typename PropertyGetterType>
auto handler_property_context_impl(PropertyGetterType getter)
{
    using type = typename meta::property_traits<PropertyGetterType>::type;
    if constexpr (std::is_reference_v<type>)
    {
        return [getter](std::any const& outer_context) -> std::any
        {
            return const_cast<meta::to_reflectable_reference_t<type>>
            (
                std::addressof((std::any_cast<ReflectableType*>(outer_context)->*getter)())
            );
        };
    }
    else
    {
        return nullptr;
    }
}

} // namespace detail

template <typename ReflectableType, typename PropertyType>
auto handler_property_context(PropertyType ReflectableType::* property)
{
    return [property](std::any const& outer_context) -> std::any
    {
        return const_cast<meta::to_reflectable_object_t<PropertyType>*>
        (
            std::addressof(std::any_cast<ReflectableType*>(outer_context)->*property)
        );
    };
}

template <typename ReflectableType, typename PropertyType>
auto handler_property_context(PropertyType (ReflectableType::* getter)(void) const)
{
    return detail::handler_property_context_impl<ReflectableType>(getter);
}

template <typename ReflectableType, typename PropertyType>
auto handler_property_context(PropertyType (ReflectableType::* getter)(void) const&)
{
    return detail::handler_property_context_impl<ReflectableType>(getter);
}

template <typename ReflectableType, typename PropertyType>
auto handler_property_context(PropertyType (ReflectableType::* getter)(void))
{
    return detail::handler_property_context_impl<ReflectableType>(getter);
}

template <typename ReflectableType, typename PropertyType>
auto handler_property_context(PropertyType (ReflectableType::* getter)(void) &)
{
    return detail::handler_property_context_impl<ReflectableType>(getter);
}

template <typename PropertyType>
auto handler_property_context(PropertyType* property)
{
    return [property](std::any const&) -> std::any
    {
        return const_cast<meta::to_reflectable_object_t<PropertyType>*>(std::addressof(property));
    };
}

template <typename PropertyType>
auto handler_property_context(PropertyType (*getter)(void))
{
    if constexpr (std::is_reference_v<PropertyType>)
    {
        return [getter](std::any const&) -> std::any
        {
            return const_cast<meta::to_reflectable_reference_t<PropertyType>>
            (
                std::addressof(getter())
            );
        };
    }
    else
    {
        return nullptr;
    }
}

} // namespace rew

#endif // REW_PROPERTY_HPP
