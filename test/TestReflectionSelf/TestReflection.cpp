#include <TestReflectionSelf/TestReflection.hpp>

#include <TestReflectionSelf/TestAttribute.hpp>
#include <TestReflectionSelf/TestParent.hpp>
#include <TestReflectionSelf/TestFunction.hpp>
#include <TestReflectionSelf/TestFactory.hpp>
#include <TestReflectionSelf/TestProperty.hpp>
#include <TestReflectionSelf/TestMeta.hpp>

#include <Rew/BuiltIn/string.hpp>

REFLECTABLE(rew::reflection_t)
    PROPERTY(name)
    PROPERTY(parent)
    PROPERTY(function)
    PROPERTY(factory)
    PROPERTY(property)
    PROPERTY(meta)
REFLECTABLE_INIT()
