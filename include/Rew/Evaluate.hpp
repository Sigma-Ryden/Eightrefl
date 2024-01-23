#ifndef REW_EVALUATE_HPP
#define REW_EVALUATE_HPP

#include <cstddef> // size_t

#include <string> // string
#include <any> // any

#include <functional> // function

#include <Rew/Reflection.hpp>
#include <Rew/Type.hpp>

namespace rew
{

struct visitor_t
{
    virtual ~visitor_t() = default;

    template <typename ReflectableType>
    void type(rew::type_t& type) {}

    template <typename ReflectableType, typename ParentReflectableType>
    void parent(rew::parent_t& parent) {}

    template <typename ReflectableType, typename FunctionType>
    void factory(rew::factory_t& factory) {}

    template <typename ReflectableType, typename FunctionType>
    void function(rew::function_t& function) {}

    template <typename ReflectableType, typename PropertyType>
    void property(rew::property_t& property) {}

    template <typename ReflectableType, typename MetaType>
    void meta(const std::string& name, std::any& meta) {}
};

namespace meta
{

template <std::size_t VisitorKey> struct visitor_traits;

} // namespace meta

struct evaluate_t
{
    const std::string name;
    const std::function<void(visitor_t& visitor)> call = nullptr;
};

namespace handler
{

template <typename ReflectionType, typename VisitorType>
auto visitor_call()
{
    return [](rew::visitor_t& visitor)
    {
        using reflection_registry = ::rew_reflection_registry_t<ReflectionType>;
        reflection_registry::evaluate(dynamic_cast<VisitorType&>(visitor));
    };
}

} // namespace handler

} // namespace rew

#endif // REW_EVALUATE_HPP
