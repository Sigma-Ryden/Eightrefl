#ifndef REW_UTILITY_HPP
#define REW_UTILITY_HPP

#include <cstddef> // size_t

#include <string> // string
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
struct function_type_traits
{
    using cast_t = ArgumentType;
};

template <typename ArgumentType>
struct function_type_traits<ArgumentType&>
{
    using cast_t = std::reference_wrapper<ArgumentType>;
};

template <typename ArgumentType>
struct function_type_traits<const ArgumentType> : function_type_traits<ArgumentType> {};

template <typename ArgumentType>
struct function_type_traits<const ArgumentType&> : function_type_traits<ArgumentType&> {};

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

std::string function_args_assembly(const std::string& lhs, const std::string& rhs)
{
    std::string result;
    result.reserve(lhs.size()+rhs.size()+1);
    result.append(lhs);
    if (not rhs.empty()) result.append(", ");
    result.append(rhs);
    return result;
}

} // namespace rew

#endif // REW_UTILITY_HPP
