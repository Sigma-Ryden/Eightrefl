// TODO: add constraint to check is_base_of
#ifndef REW_PARENT_HPP
#define REW_PARENT_HPP

#include <string> // string
#include <any> // any

#include <functional> // function

#include <Rew/Attribute.hpp>
#include <Rew/Meta.hpp>

#define CORE_PARENT(parent_address_handler, ...)                                                        \
    {                                                                                                   \
        auto __meta = reflection->parent.find(#__VA_ARGS__);                                            \
        if (__meta == nullptr) __meta = &reflection->parent.add(                                        \
            #__VA_ARGS__,                                                                               \
            {                                                                                           \
                #__VA_ARGS__,                                                                           \
                info_t::registry->find_or_add<__VA_ARGS__>(#__VA_ARGS__),                               \
                parent_cast_handler<info_t::type, __VA_ARGS__>()                                        \
            }                                                                                           \
        );                                                                                              \
        visitor.template parent<info_t::type, __VA_ARGS__>(*__meta);                                    \
    }

#define PARENT(...)                                                                                     \
    CORE_PARENT(::rew::parent_get_handler, __VA_ARGS__)

namespace rew
{

class type_t;

struct parent_meta_t
{
    const std::string name;
    type_t *const type = nullptr;
    const std::function<void*(void*)> cast = nullptr;
    meta_t meta;
};

using parent_t = attribute_t<parent_meta_t>;

template <typename ReflectableType, typename ParentReflectableType>
auto parent_cast_handler()
{
    return [](void* child) -> void*
    {
        return static_cast<ParentReflectableType*>(static_cast<ReflectableType*>(child));
    };
}

} // namespace rew

#endif // REW_PARENT_HPP
