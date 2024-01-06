#ifndef REW_UTILITY_HPP
#define REW_UTILITY_HPP

#include <cstddef> // size_t

#include <any> // any, any_cast
#include <utility> // reference_wrapper

#include <type_traits> // decay_t, enable_if_t, is_pointer_v, void_t, false_type, true_type

#include <Rew/Context.hpp>

#include <Rew/Detail/Meta.hpp>

namespace rew
{

namespace utility
{

template <typename ValueType>
ValueType argument_cast(const std::any& object)
{
    return std::any_cast<typename meta::argument_type_traits<ValueType>::type>(object);
}

template <typename ReflectableType>
auto context_access(std::any& context)
{
    return meta::context_traits_t<ReflectableType>::access(context);
}

template <typename ReflectableType, typename OtherReflectableType>
auto context_cast(std::any& context)
{
    return meta::context_traits_t<ReflectableType>::template cast<OtherReflectableType>(context);
}

template <typename... ArgumentTypes>
struct overload
{
    template <typename ClassType, typename ReturnType>
    static constexpr auto of(ReturnType (ClassType::* function)(ArgumentTypes...) const) { return function; }

    template <typename ClassType, typename ReturnType>
    static constexpr auto of(ReturnType (ClassType::* function)(ArgumentTypes...)) { return function; }

    template <typename ReturnType>
    static constexpr auto of(ReturnType (*function)(ArgumentTypes...)) { return function; }

    template <typename ClassType, typename... OtherArgumentTypes, typename ReturnType>
    static constexpr auto of(ReturnType (ClassType::* function)(OtherArgumentTypes...) const) { return function; }

    template <typename ClassType, typename... OtherArgumentTypes, typename ReturnType>
    static constexpr auto of(ReturnType (ClassType::* function)(OtherArgumentTypes...)) { return function; }

    template <typename... OtherArgumentTypes, typename ReturnType>
    static constexpr auto of(ReturnType (*function)(OtherArgumentTypes...)) { return function; }
};

template <typename... ArgumentTypes, typename ReturnType, class ClassType>
constexpr std::size_t function_arg_count(ReturnType (ClassType::* function)(ArgumentTypes...))
{
    return sizeof...(ArgumentTypes);
}

template <typename... ArgumentTypes, typename ReturnType, class ClassType>
constexpr std::size_t function_arg_count(ReturnType (ClassType::* function)(ArgumentTypes...) const)
{
    return sizeof...(ArgumentTypes);
}

template <typename... ArgumentTypes, typename ReturnType>
constexpr std::size_t function_arg_count(ReturnType (*function)(ArgumentTypes...))
{
    return sizeof...(ArgumentTypes);
}

template <typename ReflectableType, typename PropertyType>
PropertyType property_value(PropertyType ReflectableType::*);

template <typename ReflectableType, typename PropertyType>
PropertyType property_value(PropertyType (ReflectableType::*)(void) const);

template <typename ReflectableType, typename PropertyType>
PropertyType property_value(PropertyType (ReflectableType::*)(void));

template <typename ReflectableType, typename PropertyType>
PropertyType property_value();

template <typename ReflectableType>
void conditional_reflectable_register()
{
    if constexpr (meta::reflectable_traits_t<ReflectableType>::conditional)
    {
        (void)::rew_reflection_registry_t<ReflectableType>{}; // explicit instancing
    }
}

} // namespace utility

} // namespace rew

#endif // REW_UTILITY_HPP
