#ifndef EIGHTREFL_INJECTION_HPP
#define EIGHTREFL_INJECTION_HPP

#include <string> // string
#include <any> // any
#include <functional> // function

#include <Eightrefl/Reflection.hpp>
#include <Eightrefl/Utility.hpp>

#ifndef EIGHTREFL_DEFAULT_INJECTION_COUNT
    #define EIGHTREFL_DEFAULT_INJECTION_COUNT 4
#endif // EIGHTREFL_INJECTION_MAX_KEY_SIZE

template <std::size_t InjectionIndexValue>
struct xxeightrefl_injection;

namespace eightrefl
{

struct type_t;
struct parent_t;
struct factory_t;
struct function_t;
struct property_t;
struct meta_t;
struct deleter_t;

struct injectable_t
{
    template <typename ReflectableType>
    void type(eightrefl::type_t& type) {}

    template <typename ReflectableType, typename ParentReflectableType>
    void parent(eightrefl::parent_t& parent) {}

    template <typename ReflectableType, typename FunctionType>
    void factory(eightrefl::factory_t& factory) {}

    template <typename ReflectableType, typename FunctionType>
    void function(eightrefl::function_t& function) {}

    template <typename ReflectableType, typename GetterType, typename SetterType>
    void property(eightrefl::property_t& property) {}

    template <typename ReflectableType, typename MetaType>
    void meta(eightrefl::meta_t& meta) {}

    template <typename ReflectableType, typename FunctionType>
    void deleter(eightrefl::deleter_t& deleter) {}
};

struct injection_t
{
    std::string const name;
    type_t* const type = nullptr;
    std::function<void(std::any const& injectable_context)> const call = nullptr;
};

template <typename ReflectionType, class InjectionType>
auto handler_injection_call()
{
    return [](std::any const& injectable_context)
    {
        ::xxeightrefl<ReflectionType>::evaluate(utility::forward<InjectionType&>(injectable_context));
    };
}

} // namespace eightrefl

#endif // EIGHTREFL_INJECTION_HPP
