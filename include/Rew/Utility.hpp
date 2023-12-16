#ifndef REW_UTILITY_HPP
#define REW_UTILITY_HPP

#include <cstddef> // size_t

#include <string> // string

namespace rew
{

template <typename... ArgumentTypes, typename ReturnType, class ClassType>
constexpr std::size_t function_args_count(ReturnType (ClassType::* function)(ArgumentTypes...))
{
    return sizeof...(ArgumentTypes);
}

template <typename... ArgumentTypes, typename ReturnType>
constexpr std::size_t function_args_count(ReturnType (*function)(ArgumentTypes...))
{
    return sizeof...(ArgumentTypes);
}

std::string assembly(const std::string& lhs, const std::string& rhs)
{
    return rhs.empty() ? lhs : lhs + ", " + rhs;
}

} // namespace rew

#endif // REW_UTILITY_HPP
