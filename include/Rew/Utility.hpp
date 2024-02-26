#ifndef REW_UTILITY_HPP
#define REW_UTILITY_HPP

#include <string> // string
#include <any> // any, any_cast
#include <utility> // reference_wrapper

#include <type_traits> // remove_reference_t, is_void_v

#include <Rew/Detail/Meta.hpp>

namespace rew
{

namespace utility
{

template <typename ValueType>
ValueType forward(const std::any& object)
{
    if constexpr (meta::is_reference<ValueType>::value)
    {
        return std::any_cast<std::reference_wrapper<std::remove_reference_t<ValueType>>>(object);
    }
    else
    {
        return std::any_cast<ValueType>(object);
    }
}

template <typename ValueType>
std::any backward(ValueType&& result)
{
    if constexpr (meta::is_reference<ValueType>::value)
    {
        return std::reference_wrapper<std::remove_reference_t<ValueType>>(result);
    }
    else
    {
        return result;
    }
}

template <typename ReflectableType, typename ParentReflectableType, typename ReturnType, typename... ArgumentTypes>
auto member_function_ptr(ReturnType (ParentReflectableType::* function)(ArgumentTypes...) const)
{
    struct __inner : protected ReflectableType
    {
        static auto get(ReturnType (ParentReflectableType::* function)(ArgumentTypes...) const)
        {
            return static_cast<ReturnType (ReflectableType::*)(ArgumentTypes...) const>(function);
        }
    };
    return __inner::get(function);
}

template <typename ReflectableType, typename ParentReflectableType, typename ReturnType, typename... ArgumentTypes>
auto member_function_ptr(ReturnType (ParentReflectableType::* function)(ArgumentTypes...))
{
    struct __inner : protected ReflectableType
    {
        static auto get(ReturnType (ParentReflectableType::* function)(ArgumentTypes...))
        {
            return static_cast<ReturnType (ReflectableType::*)(ArgumentTypes...)>(function);
        }
    };
    return __inner::get(function);
}

template <typename ReflectableType, typename ParentReflectableType, typename ValueType>
auto member_property_ptr(ValueType ParentReflectableType::* property)
{
    struct __inner : protected ReflectableType
    {
        static auto get(ValueType ParentReflectableType::* property)
        {
            return static_cast<ValueType ReflectableType::*>(property);
        }
    };
    return __inner::get(property);
}

template <typename ReflectableType, typename ParentReflectableType, typename PropertyType>
auto member_property_get_ptr(PropertyType ParentReflectableType::* property)
{
    return member_property_ptr<ReflectableType>(property);
}

template <typename ReflectableType, typename ParentReflectableType, typename PropertyType>
auto member_property_get_ptr(PropertyType (ParentReflectableType::* function)(void) const)
{
    return member_function_ptr<ReflectableType>(function);
}

template <typename ReflectableType, typename ParentReflectableType, typename PropertyType>
auto member_property_get_ptr(PropertyType (ParentReflectableType::* function)(void))
{
    return member_function_ptr<ReflectableType>(function);
}

template <typename ReflectableType, typename ParentReflectableType, typename PropertyType>
auto member_property_set_ptr(PropertyType ParentReflectableType::* property)
{
    return member_property_ptr<ReflectableType>(property);
}

template <typename ReflectableType, typename ParentReflectableType, typename PropertyType>
auto member_property_set_ptr(void (ParentReflectableType::* function)(PropertyType))
{
    return member_function_ptr<ReflectableType>(function);
}

template <typename ReflectableType, typename PropertyType>
auto property_get_ptr(PropertyType* property)
{
    return nullptr;
}

template <typename ReflectableType, typename PropertyType>
auto property_get_ptr(PropertyType (*function)(void))
{
    return nullptr;
}

template <typename ReflectableType, typename PropertyType>
auto property_set_ptr(PropertyType* property)
{
    return nullptr;
}

template <typename ReflectableType, typename PropertyType>
auto property_set_ptr(void (*function)(PropertyType))
{
    return nullptr;
}

template <typename ReturnType, typename ArgumentType = void, typename... ArgumentTypes>
std::string overload_name()
{
    auto result = meta::reflectable_traits<ReturnType>::name();
    if constexpr (std::is_void_v<ArgumentType>)
    {
        return result + "()";
    }
    else
    {
        return result +
        "(" + (
            meta::reflectable_traits<ArgumentType>::name() +
            ... + (", " + meta::reflectable_traits<ArgumentTypes>::name())
        ) + ")";
    }
}

template <bool IsConstFunction = false, typename ReturnType, typename... ArgumentTypes>
std::string full_function_name(ReturnType (*)(ArgumentTypes...))
{
    auto result = overload_name<ReturnType, ArgumentTypes...>();
    if constexpr (IsConstFunction) result += " const";

    return result;
}

} // namespace utility

} // namespace rew

#endif // REW_UTILITY_HPP
