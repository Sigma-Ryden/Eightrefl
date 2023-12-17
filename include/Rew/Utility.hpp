#ifndef REW_UTILITY_HPP
#define REW_UTILITY_HPP

#include <cstddef> // size_t

#include <string> // string
#include <utility> // reference_wrapper

namespace rew
{

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

std::string function_args_assembly(const std::string& lhs, const std::string& rhs)
{
    return rhs.empty() ? lhs : lhs + ", " + rhs;
}

} // namespace rew

#endif // REW_UTILITY_HPP
