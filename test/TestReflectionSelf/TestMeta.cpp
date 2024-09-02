#include <TestReflectionSelf/TestMeta.hpp>

#include <TestReflectionSelf/TestAttribute.hpp>

#include <Rew/BuiltIn/string.hpp>
#include <Rew/BuiltIn/any.hpp>

REFLECTABLE(rew::meta_t)
    PROPERTY(name)
    PROPERTY(value)
    PROPERTY(meta)
REFLECTABLE_INIT()
