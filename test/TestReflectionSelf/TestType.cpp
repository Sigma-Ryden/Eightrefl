#include <TestReflectionSelf/TestType.hpp>

#include <TestReflectionSelf/TestReflection.hpp>
#include <TestReflectionSelf/TestAttribute.hpp>
#include <TestReflectionSelf/TestInjection.hpp>

#include <Rew/BuiltIn/string.hpp>
#include <Rew/BuiltIn/any.hpp>
#include <Rew/BuiltIn/function.hpp>

REFLECTABLE(rew::type_t)
    PROPERTY(name)
    PROPERTY(reflection)
    PROPERTY(size)
    PROPERTY(context)
    PROPERTY(injection)
REFLECTABLE_INIT()
