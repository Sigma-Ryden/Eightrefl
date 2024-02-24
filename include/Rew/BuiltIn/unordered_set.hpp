#ifndef REW_BUILTIN_UNORDERED_SET_HPP
#define REW_BUILTIN_UNORDERED_SET_HPP

#include <unordered_set> // unordered_set, unordered_multiset

#include <Rew/Reflectable.hpp>
#include <Rew/Common.hpp>

// default allocator for unordered_set, unordered_multiset
#include <Rew/BuiltIn/allocator.hpp>

// as function argument type
#include <Rew/BuiltIn/initializer_list.hpp>
#include <Rew/BuiltIn/iterator.hpp>

TEMPLATE_REFLECTABLE_CLEAN
(
    (template <typename KeyType, typename Hasher, typename Comparator, typename AllocatorType>),
    (std::unordered_set<KeyType, Hasher, Comparator, AllocatorType>),
    std::unordered_set<CLEANOF(KeyType), CLEANOF(Hasher), CLEANOF(Comparator), CLEANOF(AllocatorType)>
)

TEMPLATE_REFLECTABLE_DECLARATION((template <typename KeyType>), std::unordered_set<KeyType>)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME("std::unordered_set<" + NAMEOF(KeyType) + ">")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE_DECLARATION
(
    (template <typename KeyType, typename Hasher>),
    std::unordered_set<KeyType, Hasher>
)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME("std::unordered_set<" + NAMEOF(KeyType) + ", " + NAMEOF(Hasher) + ">")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE_DECLARATION
(
    (template <typename KeyType, typename Hasher, typename Comparator>),
    std::unordered_set<KeyType, Hasher, Comparator>
)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME
    (
        "std::unordered_set<" + NAMEOF(KeyType) + ", " + NAMEOF(Comparator) + ", " + NAMEOF(Hasher) + ">"
    )
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE_DECLARATION
(
    (template <typename KeyType, typename Hasher, typename Comparator, typename AllocatorType>),
    std::unordered_set<KeyType, Hasher, Comparator, AllocatorType>
)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME
    (
        "std::unordered_set<" + NAMEOF(KeyType) + ", " + NAMEOF(Hasher) + ", "
                              + NAMEOF(Comparator) + ", " + NAMEOF(AllocatorType) + ">"
    )
REFLECTABLE_DECLARATION_INIT()


TEMPLATE_REFLECTABLE_CLEAN
(
    (template <typename KeyType, typename Hasher, typename Comparator, typename AllocatorType>),
    (std::unordered_multiset<KeyType, Hasher, Comparator, AllocatorType>),
    std::unordered_multiset<CLEANOF(KeyType), CLEANOF(Hasher), CLEANOF(Comparator), CLEANOF(AllocatorType)>
)

TEMPLATE_REFLECTABLE_DECLARATION((template <typename KeyType>), std::unordered_multiset<KeyType>)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME("std::unordered_multiset<" + NAMEOF(KeyType) + ">")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE_DECLARATION
(
    (template <typename KeyType, typename Hasher>),
    std::unordered_multiset<KeyType, Hasher>
)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME("std::unordered_set<" + NAMEOF(KeyType) + ", " + NAMEOF(Hasher) + ">")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE_DECLARATION
(
    (template <typename KeyType, typename Hasher, typename Comparator>),
    std::unordered_multiset<KeyType, Hasher, Comparator>
)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME
    (
        "std::unordered_multiset<" + NAMEOF(KeyType) + ", " + NAMEOF(Comparator) + ", " + NAMEOF(Hasher) + ">"
    )
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE_DECLARATION
(
    (template <typename KeyType, typename Hasher, typename Comparator, typename AllocatorType>),
    std::unordered_multiset<KeyType, Hasher, Comparator, AllocatorType>
)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME
    (
        "std::unordered_multiset<" + NAMEOF(KeyType) + ", " + NAMEOF(Hasher) + ", "
                                   + NAMEOF(Comparator) + ", " + NAMEOF(AllocatorType) + ">"
    )
REFLECTABLE_DECLARATION_INIT()


template <typename> struct __rew_is_any_std_unordered_set : std::false_type {};

template <typename KeyType, typename Hasher, typename Comparator, typename AllocatorType>
struct __rew_is_any_std_unordered_set<std::unordered_set<KeyType, Hasher, Comparator, AllocatorType>> : std::true_type {};

template <typename KeyType, typename Hasher, typename Comparator, typename AllocatorType>
struct __rew_is_any_std_unordered_set<std::unordered_multiset<KeyType, Hasher, Comparator, AllocatorType>> : std::true_type {};

CONDITIONAL_REFLECTABLE(__rew_is_any_std_unordered_set<CLEANOF(DirtyR)>::value)
    FACTORY(R())
    FACTORY(R(typename R::size_type, typename R::hasher const&, typename R::key_equal const&, typename R::allocator const&))
    FACTORY(R(typename R::size_type, typename R::hasher const&, typename R::key_equal const&))
    FACTORY(R(typename R::size_type, typename R::hasher const&))
    FACTORY(R(typename R::size_type))
    FACTORY(R(typename R::size_type, typename R::allocator const&))
    FACTORY(R(typename R::size_type, typename R::hasher const&, typename R::allocator const&))
    FACTORY(R(std_const_iterator<R>, std_const_iterator<R>, typename R::size_type, typename R::hasher const&, typename R::key_equal const&, typename R::allocator const&))
    FACTORY(R(std_const_iterator<R>, std_const_iterator<R>, typename R::size_type, typename R::hasher const&, typename R::key_equal const&))
    FACTORY(R(std_const_iterator<R>, std_const_iterator<R>, typename R::size_type, typename R::hasher const&))
    FACTORY(R(std_const_iterator<R>, std_const_iterator<R>, typename R::size_type))
    FACTORY(R(std_const_iterator<R>, std_const_iterator<R>, typename R::size_type))
    FACTORY(R(std_const_iterator<R>, std_const_iterator<R>, typename R::size_type, typename R::allocator const&))
    FACTORY(R(std_const_iterator<R>, std_const_iterator<R>, typename R::size_type, typename R::hasher const&, typename R::allocator const&))
    FACTORY(R(std::initializer_list<typename R::value_type>, typename R::size_type, typename R::hasher const&, typename R::key_equal const&, typename R::allocator const&))
    FACTORY(R(std::initializer_list<typename R::value_type>, typename R::size_type, typename R::hasher const&, typename R::key_equal const&))
    FACTORY(R(std::initializer_list<typename R::value_type>, typename R::size_type, typename R::hasher const&))
    FACTORY(R(std::initializer_list<typename R::value_type>, typename R::size_type))
    FACTORY(R(std::initializer_list<typename R::value_type>, typename R::size_type, typename R::hasher const&, typename R::allocator const&))
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
    FUNCTION(begin, std_const_local_iterator<R>(typename R::size_type) const)
    FUNCTION(begin, std_local_iterator<R>(typename R::size_type))
    FUNCTION(cbegin, std_const_local_iterator<R>(typename R::size_type) const)
    FUNCTION(end, std_const_local_iterator<R>(typename R::size_type) const)
    FUNCTION(end, std_local_iterator<R>(typename R::size_type))
    FUNCTION(cend, std_const_local_iterator<R>(typename R::size_type) const)
    FUNCTION(bucket_count)
    FUNCTION(max_bucket_count)
    FUNCTION(bucket_size)
    FUNCTION(bucket, typename R::size_type(typename R::key_type const&) const)
    FUNCTION(load_factor)
    FUNCTION(max_load_factor, float() const)
    FUNCTION(max_load_factor, void(float))
    FUNCTION(rehash)
    FUNCTION(reserve)
//  FUNCTION(hash_function)
//  FUNCTION(key_eq)
REFLECTABLE_INIT()

#endif // REW_BUILTIN_UNORDERED_SET_HPP
