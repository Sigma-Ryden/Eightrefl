#ifndef REW_BUILT_IN_FORWARD_LIST_HPP
#define REW_BUILT_IN_FORWARD_LIST_HPP

#include <forward_list> // forward_list

#include <Rew/Reflectable.hpp>

// default allocator for forward_list
#include <Rew/BuiltIn/allocator.hpp>

// as argument type
#include <Rew/BuiltIn/initializer_list.hpp>
#include <Rew/BuiltIn/function.hpp>

TEMPLATE_REFLECTABLE_DECLARATION((template <typename ValueType, typename AllocatorType>), (std::forward_list<ValueType, AllocatorType>))
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME("std::forward_list<" + NAMEOF(ValueType) + ", " + NAMEOF(AllocatorType) + ">")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE((template <typename ValueType, typename AllocatorType>), (std::forward_list<ValueType, AllocatorType>))
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
    FUNCTION(front, typename R::const_reference() const)
    FUNCTION(front, typename R::reference())
    FUNCTION(before_begin, typename R::const_iterator() const)
    FUNCTION(before_begin, typename R::iterator())
    FUNCTION(cbefore_begin)
    FUNCTION(begin, typename R::const_iterator() const)
    FUNCTION(begin, typename R::iterator())
    FUNCTION(cbegin)
    FUNCTION(end, typename R::const_iterator() const)
    FUNCTION(end, typename R::iterator())
    FUNCTION(cend)
    FUNCTION(crend)
    FUNCTION(empty)
    FUNCTION(max_size)
    FUNCTION(clear)
    FUNCTION(insert_after, typename R::iterator(typename R::const_iterator, typename R::const_reference))
    FUNCTION(insert_after, typename R::iterator(typename R::const_iterator, typename R::size_type, typename R::const_reference))
    FUNCTION(insert_after, typename R::iterator(typename R::const_iterator, typename R::const_iterator, typename R::const_iterator))
    FUNCTION(insert_after, typename R::iterator(typename R::const_iterator, std::initializer_list<typename R::value_type>))
    FUNCTION(erase_after, typename R::iterator(typename R::const_iterator))
    FUNCTION(erase_after, typename R::iterator(typename R::const_iterator, typename R::const_iterator))
    FUNCTION(push_front, void(typename R::const_reference))
    FUNCTION(pop_front)
    FUNCTION(resize, void(typename R::size_type))
    FUNCTION(resize, void(typename R::size_type, typename R::const_reference))
    FUNCTION(swap)
    FUNCTION(merge, void(R&))
    FUNCTION(merge, void(R&, std::function<bool(typename R::const_reference, typename R::const_reference)>))
    FUNCTION(splice_after, void(typename R::const_iterator, R&))
    FUNCTION(splice_after, void(typename R::const_iterator, R&, typename R::const_iterator))
    FUNCTION(splice_after, void(typename R::const_iterator, R&, typename R::const_iterator, typename R::const_iterator))
#if __cplusplus < 202002L
    FUNCTION(remove, void(typename R::const_reference))
    FUNCTION(remove_if, void(std::function<bool(typename R::const_reference)>))
    FUNCTION(unique, void())
    FUNCTION(unique, void(std::function<bool(typename R::const_reference, typename R::const_reference)>))
#else
    FUNCTION(remove, typename R::size_type(typename R::const_reference))
    FUNCTION(remove_if, typename R::size_type(std::function<bool(typename R::const_reference)>))
    FUNCTION(unique, typename R::size_type())
    FUNCTION(unique, typename R::size_type(std::function<bool(typename R::const_reference, typename R::const_reference)>))
#endif // if
    FUNCTION(reverse)
    FUNCTION(sort, void())
    FUNCTION(sort, void(std::function<bool(typename R::const_reference, typename R::const_reference)>))
REFLECTABLE_INIT()

#endif // REW_BUILT_IN_FORWARD_LIST_HPP
