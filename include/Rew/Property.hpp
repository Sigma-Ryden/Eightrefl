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
        using __type = rew::pure_t<decltype(::rew::property_value(&info_t::type::__VA_ARGS__))>;        \
        auto __meta = reflection->property.find(#__VA_ARGS__);                                          \
        if (__meta == nullptr) __meta = &reflection->property.add(                                      \
            #__VA_ARGS__,                                                                               \
            {                                                                                           \
                #__VA_ARGS__,                                                                           \
                ::rew::find_or_add<__type>(info_t::registry, ::rew::reflection_info_t<__type>::name),   \
                property_get_handler(&info_t::type::__VA_ARGS__),                                       \
                property_set_handler(&info_t::type::__VA_ARGS__),                                       \
                property_ptr_handler(&info_t::type::__VA_ARGS__)                                        \
            }                                                                                           \
        );                                                                                              \
        visitor.template property<info_t::type, __type>(*__meta);                                       \
    }

#define PROPERTY(...)                                                                                   \
    CORE_PROPERTY(::rew::property_get_handler, ::rew::property_set_handler, ::rew::property_ptr_handler, __VA_ARGS__)

namespace rew
{

struct type_t;

struct property_meta_t
{
    const std::string name;
    type_t *const type = nullptr;
    const std::function<void(void* context, std::any& result)> get = nullptr;
    const std::function<void(void* context, const std::any& value)> set = nullptr;
    const std::function<void*(void* context)> ptr = nullptr;
    meta_t meta;
};

using property_t = attribute_t<property_meta_t>;

template <typename ReflectableType, typename PropertyType>
auto property_get_handler(PropertyType ReflectableType::* property)
{
    return [property](void* context, std::any& result)
    {
        result = static_cast<ReflectableType*>(context)->*property;
    };
}

template <typename ReflectableType, typename PropertyGetterType>
auto property_get_handler_impl(PropertyGetterType getter)
{
    return [getter](void* context, std::any& value)
    {
        value = (static_cast<ReflectableType*>(context)->*getter)();
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
auto property_set_handler(PropertyType ReflectableType::* property)
{
    return [property](void* context, const std::any& value)
    {
        static_cast<ReflectableType*>(context)->*property = std::any_cast<const PropertyType&>(value);
    };
}

template <typename ReflectableType, typename PropertyType>
auto property_set_handler(void (ReflectableType::* setter)(PropertyType))
{
    return [setter](void* context, const std::any& value)
    {
        (static_cast<ReflectableType*>(context)->*setter)(std::any_cast<const PropertyType&>(value));
    };
}

template <typename ReflectableType, typename PropertyType>
auto property_ptr_handler(PropertyType ReflectableType::* property)
{
    return [property](void* context) -> void*
    {
        return std::addressof(static_cast<ReflectableType*>(context)->*property);
    };
}

template <typename ReflectableType, typename PropertyGetterType>
auto property_ptr_handler_impl(PropertyGetterType getter)
{
    return [getter](void* context) -> void*
    {
        using result_t = decltype(property_value(getter));
        if constexpr (std::is_reference_v<result_t>)
        {
            constexpr auto is_const_value = std::is_const_v<std::remove_reference_t<result_t>>;

            auto address = std::addressof((static_cast<ReflectableType*>(context)->*getter)());

            if constexpr (is_const_value) return const_cast<void*>(static_cast<const void*>(address));
            else return address;
        }
        else
        {
            return nullptr;
        }
    };
}

template <typename ReflectableType, typename PropertyType>
auto property_ptr_handler(PropertyType (ReflectableType::* getter)(void) const)
{
    return property_ptr_handler_impl<ReflectableType>(getter);
}

template <typename ReflectableType, typename PropertyType>
auto property_ptr_handler(PropertyType (ReflectableType::* getter)(void))
{
    return property_ptr_handler_impl<ReflectableType>(getter);
}

} // namespace rew

#endif // REW_PROPERTY_HPP
