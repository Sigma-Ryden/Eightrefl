#include <RewTestingBase.hpp>

TEST(TestCommonFloatingPoint, TestFloat)
{
    auto type = rew::global.find("float");

    ASSERT("type", type != nullptr);

    EXPECT("type-name", type->name == "float");
    EXPECT("type-size", type->size == sizeof(float));
    EXPECT("type-context", type->context != nullptr);

    auto reflection = type->reflection;
    ASSERT("reflection", reflection != nullptr);

    EXPECT("reflection-name", reflection->name == "float");

    auto factory_float = reflection->factory.find("float()");
    EXPECT("factory-float()", factory_float != nullptr);

    auto factory_float_float = reflection->factory.find("float(float)");
    EXPECT("factory-float(float)", factory_float_float != nullptr);
}

TEST(TestCommonFloatingPoint, TestDouble)
{
    auto type = rew::global.find("double");

    ASSERT("type", type != nullptr);

    EXPECT("type-name", type->name == "double");
    EXPECT("type-size", type->size == sizeof(double));
    EXPECT("type-context", type->context != nullptr);

    auto reflection = type->reflection;
    ASSERT("reflection", reflection != nullptr);

    EXPECT("reflection-name", reflection->name == "double");

    auto factory_double = reflection->factory.find("double()");
    EXPECT("factory-double()", factory_double != nullptr);

    auto factory_double_double = reflection->factory.find("double(double)");
    EXPECT("factory-double(double)", factory_double_double != nullptr);
}

TEST(TestCommonFloatingPoint, TestLongDouble)
{
    auto type = rew::global.find("long double");

    ASSERT("type", type != nullptr);

    EXPECT("type-name", type->name == "long double");
    EXPECT("type-size", type->size == sizeof(long double));
    EXPECT("type-context", type->context != nullptr);

    auto reflection = type->reflection;
    ASSERT("reflection", reflection != nullptr);

    EXPECT("reflection-name", reflection->name == "long double");

    auto factory_long_double = reflection->factory.find("long double()");
    EXPECT("factory-long double()", factory_long_double != nullptr);

    auto factory_long_double_long_double = reflection->factory.find("long double(long double)");
    EXPECT("factory-long double(long double)", factory_long_double_long_double != nullptr);
}
