#ifndef EIGHTREFL_REGISTRY_HPP
#define EIGHTREFL_REGISTRY_HPP

#include <string> // string
#include <unordered_map> // unordered_map
#include <typeindex> // type_index

#include <Eightrefl/Type.hpp>
#include <Eightrefl/Reflection.hpp>

#ifndef EIGHTREFL_REGISTRY_RESERVE_SIZE
    #define EIGHTREFL_REGISTRY_RESERVE_SIZE std::size_t(1024)
#endif // EIGHTREFL_REGISTRY_RESERVE_SIZE

namespace eightrefl
{

struct registry_t
{
    std::unordered_map<std::string, type_t*> all;

    #ifdef EIGHTREFL_RTTI_ALL_ENABLE
    std::unordered_map<std::type_index, type_t*> rtti_all;
    #endif // EIGHTREFL_RTTI_ALL_ENABLE
    registry_t();
    ~registry_t();

    type_t* find(std::string const& name) const;
    #ifdef EIGHTREFL_RTTI_ALL_ENABLE
    type_t* find(std::type_index typeindex) const;
    #endif // EIGHTREFL_RTTI_ALL_ENABLE

    template <typename ReflectableType, typename DirtyReflectableType = ReflectableType>
    type_t* add(std::string const& name)
    {
        auto& type = all[name];
        if (type != nullptr) return type;

        type = new type_t
        {
            name,
            new reflection_t { name },
            this,
            type_size<ReflectableType>(),
            handler_type_context<ReflectableType>()
        };

        #ifdef EIGHTREFL_RTTI_ALL_ENABLE
        auto& rtti_type = rtti_all[typeid(ReflectableType)];
        if (rtti_type == nullptr) rtti_type = type;

        if constexpr (!std::is_same_v<ReflectableType, DirtyReflectableType>)
        {
            rtti_all.emplace(typeid(DirtyReflectableType), type);
        }
        #endif // EIGHTREFL_RTTI_ALL_ENABLE
        return type;
    }
};

extern registry_t* global();

} // namespace eightrefl

#endif // EIGHTREFL_REGISTRY_HPP
