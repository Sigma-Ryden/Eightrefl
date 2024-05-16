#ifndef REW_BUILTIN_UNORDERED_SET_HPP
#define REW_BUILTIN_UNORDERED_SET_HPP

#include <unordered_set> // unordered_set, unordered_multiset

#include <Rew/Reflectable.hpp>
#include <Rew/Common.hpp>

// default allocator for unordered_set, unordered_multiset
#include <Rew/BuiltIn/allocator.hpp>

#ifndef REW_CORE_MINIMAL
// as function argument/result type
#include <Rew/BuiltIn/initializer_list.hpp>
#include <Rew/BuiltIn/iterator.hpp>
#include <Rew/BuiltIn/pair.hpp>
#endif // REW_CORE_MINIMAL

// hash - as hasher
// equal_to - as comparator
#include <Rew/BuiltIn/functional.hpp>

TEMPLATE_REFLECTABLE_CLEAN
(
    (template <typename KeyType, typename Hasher, typename Comparator, typename AllocatorType>),
    (std::unordered_set<KeyType, Hasher, Comparator, AllocatorType>),
    std::unordered_set<rew::cleanof<KeyType>, rew::cleanof<Hasher>, rew::cleanof<Comparator>, rew::cleanof<AllocatorType>>
)

TEMPLATE_REFLECTABLE_DECLARATION(template <typename KeyType>, std::unordered_set<KeyType>)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME("std::unordered_set<" + rew::nameof<KeyType>() + ">")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE_DECLARATION
(
    (template <typename KeyType, typename Hasher>),
    std::unordered_set<KeyType, Hasher>
)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME("std::unordered_set<" + rew::nameof<KeyType>() + ", " + rew::nameof<Hasher>() + ">")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE_DECLARATION
(
    (template <typename KeyType, typename Hasher, typename Comparator>),
    std::unordered_set<KeyType, Hasher, Comparator>
)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME
    (
        "std::unordered_set<" + rew::nameof<KeyType>() + ", " + rew::nameof<Comparator>() + ", " + rew::nameof<Hasher>() + ">"
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
        "std::unordered_set<" + rew::nameof<KeyType>() + ", " + rew::nameof<Hasher>() + ", "
                              + rew::nameof<Comparator>() + ", " + rew::nameof<AllocatorType>() + ">"
    )
REFLECTABLE_DECLARATION_INIT()


TEMPLATE_REFLECTABLE_CLEAN
(
    (template <typename KeyType, typename Hasher, typename Comparator, typename AllocatorType>),
    (std::unordered_multiset<KeyType, Hasher, Comparator, AllocatorType>),
    std::unordered_multiset<rew::cleanof<KeyType>, rew::cleanof<Hasher>, rew::cleanof<Comparator>, rew::cleanof<AllocatorType>>
)

TEMPLATE_REFLECTABLE_DECLARATION(template <typename KeyType>, std::unordered_multiset<KeyType>)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME("std::unordered_multiset<" + rew::nameof<KeyType>() + ">")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE_DECLARATION
(
    (template <typename KeyType, typename Hasher>),
    std::unordered_multiset<KeyType, Hasher>
)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME("std::unordered_set<" + rew::nameof<KeyType>() + ", " + rew::nameof<Hasher>() + ">")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE_DECLARATION
(
    (template <typename KeyType, typename Hasher, typename Comparator>),
    std::unordered_multiset<KeyType, Hasher, Comparator>
)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME
    (
        "std::unordered_multiset<" + rew::nameof<KeyType>() + ", " + rew::nameof<Comparator>() + ", " + rew::nameof<Hasher>() + ">"
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
        "std::unordered_multiset<" + rew::nameof<KeyType>() + ", " + rew::nameof<Hasher>() + ", "
                                   + rew::nameof<Comparator>() + ", " + rew::nameof<AllocatorType>() + ">"
    )
REFLECTABLE_DECLARATION_INIT()


template <typename> struct __rew_is_any_std_unordered_set : std::false_type {};

template <typename KeyType, typename Hasher, typename Comparator, typename AllocatorType>
struct __rew_is_any_std_unordered_set<std::unordered_set<KeyType, Hasher, Comparator, AllocatorType>> : std::true_type {};

template <typename KeyType, typename Hasher, typename Comparator, typename AllocatorType>
struct __rew_is_any_std_unordered_set<std::unordered_multiset<KeyType, Hasher, Comparator, AllocatorType>> : std::true_type {};

CONDITIONAL_REFLECTABLE(__rew_is_any_std_unordered_set<R>::value)
    FACTORY(R())

    #ifndef REW_CORE_MINIMAL
    FACTORY(R(typename R::size_type, typename R::hasher const&, typename R::key_equal const&, typename R::allocator_type const&))
    FACTORY(R(typename R::size_type, typename R::hasher const&, typename R::key_equal const&))
    FACTORY(R(typename R::size_type, typename R::hasher const&))
    FACTORY(R(typename R::size_type))
    FACTORY(R(typename R::size_type, typename R::allocator_type const&))
    FACTORY(R(typename R::size_type, typename R::hasher const&, typename R::allocator_type const&))
    FACTORY(R(std_const_iterator<R>, std_const_iterator<R>, typename R::size_type, typename R::hasher const&, typename R::key_equal const&, typename R::allocator_type const&))
    FACTORY(R(std_const_iterator<R>, std_const_iterator<R>, typename R::size_type, typename R::hasher const&, typename R::key_equal const&))
    FACTORY(R(std_const_iterator<R>, std_const_iterator<R>, typename R::size_type, typename R::hasher const&))
    FACTORY(R(std_const_iterator<R>, std_const_iterator<R>, typename R::size_type))
    FACTORY(R(std_const_iterator<R>, std_const_iterator<R>, typename R::size_type, typename R::allocator_type const&))
    FACTORY(R(std_const_iterator<R>, std_const_iterator<R>, typename R::size_type, typename R::hasher const&, typename R::allocator_type const&))
    FACTORY(R(std::initializer_list<typename R::value_type>, typename R::size_type, typename R::hasher const&, typename R::key_equal const&, typename R::allocator_type const&))
    FACTORY(R(std::initializer_list<typename R::value_type>, typename R::size_type, typename R::hasher const&, typename R::key_equal const&))
    FACTORY(R(std::initializer_list<typename R::value_type>, typename R::size_type, typename R::hasher const&))
    FACTORY(R(std::initializer_list<typename R::value_type>, typename R::size_type))
    FACTORY(R(std::initializer_list<typename R::value_type>, typename R::size_type, typename R::hasher const&, typename R::allocator_type const&))
    #endif // REW_CORE_MINIMAL

    FACTORY(R(R const&))
    FUNCTION(operator=, R&(R const&))

    #ifndef REW_CORE_MINIMAL
    FUNCTION(get_allocator)
    FUNCTION(begin, std_const_iterator<R>() const)
    FUNCTION(begin, std_iterator<R>())
    FUNCTION(cbegin, std_const_iterator<R>() const)
    FUNCTION(end, std_const_iterator<R>() const)
    FUNCTION(end, std_iterator<R>())
    FUNCTION(cend, std_const_iterator<R>() const)
    #endif // REW_CORE_MINIMAL

    FUNCTION(empty)
    FUNCTION(size)

    #ifndef REW_CORE_MINIMAL
    FUNCTION(max_size)
    #endif // REW_CORE_MINIMAL

    FUNCTION(clear)

    #ifndef REW_CORE_MINIMAL
    FUNCTION(insert, std::pair<std_iterator<R>, bool>(typename R::const_reference))
    FUNCTION(insert, std_iterator<R>(std_const_iterator<R>, typename R::const_reference))
    FUNCTION(insert, void(std_const_iterator<R>, std_const_iterator<R>))
    FUNCTION(insert, void(std::initializer_list<typename R::value_type>))
    FUNCTION(erase, std_iterator<R>(std_iterator<R>))
    FUNCTION(erase, std_iterator<R>(std_const_iterator<R>))
    FUNCTION(erase, std_iterator<R>(std_const_iterator<R>, std_const_iterator<R>))
    FUNCTION(swap)
//  FUNCTION(extract, typename R::node_type(std_const_iterator<R>))
//  FUNCTION(extract, typename R::node_type(typename R::key_type const&))
    #endif // REW_CORE_MINIMAL

    FUNCTION(merge, void(R&))
    FUNCTION(count, typename R::size_type(typename R::key_type const&) const)

    #ifndef REW_CORE_MINIMAL
    FUNCTION(find, std_iterator<R>(typename R::key_type const&))
    FUNCTION(find, std_const_iterator<R>(typename R::key_type const&) const)
    #endif // REW_CORE_MINIMAL

#if __cplusplus >= 202002L
    FUNCTION(contains, bool(typename R::key_type const&) const)
#endif // if

    #ifndef REW_CORE_MINIMAL
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
    #endif // REW_CORE_MINIMAL

    FUNCTION(rehash)
    FUNCTION(reserve)

    #ifndef REW_CORE_MINIMAL
    FUNCTION(hash_function)
    FUNCTION(key_eq)
    #endif // REW_CORE_MINIMAL
REFLECTABLE_INIT()

#endif // REW_BUILTIN_UNORDERED_SET_HPP
