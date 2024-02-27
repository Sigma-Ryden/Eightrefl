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
    static int s_i = 123;

    auto type = rew::global.find("X");
    auto property = type->reflection->property.find("i");

    std::any context;
    property->set(context, s_i);

    std::any value;
    property->get(context, value);

    EXPECT("property-value", std::any_cast<int>(value) == s_i);
}
