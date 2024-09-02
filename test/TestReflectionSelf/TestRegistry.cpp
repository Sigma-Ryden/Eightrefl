#include <TestReflectionSelf/TestRegistry.hpp>

#include <TestReflectionSelf/TestType.hpp>

#include <Rew/BuiltIn/string.hpp>
#include <Rew/BuiltIn/typeindex.hpp>
#include <Rew/BuiltIn/unordered_map.hpp>

REFLECTABLE(rew::registry_t)
    FACTORY(rew::registry_t())
    FUNCTION(find, rew::type_t*(std::string const&) const)
    FUNCTION(find, rew::type_t*(std::type_index) const)
    PROPERTY(all)
    PROPERTY(rtti_all)
REFLECTABLE_INIT()
