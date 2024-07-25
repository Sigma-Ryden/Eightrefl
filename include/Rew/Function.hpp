#ifndef REW_FUNCTION_HPP
#define REW_FUNCTION_HPP

#include <cstddef> // size_t

#include <string> // string
#include <vector> // vector
#include <any> // any

#include <functional> // function

#include <Rew/Attribute.hpp>
#include <Rew/Meta.hpp>

#include <Rew/Utility.hpp>

#include <Rew/Detail/Macro.hpp>

// .function<R, signature>(name, &R::func)
#define NAMED_FUNCTION(name_str, name, ...)                                                             \
    {                                                                                                   \
        using xxaccess = typename rew::meta::access_traits<CleanR>::template function<__VA_ARGS__>;     \
        auto xxptr = xxaccess::of(&CleanR::REW_DEPAREN(name));                                          \
        auto xxmeta = rew::find_or_add_function<__VA_ARGS__>(xxreflection, name_str, xxptr);            \
        injection.template function<CleanR, decltype(xxptr)>(*xxmeta);                                  \
    }

#define FUNCTION(name, ...) NAMED_FUNCTION(REW_TO_STRING(name), name, __VA_ARGS__)

// .function<signature>(name, &func)
#define NAMED_FREE_FUNCTION(name_str, name, ...)                                                        \
    {                                                                                                   \
        using xxaccess = typename rew::meta::access_traits<>::template function<__VA_ARGS__>;           \
        auto xxptr = xxaccess::of(&REW_DEPAREN(name));                                                  \
        auto xxmeta = rew::find_or_add_function<__VA_ARGS__>(xxreflection, name_str, xxptr);            \
        injection.template function<CleanR, decltype(xxptr)>(*xxmeta);                                  \
    }

#define FREE_FUNCTION(name, ...) NAMED_FREE_FUNCTION(REW_TO_STRING(name), name, __VA_ARGS__)

namespace rew
{

struct type_t;

struct function_t
{
    std::string const name;
    std::function<std::any(std::any const& context, std::vector<std::any> const& args)> const call = nullptr;
    std::vector<type_t*> const arguments;
    type_t *const result = nullptr;
    std::any const pointer;
    attribute_t<std::any> meta;
};

namespace detail
{

template <typename ReflectableType, typename ReturnType, typename... ArgumentTypes,
          typename FunctionType, std::size_t... I>
auto handler_member_function_call_impl(FunctionType function, std::index_sequence<I...>)
{
    return [function](std::any const& context, std::vector<std::any> const& arguments) -> std::any
    {
        auto reflectable = std::any_cast<ReflectableType*>(context);
        if constexpr (std::is_void_v<ReturnType>)
        {
            (reflectable->*function)(utility::forward<ArgumentTypes>(arguments[I])...);
            return {};
        }
        else
        {
            return utility::backward
            (
                (reflectable->*function)(utility::forward<ArgumentTypes>(arguments[I])...)
            );
        }
    };
}

template <typename ReturnType, typename... ArgumentTypes, std::size_t... I>
auto handler_free_function_call_impl(ReturnType (*function)(ArgumentTypes...), std::index_sequence<I...>)
{
    return [function](std::any const&, std::vector<std::any> const& arguments) -> std::any
    {
        if constexpr (std::is_void_v<ReturnType>)
        {
            function(utility::forward<ArgumentTypes>(arguments[I])...);
            return {};
        }
        else
        {
            return utility::backward
            (
                function(utility::forward<ArgumentTypes>(arguments[I])...)
            );
        }
    };
}

} // namespace detail

template <typename ReflectableType, typename ReturnType, typename... ArgumentTypes>
auto handler_function_call(ReturnType (ReflectableType::* function)(ArgumentTypes...) const)
{
    return detail::handler_member_function_call_impl<ReflectableType, ReturnType, ArgumentTypes...>
    (
        function, std::index_sequence_for<ArgumentTypes...>{}
    );
}

template <typename ReflectableType, typename ReturnType, typename... ArgumentTypes>
auto handler_function_call(ReturnType (ReflectableType::* function)(ArgumentTypes...) const&)
{
    return detail::handler_member_function_call_impl<ReflectableType, ReturnType, ArgumentTypes...>
    (
        function, std::index_sequence_for<ArgumentTypes...>{}
    );
}

template <typename ReflectableType, typename ReturnType, typename... ArgumentTypes>
auto handler_function_call(ReturnType (ReflectableType::* function)(ArgumentTypes...))
{
    return detail::handler_member_function_call_impl<ReflectableType, ReturnType, ArgumentTypes...>
    (
        function, std::index_sequence_for<ArgumentTypes...>{}
    );
}

template <typename ReflectableType, typename ReturnType, typename... ArgumentTypes>
auto handler_function_call(ReturnType (ReflectableType::* function)(ArgumentTypes...)&)
{
    return detail::handler_member_function_call_impl<ReflectableType, ReturnType, ArgumentTypes...>
    (
        function, std::index_sequence_for<ArgumentTypes...>{}
    );
}

template <typename ReturnType, typename... ArgumentTypes>
auto handler_function_call(ReturnType (*function)(ArgumentTypes...))
{
    return detail::handler_free_function_call_impl(function, std::index_sequence_for<ArgumentTypes...>{});
}

} // namespace rew

#endif // REW_FUNCTION_HPP
