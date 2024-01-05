#ifndef REW_VISITOR_HPP
#define REW_VISITOR_HPP

#include <cstddef> // size_t

#include <unordered_map> // unordered_map
#include <typeindex> // type_index

#include <type_traits> // void_t

#include <Rew/Reflection.hpp>
#include <Rew/Type.hpp>

#define REFLECTABLE_VISITOR_REGISTRY(visitor_key, ...)                                                  \
    namespace rew { namespace meta {                                                                    \
        template <> struct visitor_traits<visitor_key> {                                                \
            using type = __VA_ARGS__;                                                                   \
        private:                                                                                        \
            inline static auto _ = visitor_rtti_all.emplace(typeid(type), visitor_key);                 \
        };                                                                                              \
    }}

namespace rew
{

struct visitor_t
{
    virtual ~visitor_t() = default;

    template <typename ReflectableType>
    void type(rew::type_t& type) {}

    template <typename ReflectableType, typename PropertyType>
    void property(rew::property_t& property) {}

    template <typename ReflectableType, typename FunctionType>
    void function(rew::function_t& function) {}

    template <typename ReflectableType, typename ParentReflectableType>
    void parent(rew::parent_t& parent) {}

    template <typename ReflectableType, typename FunctionType>
    void factory(rew::factory_t& factory) {}

    template <typename ReflectableType, typename MetaType>
    void meta(const std::string& name, std::any& meta) {}
};

namespace meta
{

template <std::size_t VisitorKey>
struct visitor_traits
{
    using type = visitor_t;
};

static constexpr auto visitor_rtti_all_max_size = 4;
inline std::unordered_map<std::type_index, std::size_t> visitor_rtti_all;

} // namespace meta

namespace detail
{

class polymorphic_visitor_t
{
public:
    template <typename ReflectableType>
    static void call(visitor_t& visitor)
    {
        auto key = meta::visitor_rtti_all.at(typeid(visitor));
        try_call<ReflectableType>(visitor, key);
    }

private:
    template <typename ReflectableType, std::size_t VisitorKey = 0>
    static void try_call(visitor_t& registry, std::size_t key)
    {
        if constexpr (VisitorKey < meta::visitor_rtti_all_max_size)
        {
            using eval_t = typename ::rew_reflection_registry_t<ReflectableType>::eval_t;

            if (VisitorKey == key) eval_t(dynamic_cast<typename meta::visitor_traits<VisitorKey>::type&>(registry));
            else try_call<ReflectableType, VisitorKey + 1>(registry, key);
        }
    }
};

} // namespace detail

} // namespace rew

#endif // REW_VISITOR_HPP
