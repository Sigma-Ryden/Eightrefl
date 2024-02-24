#ifndef REW_BUILTIN_SET_HPP
#define REW_BUILTIN_SET_HPP

#include <set> // set, multiset

#include <Rew/Reflectable.hpp>
#include <Rew/Common.hpp>

// default allocator for set, multiset
#include <Rew/BuiltIn/allocator.hpp>

// as function argument type
#include <Rew/BuiltIn/initializer_list.hpp>
#include <Rew/BuiltIn/iterator.hpp>

TEMPLATE_REFLECTABLE_CLEAN
(
    (template <typename KeyType, typename Comparator, typename AllocatorType>),
    (std::set<KeyType, Comparator, AllocatorType>),
    std::set<CLEANOF(KeyType), CLEANOF(Comparator), CLEANOF(AllocatorType)>
)

TEMPLATE_REFLECTABLE_DECLARATION((template <typename KeyType>), std::set<KeyType>)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME("std::set<" + NAMEOF(KeyType) + ">")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE_DECLARATION
(
    (template <typename KeyType, typename Comparator>),
    std::set<KeyType, Comparator>
)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME("std::set<" + NAMEOF(KeyType) + ", " + NAMEOF(Comparator) + ">")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE_DECLARATION
(
    (template <typename KeyType, typename Comparator, typename AllocatorType>),
    std::set<KeyType, Comparator, AllocatorType>
)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME
    (
        "std::set<" + NAMEOF(KeyType) + ", " + NAMEOF(Comparator) + ", " + NAMEOF(AllocatorType) + ">"
    )
REFLECTABLE_DECLARATION_INIT()


TEMPLATE_REFLECTABLE_CLEAN
(
    (template <typename KeyType, typename Comparator, typename AllocatorType>),
    (std::multiset<KeyType, Comparator, AllocatorType>),
    std::multiset<CLEANOF(KeyType), CLEANOF(Comparator), CLEANOF(AllocatorType)>
)

TEMPLATE_REFLECTABLE_DECLARATION((template <typename KeyType>), std::multiset<KeyType>)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME("std::multiset<" + NAMEOF(KeyType) + ">")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE_DECLARATION
(
    (template <typename KeyType, typename Comparator>),
    std::multiset<KeyType, Comparator>
)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME("std::multiset<" + NAMEOF(KeyType) + ", " + NAMEOF(Comparator) + ">")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE_DECLARATION
(
    (template <typename KeyType, typename Comparator, typename AllocatorType>),
    std::multiset<KeyType, Comparator, AllocatorType>
)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME
    (
        "std::multiset<" + NAMEOF(KeyType) + ", " + NAMEOF(Comparator) + ", " + NAMEOF(AllocatorType) + ">"
    )
REFLECTABLE_DECLARATION_INIT()


template <typename> struct __rew_is_any_std_ordered_set : std::false_type {};

template <typename KeyType, typename Comparator, typename AllocatorType>
struct __rew_is_any_std_ordered_set<std::set<KeyType, Comparator, AllocatorType>> : std::true_type {};

template <typename KeyType, typename Comparator, typename AllocatorType>
struct __rew_is_any_std_ordered_set<std::multiset<KeyType, Comparator, AllocatorType>> : std::true_type {};

CONDITIONAL_REFLECTABLE(__rew_is_any_std_ordered_set<CLEANOF(DirtyR)>::value)
    FACTORY(R())
    FACTORY(R(typename R::key_compare const&, typename R::allocator_type const&))
    FACTORY(R(typename R::key_compare const&))
    FACTORY(R(typename R::allocator_type const&))
    FACTORY(R(std_const_iterator<R>, std_const_iterator<R>, typename R::key_compare const&, typename R::allocator_type const&))
    FACTORY(R(std_const_iterator<R>, std_const_iterator<R>, typename R::allocator_type const&))
    FACTORY(R(R const&))
    FACTORY(R(R const&, typename R::allocator_type const&))
    FACTORY(R(std::initializer_list<typename R::value_type>, typename R::key_compare const&, typename R::allocator_type const&))
    FACTORY(R(std::initializer_list<typename R::value_type>))
    FUNCTION(operator=, R&(R const&))
    FUNCTION(operator=, R&(std::initializer_list<typename R::value_type>))
    FUNCTION(get_allocator)
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
    FUNCTION(clear)
    FUNCTION(insert, std::pair<std_iterator<R>, bool>(typename R::const_reference))
    FUNCTION(insert, std_iterator<R>(std_iterator<R>, typename R::const_reference))
    FUNCTION(insert, std_iterator<R>(std_const_iterator<R>, std_const_iterator<R>))
    FUNCTION(insert, void(std::initializer_list<typename R::value_type>))
    FUNCTION(erase, std_iterator<R>(std_iterator<R>))
    FUNCTION(erase, std_iterator<R>(std_const_iterator<R>))
    FUNCTION(erase, std_iterator<R>(std_const_iterator<R>, std_const_iterator<R>))
    FUNCTION(swap)
    FUNCTION(extract, typename R::node_type(std_const_iterator<R>))
    FUNCTION(extract, typename R::node_type(typename R::key_type const&))
    FUNCTION(merge, void(R&))
    FUNCTION(count, typename R::size_type(typename R::key_type const&))
    FUNCTION(find, std_iterator<R>(typename R::key_type const&))
    FUNCTION(find, std_const_iterator<R>(typename R::key_type const&) const)
#if __cplusplus >= 202002L
    FUNCTION(contains, bool(typename R::key_type const&) const)
#endif // if
    FUNCTION(equal_range, std::pair<std_iterator<R>, std_iterator<R>>(typename R::key_type const&))
    FUNCTION(equal_range, std::pair<std_const_iterator<R>, std_const_iterator<R>>(typename R::key_type const&) const)
    FUNCTION(lower_bound, std_iterator<R>(typename R::key_type const&))
    FUNCTION(lower_bound, std_const_iterator<R>(typename R::key_type const&) const)
    FUNCTION(upper_bound, std_iterator<R>(typename R::key_type const&))
    FUNCTION(upper_bound, std_const_iterator<R>(typename R::key_type const&) const)
//  FUNCTION(key_comp)
//  FUNCTION(value_comp)
REFLECTABLE_INIT()

#endif // REW_BUILTIN_SET_HPP
