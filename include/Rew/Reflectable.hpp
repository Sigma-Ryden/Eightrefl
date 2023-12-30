#ifndef REW_REFLECTABLE_HPP
#define REW_REFLECTABLE_HPP

#define CORE_REFLECTABLE(reflection_registry, ...)                                                      \
    namespace rew {                                                                                     \
        template <> struct reflection_info_t<__VA_ARGS__> {                                             \
            using type = __VA_ARGS__;                                                                   \
            inline static const auto registry = &reflection_registry;                                   \
            inline static const auto name = #__VA_ARGS__;                                               \
        };                                                                                              \
    }                                                                                                   \
    template <> struct rew_reflection_registry_t<__VA_ARGS__> {                                         \
        using info_t = ::rew::reflection_info_t<__VA_ARGS__>;                                           \
        struct eval_t {                                                                                 \
            template <class VisitorType> eval_t(VisitorType&& visitor) {                                \
                auto type = ::rew::find_or_add<info_t::type>(info_t::registry, info_t::name);           \
                auto reflection = type->reflection;                                                     \
                visitor.template type<info_t::type>(*type);

#define CORE_REFLECTABLE_INIT(...)                                                                      \
            }                                                                                           \
        };                                                                                              \
    private:                                                                                            \
        inline static auto _ = eval_t(::rew::visitor_t{});                                              \
    };                                                                                                  \

#define CORE_BUILTIN_REFLECTABLE(reflection_registry, ...)                                              \
    namespace rew { template <> struct builtin_reflection_info_t<__VA_ARGS__> {}; }                     \
    CORE_REFLECTABLE(reflection_registry, __VA_ARGS__)

#ifndef REFLECTABLE
    #define REFLECTABLE(...)                                                                            \
        CORE_REFLECTABLE(::rew::registry, __VA_ARGS__)
#endif // REFLECTABLE

#ifndef BUILTIN_REFLECTABLE
    #define BUILTIN_REFLECTABLE(...)                                                                    \
        CORE_BUILTIN_REFLECTABLE(::rew::registry, __VA_ARGS__)
#endif // BUILTIN_REFLECTABLE

#ifndef REFLECTABLE_INIT
    #define REFLECTABLE_INIT(...)                                                                       \
        CORE_REFLECTABLE_INIT(__VA_RGS__)
#endif // REFLECTABLE_INIT

#define REFLECTABLE_ACCESS(...)                                                                         \
    template <typename> friend struct rew_reflection_registry_t;

#endif // REW_REFLECTABLE_HPP
