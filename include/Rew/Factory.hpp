#ifndef REW_FACTORY_HPP
#define REW_FACTORY_HPP

#include <string> // string
#include <map> // map
#include <any> // any

#include <functional> // function
#include <initializer_list> // intializer_list

#include <Rew/Attribute.hpp>
#include <Rew/Meta.hpp>

#define FACTORY(...)                                                                                    \
    visitor.template factory<__VA_ARGS__>({                                                             \
        #__VA_ARGS__,                                                                                   \
        factory_call_handler<reflectable_type, __VA_ARGS__>()                                           \
    });

namespace rew
{

struct factory_meta_t
{
    // TODO:
    // add arguments count, arguments type
    const std::string name;
    const std::function<void*(std::initializer_list<std::any>)> call = nullptr;
    meta_t meta;
};

using factory_t = attribute_t<factory_meta_t>;

template <class OtherClassType, typename... ArgumentTypes, std::size_t... I>
auto factory_call_handler_impl(std::index_sequence<I...>)
{
    return [](std::initializer_list<std::any> arguments) -> void*
    {
        return new OtherClassType(std::any_cast<ArgumentTypes>(arguments.begin()[I])...);
    };
}

template <class OtherClassType, typename... ArgumentTypes>
auto factory_call_handler()
{
    return factory_call_handler_impl<OtherClassType, ArgumentTypes...>(std::index_sequence_for<ArgumentTypes...>{});
}

} // namespace rew

#endif // REW_FACTORY_HPP
