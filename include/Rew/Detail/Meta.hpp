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

template <typename... Bn> using all = std::conjunction<Bn...>;
template <typename... Bn> using one = std::disjunction<Bn...>;

struct base_reflectable_traits {};
struct conditional_reflectable_traits : base_reflectable_traits {};

template <typename T, typename enable = void>
struct reflectable_traits
{
    static std::string name() { return "auto"; }
};

template <> struct reflectable_traits<void> : base_reflectable_traits
{
    static std::string name() { return "void"; }
};

template <> struct reflectable_traits<std::nullptr_t> : base_reflectable_traits
{
    static std::string name() { return "std::nullptr_t"; }
};

template <typename T> struct is_reflectable : std::is_base_of<base_reflectable_traits, T> {};

template <typename T> struct is_builtin_reflectable
    : all<is_reflectable<T>,
          one<std::is_arithmetic<T>, std::is_pointer<T>, std::is_enum<T>, std::is_array<T>>> {};

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
struct function_traits;

template <typename ReturnType, typename... ArgumentTypes>
struct function_traits<ReturnType(ArgumentTypes...)>
{
    using return_type = ReturnType;
    using function_type = ReturnType(*)(ArgumentTypes...);
};

template <typename ReturnType, typename... ArgumentTypes>
struct function_traits<ReturnType(*)(ArgumentTypes...)>
{
    using return_type = ReturnType;
    using function_type = ReturnType(*)(ArgumentTypes...);
};

template <class ClassType>
struct member_function_traits
{
    template <typename...>
    struct overload
    {
        template <typename ReturnType, typename... ArgumentTypes>
        static constexpr auto of(ReturnType (ClassType::* function)(ArgumentTypes...) const) { return function; }

        template <typename ReturnType, typename... ArgumentTypes>
        static constexpr auto of(ReturnType (ClassType::* function)(ArgumentTypes...)) { return function; }

        template <class ParentClassType, typename ReturnType, typename... ArgumentTypes>
        static constexpr auto of(ReturnType (ParentClassType::* function)(ArgumentTypes...) const) { return function; }

        template <class ParentClassType, typename ReturnType, typename... ArgumentTypes>
        static constexpr auto of(ReturnType (ParentClassType::* function)(ArgumentTypes...)) { return function; }

        template <typename ReturnType, typename... ArgumentTypes>
        static constexpr auto of(ReturnType (*function)(ArgumentTypes...)) { return function; }
    };

    template <typename ReturnType, typename... ArgumentTypes>
    struct overload<ReturnType(ArgumentTypes...) const>
    {
        static constexpr auto of(ReturnType (ClassType::* function)(ArgumentTypes...) const) { return function; }

        template <class ParentClassType>
        static constexpr auto of(ReturnType (ParentClassType::* function)(ArgumentTypes...) const) { return function; }

        static constexpr auto of(ReturnType (*function)(ArgumentTypes...)) { return function; }
    };

    template <typename ReturnType, typename... ArgumentTypes>
    struct overload<ReturnType(ArgumentTypes...)>
    {
        static constexpr auto of(ReturnType (ClassType::* function)(ArgumentTypes...)) { return function; }

        template <class ParentClassType>
        static constexpr auto of(ReturnType (ParentClassType::* function)(ArgumentTypes...)) { return function; }

        static constexpr auto of(ReturnType (*function)(ArgumentTypes...)) { return function; }
    };
};

} // namespace meta

} // namespace rew

#endif // REW_DETAIL_META_HPP
