#include <RewTestingBase.hpp>

TEST(TestCommonBoolean, TestBool)
{
    auto type = rew::global.find("bool");

    ASSERT("type", type != nullptr);

    EXPECT("type-name", type->name == "bool");
    EXPECT("type-size", type->size == sizeof(bool));
    EXPECT("type-context", type->context != nullptr);

    auto reflection = type->reflection;
    ASSERT("reflection", reflection != nullptr);

    EXPECT("reflection-name", reflection->name == "bool");

    auto factory_bool = reflection->factory.find("bool()");
    EXPECT("factory-bool()", factory_bool != nullptr);

    auto factory_bool_bool = reflection->factory.find("bool(bool)");
    EXPECT("factory-bool(bool)", factory_bool_bool != nullptr);
}
