#ifndef REW_PARENT_HPP
#define REW_PARENT_HPP

#include <string> // string

#include <functional> // function

#include <type_traits> // is_base_of_v

#include <Rew/Attribute.hpp>
#include <Rew/Meta.hpp>
#include <Rew/Utility.hpp>

#define CORE_PARENT(parent_cast_handler, ...)                                                           \
    {                                                                                                   \
        using __type = __VA_ARGS__;                                                                     \
        using __traits = ::rew::meta::reflectable_traits_t<__type>;                                     \
        static_assert(std::is_base_of_v<__type, __reflectable_type>);                                   \
        ::rew::reflectable<__type>();                                                                   \
        static auto __name = __traits::name();                                                          \
        auto __meta = __reflection->parent.find(__name);                                                \
        if (__meta == nullptr) __meta = &__reflection->parent.add(                                      \
            __name,                                                                                     \
            {                                                                                           \
                __name,                                                                                 \
                __traits::registry()->all[__name],                                                      \
                parent_cast_handler<__reflectable_type, __type>()                                       \
            }                                                                                           \
        );                                                                                              \
        visitor.template parent<__reflectable_type, __type>(*__meta);                                   \
    }

#define PARENT(...)                                                                                     \
    CORE_PARENT(::rew::handler::parent_cast, __VA_ARGS__)

namespace rew
{

class type_t;

struct parent_t
{
    const std::string name;
    type_t *const& type = nullptr;
    const std::function<std::any(std::any& child_context)> cast = nullptr;
    attribute_t<std::any> meta;
};

namespace handler
{

template <typename ReflectableType, typename ParentReflectableType>
auto parent_cast()
{
    return [](std::any& child_context) -> std::any
    {
        return static_cast<ParentReflectableType*>(std::any_cast<ReflectableType*>(child_context));
    };
}

} // namespace handler

} // namespace rew

#endif // REW_PARENT_HPP
