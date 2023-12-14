#ifndef REW_REFLECTABLE_HPP
#define REW_REFLECTABLE_HPP

#include <Rew/Reflection.hpp>
#include <Rew/Visitor.hpp>
#include <Rew/Type.hpp>

// TODO:
// replace reflection_registry_impl_t to normal name and hide to class reflection_core_t
// add REFLECTABLE_ACCESS() macro for non-public registration
// add support for use custom visitor in reflectable
// move reflectbale_name, registry to to reflection_info_t
#define CORE_REFLECTABLE(reflection_registry, ...)                                                      \
    namespace rew {                                                                                     \
        template <> struct reflection_registry_t<__VA_ARGS__> {                                         \
            using reflectable_type = __VA_ARGS__;                                                       \
            inline static const auto registry = &reflection_registry;                                   \
            inline static const auto reflectable_name = #__VA_ARGS__;                                   \
            struct eval_t {                                                                             \
                template <class VisitorType> eval_t(VisitorType&& visitor) {                            \
                    visitor.template type<reflectable_type>(                                            \
                        *registry->find_or_add<reflectable_type>(reflectable_name)                      \
                    );

#define REFLECTABLE_INIT(...)                                                                           \
                }                                                                                       \
            };                                                                                          \
        private:                                                                                        \
            inline static auto _ = eval_t(::rew::reflection_visitor_t{});                               \
        };                                                                                              \
    }

#define REFLECTABLE(...) CORE_REFLECTABLE(::rew::registry, __VA_ARGS__)

namespace rew
{

struct reflection_visitor_t : visitor_t
{
    reflection_t* reflection = nullptr;

    template <typename ClassType>
    void type(const rew::type_t& type)
    {
        reflection = type.reflection;
    }

    template <typename PropertyType>
    void property(const rew::property_meta_t& meta)
    {
        reflection->property.add(meta.name, meta);
    }

    template <typename FunctionType>
    void function(const rew::function_meta_t& meta)
    {
        reflection->function.add(meta.name, meta);
    }

    template <class BaseClassType, class DerivedClassType>
    void parent(const rew::parent_meta_t& meta)
    {
        reflection->parent.add(meta.name, meta);
    }

    template <class ClassType, typename... ArgumentTypes>
    void factory(const rew::factory_meta_t& meta)
    {
        reflection->factory.add(meta.name, meta);
    }

    template <typename MetaType>
    void meta(const std::string& name, const MetaType& data)
    {
        reflection->meta.add(name, data);
    }
};

} // namespace rew

#endif // REW_REFLECTABLE_HPP

REFLECTABLE_VISITOR_REGISTRY(0, ::rew::reflection_visitor_t)
