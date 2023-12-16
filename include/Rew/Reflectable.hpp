#ifndef REW_REFLECTABLE_HPP
#define REW_REFLECTABLE_HPP

#include <Rew/Reflection.hpp>
#include <Rew/Visitor.hpp>
#include <Rew/Type.hpp>

// TODO:
// replace reflection_registry_impl_t to normal name and hide to class registry_t
// add REFLECTABLE_ACCESS() macro for non-public registration
// add support for use custom visitor in reflectable
#define CORE_REFLECTABLE(reflection_registry, ...)                                                      \
    namespace rew {                                                                                     \
        template <> struct reflection_info_t<__VA_ARGS__> {                                             \
            using type = __VA_ARGS__;                                                                   \
            inline static const auto registry = &reflection_registry;                                   \
            inline static const auto name = #__VA_ARGS__;                                               \
        };                                                                                              \
        template <> struct reflection_registry_t<__VA_ARGS__> {                                         \
            using info_t = reflection_info_t<__VA_ARGS__>;                                              \
            struct eval_t {                                                                             \
                template <class VisitorType> eval_t(VisitorType&& visitor) {                            \
                    visitor.template type<info_t::type>(                                                \
                        *info_t::registry->find_or_add<info_t::type>(info_t::name)                      \
                    );

#define REFLECTABLE_INIT(...)                                                                           \
                }                                                                                       \
            };                                                                                          \
        private:                                                                                        \
            inline static auto _ = eval_t(::rew::reflection_visitor_t{});                               \
        };                                                                                              \
    }

#define REFLECTABLE(...)                                                                                \
    CORE_REFLECTABLE(::rew::registry, __VA_ARGS__)

#define BUILTIN_REFLECTABLE(...)                                                                        \
    CORE_REFLECTABLE(::rew::registry, __VA_ARGS__)                                                      \
    BUILTIN_FACTORY()                                                                                   \
    BUILTIN_FACTORY(__VA_ARGS__)                                                                        \
    BUILTIN_PROPERTY(value)

namespace rew
{

struct reflection_visitor_t : visitor_t
{
    reflection_t* reflection = nullptr;

    template <typename ReflectableType>
    void type(const rew::type_t& type)
    {
        reflection = type.reflection;
    }

    template <typename ReflectableType, typename PropertyType>
    void property(const rew::property_meta_t& meta)
    {
        reflection->property.add(meta.name, meta);
    }

    template <typename ReflectableType, typename FunctionType>
    void function(const rew::function_meta_t& meta)
    {
        reflection->function.add(meta.name, meta);
    }

    template <typename ReflectableType, typename ParentReflectableType>
    void parent(const rew::parent_meta_t& meta)
    {
        reflection->parent.add(meta.name, meta);
    }

    template <typename ReflectableType, typename FunctionType>
    void factory(const rew::factory_meta_t& meta)
    {
        reflection->factory.add(meta.name, meta);
    }

    template <typename ReflectableType, typename MetaType>
    void meta(const std::string& name, const MetaType& data)
    {
        reflection->meta.add(name, data);
    }
};

} // namespace rew

#endif // REW_REFLECTABLE_HPP
