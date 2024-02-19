#ifndef REW_REFLECTION_HPP
#define REW_REFLECTION_HPP

#include <string> // string

#include <Rew/Parent.hpp>
#include <Rew/Function.hpp>
#include <Rew/Factory.hpp>
#include <Rew/Property.hpp>
#include <Rew/Meta.hpp>

namespace rew
{

struct reflection_t
{
    const std::string name;

    attribute_t<parent_t> parent;
    attribute_t<attribute_t<function_t>> function;
    attribute_t<factory_t> factory;
    attribute_t<property_t> property;
    attribute_t<std::any> meta;
};

} // namespace rew

#endif // REW_REFLECTION_HPP
