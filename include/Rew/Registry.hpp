#ifndef REW_REGISTRY_HPP
#define REW_REGISTRY_HPP

#include <string> // string
#include <unordered_map> // unordered_map

#include <typeindex> // type_index
#include <utility> // ref

#include <Rew/Type.hpp>
#include <Rew/Reflection.hpp>
#include <Rew/Visitor.hpp>

#define REW_REGISTRY_RESERVE_SIZE std::size_t(4096)

namespace rew
{

class registry_t
{
public:
    std::unordered_map<std::string, type_t*> all;
    std::unordered_map<std::type_index, type_t*> rtti_all;

public:
    registry_t()
    {
        all.reserve(REW_REGISTRY_RESERVE_SIZE);
    }

    ~registry_t()
    {
        for (auto& [name, meta] : all)
        {
            delete meta->reflection;
            delete meta;
        }
    }

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
        return get(meta::reflectable_name_t<ReflectableType>::get());
    }

    template <typename ReflectableType>
    type_t* add(const std::string& name)
    {
        auto& type = all[name];
        if (type != nullptr) return type;

        auto reflection = new reflection_t{ name };

        auto evaluate = [](visitor_t& visitor)
        {
            detail::polymorphic_visitor_t::call<ReflectableType>(visitor);
        };

        auto context = [](std::any& object) -> std::any
        {
            return std::addressof(std::any_cast<ReflectableType&>(object));
        };

        auto alias = [](std::any& context) -> std::any
        {
            return std::ref(*std::any_cast<ReflectableType*>(context));
        };

        type = new type_t { name, reflection, evaluate, context, alias, sizeof(ReflectableType) };

        all.emplace(name, type);
        rtti_all.emplace(typeid(ReflectableType), type);

        return type;
    }
};

inline registry_t global;

} // namespace rew

#endif // REW_REGISTRY_HPP
