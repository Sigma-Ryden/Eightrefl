#ifndef REW_BUILT_IN_HPP
#define REW_BUILT_IN_HPP

#include <cstddef> // size_t

#include <type_traits> // true_type, false_type, enable_if_t, is_arithmetic_v, is_pointer_v, is_enum_v, is_array_v

#include <memory> // unique_otr, weak_ptr, shared_ptr
#include <string> // basic_string
#include <string_view> // basic_string_view
#include <array> // array
#include <vector> // vector
#include <list> // list
#include <forward_list> // forward_list
#include <deque> // deque
#include <map> // map, multimap
#include <unordered_map> // unordered_map, unordered_multimap
#include <set> // set, multiset
#include <unordered_set> // unordered_set, unordered_multiset
#include <stack> // stack
#include <queue> // queue, priority_queue
#include <valarray> // valarray
#include <bitset> // bitset
#include <utility> // pair
#include <tuple> // tuple
#include <any> // any
#include <variant> // variant
#include <optional> // optional

#include <Rew/Utility.hpp>

namespace rew
{

namespace meta
{

template <typename T> struct reflection_info_t;
template <typename T> struct is_reflectable : is_complete<reflection_info_t<T>> {};

template <typename T> struct is_builtin_reflectable
    : one<std::is_arithmetic<T>,
          std::is_pointer<T>,
          std::is_enum<T>,
          std::is_array<T>> {};

template <typename T>
struct is_custom_reflectable : all<is_reflectable<T>, std::negation<is_builtin_reflectable<T>>> {};

template <typename T, typename Deleter>
struct is_builtin_reflectable<std::unique_ptr<T, Deleter>> : std::true_type {};

template <typename T> struct is_builtin_reflectable<std::weak_ptr<T>> : std::true_type {};
template <typename T> struct is_builtin_reflectable<std::shared_ptr<T>> : std::true_type {};

template <typename T> struct is_builtin_reflectable<std::reference_wrapper<T>> : std::true_type {};

template <typename Char, typename Traits, typename Allocator>
struct is_builtin_reflectable<std::basic_string<Char, Traits, Allocator>> : std::true_type {};

template <typename Char, typename Traits>
struct is_builtin_reflectable<std::basic_string_view<Char, Traits>> : std::true_type {};

template <typename T, std::size_t N>
struct is_builtin_reflectable<std::array<T, N>> : std::true_type {};

template <typename T, typename Allocator>
struct is_builtin_reflectable<std::vector<T, Allocator>> : std::true_type {};

template <typename T, typename Allocator>
struct is_builtin_reflectable<std::list<T, Allocator>> : std::true_type {};

template <typename T, typename Allocator>
struct is_builtin_reflectable<std::forward_list<T, Allocator>> : std::true_type {};

template <typename T, typename Allocator>
struct is_builtin_reflectable<std::deque<T, Allocator>> : std::true_type {};

template <typename Key, typename Value, typename Comparator, typename Allocator>
struct is_builtin_reflectable<std::map<Key, Value, Comparator, Allocator>> : std::true_type {};

template <typename Key, typename Value, typename Comparator, typename Allocator>
struct is_builtin_reflectable<std::multimap<Key, Value, Comparator, Allocator>> : std::true_type {};

template <typename Key, typename Value, typename Hash, typename Predicator, typename Allocator>
struct is_builtin_reflectable<std::unordered_map<Key, Value, Hash, Predicator, Allocator>> : std::true_type {};

template <typename Key, typename Value, typename Hash, typename Predicator, typename Allocator>
struct is_builtin_reflectable<std::unordered_multimap<Key, Value, Hash, Predicator, Allocator>> : std::true_type {};

template <typename Key, typename Comparator, typename Allocator>
struct is_builtin_reflectable<std::set<Key, Comparator, Allocator>> : std::true_type {};

template <typename Key, typename Comparator, typename Allocator>
struct is_builtin_reflectable<std::multiset<Key, Comparator, Allocator>> : std::true_type {};

template <typename Key, typename Hash, typename Predicator, typename Allocator>
struct is_builtin_reflectable<std::unordered_set<Key, Hash, Predicator, Allocator>> : std::true_type {};

template <typename Key, typename Hash, typename Predicator, typename Allocator>
struct is_builtin_reflectable<std::unordered_multiset<Key, Hash, Predicator, Allocator>> : std::true_type {};

template <typename T, typename Container>
struct is_builtin_reflectable<std::stack<T, Container>> : std::true_type {};

template <typename T, typename Container>
struct is_builtin_reflectable<std::queue<T, Container>> : std::true_type {};

template <typename T, typename Container, typename Comparator>
struct is_builtin_reflectable<std::priority_queue<T, Container, Comparator>> : std::true_type {};

template <typename T>
struct is_builtin_reflectable<std::valarray<T>> : std::true_type {};

template <std::size_t N>
struct is_builtin_reflectable<std::bitset<N>> : std::true_type {};

template <typename T, typename U>
struct is_builtin_reflectable<std::pair<T, U>> : std::true_type {};

template <typename... Tn>
struct is_builtin_reflectable<std::tuple<Tn...>> : std::true_type {};

template <>
struct is_builtin_reflectable<std::any> : std::true_type {};

template <typename... Tn>
struct is_builtin_reflectable<std::variant<Tn...>> : std::true_type {};

template <typename T>
struct is_builtin_reflectable<std::optional<T>> : std::true_type {};

} // namespace meta

} // namespace rew

#endif // REW_BUILT_IN_HPP
