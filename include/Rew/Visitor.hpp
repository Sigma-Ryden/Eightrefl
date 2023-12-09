#ifndef REW_VISITOR_HPP
#define REW_VISITOR_HPP

#include <cstddef> // size_t

#include <map> // map
#include <typeinfo> // typeid

#include <type_traits> // is_same

#define REFLECTABLE_VISITOR(visitor_key, ...)                                                           \
    namespace rew {                                                                                     \
        template <> struct visitor_traits<visitor_key> {                                                \
            using type = __VA_ARGS__;                                                                   \
        private:                                                                                        \
            inline static auto _ = visitor_base_traits::rtti_table.emplace(                             \
                typeid(type).hash_code(), visitor_key                                                   \
            );                                                                                          \
        };                                                                                              \
    }

namespace rew
{

struct visitor_t { virtual ~visitor_t() = default; };

struct visitor_base_traits
{
    static constexpr auto max_size = 4;
    inline static std::map<std::size_t, std::size_t> rtti_table;
};

template <std::size_t VisitorKey>
struct visitor_traits
{
    using type = void;
};

template <class ReflectableType>
class reflection_registry_t;

class polymorphic_visitor_t
{
public:
    template <class ReflectableType>
    static void call(visitor_t& visitor)
    {
        auto key = visitor_base_traits::rtti_table.at(typeid(visitor).hash_code());
        try_call<ReflectableType>(visitor, key);
    }

    template <class ReflectableType, std::size_t VisitorKey = 0>
    static void try_call(visitor_t& registry, std::size_t key)
    {
        if constexpr (VisitorKey < visitor_base_traits::max_size)
        {
            using DerivedVisitor = typename visitor_traits<VisitorKey>::type;
            if constexpr (not std::is_same_v<DerivedVisitor, void>)
            {
                using eval_t = typename reflection_registry_t<ReflectableType>::eval_t;
                if (VisitorKey == key)
                {
                    eval_t(dynamic_cast<DerivedVisitor&>(registry));
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
