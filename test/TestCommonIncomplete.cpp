#include <RewTestingBase.hpp>

TEST(TestCommonIncomplete, TestVoid)
{
    auto type = rew::global.find("void");

    ASSERT("type", type != nullptr);

    EXPECT("type-name", type->name == "void");
    EXPECT("type-size", type->size == 0);
    EXPECT("type-context", type->context == nullptr);

    auto reflection = type->reflection;
    ASSERT("reflection", reflection != nullptr);

    EXPECT("reflection-name", reflection->name == "void");
}
