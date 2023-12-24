#ifndef REW_PARENT_HPP
#define REW_PARENT_HPP

#include <string> // string

#include <functional> // function

#include <type_traits> // is_base_of_v

#include <Rew/Attribute.hpp>
#include <Rew/Meta.hpp>

#define CORE_PARENT(parent_cast_handler, ...)                                                           \
    {                                                                                                   \
        static_assert(                                                                                  \
            std::is_base_of_v<__VA_ARGS__, info_t::type>,                                               \
            "The " #__VA_ARGS__ " type is not parent of reflectable type."                              \
        );                                                                                              \
        auto __meta = reflection->parent.find(#__VA_ARGS__);                                            \
        if (__meta == nullptr) __meta = &reflection->parent.add(                                        \
            #__VA_ARGS__,                                                                               \
            {                                                                                           \
                #__VA_ARGS__,                                                                           \
                ::rew::find_or_add<__VA_ARGS__>(info_t::registry, #__VA_ARGS__),                        \
                parent_cast_handler<info_t::type, __VA_ARGS__>()                                        \
            }                                                                                           \
        );                                                                                              \
        visitor.template parent<info_t::type, __VA_ARGS__>(*__meta);                                    \
    }

#define PARENT(...)                                                                                     \
    CORE_PARENT(::rew::parent_cast_handler, __VA_ARGS__)

namespace rew
{

class type_t;

struct parent_meta_t
{
    const std::string name;
    type_t *const type = nullptr;
    const std::function<void*(void* child_context)> cast = nullptr;
    meta_t meta;
};

using parent_t = attribute_t<parent_meta_t>;

template <typename ReflectableType, typename ParentReflectableType>
auto parent_cast_handler()
{
    return [](void* child_context) -> void*
    {
        return static_cast<ParentReflectableType*>(static_cast<ReflectableType*>(child_context));
    };
}

} // namespace rew

#endif // REW_PARENT_HPP
