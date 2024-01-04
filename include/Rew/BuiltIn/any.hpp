#ifndef REW_BUILT_IN_ANY_HPP
#define REW_BUILT_IN_ANY_HPP

#include <any> // any

#include <Rew/Reflectable.hpp>

#include <Rew/Detail/Meta.hpp>

namespace rew
{

namespace meta
{

template <>
struct is_builtin_reflectable<std::any> : std::true_type {};

} // namespace meta

} // namespace rew

REFLECTABLE_DECLARATION(std::any)

#endif // REW_BUILT_IN_ANY_HPP
