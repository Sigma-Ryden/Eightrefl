#ifndef REW_META_HPP
#define REW_META_HPP

#include <any> // any

#include <Rew/Attribute.hpp>

#define CORE_META(name, ...)                                                                            \
    {                                                                                                   \
        auto __meta = __reflection->meta.find(name);                                                    \
        if (__meta == nullptr) __meta = &__reflection->meta.add(name, __VA_ARGS__);                     \
        visitor.template meta<__reflectable_type, decltype(__VA_ARGS__)>(name, *__meta);                \
    }

#define META(name, ...)                                                                                 \
    CORE_META(name, __VA_ARGS__)

#endif // REW_META_HPP
