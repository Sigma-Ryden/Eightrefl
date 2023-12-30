#ifndef REW_UTILITY_HPP
#define REW_UTILITY_HPP

#include <cstddef> // size_t

#include <any> // any
#include <utility> // reference_wrapper

#include <type_traits> // decay_t, enable_if_t, is_pointer_v

namespace rew
{
template <typename T, typename enable = void>
struct pure_type
{
    using type = std::decay_t<T>;
};

template <typename T>
struct pure_type<T, std::enable_if_t<std::is_pointer_v<T>>>
{
    using type = void*;
};

template <typename T>
using pure_t = typename pure_type<T>::type;

template <typename ArgumentType>
struct argument_type_traits
{
    using type = ArgumentType;
};

template <typename ArgumentType>
struct argument_type_traits<ArgumentType&>
{
    using type = std::reference_wrapper<ArgumentType>;
};

template <typename ArgumentType>
struct argument_type_traits<const ArgumentType> : argument_type_traits<ArgumentType> {};

template <typename ArgumentType>
struct argument_type_traits<const ArgumentType&> : argument_type_traits<ArgumentType&> {};

template <typename...>
struct function_type_traits;

template <typename ReturnType, typename... ArgumentTypes>
struct function_type_traits<ReturnType, ArgumentTypes...>
{
    using return_type = ReturnType;
    using function_type = ReturnType(*)(ArgumentTypes...);
};

template <typename ReturnType>
struct function_type_traits<ReturnType, void>
{
    using return_type = ReturnType;
    using function_type = ReturnType(*)(void);
};

template <typename ReturnType, typename... ArgumentTypes>
struct function_type_traits<ReturnType(ArgumentTypes...)>
{
    using return_type = ReturnType;
    using function_type = ReturnType(*)(ArgumentTypes...);
};

template <typename ReturnType>
struct function_type_traits<ReturnType(void)>
{
    using return_type = ReturnType;
    using function_type = ReturnType(*)(void);
};

template <typename T, typename enable = void>
struct is_complete : std::false_type {};

template <typename T>
struct is_complete<T, std::void_t<decltype(sizeof(T))>> : std::true_type {};

template <typename T>
struct builtin_reflection_info_t;

template <typename T>
struct reflection_info_t;

template <typename T>
struct is_builtin_reflectable : is_complete<builtin_reflection_info_t<T>> {};

template <typename T>
struct is_reflectable : is_complete<reflection_info_t<T>> {};

template <typename T>
struct is_custom_reflectable
    : std::conjunction<rew::is_reflectable<T>,
                       std::negation<rew::is_builtin_reflectable<T>>> {};

template <typename ValueType>
ValueType argument_cast(const std::any& object)
{
    return std::any_cast<typename argument_type_traits<ValueType>::type>(object);
}

template <typename... ArgumentTypes, typename ReturnType, class ClassType>
constexpr std::size_t function_args_count(ReturnType (ClassType::* function)(ArgumentTypes...))
{
    return sizeof...(ArgumentTypes);
}

template <typename... ArgumentTypes, typename ReturnType, class ClassType>
constexpr std::size_t function_args_count(ReturnType (ClassType::* function)(ArgumentTypes...) const)
{
    return sizeof...(ArgumentTypes);
}

template <typename... ArgumentTypes, typename ReturnType>
constexpr std::size_t function_args_count(ReturnType (*function)(ArgumentTypes...))
{
    return sizeof...(ArgumentTypes);
}

template <typename ReturnType, class ClassType>
auto function_overload(ReturnType (ClassType::* function)(void) const) { return function; }

template <typename ReturnType, class ClassType>
auto function_overload(ReturnType (ClassType::* function)(void)) { return function; }

template <typename... ArgumentTypes, typename ReturnType, class ClassType>
auto function_overload(ReturnType (ClassType::* function)(ArgumentTypes...) const) { return function; }

template <typename... ArgumentTypes, typename ReturnType, class ClassType>
auto function_overload(ReturnType (ClassType::* function)(ArgumentTypes...)) { return function; }

template <typename... ArgumentTypes, typename ReturnType>
auto function_overload(ReturnType (*function)(ArgumentTypes...)) { return function; }

template <typename ReturnType>
auto function_overload(ReturnType (*function)(void)) { return function; }

template <typename ReflectableType, typename PropertyType>
PropertyType property_value(PropertyType ReflectableType::*);

template <typename ReflectableType, typename PropertyType>
PropertyType property_value(PropertyType (ReflectableType::*)(void) const);

template <typename ReflectableType, typename PropertyType>
PropertyType property_value(PropertyType (ReflectableType::*)(void));

template <typename ReflectableType, typename PropertyType>
PropertyType property_value();

} // namespace rew

#endif // REW_UTILITY_HPP
