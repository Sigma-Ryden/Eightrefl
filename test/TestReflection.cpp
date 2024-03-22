#include <RewTestingBase.hpp>

TEST(TestReflection, TestSelfReflection)
{
}

struct TestReflectionProperty
{
    int f() { return 0; }
    void f(int) {}

    const int g() { return 0; }

    const int i = 0;

    int j = 0;

    void boo(const void *const& p) {}
};

REFLECTABLE_DECLARATION(TestReflectionProperty)
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE(TestReflectionProperty)
    PROPERTY(f)
    PROPERTY(g)
    PROPERTY(i)
    PROPERTY(j)
    FUNCTION(boo)
REFLECTABLE_INIT()

TEST(TestReflection, TestProperty)
{
}
