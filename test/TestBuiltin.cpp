#include <RewTestingBase.hpp>

void f() {}

int i = 8;

struct X{};

REFLECTABLE_DECLARATION(X)
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE(X)
    FREE_FUNCTION(f)
    FREE_PROPERTY(i)
REFLECTABLE_INIT()

TEST(TestLibrary, Test)
{
}
