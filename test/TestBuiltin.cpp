#include <RewTestingBase.hpp>

void f() {}

struct X{};

REFLECTABLE_DECLARATION(X)
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE(X)
    GLOBAL_FUNCTION(f)
REFLECTABLE_INIT()

TEST(TestLibrary, Test)
{
    (void)rew::global.find("X");
}
