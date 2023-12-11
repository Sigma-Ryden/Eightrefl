#ifndef REW_REGISTRY_HPP
#define REW_REGISTRY_HPP

#include <string> // string
#include <map> // map

#include <Rew/Type.hpp>
#include <Rew/Reflection.hpp>
#include <Rew/Visitor.hpp>

namespace rew
{

template <class ReflectableType>
class reflection_registry_t;

class registry_t
{
public:
    std::map<std::string, type_t> all;

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

    template <class ReflectableType>
    type_t* find()
    {
        return get(reflection_registry_t<ReflectableType>::name);
    }

    template <class ReflectableType>
    type_t* add(const std::string& name)
    {
        auto reflection = new reflection_t{ name };

        auto evaluate = [](visitor_t& visitor)
        {
            polymorphic_visitor_t::call<ReflectableType>(visitor);
        };

        auto [it, success] = all.emplace(name, type_t{ name, reflection, evaluate });
        return &it->second;
    }

    template <class ReflectableType>
    type_t* find_or_add(const std::string& name)
    {
        auto type = find(name);
        return type == nullptr ? add<ReflectableType>(name) : type;
    }
};

inline registry_t registry;

} // namespace rew

#endif // REW_REGISTRY_HPP
