#ifndef REW_BUILTIN_VECTOR_HPP
#define REW_BUILTIN_VECTOR_HPP

#include <vector> // vector

#include <Rew/Reflectable.hpp>
#include <Rew/Common.hpp>

// default allocator for vector
#include <Rew/BuiltIn/allocator.hpp>

// as function argument type
#include <Rew/BuiltIn/initializer_list.hpp>

template <typename ValueType, typename AllocatorType>
REFLECTABLE_USING(std_vector_iterator, typename std::vector<ValueType, AllocatorType>::iterator)

TEMPLATE_REFLECTABLE_DECLARATION
(
    (template <typename ValueType, typename AllocatorType>), (std_vector_iterator<ValueType, AllocatorType>)
)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME(NAMEOF(std::vector<ValueType, AllocatorType>) + "::iterator")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE
(
    (template <typename ValueType, typename AllocatorType>), (std_vector_iterator<ValueType, AllocatorType>)
)
REFLECTABLE_INIT()

template <typename ValueType, typename AllocatorType>
REFLECTABLE_USING(std_vector_const_iterator, typename std::vector<ValueType, AllocatorType>::const_iterator)

TEMPLATE_REFLECTABLE_DECLARATION
(
    (template <typename ValueType, typename AllocatorType>), (std_vector_const_iterator<ValueType, AllocatorType>)
)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME(NAMEOF(std::vector<ValueType, AllocatorType>) + "::const_iterator")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE
(
    (template <typename ValueType, typename AllocatorType>), (std_vector_const_iterator<ValueType, AllocatorType>)
)
REFLECTABLE_INIT()


template <typename ValueType, typename AllocatorType>
REFLECTABLE_USING(std_vector_reverse_iterator, typename std::vector<ValueType, AllocatorType>::reverse_iterator)

TEMPLATE_REFLECTABLE_DECLARATION
(
    (template <typename ValueType, typename AllocatorType>), (std_vector_reverse_iterator<ValueType, AllocatorType>)
)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME(NAMEOF(std::vector<ValueType, AllocatorType>) + "::reverse_iterator")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE
(
    (template <typename ValueType, typename AllocatorType>), (std_vector_reverse_iterator<ValueType, AllocatorType>)
)
REFLECTABLE_INIT()

template <typename ValueType, typename AllocatorType>
REFLECTABLE_USING(std_vector_const_reverse_iterator, typename std::vector<ValueType, AllocatorType>::const_reverse_iterator)

TEMPLATE_REFLECTABLE_DECLARATION
(
    (template <typename ValueType, typename AllocatorType>), (std_vector_const_reverse_iterator<ValueType, AllocatorType>)
)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME(NAMEOF(std::vector<ValueType, AllocatorType>) + "::const_reverse_iterator")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE
(
    (template <typename ValueType, typename AllocatorType>), (std_vector_const_reverse_iterator<ValueType, AllocatorType>)
)
REFLECTABLE_INIT()


TEMPLATE_REFLECTABLE_DECLARATION((template <typename ValueType>), (std::vector<ValueType>))
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME("std::vector<" + NAMEOF(ValueType) + ">")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE_DECLARATION
(
    (template <typename ValueType, typename AllocatorType>), (std::vector<ValueType, AllocatorType>)
)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME("std::vector<" + NAMEOF(ValueType) + ", " + NAMEOF(AllocatorType) + ">")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE
(
    (template <typename ValueType, typename AllocatorType>), (std::vector<ValueType, AllocatorType>)
)
    USING(allocator_type, typename R::allocator_type)
    USING(size_type, typename R::size_type)
    USING(value_type, typename R::value_type)
    USING(const_pointer, typename R::const_pointer)
    USING(pointer, typename R::pointer)
    USING(const_reference, typename R::const_reference)
    USING(reference, typename R::reference)
    USING(const_iterator, std_vector_const_iterator<ValueType, AllocatorType>)
    USING(const_reverse_iterator, std_vector_const_reverse_iterator<ValueType, AllocatorType>)
    USING(iterator, std_vector_iterator<ValueType, AllocatorType>)
    USING(reverse_iterator, std_vector_reverse_iterator<ValueType, AllocatorType>)
    FACTORY(R())
    FACTORY(R(allocator_type const&))
    FACTORY(R(size_type, const_reference, allocator_type const&))
    FACTORY(R(size_type, const_reference))
    FACTORY(R(size_type, allocator_type const&))
    FACTORY(R(size_type))
    FACTORY(R(const_iterator, const_iterator, allocator_type const&))
    FACTORY(R(const_iterator, const_iterator))
    FACTORY(R(R const&,  allocator_type const&))
    FACTORY(R(R const&))
    FACTORY(R(std::initializer_list<value_type>, allocator_type const&))
    FACTORY(R(std::initializer_list<value_type>))
    FUNCTION(operator=, R&(R const&))
    FUNCTION(operator=, R&(std::initializer_list<value_type>))
    FUNCTION(assign, void(size_type, const_reference))
    FUNCTION(assign, void(const_iterator, const_iterator))
    FUNCTION(assign, void(std::initializer_list<value_type>))
    FUNCTION(get_allocator)
    FUNCTION(at, const_reference(size_type) const)
    FUNCTION(at, reference(size_type))
    FUNCTION(operator[], const_reference(size_type) const)
    FUNCTION(operator[], reference(size_type))
    FUNCTION(front, const_reference() const)
    FUNCTION(front, reference())
    FUNCTION(back, const_reference() const)
    FUNCTION(back, reference())
    FUNCTION(data, const_pointer() const)
    FUNCTION(data, pointer())
    FUNCTION(begin, const_iterator() const)
    FUNCTION(begin, iterator())
    FUNCTION(cbegin, const_iterator() const)
    FUNCTION(end, const_iterator() const)
    FUNCTION(end, iterator())
    FUNCTION(cend, const_iterator() const)
    FUNCTION(rbegin, const_reverse_iterator() const)
    FUNCTION(rbegin, reverse_iterator())
    FUNCTION(crbegin, const_reverse_iterator() const)
    FUNCTION(rend, const_reverse_iterator() const)
    FUNCTION(rend, reverse_iterator())
    FUNCTION(crend, const_reverse_iterator() const)
    FUNCTION(empty)
    FUNCTION(size)
    FUNCTION(max_size)
    FUNCTION(capacity)
    FUNCTION(reserve)
    FUNCTION(shrink_to_fit)
    FUNCTION(clear)
    FUNCTION(insert, iterator(const_iterator, const_reference))
    FUNCTION(insert, iterator(const_iterator, size_type, const_reference))
    FUNCTION(insert, iterator(const_iterator, const_iterator, const_iterator))
    FUNCTION(insert, iterator(const_iterator, std::initializer_list<value_type>))
    FUNCTION(erase, iterator(const_iterator))
    FUNCTION(erase, iterator(const_iterator, const_iterator))
    FUNCTION(push_back, void(const_reference))
    FUNCTION(pop_back)
    FUNCTION(resize, void(size_type))
    FUNCTION(resize, void(size_type, const_reference))
    FUNCTION(swap)
REFLECTABLE_INIT()

#endif // REW_BUILTIN_VECTOR_HPP
