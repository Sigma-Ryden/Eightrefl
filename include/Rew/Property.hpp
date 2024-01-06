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

#define CORE_PROPERTY(property_get_handler, property_set_handler, property_ptr_handler, ...)            \
    {                                                                                                   \
        using __type = ::rew::meta::pure_t<                                                             \
            decltype(::rew::utility::property_value(&reflectable::__VA_ARGS__))                         \
        >;                                                                                              \
        ::rew::utility::conditional_reflectable_register<__type>();                                     \
        auto __meta = reflection->property.find(#__VA_ARGS__);                                          \
        if (__meta == nullptr) __meta = &reflection->property.add(                                      \
            #__VA_ARGS__,                                                                               \
            {                                                                                           \
                #__VA_ARGS__,                                                                           \
                ::rew::meta::reflectable_traits_t<__type>::registry()->all[                             \
                    ::rew::meta::reflectable_name_t<__type>::get()                                      \
                ],                                                                                      \
                property_get_handler<reflectable>(&reflectable::__VA_ARGS__),                           \
                property_set_handler<reflectable>(&reflectable::__VA_ARGS__),                           \
                property_ptr_handler<reflectable>(&reflectable::__VA_ARGS__)                            \
            }                                                                                           \
        );                                                                                              \
        visitor.template property<reflectable, __type>(*__meta);                                        \
    }

#define PROPERTY(...)                                                                                   \
    CORE_PROPERTY(                                                                                      \
        ::rew::handler::property_get,                                                                   \
        ::rew::handler::property_set,                                                                   \
        ::rew::handler::property_ptr,                                                                   \
        __VA_ARGS__)

namespace rew
{

struct type_t;

struct property_t
{
    const std::string name;
    type_t *const& type = nullptr;
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

template <typename ReflectableType, typename ParentReflectableType, typename PropertyType>
auto property_get(PropertyType ParentReflectableType::* property)
{
    return [property](std::any& context, std::any& result)
    {
        result = std::any_cast<ReflectableType*>(context)->*property;
    };
}

template <typename ReflectableType, typename ParentReflectableType, typename PropertyType>
auto property_get(PropertyType (ParentReflectableType::* getter)(void) const)
{
    return detail::property_get_impl<ReflectableType>(getter);
}

template <typename ReflectableType, typename ParentReflectableType, typename PropertyType>
auto property_get(PropertyType (ParentReflectableType::* getter)(void))
{
    return detail::property_get_impl<ReflectableType>(getter);
}

template <typename ReflectableType, typename ParentReflectableType, typename PropertyType>
auto property_set(PropertyType ParentReflectableType::* property)
{
    return [property](std::any& context, const std::any& value)
    {
        std::any_cast<ReflectableType*>(context)->*property = std::any_cast<const PropertyType&>(value);
    };
}

template <typename ReflectableType, typename ParentReflectableType, typename PropertyType>
auto property_set(void (ParentReflectableType::* setter)(PropertyType))
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
        using result_t = decltype(utility::property_value(getter));
        if constexpr (std::is_reference_v<result_t>)
        {
            constexpr auto is_const_value = std::is_const_v<std::remove_reference_t<result_t>>;

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

template <typename ReflectableType, typename ParentReflectableType, typename PropertyType>
auto property_ptr(PropertyType ParentReflectableType::* property)
{
    return [property](std::any& context) -> std::any
    {
        return std::addressof(std::any_cast<ReflectableType*>(context)->*property);
    };
}

template <typename ReflectableType, typename ParentReflectableType, typename PropertyType>
auto property_ptr(PropertyType (ParentReflectableType::* getter)(void) const)
{
    return detail::property_ptr_impl<ReflectableType>(getter);
}

template <typename ReflectableType, typename ParentReflectableType, typename PropertyType>
auto property_ptr(PropertyType (ParentReflectableType::* getter)(void))
{
    return detail::property_ptr_impl<ReflectableType>(getter);
}

} // namespace handler

} // namespace rew

#endif // REW_PROPERTY_HPP
