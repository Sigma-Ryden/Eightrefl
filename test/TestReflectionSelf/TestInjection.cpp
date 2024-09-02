#include <TestReflectionSelf/TestInjection.hpp>

#include <Rew/BuiltIn/string.hpp>
#include <Rew/BuiltIn/any.hpp>
#include <Rew/BuiltIn/function.hpp>

REFLECTABLE(rew::injection_t)
    PROPERTY(name)
    PROPERTY(call)
REFLECTABLE_INIT()
