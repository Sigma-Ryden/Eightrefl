#include <TestReflectionSelf/TestParent.hpp>

#include <TestReflectionSelf/TestType.hpp>
#include <TestReflectionSelf/TestAttribute.hpp>
#include <TestReflectionSelf/TestMeta.hpp>

#include <Rew/BuiltIn/string.hpp>
#include <Rew/BuiltIn/any.hpp>
#include <Rew/BuiltIn/function.hpp>

REFLECTABLE(rew::parent_t)
    PROPERTY(name)
    PROPERTY(type)
    PROPERTY(cast)
    PROPERTY(meta)
REFLECTABLE_INIT()
