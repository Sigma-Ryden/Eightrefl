#ifndef REW_REFLECTABLE_HPP
#define REW_REFLECTABLE_HPP

#include <Rew/Reflection.hpp>
#include <Rew/Visitor.hpp>

// TODO:
// replace reflection_registry_impl_t to normal name and hide to class reflection_core_t
// add REFLECTABLE_ACCESS() macro for non-public registration

#define CORE_REFLECTABLE(reflection_registry, ...)                                                      \
    namespace rew {                                                                                     \
        template <> struct reflection_registry_t<__VA_ARGS__> {                                         \
            using type = __VA_ARGS__;                                                                   \
            inline static const auto registry = &reflection_registry;                                   \
            inline static const auto name = #__VA_ARGS__;                                               \
            struct eval_t {                                                                             \
                template <class VisitorType> eval_t(VisitorType&& visitor) {

#define REFLECTABLE_INIT()                                                                              \
                }                                                                                       \
            };                                                                                          \
        private:                                                                                        \
            inline static auto _ = eval_t(                                                              \
                reflection_visitor_t{ registry->add<type>(name)->reflection }                           \
            );                                                                                          \
        };                                                                                              \
    }

#define REFLECTABLE(...) CORE_REFLECTABLE(::rew::registry, __VA_ARGS__)

namespace rew
{

struct reflection_visitor_t : visitor_t
{
    reflection_t* reflection = nullptr;

    reflection_visitor_t(reflection_t* reflection) : reflection(reflection) {}

    template <typename PropertyType>
    void property(const rew::property_t::meta_t& meta)
    {
        reflection->property.add(meta.name, meta);
    }

    template <typename FunctionType>
    void function(const rew::function_t::meta_t& meta)
    {
        reflection->function.add(meta.name, meta);
    }

    template <class BaseClassType, class DerivedClassType>
    void parent(const rew::parent_t::meta_t& meta)
    {
        reflection->parent.add(meta.name, meta);
    }

    template <class OtherClassType, typename... ArgumentTypes>
    void factory(const rew::factory_t::meta_t& meta)
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

REFLECTABLE_VISITOR(0, ::rew::reflection_visitor_t)
