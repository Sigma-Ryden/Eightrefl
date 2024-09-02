#include <TestReflectionSelf/TestProperty.hpp>

#include <TestReflectionSelf/TestType.hpp>
#include <TestReflectionSelf/TestAttribute.hpp>
#include <TestReflectionSelf/TestMeta.hpp>

#include <Rew/BuiltIn/string.hpp>
#include <Rew/BuiltIn/any.hpp>
#include <Rew/BuiltIn/function.hpp>
#include <Rew/BuiltIn/pair.hpp>

REFLECTABLE(rew::property_t)
    PROPERTY(name)
    PROPERTY(type)
    PROPERTY(get)
    PROPERTY(set)
    PROPERTY(context)
    PROPERTY(meta)
REFLECTABLE_INIT()
