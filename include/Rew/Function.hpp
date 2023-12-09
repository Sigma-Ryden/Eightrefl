#ifndef REW_FUNCTION_HPP
#define REW_FUNCTION_HPP

#include <string> // string
#include <map> // map
#include <any> // any

#include <functional> // function
#include <initializer_list> // initializer_list

#include <Rew/Attribute.hpp>

#define FUNCTION(...)                                                                                   \
    visitor.template function<decltype(&type::__VA_ARGS__)>({                                           \
        #__VA_ARGS__,                                                                                   \
        function_is_static(&type::__VA_ARGS__),                                                         \
        function_call_handler(&type::__VA_ARGS__)                                                       \
    });

namespace rew
{

struct function_t : attribute_t<function_t>
{
    struct meta_t
    {
        const std::string name;
        const bool is_static = false;

        const std::function<void(void*, std::any&, std::initializer_list<std::any>)> call = nullptr;
        // TODO:
        // add arguments count, arguments types
        // replace is_static to type
    };

    std::map<std::string, meta_t> all;
};

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

template <class ClassType, typename ReturnType, typename... ArgumentTypes>
auto function_is_static(ReturnType (ClassType::* function)(ArgumentTypes...))
{
    return false;
}

template <typename ReturnType, typename... ArgumentTypes>
auto function_is_static(ReturnType (*function)(ArgumentTypes...))
{
    return true;
}

} // namespace rew

#endif // REW_FUNCTION_HPP
