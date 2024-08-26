#ifndef REW_INJECTION_HPP
#define REW_INJECTION_HPP

#include <string> // string
#include <any> // any

#include <functional> // function

#include <Rew/Reflection.hpp>

template <std::size_t InjectionIndexValue>
struct xxrew_injection;

namespace rew
{

struct type_t;
struct parent_t;
struct factory_t;
struct function_t;
struct property_t;

struct injectable_t
{
    virtual ~injectable_t() = default;

    template <typename ReflectableType>
    void type(rew::type_t& type) {}

    template <typename ReflectableType, typename ParentReflectableType>
    void parent(rew::parent_t& parent) {}

    template <typename ReflectableType, typename FunctionType>
    void factory(rew::factory_t& factory) {}

    template <typename ReflectableType, typename FunctionType>
    void function(rew::function_t& function) {}

    template <typename ReflectableType, typename GetterType, typename SetterType>
    void property(rew::property_t& property) {}

    template <typename ReflectableType, typename MetaType>
    void meta(rew::meta_t& meta) {}
};

struct injection_t
{
    std::string const name;
    std::function<void(injectable_t& injection)> const call = nullptr;
};

template <typename ReflectionType, class InjectionType>
auto handler_injection_call()
{
    return [](injectable_t& injection)
    {
        ::xxrew<ReflectionType>::evaluate(static_cast<InjectionType&>(injection));
    };
}

} // namespace rew

#endif // REW_INJECTION_HPP
