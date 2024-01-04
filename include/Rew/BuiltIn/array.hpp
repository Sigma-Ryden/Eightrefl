#ifndef SF_BUILT_IN_ARRAY_HPP
#define SF_BUILT_IN_ARRAY_HPP

#include <array> // array

#include <Rew/Reflectable.hpp>

#include <Rew/Detail/Meta.hpp>

namespace rew
{

namespace meta
{

template <typename T, std::size_t N>
struct is_builtin_reflectable<std::array<T, N>> : std::true_type {};

} // namespace meta

} // namespace rew

REFLECTABLE_DECLARATION(std::any)

#endif // SF_BUILT_IN_ARRAY_HPP
