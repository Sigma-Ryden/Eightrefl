#ifndef REW_FACTORY_HPP
#define REW_FACTORY_HPP

#include <string> // string
#include <vector> // vector
#include <any> // any

#include <functional> // function

#include <Rew/Attribute.hpp>
#include <Rew/Meta.hpp>

#include <Rew/Utility.hpp>

// .factory<signature>()
#define FACTORY(...)                                                                                    \
    {                                                                                                   \
        using xxtraits = rew::meta::function_traits<__VA_ARGS__>;                                       \
        auto xxmeta = rew::find_or_add_factory<typename xxtraits::dirty_pointer>(xxreflection);         \
        injection.template factory<CleanR, typename xxtraits::pointer>(*xxmeta);                        \
    }

namespace rew
{

struct type_t;

struct factory_t
{
    std::string const name;
    std::function<std::any(std::vector<std::any> const& args)> const call = nullptr;
    std::vector<type_t*> const arguments;
    type_t *const result = nullptr;
    attribute_t<std::any> meta;
};

namespace detail
{

template <typename ReflectableType, typename... ArgumentTypes, std::size_t... I>
auto handler_factory_call_impl(std::index_sequence<I...>)
{
    return [](std::vector<std::any> const& arguments) -> std::any
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

template <typename ReflectableType, typename... ArgumentTypes>
auto handler_factory_call(ReflectableType(*)(ArgumentTypes...))
{
    return detail::handler_factory_call_impl<ReflectableType, ArgumentTypes...>
    (
        std::index_sequence_for<ArgumentTypes...>{}
    );
}

} // namespace rew

#endif // REW_FACTORY_HPP
