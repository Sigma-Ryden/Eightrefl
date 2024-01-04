#ifndef REW_REFLECTABLE_HPP
#define REW_REFLECTABLE_HPP

#include <Rew/Registry.hpp>
#include <Rew/Detail/Meta.hpp>

#define CORE_REFLECTABLE_DECLARATION(reflection_registry, ...)                                          \
    namespace rew { namespace meta {                                                                    \
        template <> struct reflection_info_t<__VA_ARGS__> {                                             \
            using type = __VA_ARGS__;                                                                   \
            inline static const auto registry = &reflection_registry;                                   \
            inline static const auto name = #__VA_ARGS__;                                               \
        };                                                                                              \
    }}

#define CORE_REFLECTABLE_BODY(reflection_registry, ...)                                                 \
    template <> struct rew_reflection_registry_t<__VA_ARGS__> {                                         \
        using info_t = ::rew::meta::reflection_info_t<__VA_ARGS__>;                                     \
        struct eval_t {                                                                                 \
            template <class VisitorType> eval_t(VisitorType&& visitor) {                                \
                auto type = info_t::registry->all[info_t::name];                                        \
                if (type == nullptr) type = info_t::registry->add<info_t::type>(info_t::name);          \
                auto reflection = type->reflection;                                                     \
                visitor.template type<info_t::type>(*type);

#define CORE_REFLECTABLE(reflection_registry, ...)                                                      \
    CORE_REFLECTABLE_DECLARATION(reflection_registry, __VA_ARGS__)                                      \
    CORE_REFLECTABLE_BODY(reflection_registry, __VA_ARGS__)

// TODO: remove reflectable_name replace to typeid hash
#define CORE_CONDITIONAL_REFLECTABLE_DECLARATION(reflection_registry, ...)                              \
    namespace rew { namespace meta {                                                                    \
        template <typename T> struct reflection_info_t<T, std::enable_if_t<__VA_ARGS__>> {              \
            using type = T;                                                                             \
            inline static const auto registry = &reflection_registry;                                   \
        };                                                                                              \
    }}

#define CORE_CONDITIONAL_REFLECTABLE_BODY(reflection_registry, ...)                                     \
    template <typename T> struct rew_reflection_registry_t<T, std::enable_if_t<__VA_ARGS__>> {          \
        using info_t = ::rew::meta::reflection_info_t<T>;                                               \
        struct eval_t {                                                                                 \
            template <class VisitorType> eval_t(VisitorType&& visitor) {                                \
                static auto name = name_t<typename info_t::type>::value();                              \
                auto type = info_t::registry->all[name];                                                \
                if (type == nullptr) {                                                                  \
                    type = info_t::registry->template add<typename info_t::type>(name);                 \
                }                                                                                       \
                auto reflection = type->reflection;                                                     \
                visitor.template type<typename info_t::type>(*type);

#define CORE_CONDITIONAL_REFLECTABLE(reflection_registry, ...)                                          \
    CORE_CONDITIONAL_REFLECTABLE_DECLARATION(reflection_registry, __VA_ARGS__)                          \
    CORE_CONDITIONAL_REFLECTABLE_BODY(reflection_registry, __VA_ARGS__)

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
        CORE_REFLECTABLE_DECLARATION(::rew::global, __VA_ARGS__)
#endif // REFLECTABLE_DECLARATION

#ifndef REFLECTABLE
    #define REFLECTABLE(...)                                                                            \
        CORE_REFLECTABLE(::rew::global, __VA_ARGS__)
#endif // REFLECTABLE

#ifndef CONDITIONAL_REFLECTABLE
    #define CONDITIONAL_REFLECTABLE(...)                                                                \
        CORE_CONDITIONAL_REFLECTABLE(::rew::global, __VA_ARGS__)
#endif // REFLECTABLE

#ifndef REFLECTABLE_INIT
    #define REFLECTABLE_INIT(...)                                                                       \
        CORE_REFLECTABLE_INIT(__VA_RGS__)
#endif // REFLECTABLE_INIT

#endif // REW_REFLECTABLE_HPP
