// TODO:
// replace reflection_registry_impl_t to normal name and hide to class registry_t
// add REFLECTABLE_ACCESS() macro for non-public registration
// add support for use custom visitor in reflectable
#ifndef REW_REFLECTABLE_HPP
#define REW_REFLECTABLE_HPP

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
                    auto type = info_t::registry->find_or_add<info_t::type>(info_t::name);              \
                    auto reflection = type->reflection;                                                 \
                    visitor.template type<info_t::type>(*type);

#define REFLECTABLE_INIT(...)                                                                           \
                }                                                                                       \
            };                                                                                          \
        private:                                                                                        \
            inline static auto _ = eval_t(::rew::visitor_t{});                                          \
        };                                                                                              \
    }

#define REFLECTABLE(...)                                                                                \
    CORE_REFLECTABLE(::rew::registry, __VA_ARGS__)

#endif // REW_REFLECTABLE_HPP
