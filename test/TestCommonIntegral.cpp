#include <RewTestingBase.hpp>

TEST(TestCommonIntegral, TestSignedChar)
{
    auto type = rew::global.find("signed char");

    ASSERT("type", type != nullptr);

    EXPECT("type-name", type->name == "signed char");
    EXPECT("type-size", type->size == sizeof(signed char));
    EXPECT("type-context", type->context != nullptr);

    auto reflection = type->reflection;
    ASSERT("reflection", reflection != nullptr);

    EXPECT("reflection-name", reflection->name == "signed char");

    auto factory_signed_char = reflection->factory.find("signed char()");
    EXPECT("factory-signed char()", factory_signed_char != nullptr);

    auto factory_signed_char_signed_char = reflection->factory.find("signed char(signed char)");
    EXPECT("factory-signed char(signed char)", factory_signed_char_signed_char != nullptr);
}

TEST(TestCommonIntegral, TestUnsignedChar)
{
    auto type = rew::global.find("unsigned char");

    ASSERT("type", type != nullptr);

    EXPECT("type-name", type->name == "unsigned char");
    EXPECT("type-size", type->size == sizeof(unsigned char));
    EXPECT("type-context", type->context != nullptr);

    auto reflection = type->reflection;
    ASSERT("reflection", reflection != nullptr);

    EXPECT("reflection-name", reflection->name == "unsigned char");

    auto factory_unsigned_char = reflection->factory.find("unsigned char()");
    EXPECT("factory-unsigned char()", factory_unsigned_char != nullptr);

    auto factory_unsigned_char_unsigned_char = reflection->factory.find("unsigned char(unsigned char)");
    EXPECT("factory-unsigned char(unsigned char)", factory_unsigned_char_unsigned_char != nullptr);
}

TEST(TestCommonIntegral, TestInt)
{
    auto type = rew::global.find("int");

    ASSERT("type", type != nullptr);

    EXPECT("type-name", type->name == "int");
    EXPECT("type-size", type->size == sizeof(int));
    EXPECT("type-context", type->context != nullptr);

    auto reflection = type->reflection;
    ASSERT("reflection", reflection != nullptr);

    EXPECT("reflection-name", reflection->name == "int");

    auto factory_int = reflection->factory.find("int()");
    EXPECT("factory-int()", factory_int != nullptr);

    auto factory_int_int = reflection->factory.find("int(int)");
    EXPECT("factory-int(int)", factory_int_int != nullptr);
}

TEST(TestCommonIntegral, TestUnsignedInt)
{
    auto type = rew::global.find("unsigned int");

    ASSERT("type", type != nullptr);

    EXPECT("type-name", type->name == "unsigned int");
    EXPECT("type-size", type->size == sizeof(unsigned int));
    EXPECT("type-context", type->context != nullptr);

    auto reflection = type->reflection;
    ASSERT("reflection", reflection != nullptr);

    EXPECT("reflection-name", reflection->name == "unsigned int");

    auto factory_unsigned_int = reflection->factory.find("unsigned int()");
    EXPECT("factory-unsigned int()", factory_unsigned_int != nullptr);

    auto factory_unsigned_int_unsigned_int = reflection->factory.find("unsigned int(unsigned int)");
    EXPECT("factory-unsigned int(unsigned int)", factory_unsigned_int_unsigned_int != nullptr);
}

TEST(TestCommonIntegral, TestLong)
{
    auto type = rew::global.find("long");

    ASSERT("type", type != nullptr);

    EXPECT("type-name", type->name == "long");
    EXPECT("type-size", type->size == sizeof(long));
    EXPECT("type-context", type->context != nullptr);

    auto reflection = type->reflection;
    ASSERT("reflection", reflection != nullptr);

    EXPECT("reflection-name", reflection->name == "long");

    auto factory_long = reflection->factory.find("long()");
    EXPECT("factory-long()", factory_long != nullptr);

    auto factory_long_long = reflection->factory.find("long(long)");
    EXPECT("factory-long(long)", factory_long_long != nullptr);
}

TEST(TestCommonIntegral, TestUnsignedLong)
{
    auto type = rew::global.find("unsigned long");

    ASSERT("type", type != nullptr);

    EXPECT("type-name", type->name == "unsigned long");
    EXPECT("type-size", type->size == sizeof(unsigned long));
    EXPECT("type-context", type->context != nullptr);

    auto reflection = type->reflection;
    ASSERT("reflection", reflection != nullptr);

    EXPECT("reflection-name", reflection->name == "unsigned long");

    auto factory_unsigned_long = reflection->factory.find("unsigned long()");
    EXPECT("factory-unsigned long()", factory_unsigned_long != nullptr);

    auto factory_unsigned_long_unsigned_long = reflection->factory.find("unsigned long(unsigned long)");
    EXPECT("factory-unsigned long(unsigned long)", factory_unsigned_long_unsigned_long != nullptr);
}

TEST(TestCommonIntegral, TestLongLong)
{
    auto type = rew::global.find("long long");

    ASSERT("type", type != nullptr);

    EXPECT("type-name", type->name == "long long");
    EXPECT("type-size", type->size == sizeof(long long));
    EXPECT("type-context", type->context != nullptr);

    auto reflection = type->reflection;
    ASSERT("reflection", reflection != nullptr);

    EXPECT("reflection-name", reflection->name == "long long");

    auto factory_long = reflection->factory.find("long long()");
    EXPECT("factory-long()", factory_long != nullptr);

    auto factory_long_long_long_long = reflection->factory.find("long long(long long)");
    EXPECT("factory-long long(long long)", factory_long_long_long_long != nullptr);
}

TEST(TestCommonIntegral, TestUnsignedLongLong)
{
    auto type = rew::global.find("unsigned long long");

    ASSERT("type", type != nullptr);

    EXPECT("type-name", type->name == "unsigned long long");
    EXPECT("type-size", type->size == sizeof(unsigned long long));
    EXPECT("type-context", type->context != nullptr);

    auto reflection = type->reflection;
    ASSERT("reflection", reflection != nullptr);

    EXPECT("reflection-name", reflection->name == "unsigned long long");

    auto factory_unsigned_long_long = reflection->factory.find("unsigned long long()");
    EXPECT("factory-unsigned long long()", factory_unsigned_long_long != nullptr);

    auto factory_unsigned_long_long_unsigned_long_long = reflection->factory.find("unsigned long long(unsigned long long)");
    EXPECT("factory-unsigned long long(unsigned long long)", factory_unsigned_long_long_unsigned_long_long != nullptr);
}
