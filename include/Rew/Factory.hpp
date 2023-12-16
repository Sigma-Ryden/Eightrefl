#ifndef REW_FACTORY_HPP
#define REW_FACTORY_HPP

#include <cstddef> // size_t

#include <string> // string
#include <any> // any

#include <functional> // function
#include <initializer_list> // intializer_list

#include <Rew/Attribute.hpp>
#include <Rew/Meta.hpp>

#include <Rew/Utility.hpp>

#define FACTORY(...)                                                                                    \
    visitor.template factory<info_t::type, info_t::type* (*)(__VA_ARGS__)>({                            \
        #__VA_ARGS__,                                                                                   \
        factory_call_handler(static_cast<info_t::type* (*)(__VA_ARGS__)>(nullptr)),                     \
        function_args_count(static_cast<info_t::type* (*)(__VA_ARGS__)>(nullptr))                       \
    });

namespace rew
{

struct factory_meta_t
{
    const std::string name;
    const std::function<void*(std::initializer_list<std::any>)> call = nullptr;
    const std::size_t args_count = 0;
    meta_t meta;
};

using factory_t = attribute_t<factory_meta_t>;

template <typename ReflectableType, typename... ArgumentTypes, std::size_t... I>
auto factory_call_handler_impl(std::index_sequence<I...>)
{
    return [](std::initializer_list<std::any> arguments) -> void*
    {
        return new ReflectableType(std::any_cast<ArgumentTypes>(arguments.begin()[I])...);
    };
}

template <typename ReflectableType, typename... ArgumentTypes>
auto factory_call_handler(ReflectableType* (*unused)(ArgumentTypes...))
{
    return factory_call_handler_impl<ReflectableType, ArgumentTypes...>(std::index_sequence_for<ArgumentTypes...>{});
}

} // namespace rew

#endif // REW_FACTORY_HPP
