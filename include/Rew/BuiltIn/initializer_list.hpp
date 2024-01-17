#ifndef REW_BUILT_IN_INITIALIZER_LIST_HPP
#define REW_BUILT_IN_INITIALIZER_LIST_HPP

#include <initializer_list> // initializer_list

#include <Rew/Reflectable.hpp>

namespace rew
{

namespace meta
{

template <typename T> struct is_std_initializer_list : std::false_type {};
template <typename T> struct is_std_initializer_list<std::initializer_list<T>> : std::true_type {};

} // namespace meta

} // namespace rew

CONDITIONAL_REFLECTABLE_DECLARATION(rew::meta::is_std_initializer_list<T>::value)
    REFLECTABLE_NAME("std::initializer_list<"+NAMEOF(typename T::value_type)+">")
REFLECTABLE_DECLARATION_INIT()

CONDITIONAL_REFLECTABLE(rew::meta::is_std_initializer_list<T>::value)
    FACTORY(T())
    FUNCTION(begin)
    FUNCTION(end)
    FUNCTION(size)
REFLECTABLE_INIT()

#endif // REW_BUILT_IN_INITIALIZER_LIST_HPP
