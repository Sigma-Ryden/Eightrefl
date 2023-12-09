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

    parent_t parent;
    function_t function;
    factory_t factory;
    property_t property;
    meta_t meta;
};

} // namespace rew

#endif // REW_REFLECTION_HPP
