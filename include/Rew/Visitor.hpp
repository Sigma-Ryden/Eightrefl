#ifndef REW_VISITOR_HPP
#define REW_VISITOR_HPP

#include <cstddef> // size_t

#include <unordered_map> // unordered_map
#include <typeindex> // type_index

#include <type_traits> // void_t

#include <Rew/Reflection.hpp>
#include <Rew/Type.hpp>

#define REW_MAX_VISITOR_NUMBER std::size_t(4)

#define REFLECTABLE_VISITOR_REGISTRY(visitor_key, ...)                                                  \
    namespace rew { namespace meta {                                                                    \
        template <> struct visitor_traits<visitor_key> { using type = __VA_ARGS__; };                   \
    }}

namespace rew
{

struct visitor_t
{
    virtual ~visitor_t() = default;

    template <typename ReflectableType>
    void type(rew::type_t& type) {}

    template <typename ReflectableType, typename ParentReflectableType>
    void parent(rew::parent_t& parent) {}

    template <typename ReflectableType, typename FunctionType>
    void factory(rew::factory_t& factory) {}

    template <typename ReflectableType, typename FunctionType>
    void function(rew::function_t& function) {}

    template <typename ReflectableType, typename PropertyType>
    void property(rew::property_t& property) {}

    template <typename ReflectableType, typename MetaType>
    void meta(const std::string& name, std::any& meta) {}
};

namespace meta
{

template <std::size_t VisitorKey> struct visitor_traits { using type = visitor_t; };

} // namespace meta

namespace detail
{

template <typename ReflectableType, std::size_t VisitorKey = 0>
static void fill_evaluation()
{
    if constexpr (VisitorKey < REW_MAX_VISITOR_NUMBER)
    {
        using visitor_type = typename meta::visitor_traits<VisitorKey>::type;
        using reflection_registry = ::rew_reflection_registry_t<ReflectableType>;
        using reflection_traits = meta::reflectable_traits<ReflectableType>;

        auto __ptr = &reflection_registry::template evaluate<visitor_type&>;
        reflection_traits::evaluation()[typeid(visitor_type)] = __ptr;

        fill_evaluation<ReflectableType, VisitorKey + 1>();
    }
}

} // namespace detail

} // namespace rew

#endif // REW_VISITOR_HPP
