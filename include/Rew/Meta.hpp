#ifndef REW_META_HPP
#define REW_META_HPP

#include <string> // string
#include <any> // any

#include <Rew/Attribute.hpp>

#define CORE_META(name, ...)                                                                            \
    {                                                                                                   \
        auto __meta = reflection->meta.find(name);                                                      \
        if (__meta == nullptr) __meta = &reflection->meta.add(name, __VA_ARGS__);                       \
        visitor.template meta<info_t::type, decltype(__VA_ARGS__)>(name, *__meta);                      \
    }

#define META(name, ...)                                                                                 \
    CORE_META(name, __VA_ARGS__)

namespace rew
{

using meta_t = attribute_t<std::any>;

} // namespace rew

#endif // REW_META_HPP
