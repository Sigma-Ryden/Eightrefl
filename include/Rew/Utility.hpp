#ifndef REW_UTILITY_HPP
#define REW_UTILITY_HPP

#include <cstddef> // size_t

#include <string> // string
#include <any> // any, any_cast
#include <utility> // reference_wrapper

#include <type_traits> // decay_t, enable_if_t, is_pointer_v, void_t, false_type, true_type

#include <Rew/Detail/Meta.hpp>

namespace rew
{

namespace utility
{

template <typename ValueType>
ValueType argument_cast(const std::any& object)
{
    return std::any_cast<typename meta::argument_type_traits<ValueType>::type>(object);
}

template <typename ArgumentType>
std::string argument_name()
{
    return meta::reflectable_traits<ArgumentType>::name();
}

template <>
inline std::string argument_name<void>()
{
    return "";
}

template <typename... ArgumentTypes, typename ReturnType, class ClassType>
constexpr std::size_t function_arg_count(ReturnType (ClassType::* function)(ArgumentTypes...))
{
    return sizeof...(ArgumentTypes);
}

template <typename... ArgumentTypes, typename ReturnType, class ClassType>
constexpr std::size_t function_arg_count(ReturnType (ClassType::* function)(ArgumentTypes...) const)
{
    return sizeof...(ArgumentTypes);
}

template <typename... ArgumentTypes, typename ReturnType>
constexpr std::size_t function_arg_count(ReturnType (*function)(ArgumentTypes...))
{
    return sizeof...(ArgumentTypes);
}

template <typename ReflectableType, typename PropertyType>
PropertyType property_value(PropertyType ReflectableType::*);

template <typename ReflectableType, typename PropertyType>
PropertyType property_value(PropertyType (ReflectableType::*)(void) const);

template <typename ReflectableType, typename PropertyType>
PropertyType property_value(PropertyType (ReflectableType::*)(void));

template <typename ReflectableType, typename PropertyType>
PropertyType property_value();

template <typename ReflectableType, typename ParentReflectableType, typename ReturnType, typename... ArgumentTypes>
static auto member_function_ptr(ReturnType (ParentReflectableType::* function)(ArgumentTypes...) const)
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
static auto member_function_ptr(ReturnType (ParentReflectableType::* function)(ArgumentTypes...))
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
static auto member_property_ptr(ValueType ParentReflectableType::* property)
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
static auto member_property_get_ptr(PropertyType ParentReflectableType::* property)
{
    return member_property_ptr<ReflectableType>(property);
}

template <typename ReflectableType, typename ParentReflectableType, typename PropertyType>
static auto member_property_get_ptr(PropertyType (ParentReflectableType::* function)(void) const)
{
    return member_function_ptr<ReflectableType>(function);
}

template <typename ReflectableType, typename ParentReflectableType, typename PropertyType>
static auto member_property_get_ptr(PropertyType (ParentReflectableType::* function)(void))
{
    return member_function_ptr<ReflectableType>(function);
}

template <typename ReflectableType, typename ParentReflectableType, typename PropertyType>
static auto member_property_set_ptr(PropertyType ParentReflectableType::* property)
{
    return member_property_ptr<ReflectableType>(property);
}

template <typename ReflectableType, typename ParentReflectableType, typename PropertyType>
static auto member_property_set_ptr(void (ParentReflectableType::* function)(PropertyType))
{
    return member_function_ptr<ReflectableType>(function);
}

template <typename ArgumentType = void, typename ... ArgumentTypes>
std::string full_function_name(const std::string& name)
{
    return name +
        "(" + (
            argument_name<ArgumentType>() + ... + (", " + argument_name<ArgumentTypes>())
        ) + ")";
}

template <typename ReturnType, typename ClassType, typename ... ArgumentTypes>
std::string full_function_name(const std::string& name, ReturnType (ClassType::*)(ArgumentTypes...))
{
    return full_function_name<ArgumentTypes...>(name);
}

template <typename ReturnType, typename ClassType, typename ... ArgumentTypes>
std::string full_function_name(const std::string& name, ReturnType (*)(ArgumentTypes...))
{
    return full_function_name<ArgumentTypes...>(name);
}

template <typename ReturnType, typename ClassType, typename ... ArgumentTypes>
std::string full_function_name(const std::string& name, ReturnType (ClassType::*)(ArgumentTypes...) const)
{
    return full_function_name<ArgumentTypes...>(name) + " const";
}

template <typename ReturnType, typename ... ArgumentTypes>
std::string full_factory_name(ReturnType (*)(ArgumentTypes...))
{
    return full_function_name<ArgumentTypes...>
    (
        meta::reflectable_traits<ReturnType>::name()
    );
}

} // namespace utility

} // namespace rew

#endif // REW_UTILITY_HPP
