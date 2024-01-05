#ifndef REW_PARENT_HPP
#define REW_PARENT_HPP

#include <string> // string

#include <functional> // function

#include <type_traits> // is_base_of_v

#include <Rew/Attribute.hpp>
#include <Rew/Meta.hpp>

#define CORE_PARENT(parent_cast_handler, ...)                                                           \
    {                                                                                                   \
        using __type = __VA_ARGS__;                                                                     \
        static_assert(                                                                                  \
            std::is_base_of_v<__type, reflectable>,                                                     \
            "The " #__VA_ARGS__ " type is not parent of reflectable type."                              \
        );                                                                                              \
        ::rew::utility::conditional_reflectable_register<__type>();                                     \
        auto __meta = reflection->parent.find(#__VA_ARGS__);                                            \
        if (__meta == nullptr) __meta = &reflection->parent.add(                                        \
            #__VA_ARGS__,                                                                               \
            {                                                                                           \
                #__VA_ARGS__,                                                                           \
                ::rew::meta::reflectable_traits_t<__type>::registry()->all[#__VA_ARGS__],               \
                parent_cast_handler<reflectable, __type>()                                              \
            }                                                                                           \
        );                                                                                              \
        visitor.template parent<reflectable, __type>(*__meta);                                          \
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
    const std::function<void*(void* child_context)> cast = nullptr;
    attribute_t<std::any> meta;
};

namespace handler
{

template <typename ReflectableType, typename ParentReflectableType>
auto parent_cast()
{
    return [](void* child_context) -> void*
    {
        return static_cast<ParentReflectableType*>(static_cast<ReflectableType*>(child_context));
    };
}

} // namespace handler

} // namespace rew

#endif // REW_PARENT_HPP
