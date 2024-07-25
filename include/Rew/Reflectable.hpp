#ifndef REW_REFLECTABLE_HPP
#define REW_REFLECTABLE_HPP

#include <cstddef> // size_t

#include <Rew/Registry.hpp>
#include <Rew/Injection.hpp>

#include <Rew/Detail/Meta.hpp>
#include <Rew/Detail/Macro.hpp>

#define CUSTOM_TEMPLATE_REFLECTABLE_DECLARATION(template_header, ...)                                   \
    namespace rew { namespace meta {                                                                    \
        REW_DEPAREN(template_header) struct reflectable_traits<__VA_ARGS__> {                           \
            using R = typename rew::meta::reflectable_using<__VA_ARGS__>::R;

#define CUSTOM_CONDITIONAL_REFLECTABLE_DECLARATION(...)                                                 \
    namespace rew { namespace meta {                                                                    \
        template <typename DirtyR> struct reflectable_traits<DirtyR, std::enable_if_t<__VA_ARGS__>> {   \
            using R = typename rew::meta::reflectable_using<DirtyR>::R;                                 \

#define CUSTOM_REFLECTABLE_DECLARATION(...)                                                             \
    namespace rew { namespace meta {                                                                    \
        template <> struct reflectable_traits<__VA_ARGS__> {                                            \
            using R = typename rew::meta::reflectable_using<__VA_ARGS__>::R;

#define TEMPLATE_REFLECTABLE_DECLARATION(template_header, ...)                                          \
    CUSTOM_TEMPLATE_REFLECTABLE_DECLARATION(template_header, __VA_ARGS__)                               \
        LAZY_REFLECTABLE()                                                                              \
        REFLECTABLE_REGISTRY(&rew::global)

#define CONDITIONAL_REFLECTABLE_DECLARATION(...)                                                        \
    CUSTOM_CONDITIONAL_REFLECTABLE_DECLARATION(__VA_ARGS__)                                             \
        LAZY_REFLECTABLE()                                                                              \
        REFLECTABLE_REGISTRY(&rew::global)

#define REFLECTABLE_DECLARATION(...)                                                                    \
    CUSTOM_REFLECTABLE_DECLARATION(__VA_ARGS__)                                                         \
        REFLECTABLE_REGISTRY(&rew::global)                                                              \
        REFLECTABLE_NAME(#__VA_ARGS__)

#define REFLECTABLE_REGISTRY(...)  static auto registry() { return __VA_ARGS__; }
#define REFLECTABLE_NAME(...) static auto name() { return __VA_ARGS__; }
#define LAZY_REFLECTABLE(...) static auto lazy() { __VA_ARGS__ }
#define BUILTIN_REFLECTABLE(...) static auto builtin() { __VA_ARGS__ }

#define REFLECTABLE_DECLARATION_INIT(...)                                                               \
        };                                                                                              \
    }}

#define REW_REFLECTABLE_BODY()                                                                          \
    template <class InjectionType>                                                                      \
    static void evaluate(InjectionType&& injection) {                                                   \
        auto xxtype = rew::find_or_add_type<R>();                                                       \
        auto xxreflection = xxtype->reflection; (void)xxreflection;                                     \
        injection.template type<R>(*xxtype);

#define TEMPLATE_REFLECTABLE(template_header, ...)                                                      \
    REW_DEPAREN(template_header) struct xxrew<__VA_ARGS__> {                                            \
        using R = __VA_ARGS__;                                                                          \
        using CleanR = typename rew::meta::reflectable_traits<R>::R;                                    \
        REW_REFLECTABLE_BODY()

#define CONDITIONAL_REFLECTABLE(...)                                                                    \
    template <typename R> struct xxrew<R, std::enable_if_t<__VA_ARGS__>> {                              \
        using CleanR = typename rew::meta::reflectable_traits<R>::R;                                    \
        REW_REFLECTABLE_BODY()

#define REFLECTABLE(...)                                                                                \
    template <> struct xxrew<__VA_ARGS__> {                                                             \
        using R = __VA_ARGS__;                                                                          \
        using CleanR = typename rew::meta::reflectable_traits<R>::R;                                    \
        REW_REFLECTABLE_BODY()

#define REFLECTABLE_INIT(...)                                                                           \
            rew::add_default_injection_set<R>(xxtype);                                                  \
        }                                                                                               \
    private:                                                                                            \
        inline static auto xxautogenerated = (evaluate(rew::injectable_t{}), true);                     \
    };


#define CUSTOM_REFLECTABLE_INJECTION_DECLARATION(injection_index, ...)                                  \
    namespace rew { namespace meta {                                                                    \
        template <> struct injection_traits<injection_index> { using type = __VA_ARGS__; };             \
    }}                                                                                                  \
    CUSTOM_REFLECTABLE_DECLARATION(__VA_ARGS__)                                                         \

#define REFLECTABLE_INJECTION_DECLARATION(injection_index, ...)                                         \
    CUSTOM_REFLECTABLE_INJECTION_DECLARATION(injection_index, __VA_ARGS__)                              \
        REFLECTABLE_NAME(#__VA_ARGS__)

#define TEMPLATE_REFLECTABLE_CLEAN(template_header, reflectable_clean, ...)                             \
   namespace rew { namespace meta {                                                                     \
        REW_DEPAREN(template_header) struct reflectable_using<REW_DEPAREN(reflectable_clean)> {         \
            using R = __VA_ARGS__;                                                                      \
        };                                                                                              \
    }}

#define REFLECTABLE_CLEAN(reflectable_clean, ...)                                                       \
    namespace rew { namespace meta {                                                                    \
        template <> struct reflectable_using<reflectable_clean> {                                       \
            using R = __VA_ARGS__;                                                                      \
        };                                                                                              \
    }}

#define TEMPLATE_REFLECTABLE_USING(template_header, reflectable_using, reflectable_using_full, ...)     \
    REW_DEPAREN(template_header)                                                                        \
    struct reflectable_using : rew::meta::reflectable_using_base<__VA_ARGS__> {};                       \
    TEMPLATE_REFLECTABLE_CLEAN(template_header, reflectable_using_full, __VA_ARGS__)

#define REFLECTABLE_USING(reflectable_using, ...)                                                       \
    struct reflectable_using : rew::meta::reflectable_using_base<__VA_ARGS__> {};                       \
    REFLECTABLE_CLEAN(reflectable_using, __VA_ARGS__)

#define REFLECTABLE_ACCESS(...) template <typename, typename> friend struct xxrew;

namespace rew
{

template <typename ReflectableType>
std::string nameof()
{
    return meta::reflectable_traits<ReflectableType>::name();
}

template <typename ReflectableType>
using cleanof = meta::reflectable_using_t<ReflectableType>;

template <typename ReflectableType>
void reflectable()
{
    static auto locked = false;
    if (!locked)
    {
        locked = true;
        ::xxrew<ReflectableType>::evaluate(injectable_t{});
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
    using dirty_reflectable_type = typename std::conditional_t
    <
        std::is_reference_v<DirtyReflectableType>,
        meta::to_reflectable_reference<DirtyReflectableType>,
        std::conditional_t
        <
            std::is_pointer_v<DirtyReflectableType>,
            meta::to_reflectable_pointer<DirtyReflectableType>,
            meta::to_reflectable_object<DirtyReflectableType>
        >
    >::type;

    using reflectable_type = typename meta::reflectable_using<dirty_reflectable_type>::R;
    using reflectable_traits = meta::reflectable_traits<dirty_reflectable_type>;

    if constexpr (meta::is_lazy_reflectable<dirty_reflectable_type>::value)
    {
        reflectable<dirty_reflectable_type>();
    }

    auto xxname = reflectable_traits::name();
    auto xxregistry = reflectable_traits::registry();

    auto xxtype = xxregistry->all[xxname];
    if (xxtype == nullptr)
    {
        xxtype = xxregistry->template add<reflectable_type, dirty_reflectable_type>(xxname);
    }

    return xxtype;
}

template <typename ReflectableType, typename ParentReflectableType>
parent_t* find_or_add_parent(reflection_t* reflection)
{
    static_assert(std::is_base_of_v<ParentReflectableType, ReflectableType>);

    using reflectable_traits = meta::reflectable_traits<ParentReflectableType>;

    auto xxname = reflectable_traits::name();

    auto xxmeta = reflection->parent.find(xxname);
    if (xxmeta == nullptr) xxmeta = &reflection->parent.add
    (
        xxname,
        {
            xxname,
            find_or_add_type<ParentReflectableType>(),
            handler_parent_cast<ReflectableType, ParentReflectableType>()
        }
    );

    return xxmeta;
}

namespace detail
{

template <typename... ArgumentTypes, typename ReturnType>
auto function_argument_types(ReturnType (*unused)(ArgumentTypes...))
{
    return std::vector<type_t*>({ find_or_add_type<ArgumentTypes>()... });
}

template <typename... ArgumentTypes, typename ReturnType>
auto function_return_type(ReturnType (*unused)(ArgumentTypes...))
{
    return find_or_add_type<ReturnType>();
}

} // namespace detail

template <typename DirtyFactoryType>
factory_t* find_or_add_factory(reflection_t* reflection)
{
    using function_traits = meta::function_traits<DirtyFactoryType>;
    using dirty_type = typename function_traits::dirty_type;
    using dirty_pointer = typename function_traits::dirty_pointer;
    using pointer = typename function_traits::pointer;

    auto xxname = meta::reflectable_traits<dirty_type>::name();

    auto xxmeta = reflection->factory.find(xxname);
    if (xxmeta == nullptr) xxmeta = &reflection->factory.add
    (
        xxname,
        {
            xxname,
            handler_factory_call(pointer{}),
            detail::function_argument_types(dirty_pointer{}),
            detail::function_return_type(dirty_pointer{})
        }
    );

    return xxmeta;
}

template <typename DirtyFunctionType = void, typename FunctionType>
function_t* find_or_add_function(reflection_t* reflection, std::string const& name, FunctionType ptr)
{
    using function_traits = meta::function_traits
    <
        std::conditional_t<std::is_void_v<DirtyFunctionType>, FunctionType, DirtyFunctionType>
    >;

    using dirty_type = typename function_traits::dirty_type;
    using dirty_pointer = typename function_traits::dirty_pointer;

    auto xxfunction = reflection->function.find(name);
    if (xxfunction == nullptr) xxfunction = &reflection->function.add(name, {});

    auto xxoverload = meta::reflectable_traits<dirty_type>::name();

    auto xxmeta = xxfunction->find(xxoverload);
    if (xxmeta == nullptr) xxmeta = &xxfunction->add
    (
        xxoverload,
        {
            xxoverload,
            handler_function_call(ptr),
            detail::function_argument_types(dirty_pointer{}),
            detail::function_return_type(dirty_pointer{}),
            ptr
        }
    );

    return xxmeta;
}

template <typename DirtyPropertyType = void, typename GetterType, typename SetterType>
property_t* find_or_add_property(reflection_t* reflection, std::string const& name,
                                 GetterType getter, SetterType setter)
{
    using property_traits = meta::property_traits
    <
        std::conditional_t<std::is_void_v<DirtyPropertyType>, GetterType, DirtyPropertyType>
    >;

    using type = typename property_traits::type;

    auto xxmeta = reflection->property.find(name);
    if (xxmeta == nullptr) xxmeta = &reflection->property.add
    (
        name,
        {
            name,
            find_or_add_type<type>(),
            handler_property_get(getter),
            handler_property_set(setter),
            handler_property_context(getter),
            property_pointer(getter, setter)
        }
    );

    return xxmeta;
}

template <typename MetaType>
std::any* find_or_add_meta(reflection_t* reflection, std::string const& name, const MetaType& data)
{
    auto xxmeta = reflection->meta.find(name);
    if (xxmeta == nullptr) xxmeta = &reflection->meta.add(name, data);

    return xxmeta;
}

template <typename ReflectableType, class InjectionType>
injection_t* find_or_add_injection(type_t* type)
{
    static_assert(std::is_base_of_v<injectable_t, InjectionType>);

    using reflectable_injection_traits = meta::reflectable_traits<InjectionType>;

    auto xxname = reflectable_injection_traits::name();

    auto xxmeta = type->injection.find(xxname);
    if (xxmeta == nullptr) xxmeta = &type->injection.add
    (
        xxname,
        {
            xxname,
            handler_injection_call<ReflectableType, InjectionType>()
        }
    );

    return xxmeta;
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
