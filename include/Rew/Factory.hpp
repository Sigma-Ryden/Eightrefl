#ifndef REW_FACTORY_HPP
#define REW_FACTORY_HPP

#include <string> // string
#include <vector> // vector
#include <any> // any

#include <functional> // function

#include <Rew/Attribute.hpp>
#include <Rew/Meta.hpp>

#include <Rew/Utility.hpp>

#define RAW_FACTORY(...)                                                                                \
    {                                                                                                   \
        using __traits = ::rew::meta::function_traits<__VA_ARGS__>;                                     \
        auto __meta = ::rew::find_or_add_factory<typename __traits::dirty_pointer>(__reflection);       \
        injection.template factory<R, typename __traits::pointer>(*__meta);                             \
    }

#define FACTORY(...) RAW_FACTORY(__VA_ARGS__)

namespace rew
{

struct type_t;

struct factory_t
{
    const std::string name;
    const std::function<std::any(std::vector<std::any> args)> call = nullptr;
    const std::vector<type_t*> arguments;
    attribute_t<std::any> meta;
};

namespace detail
{

template <typename ReflectableType, typename... ArgumentTypes, std::size_t... I>
auto factory_call_impl(std::index_sequence<I...>)
{
    return [](std::vector<std::any> arguments) -> std::any
    {
        if constexpr (std::is_aggregate_v<ReflectableType>)
        {
            return ReflectableType{ utility::forward<ArgumentTypes>(arguments[I])... };
        }
        else
        {
            return ReflectableType( utility::forward<ArgumentTypes>(arguments[I])... );
        }
    };
}

} // namespace detail

namespace handler
{

template <typename ReflectableType, typename... ArgumentTypes>
auto factory_call(ReflectableType (*)(ArgumentTypes...))
{
    return detail::factory_call_impl<ReflectableType, ArgumentTypes...>
    (
        std::index_sequence_for<ArgumentTypes...>{}
    );
}

} // namespace handler

} // namespace rew

#endif // REW_FACTORY_HPP
