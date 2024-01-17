#ifndef REW_BUILT_IN_ALLOCATOR_HPP
#define REW_BUILT_IN_ALLOCATOR_HPP

#include <memory> // allocator

#include <Rew/Reflectable.hpp>

namespace rew
{

namespace meta
{

template <typename T> struct is_std_allocator : std::false_type {};
template <typename T> struct is_std_allocator<std::allocator<T>> : std::true_type {};

} // namespace meta

} // namespace rew

CONDITIONAL_REFLECTABLE_DECLARATION(rew::meta::is_std_allocator<T>::value)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME("std::allocator<"+NAMEOF(typename T::value_type)+">")
REFLECTABLE_DECLARATION_INIT()

CONDITIONAL_REFLECTABLE(rew::meta::is_std_allocator<T>::value)
    FACTORY(T())
    FACTORY(T(T const&))
    FUNCTION(allocate, typename T::value_type*(std::size_t, const void*))
    FUNCTION(deallocate, void(typename T::value_type*, std::size_t))
REFLECTABLE_INIT()

#endif // REW_BUILT_IN_ALLOCATOR_HPP
