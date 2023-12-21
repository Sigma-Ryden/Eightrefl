#ifndef REW_FACTORY_HPP
#define REW_FACTORY_HPP

#include <cstddef> // size_t

#include <string> // string
#include <vector> // vector
#include <any> // any

#include <functional> // function

#include <Rew/Attribute.hpp>
#include <Rew/Cast.hpp>
#include <Rew/Meta.hpp>

#include <Rew/Utility.hpp>

#define CORE_FACTORY(factory_call_handler, ...)                                                         \
    {                                                                                                   \
        using __function_type = ::rew::function_type_traits<__VA_ARGS__>::function_type;                \
        using __return_type = ::rew::function_type_traits<__VA_ARGS__>::return_type;                    \
        auto __meta = reflection->factory.find(#__VA_ARGS__);                                           \
        if (__meta == nullptr) __meta = &reflection->factory.add(                                       \
            #__VA_ARGS__,                                                                               \
            {                                                                                           \
                #__VA_ARGS__,                                                                           \
                factory_call_handler(__function_type{}),                                                \
                ::rew::function_args_count(__function_type{}),                                          \
            }                                                                                           \
        );                                                                                              \
        visitor.template factory<info_t::type, __function_type>(*__meta);                               \
    }

#define FACTORY(...)                                                                                    \
    CORE_FACTORY(::rew::factory_call_handler, __VA_ARGS__)

namespace rew
{

struct factory_meta_t
{
    const std::string name;
    const std::function<std::any(const std::vector<std::any>&)> call = nullptr;
    const std::size_t arg_count = 0;
    meta_t meta;
};

using factory_t = attribute_t<factory_meta_t>;

template <typename ReflectableType, typename... ArgumentTypes, std::size_t... I>
auto factory_call_handler_impl(std::index_sequence<I...>)
{
    return [](const std::vector<std::any>& arguments) -> std::any
    {
        return ReflectableType{ argument_cast<ArgumentTypes>(arguments[I])... };
    };
}

template <typename ReflectableType, typename... ArgumentTypes>
auto factory_call_handler(ReflectableType (*)(ArgumentTypes...))
{
    return factory_call_handler_impl<ReflectableType, ArgumentTypes...>(std::index_sequence_for<ArgumentTypes...>{});
}

} // namespace rew

#endif // REW_FACTORY_HPP
