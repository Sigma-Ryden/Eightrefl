#ifndef REW_REFLECTABLE_HPP
#define REW_REFLECTABLE_HPP

#include <Rew/Registry.hpp>
#include <Rew/Detail/Meta.hpp>

#define NAMEOF(...)                                                                                     \
    ::rew::meta::reflectable_traits_t<__VA_ARGS__>::name()

#define CORE_REFLECTABLE_BODY(...)                                                                      \
    struct eval_t {                                                                                     \
        template <class VisitorType>                                                                    \
        eval_t(VisitorType&& visitor) {                                                                 \
            using __reflectable_traits = __VA_ARGS__;                                                   \
            using __reflectable_type = typename __reflectable_traits::type;                             \
            static auto __reflectable_name = __reflectable_traits::name();                              \
            static auto __reflectable_registry = __reflectable_traits::registry();                      \
            auto __type = __reflectable_registry->all[__reflectable_name];                              \
            if (__type == nullptr) __type = __reflectable_registry->template add<__reflectable_type>(   \
                __reflectable_name                                                                      \
            );                                                                                          \
            auto __reflection = __type->reflection;                                                     \
            visitor.template type<__reflectable_type>(*__type);

#define CORE_REFLECTABLE_DECLARATION(reflection_registry, ...)                                          \
    namespace rew { namespace meta {                                                                    \
        template <> struct reflectable_traits_t<__VA_ARGS__> : base_reflectable_traits_t {              \
            using type = __VA_ARGS__;                                                                   \
            static const auto registry() { return &reflection_registry; }                               \
            static const std::string name() { return #__VA_ARGS__; }                                    \
        };                                                                                              \
    }}

#define CORE_CONDITIONAL_REFLECTABLE_DECLARATION(...)                                                   \
    namespace rew { namespace meta {                                                                    \
        template <typename T>                                                                           \
        struct reflectable_traits_t<T, std::enable_if_t<__VA_ARGS__>>                                   \
            : conditional_reflectable_traits_t {                                                        \
            using type = T;                                                                             \

#define CORE_REFLECTABLE_DECLARATION(...)                                                               \
    namespace rew { namespace meta {                                                                    \
        template <> struct reflectable_traits_t<__VA_ARGS__> : base_reflectable_traits_t {              \
            using type = __VA_ARGS__;                                                                   \

#define REFLECTABLE_REGISTRY(...)                                                                       \
    static const auto registry() { return __VA_ARGS__; }                                                \

#define REFLECTABLE_NAME(...)                                                                           \
    static const std::string name() { return __VA_ARGS__; }

#define REFLECTABLE_DECLARATION_INIT(...)                                                               \
        };                                                                                              \
    }}

#define CORE_REFLECTABLE(...)                                                                           \
    template <> struct rew_reflection_registry_t<__VA_ARGS__> {                                         \
        using T = __VA_ARGS__;                                                                          \
        CORE_REFLECTABLE_BODY(::rew::meta::reflectable_traits_t<__VA_ARGS__>)

#define CORE_CONDITIONAL_REFLECTABLE(...)                                                               \
    template <typename T>                                                                               \
    struct rew_reflection_registry_t<T, std::enable_if_t<__VA_ARGS__>> {                                \
        CORE_REFLECTABLE_BODY(::rew::meta::reflectable_traits_t<T>)

#define CORE_REFLECTABLE_INIT(...)                                                                      \
            }                                                                                           \
        };                                                                                              \
    private:                                                                                            \
        inline static auto _ = eval_t(::rew::visitor_t{});                                              \
    };                                                                                                  \

#define REFLECTABLE_ACCESS(...)                                                                         \
    template <typename, typename> friend struct rew_reflection_registry_t;

#ifndef REFLECTABLE_DECLARATION
    #define REFLECTABLE_DECLARATION(...)                                                                \
        CORE_REFLECTABLE_DECLARATION(__VA_ARGS__)                                                       \
        REFLECTABLE_REGISTRY(&::rew::global)                                                            \
        REFLECTABLE_NAME(#__VA_ARGS__)
#endif // REFLECTABLE_DECLARATION

#ifndef CONDITIONAL_REFLECTABLE_DECLARATION
    #define CONDITIONAL_REFLECTABLE_DECLARATION(...)                                                    \
        CORE_CONDITIONAL_REFLECTABLE_DECLARATION(__VA_ARGS__)                                           \
        REFLECTABLE_REGISTRY(&::rew::global)
#endif // CONDITIONAL_REFLECTABLE_DECLARATION

#ifndef REFLECTABLE
    #define REFLECTABLE(...) CORE_REFLECTABLE(__VA_ARGS__)
#endif // REFLECTABLE

#ifndef CONDITIONAL_REFLECTABLE
    #define CONDITIONAL_REFLECTABLE(...) CORE_CONDITIONAL_REFLECTABLE(__VA_ARGS__)
#endif // CONDITIONAL_REFLECTABLE

#ifndef REFLECTABLE_INIT
    #define REFLECTABLE_INIT(...) CORE_REFLECTABLE_INIT(__VA_RGS__)
#endif // REFLECTABLE_INIT

namespace rew
{

template <typename ReflectableType>
void reflectable()
{
    if constexpr (std::is_base_of_v<meta::conditional_reflectable_traits_t,
                                    meta::reflectable_traits_t<ReflectableType>>)
    {
        (void)::rew_reflection_registry_t<ReflectableType>{}; // explicit instancing
    }
}

template <typename ReflectableType>
ReflectableType&& reflectable(ReflectableType&& object)
{
    reflectable<std::decay_t<ReflectableType>>();
    return std::forward<ReflectableType>(object);
}

} // namespace rew

#endif // REW_REFLECTABLE_HPP
