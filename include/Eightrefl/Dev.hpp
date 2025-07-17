#ifndef EIGHTREFL_DEV_HPP
#define EIGHTREFL_DEV_HPP

#ifdef EIGHTREFL_DEV_ENABLE
#include <Eightrefl/Reflectable.hpp>
#include <Eightrefl/Common.hpp>

#include <Eightrefl/BuiltIn/string.hpp>
#include <Eightrefl/BuiltIn/unordered_map.hpp>

namespace eightrefl
{

extern registry_t dev;

} // namespace eightrefl

TEMPLATE_REFLECTABLE_DECLARATION((template <typename MetaType>), eightrefl::attribute_t<MetaType>)
    REFLECTABLE_REGISTRY(&::eightrefl::dev)
    REFLECTABLE_NAME("eightrefl::attribute_t<" + eightrefl::name_of<MetaType>() + ">")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE((template <typename MetaType>), eightrefl::attribute_t<MetaType>)
    FUNCTION(find)
    FUNCTION(add)
    PROPERTY(all)
REFLECTABLE_INIT()

REFLECTABLE_DECLARATION(eightrefl::parent_t)
    REFLECTABLE_REGISTRY(&::eightrefl::dev)
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE_DECLARATION(eightrefl::factory_t)
    REFLECTABLE_REGISTRY(&::eightrefl::dev)
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE_DECLARATION(eightrefl::function_t)
    REFLECTABLE_REGISTRY(&::eightrefl::dev)
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE_DECLARATION(eightrefl::property_t)
    REFLECTABLE_REGISTRY(&::eightrefl::dev)
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE_DECLARATION(eightrefl::deleter_t)
    REFLECTABLE_REGISTRY(&::eightrefl::dev)
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE_DECLARATION(eightrefl::meta_t)
    REFLECTABLE_REGISTRY(&::eightrefl::dev)
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE_DECLARATION(eightrefl::reflection_t)
    REFLECTABLE_REGISTRY(&::eightrefl::dev)
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE_DECLARATION(eightrefl::injectable_t)
    REFLECTABLE_REGISTRY(&::eightrefl::dev)
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE_DECLARATION(eightrefl::injection_t)
    REFLECTABLE_REGISTRY(&::eightrefl::dev)
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE_DECLARATION(eightrefl::type_t)
    REFLECTABLE_REGISTRY(&::eightrefl::dev)
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE_DECLARATION(eightrefl::registry_t)
    REFLECTABLE_REGISTRY(&::eightrefl::dev)
REFLECTABLE_DECLARATION_INIT()
#endif // EIGHTREFL_DEV_ENABLE

#endif // EIGHTREFL_DEV_HPP
