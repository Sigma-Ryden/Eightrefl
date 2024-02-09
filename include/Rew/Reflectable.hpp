#ifndef REW_REFLECTABLE_HPP
#define REW_REFLECTABLE_HPP

#include <Rew/Registry.hpp>
#include <Rew/Injection.hpp>

#include <Rew/Detail/Meta.hpp>

#define NAMEOF(...)                                                                                     \
    (::rew::meta::reflectable_traits<__VA_ARGS__>::name())

#define CORE_REFLECTABLE_BODY()                                                                         \
        template <class InjectionType>                                                                  \
        static void evaluate(InjectionType&& injection) {                                               \
            auto __type = ::rew::find_or_add_type<DirtyR>();                                            \
            auto __reflection = __type->reflection;                                                     \
            injection.template type<R>(*__type);

#define __REW_EXPAND(...) __VA_ARGS__

#define TEMPLATE_REFLECTABLE_USING(template_header, reflectable_using, ...)                             \
   __REW_EXPAND template_header struct reflectable_using {                                              \
        using __rew_reflectable_using = __VA_ARGS__;                                                    \
    };

#define REFLECTABLE_USING(reflectable_using, ...)                                                       \
   struct reflectable_using {                                                                           \
        using __rew_reflectable_using = __VA_ARGS__;                                                    \
    };

#define CORE_TEMPLATE_REFLECTABLE_DECLARATION(template_header, ...)                                     \
    namespace rew { namespace meta {                                                                    \
        __REW_EXPAND template_header                                                                    \
        struct reflectable_traits<__VA_ARGS__> {                                                        \
            using R = typename ::rew::meta::reflectable_using<__VA_ARGS__>::R;                          \
            LAZY_REFLECTABLE()

#define CORE_CONDITIONAL_REFLECTABLE_DECLARATION(...)                                                   \
    namespace rew { namespace meta {                                                                    \
        template <typename DirtyR>                                                                      \
        struct reflectable_traits<DirtyR, std::enable_if_t<__VA_ARGS__>> {                              \
            using R = typename ::rew::meta::reflectable_using<DirtyR>::R;                               \
            LAZY_REFLECTABLE()

#define CORE_REFLECTABLE_DECLARATION(...)                                                               \
    namespace rew { namespace meta {                                                                    \
        template <> struct reflectable_traits<__VA_ARGS__> {                                            \
            using R = typename ::rew::meta::reflectable_using<__VA_ARGS__>::R;

#define REFLECTABLE_INJECTION_DECLARATION(injection_index, ...)                                         \
    namespace rew { namespace meta {                                                                    \
        template <> struct injection_traits<injection_index> { using type = __VA_ARGS__; };             \
    }}                                                                                                  \
    CORE_REFLECTABLE_DECLARATION(__VA_ARGS__)                                                           \
    REFLECTABLE_NAME(#__VA_ARGS__)

#define REFLECTABLE_REGISTRY(...)                                                                       \
    static auto registry() { return __VA_ARGS__; }

#define REFLECTABLE_NAME(...)                                                                           \
    static std::string name() { return __VA_ARGS__; }

#define LAZY_REFLECTABLE(...)                                                                           \
    static auto lazy() { __VA_ARGS__ }

#define BUILTIN_REFLECTABLE(...)                                                                        \
    static auto builtin() { __VA_ARGS__ }

#define REFLECTABLE_DECLARATION_INIT(...)                                                               \
        };                                                                                              \
    }}

#define CORE_TEMPLATE_REFLECTABLE(template_header, ...)                                                 \
    __REW_EXPAND template_header                                                                        \
    struct rew_reflection_registry_t<__VA_ARGS__> {                                                     \
        using DirtyR = __VA_ARGS__;                                                                     \
        using R = typename ::rew::meta::reflectable_traits<DirtyR>::R;                                  \
        CORE_REFLECTABLE_BODY()

#define CORE_CONDITIONAL_REFLECTABLE(...)                                                               \
    template <typename DirtyR>                                                                          \
    struct rew_reflection_registry_t<DirtyR, std::enable_if_t<__VA_ARGS__>> {                           \
        using R = typename ::rew::meta::reflectable_traits<DirtyR>::R;                                  \
        CORE_REFLECTABLE_BODY()

#define CORE_REFLECTABLE(...)                                                                           \
    template <> struct rew_reflection_registry_t<__VA_ARGS__> {                                         \
        using DirtyR = __VA_ARGS__;                                                                     \
        using R = typename ::rew::meta::reflectable_traits<DirtyR>::R;                                  \
        CORE_REFLECTABLE_BODY()

#define CORE_REFLECTABLE_INIT(...)                                                                      \
            ::rew::add_default_injection_set<R>(__type);                                                \
        }                                                                                               \
    private:                                                                                            \
        inline static auto _ = (evaluate(::rew::injectable_t{}), true);                                 \
    };                                                                                                  \

#define REFLECTABLE_ACCESS(...)                                                                         \
    template <typename, typename> friend struct rew_reflection_registry_t;

#ifndef TEMPLATE_REFLECTABLE_DECLARATION
    #define TEMPLATE_REFLECTABLE_DECLARATION(template_header, ...)                                      \
        CORE_TEMPLATE_REFLECTABLE_DECLARATION(template_header, __VA_ARGS__)                             \
        REFLECTABLE_REGISTRY(&::rew::global)
#endif // CONDITIONAL_REFLECTABLE_DECLARATION

#ifndef CONDITIONAL_REFLECTABLE_DECLARATION
    #define CONDITIONAL_REFLECTABLE_DECLARATION(...)                                                    \
        CORE_CONDITIONAL_REFLECTABLE_DECLARATION(__VA_ARGS__)                                           \
        REFLECTABLE_REGISTRY(&::rew::global)
#endif // CONDITIONAL_REFLECTABLE_DECLARATION

#ifndef REFLECTABLE_DECLARATION
    #define REFLECTABLE_DECLARATION(...)                                                                \
        CORE_REFLECTABLE_DECLARATION(__VA_ARGS__)                                                       \
        REFLECTABLE_REGISTRY(&::rew::global)                                                            \
        REFLECTABLE_NAME(#__VA_ARGS__)
#endif // REFLECTABLE_DECLARATION

#ifndef TEMPLATE_REFLECTABLE
    #define TEMPLATE_REFLECTABLE(template_header, ...)                                                  \
        CORE_TEMPLATE_REFLECTABLE(template_header, __VA_ARGS__)
#endif // CONDITIONAL_REFLECTABLE

#ifndef CONDITIONAL_REFLECTABLE
    #define CONDITIONAL_REFLECTABLE(...) CORE_CONDITIONAL_REFLECTABLE(__VA_ARGS__)
#endif // CONDITIONAL_REFLECTABLE

#ifndef REFLECTABLE
    #define REFLECTABLE(...) CORE_REFLECTABLE(__VA_ARGS__)
#endif // REFLECTABLE

#ifndef REFLECTABLE_INIT
    #define REFLECTABLE_INIT(...) CORE_REFLECTABLE_INIT(__VA_RGS__)
#endif // REFLECTABLE_INIT

namespace rew
{

template <typename ReflectableType>
void reflectable()
{
    static auto locked = false;
    if (!locked)
    {
        locked = true;
        ::rew_reflection_registry_t<ReflectableType>::evaluate(injectable_t{});
    }
}

template <typename ReflectableType>
ReflectableType&& reflectable(ReflectableType&& object)
{
    reflectable<std::decay_t<ReflectableType>>();
    return std::forward<ReflectableType>(object);
}

template <typename DirtyReflectableType>
type_t* find_or_add_type()
{
    using dirty_reflectable_type = std::decay_t<DirtyReflectableType>;
    using reflectable_type = typename meta::reflectable_using<dirty_reflectable_type>::R;
    using reflectable_traits = meta::reflectable_traits<dirty_reflectable_type>;

    if constexpr (meta::is_lazy_reflectable<dirty_reflectable_type>::value)
    {
        reflectable<dirty_reflectable_type>();
    }

    auto __name = reflectable_traits::name();
    auto __registry = reflectable_traits::registry();

    auto __type = __registry->all[__name];
    if (__type == nullptr) __type = __registry->template add<reflectable_type>(__name);

    return __type;
}

template <typename ReflectableType, typename ParentReflectableType>
parent_t* find_or_add_parent(reflection_t* reflection)
{
    using reflectable_traits = meta::reflectable_traits<ParentReflectableType>;
    static_assert(std::is_base_of_v<ParentReflectableType, ReflectableType>);

    auto __name = reflectable_traits::name();

    auto __meta = reflection->parent.find(__name);
    if (__meta == nullptr) __meta = &reflection->parent.add
    (
        __name,
        {
            __name,
            find_or_add_type<ParentReflectableType>(),
            handler::parent_cast<ReflectableType, ParentReflectableType>()
        }
    );

    return __meta;
}

namespace detail
{

template <typename... ArgumentTypes, typename ReturnType>
auto function_argument_types(ReturnType (*unused)(ArgumentTypes...))
{
    return std::vector<type_t*>({ find_or_add_type<ArgumentTypes>()... });
}

} // namespace detail

template <typename DirtyFactoryType>
factory_t* find_or_add_factory(reflection_t* reflection)
{
    using function_traits = meta::function_traits<DirtyFactoryType>;
    using dirty_function_pointer = typename function_traits::dirty_function_pointer;
    using function_pointer = typename function_traits::function_pointer;

    auto __name = utility::full_function_name(dirty_function_pointer{});

    auto __meta = reflection->factory.find(__name);
    if (__meta == nullptr) __meta = &reflection->factory.add
    (
        __name,
        {
            __name,
            handler::factory_call(function_pointer{}),
            detail::function_argument_types(dirty_function_pointer{})
        }
    );

    return __meta;
}

template <typename DirtyFunctionType = void, typename FunctionType>
function_t* find_or_add_function(reflection_t* reflection, const std::string& name, FunctionType ptr)
{
    using function_traits = meta::function_traits
    <
        std::conditional_t<std::is_void_v<DirtyFunctionType>, FunctionType, DirtyFunctionType>
    >;

    using dirty_function_pointer = typename function_traits::dirty_function_pointer;
    using dirty_return_type = typename function_traits::dirty_return_type;

    auto __function = reflection->function.find(name);
    if (__function == nullptr) __function = &reflection->function.add(name, {});

    auto __overload = utility::full_function_name<function_traits::is_const>(dirty_function_pointer{});

    auto __meta = __function->find(__overload);
    if (__meta == nullptr) __meta = &__function->add
    (
        __overload,
        {
            __overload,
            handler::function_call(ptr),
            detail::function_argument_types(dirty_function_pointer{}),
            find_or_add_type<dirty_return_type>()
        }
    );

    return __meta;
}

template <typename DirtyPropertyGetterType, typename DirtyPropertySetterType>
property_t* find_or_add_property(reflection_t* reflection, const std::string& name,
                                 DirtyPropertyGetterType getter, DirtyPropertySetterType setter)
{
    using property_traits = meta::property_traits<DirtyPropertyGetterType>;
    using dirty_property_type = typename property_traits::dirty_property_type;

    auto __meta = reflection->property.find(name);
    if (__meta == nullptr) __meta = &reflection->property.add
    (
        name,
        {
            name,
            find_or_add_type<dirty_property_type>(),
            handler::property_get(getter),
            handler::property_set(setter),
            handler::property_ptr(getter)
        }
    );

    return __meta;
}

template <typename MetaType>
std::any* find_or_add_meta(reflection_t* reflection, const std::string& name, MetaType& data)
{
    auto __meta = reflection->meta.find(name);
    if (__meta == nullptr) __meta = &reflection->meta.add(name, data);

    return __meta;
}

template <typename ReflectableType, class InjectionType>
injection_t* find_or_add_injection(type_t* type)
{
    using reflectable_injection_traits = meta::reflectable_traits<InjectionType>;

    auto __name = reflectable_injection_traits::name();

    auto __meta = type->injection.find(__name);
    if (__meta == nullptr) __meta = &type->injection.add
    (
        __name,
        {
            __name,
            handler::injection_call<ReflectableType, InjectionType>()
        }
    );

    return __meta;
}

template <typename ReflectableType, std::size_t CurrentKey = 0, std::size_t MaxKey = 4>
void add_default_injection_set(type_t* type)
{
    if constexpr (meta::is_complete<meta::injection_traits<CurrentKey>>::value)
    {
        using injection_traits = meta::injection_traits<CurrentKey>;

        find_or_add_injection<ReflectableType, typename injection_traits::type>(type);
        if constexpr (CurrentKey < MaxKey)
        {
            add_default_injection_set<ReflectableType, CurrentKey + 1>(type);
        }
    }
}

} // namespace rew

#endif // REW_REFLECTABLE_HPP
