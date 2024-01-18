#ifndef REW_BUILT_IN_STRING_HPP
#define REW_BUILT_IN_STRING_HPP

#include <vector> // vector

#include <Rew/Reflectable.hpp>

// default allocator for vector
#include <Rew/BuiltIn/allocator.hpp>

// as argument type
#include <Rew/BuiltIn/initializer_list.hpp>

namespace rew
{

namespace meta
{

template <typename T> struct is_std_vector : std::false_type {};
template <typename T, typename Allocator> struct is_std_vector<std::vector<T, Allocator>> : std::true_type {};

} // namespace meta

} // namespace rew

CONDITIONAL_REFLECTABLE_DECLARATION(rew::meta::is_std_vector<T>::value)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME("std::vector<" + NAMEOF(typename T::value_type) + ", " + NAMEOF(typename T::allocator_type) + ">")
REFLECTABLE_DECLARATION_INIT()

CONDITIONAL_REFLECTABLE(rew::meta::is_std_vector<T>::value)
    FACTORY(T(T const&))
    FACTORY(T())
    FACTORY(T(typename T::size_type))
    FACTORY(T(typename T::size_type, typename T::const_reference))
    FACTORY(T(T const&))
    FACTORY(T(std::initializer_list<typename T::value_type>))
    FUNCTION(operator=, T&(T const&))
    FUNCTION(operator=, T&(std::initializer_list<typename T::value_type>))
    FUNCTION(assign, void(typename T::size_type, typename T::const_reference))
    FUNCTION(assign, void(std::initializer_list<typename T::value_type>))
    FUNCTION(get_allocator)
    FUNCTION(at, typename T::const_reference(typename T::size_type) const)
    FUNCTION(at, typename T::reference(typename T::size_type))
    FUNCTION(operator[], typename T::const_reference(typename T::size_type) const)
    FUNCTION(operator[], typename T::reference(typename T::size_type))
    FUNCTION(front, typename T::const_reference() const)
    FUNCTION(front, typename T::reference())
    FUNCTION(back, typename T::const_reference() const)
    FUNCTION(back, typename T::reference())
    FUNCTION(data, typename T::const_pointer() const)
    FUNCTION(data, typename T::pointer())
    FUNCTION(begin, typename T::const_iterator() const)
    FUNCTION(begin, typename T::iterator())
    FUNCTION(cbegin, typename T::const_iterator() const)
    FUNCTION(end, typename T::const_iterator() const)
    FUNCTION(end, typename T::iterator())
    FUNCTION(cend, typename T::const_iterator() const)
    FUNCTION(rbegin, typename T::const_reverse_iterator() const)
    FUNCTION(rbegin, typename T::reverse_iterator())
    FUNCTION(crbegin, typename T::const_reverse_iterator() const)
    FUNCTION(rend, typename T::const_reverse_iterator() const)
    FUNCTION(rend, typename T::reverse_iterator())
    FUNCTION(crend, typename T::const_reverse_iterator() const)
    FUNCTION(empty)
    FUNCTION(size)
    FUNCTION(max_size)
    FUNCTION(capacity)
    FUNCTION(reserve)
    FUNCTION(shrink_to_fit)
    FUNCTION(clear)
    FUNCTION(insert, typename T::iterator(typename T::const_iterator, typename T::const_reference))
    FUNCTION(insert, typename T::iterator(typename T::const_iterator, typename T::size_type, typename T::const_reference))
    FUNCTION(insert, typename T::iterator(typename T::const_iterator, std::initializer_list<typename T::value_type>))
    FUNCTION(erase, typename T::iterator(typename T::const_iterator))
    FUNCTION(erase, typename T::iterator(typename T::const_iterator, typename T::const_iterator))
    FUNCTION(push_back, void(typename T::const_reference))
    FUNCTION(pop_back)
    FUNCTION(resize, void(typename T::size_type))
    FUNCTION(resize, void(typename T::size_type, typename T::const_reference))
    FUNCTION(swap)
REFLECTABLE_INIT()

#endif // REW_BUILT_IN_STRING_HPP
