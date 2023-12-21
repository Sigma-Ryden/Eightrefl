#ifndef REW_VISITOR_HPP
#define REW_VISITOR_HPP

#include <cstddef> // size_t

#include <unordered_map> // unordered_map
#include <typeindex> // type_index

#include <type_traits> // void_t

#include <Rew/Reflection.hpp>
#include <Rew/Type.hpp>

#define REFLECTABLE_VISITOR_REGISTRY(visitor_key, ...)                                                  \
    namespace rew {                                                                                     \
        template <> struct visitor_traits<visitor_key> {                                                \
            using type = __VA_ARGS__;                                                                   \
        private:                                                                                        \
            inline static auto _ = visitor_rtti_all.emplace(typeid(type), visitor_key);                 \
        };                                                                                              \
    }

template <typename ReflectableType>
struct rew_reflection_registry_t;

namespace rew
{

struct visitor_t
{
    virtual ~visitor_t() = default;

    template <typename ReflectableType>
    void type(rew::type_t& type) {}

    template <typename ReflectableType, typename PropertyType>
    void property(rew::property_meta_t& meta) {}

    template <typename ReflectableType, typename FunctionType>
    void function(rew::function_meta_t& meta) {}

    template <typename ReflectableType, typename ParentReflectableType>
    void parent(rew::parent_meta_t& meta) {}

    template <typename ReflectableType, typename FunctionType>
    void factory(rew::factory_meta_t& meta) {}

    template <typename ReflectableType, typename MetaType>
    void meta(const std::string& name, std::any& meta) {}
};

template <std::size_t VisitorKey>
struct visitor_traits;

static constexpr auto visitor_rtti_all_max_size = 4;
inline std::unordered_map<std::type_index, std::size_t> visitor_rtti_all;

class polymorphic_visitor_t
{
public:
    template <typename ReflectableType>
    static void call(visitor_t& visitor)
    {
        auto key = visitor_rtti_all.at(typeid(visitor));
        try_call<ReflectableType>(visitor, key);
    }

private:
    template <typename T, typename enable = void>
    struct is_complete : std::false_type {};

    template <typename T>
    struct is_complete<T, std::void_t<decltype(sizeof(T))>> : std::true_type {};

    template <typename ReflectableType, std::size_t VisitorKey = 1>
    static void try_call(visitor_t& registry, std::size_t key)
    {
        if constexpr (VisitorKey < visitor_rtti_all_max_size)
        {
            if constexpr (is_complete<visitor_traits<VisitorKey>>::value)
            {
                using eval_t = typename ::rew_reflection_registry_t<ReflectableType>::eval_t;
                if (VisitorKey == key)
                {
                    eval_t(dynamic_cast<typename visitor_traits<VisitorKey>::type&>(registry));
                    return;
                }
            }
            try_call<ReflectableType, VisitorKey + 1>(registry, key);
        }
        else
        {
            throw "The visitor is not registered.";
        }
    }
};

} // namespace rew

#endif // REW_VISITOR_HPP
