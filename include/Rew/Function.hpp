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

#define CORE_FUNCTION(function_call_handler, name, ...)                                                 \
    {                                                                                                   \
        auto __ptr = ::rew::utility::overload<__VA_ARGS__>::of(&reflectable::name);                     \
        auto __name = ::rew::utility::full_function_name(#name, __ptr);                                 \
        auto __meta = reflection->function.find(__name);                                                \
        if (__meta == nullptr) __meta = &reflection->function.add(                                      \
            __name,                                                                                     \
            {                                                                                           \
                __name,                                                                                 \
                function_call_handler<reflectable>(__ptr),                                              \
                ::rew::utility::function_arg_count(__ptr)                                               \
            }                                                                                           \
        );                                                                                              \
        visitor.template function<reflectable, decltype(__ptr)>(*__meta);                               \
    }

#define FUNCTION(name, ...)                                                                             \
    CORE_FUNCTION(::rew::handler::function_call, name, __VA_ARGS__)

namespace rew
{

struct function_t
{
    const std::string name;
    const std::function<std::any(std::any& context, const std::vector<std::any>& args)> call = nullptr;
    const std::size_t arg_count = 0;
    attribute_t<std::any> meta;
};

namespace detail
{

template <typename ReflectableType, typename... ArgumentTypes, typename FunctionType, std::size_t... I>
auto function_call_void_impl(FunctionType function, std::index_sequence<I...>)
{
    return [function](std::any& context, const std::vector<std::any>& arguments) -> std::any
    {
        (std::any_cast<ReflectableType*>(context)->*function)(utility::argument_cast<ArgumentTypes>(arguments[I])...);
        return {};
    };
}

template <typename ReflectableType, typename... ArgumentTypes, typename FunctionType, std::size_t... I>
auto function_call_return_impl(FunctionType function, std::index_sequence<I...>)
{
    return [function](std::any& context, const std::vector<std::any>& arguments) -> std::any
    {
        return (std::any_cast<ReflectableType*>(context)->*function)(utility::argument_cast<ArgumentTypes>(arguments[I])...);
    };
}

template <typename... ArgumentTypes, std::size_t... I>
auto function_call_void_impl(void (*function)(ArgumentTypes...), std::index_sequence<I...>)
{
    return [function](std::any& context, const std::vector<std::any>& arguments) -> std::any
    {
        function(utility::argument_cast<ArgumentTypes>(arguments[I])...);
        return {};
    };
}

template <typename ReturnType, typename... ArgumentTypes, std::size_t... I>
auto function_call_return_impl(ReturnType (*function)(ArgumentTypes...), std::index_sequence<I...>)
{
    return [function](std::any& context, const std::vector<std::any>& arguments) -> std::any
    {
        return function(utility::argument_cast<ArgumentTypes>(arguments[I])...);
    };
}

} // namespace detail

namespace handler
{

template <typename ReflectableType, typename ParentReflectableType, typename... ArgumentTypes>
auto function_call(void (ParentReflectableType::* function)(ArgumentTypes...) const)
{
    return detail::function_call_void_impl<ReflectableType, ArgumentTypes...>(function, std::index_sequence_for<ArgumentTypes...>{});
}

template <typename ReflectableType, typename ParentReflectableType, typename... ArgumentTypes>
auto function_call(void (ParentReflectableType::* function)(ArgumentTypes...))
{
    return detail::function_call_void_impl<ReflectableType, ArgumentTypes...>(function, std::index_sequence_for<ArgumentTypes...>{});
}

template <typename ReflectableType, typename ParentReflectableType, typename ReturnType, typename... ArgumentTypes>
auto function_call(ReturnType (ParentReflectableType::* function)(ArgumentTypes...) const)
{
    return detail::function_call_return_impl<ReflectableType, ArgumentTypes...>(function, std::index_sequence_for<ArgumentTypes...>{});
}

template <typename ReflectableType, typename ParentReflectableType, typename ReturnType, typename... ArgumentTypes>
auto function_call(ReturnType (ParentReflectableType::* function)(ArgumentTypes...))
{
    return detail::function_call_return_impl<ReflectableType, ArgumentTypes...>(function, std::index_sequence_for<ArgumentTypes...>{});
}
template <typename... ArgumentTypes>
auto function_call(void (*function)(ArgumentTypes...))
{
    return detail::function_call_void_impl(function, std::index_sequence_for<ArgumentTypes...>{});
}

template <typename ReturnType, typename... ArgumentTypes>
auto function_call(ReturnType (*function)(ArgumentTypes...))
{
    return detail::function_call_return_impl(function, std::index_sequence_for<ArgumentTypes...>{});
}

} // namespace handler

} // namespace rew

#endif // REW_FUNCTION_HPP
