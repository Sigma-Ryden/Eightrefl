#ifndef REW_FUNCTION_HPP
#define REW_FUNCTION_HPP

#include <cstddef> // size_t

#include <string> // string
#include <any> // any

#include <functional> // function
#include <initializer_list> // initializer_list

#include <Rew/Attribute.hpp>
#include <Rew/Meta.hpp>

#include <Rew/Utility.hpp>

// TODO: add cv func support
#define FUNCTION(name, ...)                                                                             \
    visitor.template function<info_t, decltype(function_overload<__VA_ARGS__>(&info_t::type::name))>({  \
        assembly(#name, #__VA_ARGS__),                                                                  \
        function_call_handler(function_overload<__VA_ARGS__>(&info_t::type::name)),                     \
        function_args_count(function_overload<__VA_ARGS__>(&info_t::type::name))                        \
    });

namespace rew
{

struct function_meta_t
{
    const std::string name;
    const std::function<void(std::any&, std::any&, std::initializer_list<std::any>)> call = nullptr;
    const std::size_t args_count = 0;
    meta_t meta;
};

using function_t = attribute_t<function_meta_t>;

template <typename ReflectableType, typename ReturnType, typename... ArgumentTypes, std::size_t... I>
auto function_call_handler_impl(ReturnType (ReflectableType::* function)(ArgumentTypes...), std::index_sequence<I...>)
{
    return [function](std::any& self, std::any& result, std::initializer_list<std::any> arguments)
    {
        result = (std::any_cast<ReflectableType*>(self)->*function)(std::any_cast<ArgumentTypes>(arguments.begin()[I])...);
    };
}

template <typename ReflectableType, typename... ArgumentTypes, std::size_t... I>
auto function_call_handler_impl(void (ReflectableType::* function)(ArgumentTypes...), std::index_sequence<I...>)
{
    return [function](std::any& self, std::any& result, std::initializer_list<std::any> arguments)
    {
        (std::any_cast<ReflectableType*>(self)->*function)(std::any_cast<ArgumentTypes>(arguments.begin()[I])...);
        result.reset();
    };
}

template <typename ReturnType, typename... ArgumentTypes, std::size_t... I>
auto function_call_handler_impl(ReturnType (*function)(ArgumentTypes...), std::index_sequence<I...>)
{
    return [function](std::any& self, std::any& result, std::initializer_list<std::any> arguments)
    {
        result = function(std::any_cast<ArgumentTypes>(arguments.begin()[I])...);
    };
}

template <typename... ArgumentTypes, std::size_t... I>
auto function_call_handler_impl(void (*function)(ArgumentTypes...), std::index_sequence<I...>)
{
    return [function](std::any& self, std::any& result, std::initializer_list<std::any> arguments)
    {
        function(std::any_cast<ArgumentTypes>(arguments.begin()[I])...);
        result.reset();
    };
}

template <typename ReflectableType, typename ReturnType, typename... ArgumentTypes>
auto function_call_handler(ReturnType (ReflectableType::* function)(ArgumentTypes...))
{
    return function_call_handler_impl(function, std::index_sequence_for<ArgumentTypes...>{});
}

template <typename ReturnType, typename... ArgumentTypes>
auto function_call_handler(ReturnType (*function)(ArgumentTypes...))
{
    return function_call_handler_impl(function, std::index_sequence_for<ArgumentTypes...>{});
}

template <typename ReturnType, class ClassType>
auto function_overload(ReturnType (ClassType::* function)(void)) { return function; }

template <typename... ArgumentTypes, typename ReturnType, class ClassType>
auto function_overload(ReturnType (ClassType::* function)(ArgumentTypes...)) { return function; }

template <typename... ArgumentTypes, typename ReturnType>
auto function_overload(ReturnType (*function)(ArgumentTypes...)) { return function; }

template <typename ReturnType>
auto function_overload(ReturnType (*function)(void)) { return function; }

} // namespace rew

#endif // REW_FUNCTION_HPP
