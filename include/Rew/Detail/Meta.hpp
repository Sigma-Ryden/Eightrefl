#ifndef REW_DETAIL_META_HPP
#define REW_DETAIL_META_HPP

#include <cstddef> // size_t

#include <string> // string
#include <any> // any, any_cast

#include <utility> // reference_wrapper

#include <type_traits> // decay_t, enable_if_t, is_pointer_v, void_t, false_type, true_type

template <typename ReflectableType, typename enable = void>
struct rew_reflection_registry_t;

namespace rew
{

namespace meta
{

template <typename T, typename enable = void>
struct is_complete : std::false_type {};

template <typename T>
struct is_complete<T, std::void_t<decltype(sizeof(T))>> : std::true_type {};

template <typename... Bn> using all = std::conjunction<Bn...>;
template <typename... Bn> using one = std::disjunction<Bn...>;

template <typename T, typename enable = void>
struct reflectable_name_t
{
    static std::string get() { return "auto"; }
};

template <> struct reflectable_name_t<void>
{
    static std::string get() { return "void"; }
};

template <> struct reflectable_name_t<std::nullptr_t>
{
    static std::string get() { return "std::nullptr_t"; }
};

template <typename T, typename enable = void>
struct reflectable_traits_t;

template <typename T> struct is_reflectable : is_complete<reflectable_traits_t<T>> {};

template <typename T> struct is_builtin_reflectable
    : one<std::is_arithmetic<T>,
          std::is_pointer<T>,
          std::is_enum<T>,
          std::is_array<T>> {};

template <typename T>
struct is_custom_reflectable : all<is_reflectable<T>, std::negation<is_builtin_reflectable<T>>> {};

template <typename ArgumentType>
struct argument_type_traits
{
    using type = ArgumentType;
};

template <typename ArgumentType>
struct argument_type_traits<ArgumentType&>
{
    using type = std::reference_wrapper<ArgumentType>;
};

template <typename ArgumentType>
struct argument_type_traits<const ArgumentType> : argument_type_traits<ArgumentType> {};

template <typename ArgumentType>
struct argument_type_traits<const ArgumentType&> : argument_type_traits<ArgumentType&> {};

template <typename...>
struct function_type_traits;

template <typename ReturnType, typename... ArgumentTypes>
struct function_type_traits<ReturnType, ArgumentTypes...>
{
    using return_type = ReturnType;
    using function_type = ReturnType(*)(ArgumentTypes...);
};

template <typename ReturnType>
struct function_type_traits<ReturnType, void>
{
    using return_type = ReturnType;
    using function_type = ReturnType(*)(void);
};

template <typename ReturnType, typename... ArgumentTypes>
struct function_type_traits<ReturnType(ArgumentTypes...)>
{
    using return_type = ReturnType;
    using function_type = ReturnType(*)(ArgumentTypes...);
};

template <typename ReturnType>
struct function_type_traits<ReturnType(void)>
{
    using return_type = ReturnType;
    using function_type = ReturnType(*)(void);
};

} // namespace meta

} // namespace rew

#endif // REW_DETAIL_META_HPP
