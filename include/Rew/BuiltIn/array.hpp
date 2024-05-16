#ifndef REW_BUILTIN_ARRAY_HPP
#define REW_BUILTIN_ARRAY_HPP

#include <array> // array
#include <string> // to_string

#include <Rew/Reflectable.hpp>
#include <Rew/Common.hpp>

#ifndef REW_CORE_MINIMAL
// as function argument type
#include <Rew/BuiltIn/iterator.hpp>
#endif // REW_CORE_MINIMAL

TEMPLATE_REFLECTABLE_CLEAN
(
    (template <typename ValueType, std::size_t ArraySize>),
    (std::array<ValueType, ArraySize>), std::array<rew::cleanof<ValueType>, ArraySize>
)

TEMPLATE_REFLECTABLE_DECLARATION((template <typename ValueType, std::size_t ArraySize>), std::array<ValueType, ArraySize>)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME("std::array<" + rew::nameof<ValueType>() + ", " + std::to_string(ArraySize) + ">")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE((template <typename ValueType, std::size_t ArraySize>), std::array<ValueType, ArraySize>)
    FACTORY(R())
    FACTORY(R(R const&))
    FUNCTION(operator=, R&(R const&))

    #ifndef REW_CORE_MINIMAL
    FUNCTION(at, typename R::const_reference(typename R::size_type) const)
    FUNCTION(at, typename R::reference(typename R::size_type))
    #endif // REW_CORE_MINIMAL

    FUNCTION(operator[], typename R::const_reference(typename R::size_type) const)
    FUNCTION(operator[], typename R::reference(typename R::size_type))

    #ifndef REW_CORE_MINIMAL
    FUNCTION(front, typename R::const_reference() const)
    FUNCTION(front, typename R::reference())
    FUNCTION(back, typename R::const_reference() const)
    FUNCTION(back, typename R::reference())
    #endif // REW_CORE_MINIMAL

    FUNCTION(data, typename R::const_pointer() const)
    FUNCTION(data, typename R::pointer())

    #ifndef REW_CORE_MINIMAL
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
    #endif // REW_CORE_MINIMAL

    FUNCTION(empty)
    FUNCTION(size)

    #ifndef REW_CORE_MINIMAL
    FUNCTION(max_size)
    FUNCTION(fill)
    FUNCTION(swap)
    #endif // REW_CORE_MINIMAL

    //FREE_FUNCTION(std::operator==, bool(R const&, R const&))
REFLECTABLE_INIT()

#endif // REW_BUILTIN_ARRAY_HPP
