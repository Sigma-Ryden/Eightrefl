#include <TestReflectionSelf/TestFunction.hpp>

#include <TestReflectionSelf/TestType.hpp>
#include <TestReflectionSelf/TestAttribute.hpp>
#include <TestReflectionSelf/TestMeta.hpp>

#include <Rew/BuiltIn/string.hpp>
#include <Rew/BuiltIn/any.hpp>
#include <Rew/BuiltIn/function.hpp>
#include <Rew/BuiltIn/vector.hpp>

REFLECTABLE(rew::function_t)
    PROPERTY(name)
    PROPERTY(call)
    PROPERTY(arguments)
    PROPERTY(result)
    PROPERTY(pointer)
    PROPERTY(meta)
REFLECTABLE_INIT()
