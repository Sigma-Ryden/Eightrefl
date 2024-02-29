#ifndef REW_BUILTIN_ARRAY_HPP
#define REW_BUILTIN_ARRAY_HPP

#include <array> // array

#include <Rew/Reflectable.hpp>
#include <Rew/Common.hpp>

// as function argument type
#include <Rew/BuiltIn/iterator.hpp>

TEMPLATE_REFLECTABLE_CLEAN
(
    (template <typename ValueType, std::size_t ArraySize>), (std::array<ValueType, ArraySize>),
    std::array<CLEANOF(ValueType), ArraySize>
)

TEMPLATE_REFLECTABLE_DECLARATION((template <typename ValueType, std::size_t ArraySize>), std::array<ValueType, ArraySize>)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME("std::array<" + NAMEOF(ValueType) + ", " + std::to_string(ArraySize) + ">")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE((template <typename ValueType, std::size_t ArraySize>), std::array<ValueType, ArraySize>)
    FACTORY(R())
    FACTORY(R(R const&))
    FUNCTION(operator=, R&(R const&))
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
    FUNCTION(begin, std_const_iterator<R>() const)
    FUNCTION(begin, std_iterator<R>())
    FUNCTION(cbegin, std_const_iterator<R>() const)
    FUNCTION(end, std_const_iterator<R>() const)
    FUNCTION(end, std_iterator<R>())
    FUNCTION(cend, std_const_iterator<R>() const)
    FUNCTION(rbegin, std_const_reverse_iterator<R>() const)
    FUNCTION(rbegin, std_reverse_iterator<R>())
    FUNCTION(crbegin, std_const_reverse_iterator<R>() const)
    FUNCTION(rend, std_const_reverse_iterator<R>() const)
    FUNCTION(rend, std_reverse_iterator<R>())
    FUNCTION(crend, std_const_reverse_iterator<R>() const)
    FUNCTION(empty)
    FUNCTION(size)
    FUNCTION(max_size)
    FUNCTION(fill)
    FUNCTION(swap)
    FREE_FUNCTION(std::operator==, bool(const R&, const R&))
REFLECTABLE_INIT()

#endif // REW_BUILTIN_ARRAY_HPP
