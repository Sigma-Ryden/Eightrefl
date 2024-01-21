#ifndef REW_BUILT_IN_DEQUE_HPP
#define REW_BUILT_IN_DEQUE_HPP

#include <deque> // deque

#include <Rew/Reflectable.hpp>
#include <Rew/Common.hpp>

// default allocator for deque
#include <Rew/BuiltIn/allocator.hpp>

// as function argument type
#include <Rew/BuiltIn/initializer_list.hpp>

TEMPLATE_REFLECTABLE_DECLARATION((template <typename ValueType, typename AllocatorType>), (std::deque<ValueType, AllocatorType>))
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME("std::deque<" + NAMEOF(ValueType) + ", " + NAMEOF(AllocatorType) + ">")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE((template <typename ValueType, typename AllocatorType>), (std::deque<ValueType, AllocatorType>))
    FACTORY(R())
    FACTORY(R(typename R::allocator_type const&))
    FACTORY(R(typename R::size_type, typename R::const_reference, typename R::allocator_type const&))
    FACTORY(R(typename R::size_type, typename R::const_reference))
    FACTORY(R(typename R::size_type, typename R::allocator_type const&))
    FACTORY(R(typename R::size_type))
    FACTORY(R(typename R::const_iterator, typename R::const_iterator, typename R::allocator_type const&))
    FACTORY(R(typename R::const_iterator, typename R::const_iterator))
    FACTORY(R(R const&, typename R::allocator_type const&))
    FACTORY(R(R const&))
    FACTORY(R(std::initializer_list<typename R::value_type>, typename R::allocator_type const&))
    FACTORY(R(std::initializer_list<typename R::value_type>))
    FUNCTION(operator=, R&(R const&))
    FUNCTION(operator=, R&(std::initializer_list<typename R::value_type>))
    FUNCTION(assign, void(typename R::size_type, typename R::const_reference))
    FUNCTION(assign, void(typename R::const_iterator, typename R::const_iterator))
    FUNCTION(assign, void(std::initializer_list<typename R::value_type>))
    FUNCTION(get_allocator)
    FUNCTION(at, typename R::const_reference(typename R::size_type) const)
    FUNCTION(at, typename R::reference(typename R::size_type))
    FUNCTION(operator[], typename R::const_reference(typename R::size_type) const)
    FUNCTION(operator[], typename R::reference(typename R::size_type))
    FUNCTION(front, typename R::const_reference() const)
    FUNCTION(front, typename R::reference())
    FUNCTION(back, typename R::const_reference() const)
    FUNCTION(back, typename R::reference())
    FUNCTION(begin, typename R::const_iterator() const)
    FUNCTION(begin, typename R::iterator())
    FUNCTION(cbegin)
    FUNCTION(end, typename R::const_iterator() const)
    FUNCTION(end, typename R::iterator())
    FUNCTION(cend)
    FUNCTION(rbegin, typename R::const_reverse_iterator() const)
    FUNCTION(rbegin, typename R::reverse_iterator())
    FUNCTION(crbegin)
    FUNCTION(rend, typename R::const_reverse_iterator() const)
    FUNCTION(rend, typename R::reverse_iterator())
    FUNCTION(crend)
    FUNCTION(empty)
    FUNCTION(size)
    FUNCTION(max_size)
    FUNCTION(shrink_to_fit)
    FUNCTION(clear)
    FUNCTION(insert, typename R::iterator(typename R::const_iterator, typename R::const_reference))
    FUNCTION(insert, typename R::iterator(typename R::const_iterator, typename R::size_type, typename R::const_reference))
    FUNCTION(insert, typename R::iterator(typename R::const_iterator, typename R::const_reference, typename R::const_reference))
    FUNCTION(insert, typename R::iterator(typename R::const_iterator, std::initializer_list<typename R::value_type>))
    FUNCTION(erase, typename R::iterator(typename R::const_iterator))
    FUNCTION(erase, typename R::iterator(typename R::const_iterator, typename R::const_iterator))
    FUNCTION(push_back, void(typename R::const_reference))
    FUNCTION(push_front, void(typename R::const_reference))
    FUNCTION(pop_back)
    FUNCTION(pop_front)
    FUNCTION(resize, void(typename R::size_type))
    FUNCTION(resize, void(typename R::size_type, typename R::const_reference))
    FUNCTION(swap)
REFLECTABLE_INIT()

#endif // REW_BUILT_IN_DEQUE_HPP
