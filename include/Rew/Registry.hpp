#ifndef REW_REGISTRY_HPP
#define REW_REGISTRY_HPP

#include <string> // string
#include <unordered_map> // unordered_map

#include <typeindex> // type_index
#include <utility> // ref

#include <Rew/Type.hpp>
#include <Rew/Cast.hpp>
#include <Rew/Reflection.hpp>
#include <Rew/Visitor.hpp>

template <typename ReflectableType>
struct rew_reflection_registry_t;

namespace rew
{

template <typename T>
struct builtin_reflection_info_t;

template <typename T>
struct reflection_info_t;

class registry_t
{
public:
    std::unordered_map<std::string, type_t*> all;
    std::unordered_map<std::type_index, type_t*> rtti_all;

public:
    ~registry_t()
    {
        for (auto& [name, meta] : all)
        {
            delete meta->reflection;
            delete meta;
        }
    }

public:
    type_t* find(const std::string& name)
    {
        auto it = all.find(name);
        return it != all.end() ? it->second : nullptr;
    }

    type_t* find(std::type_index typeindex)
    {
        auto it = rtti_all.find(typeindex);
        return it != rtti_all.end() ? it->second : nullptr;
    }

    type_t* find(std::any& object)
    {
        return find(object.type());
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
        auto& type = all[name];
        if (type != nullptr) return type;

        auto reflection = new reflection_t{ name };

        auto evaluate = [](visitor_t& visitor)
        {
            polymorphic_visitor_t::call<ReflectableType>(visitor);
        };

        auto ptr = [](std::any& object) -> void*
        {
            return cast_type_traits<ReflectableType>{}(object);
        };

        auto ref = [](void* object) -> std::any
        {
            return std::ref(*static_cast<ReflectableType*>(object));
        };

        type = new type_t { name, reflection, evaluate, ptr, ref, sizeof(ReflectableType) };

        all.emplace(name, type);
        rtti_all.emplace(typeid(ReflectableType), type);

        return type;
    }
};

inline registry_t registry;

template <typename ReflectableType>
type_t* find_or_add(registry_t* registry, const std::string& name)
{
    auto type = registry->find(name);
    return type == nullptr ? registry->add<ReflectableType>(name) : type;
}

} // namespace rew

#endif // REW_REGISTRY_HPP
