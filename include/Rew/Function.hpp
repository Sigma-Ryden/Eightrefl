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

#define CORE_FUNCTION(function_name_str, function_name, ...)                                            \
    {                                                                                                   \
        using __traits = ::rew::meta::member_function_traits<R>;                                        \
        auto __dirty_ptr = __traits::template overload<__VA_ARGS__>::of(&R::function_name);             \
        auto __ptr = ::rew::utility::member_function_ptr<R>(__dirty_ptr);                               \
        auto __meta = ::rew::find_or_add_function(__reflection, function_name_str, __ptr);              \
        injection.template function<R, decltype(__ptr)>(*__meta);                                       \
    }

#define FUNCTION(name, ...) CORE_FUNCTION(#name, name, __VA_ARGS__)

namespace rew
{

struct type_t;

struct function_t
{
    const std::string name;
    const std::function<std::any(std::any& context, const std::vector<std::any>& args)> call = nullptr;
    const std::vector<type_t*> arguments;
    type_t *const result = nullptr;
    attribute_t<std::any> meta;
};

namespace detail
{

template <typename ReflectableType, typename ReturnType, typename... ArgumentTypes,
          typename FunctionType, std::size_t... I>
auto member_function_call_impl(FunctionType function, std::index_sequence<I...>)
{
    return [function](std::any& context, const std::vector<std::any>& arguments) -> std::any
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
auto static_function_call_impl(ReturnType (*function)(ArgumentTypes...), std::index_sequence<I...>)
{
    return [function](std::any& context, const std::vector<std::any>& arguments) -> std::any
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

namespace handler
{

template <typename ReflectableType, typename ReturnType, typename... ArgumentTypes>
auto function_call(ReturnType (ReflectableType::* function)(ArgumentTypes...) const)
{
    return detail::member_function_call_impl<ReflectableType, ReturnType, ArgumentTypes...>
    (
        function, std::index_sequence_for<ArgumentTypes...>{}
    );
}

template <typename ReflectableType, typename ReturnType, typename... ArgumentTypes>
auto function_call(ReturnType (ReflectableType::* function)(ArgumentTypes...))
{
    return detail::member_function_call_impl<ReflectableType, ReturnType, ArgumentTypes...>
    (
        function, std::index_sequence_for<ArgumentTypes...>{}
    );
}

template <typename ReturnType, typename... ArgumentTypes>
auto function_call(ReturnType (*function)(ArgumentTypes...))
{
    return detail::static_function_call_impl(function, std::index_sequence_for<ArgumentTypes...>{});
}

} // namespace handler

} // namespace rew

#endif // REW_FUNCTION_HPP
