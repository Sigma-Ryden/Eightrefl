#include <Eightrefl/Dev.hpp>

#ifdef EIGHTREFL_DEV_ENABLE
#include <Eightrefl/BuiltIn/string.hpp>
#include <Eightrefl/BuiltIn/any.hpp>
#include <Eightrefl/BuiltIn/function.hpp>
#include <Eightrefl/BuiltIn/vector.hpp>
#include <Eightrefl/BuiltIn/pair.hpp>
#include <Eightrefl/BuiltIn/typeindex.hpp>
#include <Eightrefl/BuiltIn/unordered_map.hpp>

namespace eightrefl
{

registry_t dev;

} // namespace eightrefl

REFLECTABLE(eightrefl::parent_t)
    PROPERTY(name)
    PROPERTY(type)
    PROPERTY(cast)
    PROPERTY(meta)
REFLECTABLE_INIT()

REFLECTABLE(eightrefl::factory_t)
    PROPERTY(name)
    PROPERTY(call)
    PROPERTY(arguments)
    PROPERTY(meta)
REFLECTABLE_INIT()

REFLECTABLE(eightrefl::function_t)
    PROPERTY(name)
    PROPERTY(call)
    PROPERTY(arguments)
    PROPERTY(result)
    PROPERTY(pointer)
    PROPERTY(meta)
REFLECTABLE_INIT()

REFLECTABLE(eightrefl::property_t)
    PROPERTY(name)
    PROPERTY(type)
    PROPERTY(get)
    PROPERTY(set)
    PROPERTY(context)
    PROPERTY(meta)
REFLECTABLE_INIT()

REFLECTABLE(eightrefl::deleter_t)
    PROPERTY(name)
    PROPERTY(call)
    PROPERTY(meta)
REFLECTABLE_INIT()

REFLECTABLE(eightrefl::meta_t)
    PROPERTY(name)
    PROPERTY(value)
    PROPERTY(meta)
REFLECTABLE_INIT()

REFLECTABLE(eightrefl::reflection_t)
    PROPERTY(name)
    PROPERTY(parent)
    PROPERTY(function)
    PROPERTY(factory)
    PROPERTY(property)
    PROPERTY(meta)
REFLECTABLE_INIT()

REFLECTABLE(eightrefl::injection_t)
    PROPERTY(name)
    PROPERTY(call)
REFLECTABLE_INIT()

REFLECTABLE(eightrefl::type_t)
    PROPERTY(name)
    PROPERTY(reflection)
    PROPERTY(registry)
    PROPERTY(size)
    PROPERTY(context)
    PROPERTY(injection)
REFLECTABLE_INIT()

REFLECTABLE(eightrefl::registry_t)
    FACTORY(eightrefl::registry_t())
    FUNCTION(find, eightrefl::type_t*(std::string const&) const)
    FUNCTION(find, eightrefl::type_t*(std::type_index) const)
    PROPERTY(all)
    PROPERTY(rtti_all)
REFLECTABLE_INIT()
#endif // EIGHTREFL_DEV_ENABLE
