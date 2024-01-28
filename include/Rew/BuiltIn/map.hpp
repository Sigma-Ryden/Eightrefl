#ifndef REW_BUILTIN_MAP_HPP
#define REW_BUILTIN_MAP_HPP

#include <map> // map, multimap

#include <Rew/Reflectable.hpp>
#include <Rew/Common.hpp>

// default allocator for map, multimap
#include <Rew/BuiltIn/allocator.hpp>

// as function argument type
#include <Rew/BuiltIn/initializer_list.hpp>

// as mapped type
#include <Rew/BuiltIn/pair.hpp>

namespace rew
{

namespace detail
{

template <typename> struct is_std_map : std::false_type {};
template <typename KeyType, typename ValueType, typename Comparator, typename AllocatorType>
struct is_std_map<std::map<KeyType, ValueType, Comparator, AllocatorType>> : std::true_type {};

template <typename> struct is_std_multimap : std::false_type {};
template <typename KeyType, typename ValueType, typename Comparator, typename AllocatorType>
struct is_std_map<std::multimap<KeyType, ValueType, Comparator, AllocatorType>> : std::true_type {};

} // namespace detail

} // namespace rew

CONDITIONAL_REFLECTABLE_DECLARATION(rew::detail::is_std_map<R>::value)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME
    (
        "std::map<" + NAMEOF(typename R::key_type) + ", " + NAMEOF(typename R::mapped_type) + ", "
                    + NAMEOF(typename R::key_compare) + ", " + NAMEOF(typename R::allocator_type) + ">"
    )
REFLECTABLE_DECLARATION_INIT()

CONDITIONAL_REFLECTABLE_DECLARATION(rew::detail::is_std_multimap<R>::value)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME
    (
        "std::multimap<" + NAMEOF(typename R::key_type) + ", " + NAMEOF(typename R::mapped_type) + ", "
                         + NAMEOF(typename R::key_compare) + ", " + NAMEOF(typename R::allocator_type) + ">"
    )
REFLECTABLE_DECLARATION_INIT()

CONDITIONAL_REFLECTABLE(rew::meta::one<rew::detail::is_std_map<R>, rew::detail::is_std_multimap<R>>::value)
    FACTORY(R())
    FACTORY(R(typename R::key_compare const&, typename R::allocator_type const&))
    FACTORY(R(typename R::key_compare const&))
    FACTORY(R(typename R::allocator_type const&))
    FACTORY(R(typename R::const_iterator, typename R::const_iterator, typename R::key_compare const&, typename R::allocator_type const&))
    FACTORY(R(typename R::const_iterator, typename R::const_iterator, typename R::allocator_type const&))
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
    FUNCTION(clear)
    FUNCTION(insert, std::pair<typename R::iterator, bool>(typename R::const_reference))
    FUNCTION(insert, typename R::iterator(typename R::iterator, typename R::const_reference))
    FUNCTION(insert, typename R::iterator(typename R::const_iterator, typename R::const_iterator))
    FUNCTION(insert, void(std::initializer_list<typename R::value_type>))
    FUNCTION(erase, typename R::iterator(typename R::iterator))
    FUNCTION(erase, typename R::iterator(typename R::const_iterator))
    FUNCTION(erase, typename R::iterator(typename R::const_iterator, typename R::const_iterator))
    FUNCTION(swap)
    FUNCTION(extract, typename R::node_type(typename R::const_iterator))
    FUNCTION(extract, typename R::node_type(typename R::key_type const&))
    FUNCTION(merge, void(R&))
    FUNCTION(count, typename R::size_type(typename R::key_type const&))
    FUNCTION(find, typename R::iterator(typename R::key_type const&))
    FUNCTION(find, typename R::const_iterator(typename R::key_type const&) const)
#if __cplusplus >= 202002L
    FUNCTION(contains, bool(typename R::key_type const&) const)
#endif // if
    FUNCTION(equal_range, std::pair<typename R::iterator, typename R::iterator>(typename R::key_type const&))
    FUNCTION(equal_range, std::pair<typename R::const_iterator, typename R::const_iterator>(typename R::key_type const&) const)
    FUNCTION(lower_bound, typename R::iterator(typename R::key_type const&))
    FUNCTION(lower_bound, typename R::const_iterator(typename R::key_type const&) const)
    FUNCTION(upper_bound, typename R::iterator(typename R::key_type const&))
    FUNCTION(upper_bound, typename R::const_iterator(typename R::key_type const&) const)
    FUNCTION(key_comp)
//  FUNCTION(value_comp)
REFLECTABLE_INIT()

#endif // REW_BUILTIN_MAP_HPP
