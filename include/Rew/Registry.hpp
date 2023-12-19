#ifndef REW_REGISTRY_HPP
#define REW_REGISTRY_HPP

#include <string> // string
#include <map> // map

#include <typeindex> // type_index

#include <Rew/Type.hpp>
#include <Rew/Cast.hpp>
#include <Rew/Reflection.hpp>
#include <Rew/Visitor.hpp>

namespace rew
{

template <typename ReflectableType>
class reflection_registry_t;

template <typename ReflectableType>
class reflection_info_t;

class registry_t
{
public:
    std::map<std::string, type_t> all;
    std::map<std::type_index, cast_meta_t> cast;

public:
    ~registry_t()
    {
        for (auto& [name, meta] : all) delete meta.reflection;
    }

public:
    type_t* find(const std::string& name)
    {
        auto it = all.find(name);
        return it != all.end() ? &it->second : nullptr;
    }

    template <typename ReflectableType>
    type_t* find()
    {
        return get(reflection_info_t<ReflectableType>::name);
    }

public:
    template <typename ReflectableType>
    type_t* add(const std::string& name)
    {
        auto reflection = new reflection_t{ name };

        auto evaluate = [](visitor_t& visitor)
        {
            polymorphic_visitor_t::call<ReflectableType>(visitor);
        };

        auto ptr = [this](std::any& object) -> void*
        {
            auto it = cast.find(object.type());
            return it == cast.end() ? nullptr : it->second.call(object);
        };

        auto ref = [](void* object) -> std::any
        {
            return std::ref(*static_cast<ReflectableType*>(object));
        };

        auto [it, success] = all.emplace
        (
            name,
            type_t { name, reflection, evaluate, ptr, ref, sizeof(ReflectableType) }
        );
        return &it->second;
    }

    template <typename ReflectableType>
    type_t* find_or_add(const std::string& name)
    {
        auto type = find(name);
        return type == nullptr ? add<ReflectableType>(name) : type;
    }
};

inline registry_t registry;

} // namespace rew

#endif // REW_REGISTRY_HPP
