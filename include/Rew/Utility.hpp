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
    return meta::reflectable_name_t<ArgumentType>::get();
}

template <>
inline std::string argument_name<void>()
{
    return "";
}

template <typename... ArgumentTypes>
struct readonly_overload
{
    template <typename ClassType, typename ReturnType>
    static constexpr auto of(ReturnType (ClassType::* function)(ArgumentTypes...) const) { return function; }

    template <typename ReturnType>
    static constexpr auto of(ReturnType (*function)(ArgumentTypes...)) { return function; }

    template <typename ClassType, typename... OtherArgumentTypes, typename ReturnType>
    static constexpr auto of(ReturnType (ClassType::* function)(OtherArgumentTypes...) const) { return function; }

    template <typename... OtherArgumentTypes, typename ReturnType>
    static constexpr auto of(ReturnType (*function)(OtherArgumentTypes...)) { return function; }
};

template <typename... ArgumentTypes>
struct overload
{
    template <typename ClassType, typename ReturnType>
    static constexpr auto of(ReturnType (ClassType::* function)(ArgumentTypes...)) { return function; }

    template <typename ReturnType>
    static constexpr auto of(ReturnType (*function)(ArgumentTypes...)) { return function; }

    template <typename ClassType, typename... OtherArgumentTypes, typename ReturnType>
    static constexpr auto of(ReturnType (ClassType::* function)(OtherArgumentTypes...)) { return function; }

    template <typename... OtherArgumentTypes, typename ReturnType>
    static constexpr auto of(ReturnType (*function)(OtherArgumentTypes...)) { return function; }
};

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

template <typename ArgumentType = void, typename ... ArgumentTypes>
std::string readonly_full_function_name(const std::string& name)
{
    return full_function_name<ArgumentType, ArgumentTypes...>(name) + " const";
}

template <typename ReturnType, typename ClassType, typename ... ArgumentTypes>
std::string readonly_full_function_name(const std::string& name, ReturnType (ClassType::*)(ArgumentTypes...) const)
{
    return readonly_full_function_name<ArgumentTypes...>(name);
}

template <typename ReturnType, typename ... ArgumentTypes>
std::string full_factory_name(ReturnType (*)(ArgumentTypes...))
{
    return full_function_name<ArgumentTypes...>
    (
        meta::reflectable_name_t<ReturnType>::get()
    );
}

} // namespace utility

} // namespace rew

#endif // REW_UTILITY_HPP
