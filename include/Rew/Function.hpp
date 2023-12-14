#ifndef REW_FUNCTION_HPP
#define REW_FUNCTION_HPP

#include <string> // string
#include <map> // map
#include <any> // any

#include <functional> // function
#include <initializer_list> // initializer_list

#include <Rew/Attribute.hpp>
#include <Rew/Meta.hpp>

#define FUNCTION(name, ...)                                                                             \
    visitor.template function<decltype(overload<__VA_ARGS__>(&reflectable_type::name))>({               \
        assembly(#name, #__VA_ARGS__),                                                                  \
        function_call_handler(overload<__VA_ARGS__>(&reflectable_type::name))                           \
    });

namespace rew
{

struct function_meta_t
{
    // TODO:
    // add arguments count, arguments types
    const std::string name;
    const std::function<void(void*, std::any&, std::initializer_list<std::any>)> call = nullptr;
    meta_t meta;
};

using function_t = attribute_t<function_meta_t>;

template <class ClassType, typename ReturnType, typename... ArgumentTypes, std::size_t... I>
auto function_call_handler_impl(ReturnType (ClassType::* function)(ArgumentTypes...), std::index_sequence<I...>)
{
    return [function](void* self, std::any& result, std::initializer_list<std::any> arguments)
    {
        result = (static_cast<ClassType*>(self)->*function)(std::any_cast<ArgumentTypes>(arguments.begin()[I])...);
    };
}

template <class ClassType, typename... ArgumentTypes, std::size_t... I>
auto function_call_handler_impl(void (ClassType::* function)(ArgumentTypes...), std::index_sequence<I...>)
{
    return [function](void* self, std::any& result, std::initializer_list<std::any> arguments)
    {
        (static_cast<ClassType*>(self)->*function)(std::any_cast<ArgumentTypes>(arguments.begin()[I])...);
        result.reset();
    };
}

template <typename ReturnType, typename... ArgumentTypes, std::size_t... I>
auto function_call_handler_impl(ReturnType (*function)(ArgumentTypes...), std::index_sequence<I...>)
{
    return [function](void* self, std::any& result, std::initializer_list<std::any> arguments)
    {
        result = function(std::any_cast<ArgumentTypes>(arguments.begin()[I])...);
    };
}

template <typename... ArgumentTypes, std::size_t... I>
auto function_call_handler_impl(void (*function)(ArgumentTypes...), std::index_sequence<I...>)
{
    return [function](void* self, std::any& result, std::initializer_list<std::any> arguments)
    {
        function(std::any_cast<ArgumentTypes>(arguments.begin()[I])...);
        result.reset();
    };
}

template <class ClassType, typename ReturnType, typename... ArgumentTypes>
auto function_call_handler(ReturnType (ClassType::* function)(ArgumentTypes...))
{
    return function_call_handler_impl(function, std::index_sequence_for<ArgumentTypes...>{});
}

template <typename ReturnType, typename... ArgumentTypes>
auto function_call_handler(ReturnType (*function)(ArgumentTypes...))
{
    return function_call_handler_impl(function, std::index_sequence_for<ArgumentTypes...>{});
}

template <typename... ArgumentTypes, typename ReturnType, class ClassType>
auto overload(ReturnType (ClassType::* function)(ArgumentTypes...)) { return function; }

template <typename ReturnType, class ClassType>
auto overload(ReturnType (ClassType::* function)(void)) { return function; }

template <typename... ArgumentTypes, typename ReturnType>
auto overload(ReturnType (*function)(ArgumentTypes...)) { return function; }

template <typename ReturnType>
auto overload(ReturnType (*function)(void)) { return function; }

std::string assembly(const std::string& lhs, const std::string& rhs)
{
    return rhs.empty() ? lhs : lhs + ", " + rhs;
}

} // namespace rew

#endif // REW_FUNCTION_HPP
