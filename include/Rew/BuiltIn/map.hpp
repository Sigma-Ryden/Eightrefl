// TODO: add std::less, std::greater
#ifndef REW_BUILTIN_MAP_HPP
#define REW_BUILTIN_MAP_HPP

#include <map> // map, multimap

#include <Rew/Reflectable.hpp>
#include <Rew/Common.hpp>

// default allocator for map, multimap
#include <Rew/BuiltIn/allocator.hpp>

// as function argument type
#include <Rew/BuiltIn/initializer_list.hpp>
#include <Rew/BuiltIn/iterator.hpp>

// as mapped type
#include <Rew/BuiltIn/pair.hpp>

TEMPLATE_REFLECTABLE_CLEAN
(
    (template <typename KeyType, typename ValueType, typename Comparator, typename AllocatorType>),
    (std::map<KeyType, ValueType, Comparator, AllocatorType>),
    std::map<CLEANOF(KeyType), CLEANOF(ValueType), CLEANOF(Comparator), CLEANOF(AllocatorType)>
)

TEMPLATE_REFLECTABLE_DECLARATION((template <typename KeyType, typename ValueType>), std::map<KeyType, ValueType>)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME( "std::map<" + NAMEOF(KeyType) + ", " + NAMEOF(ValueType) + ">")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE_DECLARATION
(
    (template <typename KeyType, typename ValueType, typename Comparator>),
    std::map<KeyType, ValueType, Comparator>
)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME
    (
        "std::map<" + NAMEOF(KeyType) + ", " + NAMEOF(ValueType) + ", " + NAMEOF(Comparator) + ">"
    )
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE_DECLARATION
(
    (template <typename KeyType, typename ValueType, typename Comparator, typename AllocatorType>),
    std::map<KeyType, ValueType, Comparator, AllocatorType>
)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME
    (
        "std::map<" + NAMEOF(KeyType) + ", " + NAMEOF(ValueType) + ", " + NAMEOF(Comparator) + ", " + NAMEOF(AllocatorType) + ">"
    )
REFLECTABLE_DECLARATION_INIT()


TEMPLATE_REFLECTABLE_CLEAN
(
    (template <typename KeyType, typename ValueType, typename Comparator, typename AllocatorType>),
    (std::multimap<KeyType, ValueType, Comparator, AllocatorType>),
    std::multimap<CLEANOF(KeyType), CLEANOF(ValueType), CLEANOF(Comparator), CLEANOF(AllocatorType)>
)

TEMPLATE_REFLECTABLE_DECLARATION((template <typename KeyType, typename ValueType>), std::multimap<KeyType, ValueType>)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME( "std::multimap<" + NAMEOF(KeyType) + ", " + NAMEOF(ValueType) + ">")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE_DECLARATION
(
    (template <typename KeyType, typename ValueType, typename Comparator>),
    std::multimap<KeyType, ValueType, Comparator>
)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME
    (
        "std::multimap<" + NAMEOF(KeyType) + ", " + NAMEOF(ValueType) + ", " + NAMEOF(Comparator) + ">"
    )
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE_DECLARATION
(
    (template <typename KeyType, typename ValueType, typename Comparator, typename AllocatorType>),
    std::multimap<KeyType, ValueType, Comparator, AllocatorType>
)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME
    (
        "std::multimap<" + NAMEOF(KeyType) + ", " + NAMEOF(ValueType) + ", " + NAMEOF(Comparator) + ", " + NAMEOF(AllocatorType) + ">"
    )
REFLECTABLE_DECLARATION_INIT()


template <typename> struct __rew_is_any_std_ordered_map : std::false_type {};

template <typename KeyType, typename ValueType, typename Comparator, typename AllocatorType>
struct __rew_is_any_std_ordered_map<std::map<KeyType, ValueType, Comparator, AllocatorType>> : std::true_type {};

template <typename KeyType, typename ValueType, typename Comparator, typename AllocatorType>
struct __rew_is_any_std_ordered_map<std::multimap<KeyType, ValueType, Comparator, AllocatorType>> : std::true_type {};

CONDITIONAL_REFLECTABLE(__rew_is_any_std_ordered_map<CLEANOF(DirtyR)>::value)
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
    FUNCTION(at, typename R::mapped_type&(typename R::key_type const&))
    FUNCTION(at, typename R::mapped_type const&(typename R::key_type const&) const)
    FUNCTION(operator[], typename R::mapped_type&(typename R::key_type const&))
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
    FUNCTION(insert, std_iterator<R>(std_const_iterator<R>, typename R::const_reference))
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

#endif // REW_BUILTIN_MAP_HPP
