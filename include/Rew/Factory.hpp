#ifndef REW_FACTORY_HPP
#define REW_FACTORY_HPP

#include <cstddef> // size_t

#include <string> // string
#include <vector> // vector
#include <any> // any

#include <functional> // function

#include <Rew/Attribute.hpp>
#include <Rew/Meta.hpp>

#include <Rew/Utility.hpp>

#define CORE_FACTORY(...)                                                                               \
    {                                                                                                   \
        using __traits = ::rew::meta::function_traits<__VA_ARGS__>;                                     \
        using __function_type = typename __traits::function_type;                                       \
        auto __meta = ::rew::find_or_add_factory<__function_type>(__reflection);                        \
        visitor.template factory<__reflectable_type, __function_type>(*__meta);                         \
    }

#define FACTORY(...) CORE_FACTORY(__VA_ARGS__)

namespace rew
{

struct factory_t
{
    const std::string name;
    const std::function<std::any(const std::vector<std::any>& args)> call = nullptr;
    const std::size_t arg_count = 0;
    attribute_t<std::any> meta;
};

namespace detail
{

template <typename ReflectableType, typename... ArgumentTypes, std::size_t... I>
auto factory_call_impl(std::index_sequence<I...>)
{
    return [](const std::vector<std::any>& arguments) -> std::any
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
