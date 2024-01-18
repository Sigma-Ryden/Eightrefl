#ifndef REW_BUILT_IN_ARRAY_HPP
#define REW_BUILT_IN_ARRAY_HPP

#include <array> // array

#include <Rew/Reflectable.hpp>

// as argument type
#include <Rew/BuiltIn/initializer_list.hpp>

TEMPLATE_REFLECTABLE_DECLARATION((template <typename ValueType, std::size_t ArraySize>), (std::array<ValueType, ArraySize>))
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME("std::array<" + NAMEOF(ValueType) + ", " + std::to_string(ArraySize) + ">")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE((template <typename ValueType, std::size_t ArraySize>), (std::array<ValueType, ArraySize>))
    FACTORY(R(std::initializer_list<typename R::value_type>))
    FUNCTION(at, typename R::const_reference(typename R::size_type) const)
    FUNCTION(at, typename R::reference(typename R::size_type))
    FUNCTION(operator[], typename R::const_reference(typename R::size_type) const)
    FUNCTION(operator[], typename R::reference(typename R::size_type))
    FUNCTION(front, typename R::const_reference() const)
    FUNCTION(front, typename R::reference())
    FUNCTION(back, typename R::const_reference() const)
    FUNCTION(back, typename R::reference())
    FUNCTION(data, typename R::const_pointer() const)
    FUNCTION(data, typename R::pointer())
    FUNCTION(begin, typename R::const_iterator() const)
    FUNCTION(begin, typename R::iterator())
    FUNCTION(cbegin, typename R::const_iterator() const)
    FUNCTION(end, typename R::const_iterator() const)
    FUNCTION(end, typename R::iterator())
    FUNCTION(cend, typename R::const_iterator() const)
    FUNCTION(rbegin, typename R::const_reverse_iterator() const)
    FUNCTION(rbegin, typename R::reverse_iterator())
    FUNCTION(crbegin, typename R::const_reverse_iterator() const)
    FUNCTION(rend, typename R::const_reverse_iterator() const)
    FUNCTION(rend, typename R::reverse_iterator())
    FUNCTION(crend, typename R::const_reverse_iterator() const)
    FUNCTION(empty)
    FUNCTION(size)
    FUNCTION(max_size)
    FUNCTION(fill)
    FUNCTION(swap)
REFLECTABLE_INIT()

#endif // REW_BUILT_IN_ARRAY_HPP
