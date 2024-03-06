#include <RewTestingBase.hpp>

TEST(TesBuiltInIncomplete, TestVoid)
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

TEST(TestBuiltInBoolean, TestBool)
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

TEST(TestBuiltInChar, TestChar)
{
    auto type = rew::global.find("char");

    ASSERT("type", type != nullptr);

    EXPECT("type-name", type->name == "char");
    EXPECT("type-size", type->size == sizeof(char));
    EXPECT("type-context", type->context != nullptr);

    auto reflection = type->reflection;
    ASSERT("reflection", reflection != nullptr);

    EXPECT("reflection-name", reflection->name == "char");

    auto factory_char = reflection->factory.find("char()");
    EXPECT("factory-char()", factory_char != nullptr);

    auto factory_char_char = reflection->factory.find("char(char)");
    EXPECT("factory-char(char)", factory_char_char != nullptr);
}

TEST(TestBuiltInChar, TestWChar)
{
    auto type = rew::global.find("wchar_t");

    ASSERT("type", type != nullptr);

    EXPECT("type-name", type->name == "wchar_t");
    EXPECT("type-size", type->size == sizeof(wchar_t));
    EXPECT("type-context", type->context != nullptr);

    auto reflection = type->reflection;
    ASSERT("reflection", reflection != nullptr);

    EXPECT("reflection-name", reflection->name == "wchar_t");

    auto factory_wchar = reflection->factory.find("wchar_t()");
    EXPECT("factory-wchar_t()", factory_wchar != nullptr);

    auto factory_wchar_wchar = reflection->factory.find("wchar_t(wchar_t)");
    EXPECT("factory-wchar_t(wchar_t)", factory_wchar_wchar != nullptr);
}

#if __cplusplus >= 202002L
TEST(TestBuiltInChar, TestChar8)
{
    auto type = rew::global.find("char8_t");

    ASSERT("type", type != nullptr);

    EXPECT("type-name", type->name == "char8_t");
    EXPECT("type-size", type->size == sizeof(char8_t));
    EXPECT("type-context", type->context != nullptr);

    auto reflection = type->reflection;
    ASSERT("reflection", reflection != nullptr);

    EXPECT("reflection-name", reflection->name == "char8_t");

    auto factory_char8 = reflection->factory.find("char8_t()");
    EXPECT("factory-char8_t()", factory_char8 != nullptr);

    auto factory_char8_char8 = reflection->factory.find("char8_t(char8_t)");
    EXPECT("factory-char8_t(char8_t)", factory_char8_char8 != nullptr);
}
#endif // if

TEST(TestBuiltInChar, TestChar16)
{
    auto type = rew::global.find("char16_t");

    ASSERT("type", type != nullptr);

    EXPECT("type-name", type->name == "char16_t");
    EXPECT("type-size", type->size == sizeof(char16_t));
    EXPECT("type-context", type->context != nullptr);

    auto reflection = type->reflection;
    ASSERT("reflection", reflection != nullptr);

    EXPECT("reflection-name", reflection->name == "char16_t");

    auto factory_char16 = reflection->factory.find("char16_t()");
    EXPECT("factory-char16_t()", factory_char16 != nullptr);

    auto factory_char16_char16 = reflection->factory.find("char16_t(char16_t)");
    EXPECT("factory-char16_t(char16_t)", factory_char16_char16 != nullptr);
}

TEST(TestBuiltInChar, TestChar32)
{
    auto type = rew::global.find("char32_t");

    ASSERT("type", type != nullptr);

    EXPECT("type-name", type->name == "char32_t");
    EXPECT("type-size", type->size == sizeof(char32_t));
    EXPECT("type-context", type->context != nullptr);

    auto reflection = type->reflection;
    ASSERT("reflection", reflection != nullptr);

    EXPECT("reflection-name", reflection->name == "char32_t");

    auto factory_char32 = reflection->factory.find("char32_t()");
    EXPECT("factory-char32_t()", factory_char32 != nullptr);

    auto factory_char32_char32 = reflection->factory.find("char32_t(char32_t)");
    EXPECT("factory-char32_t(char32_t)", factory_char32_char32 != nullptr);
}

TEST(TestBuiltInIntegral, TestSignedChar)
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

TEST(TestBuiltInIntegral, TestUnsignedChar)
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

TEST(TestBuiltInIntegral, TestInt)
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

TEST(TestBuiltInIntegral, TestUnsignedInt)
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

TEST(TestBuiltInIntegral, TestLong)
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

TEST(TestBuiltInIntegral, TestUnsignedLong)
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

TEST(TestBuiltInIntegral, TestLongLong)
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

TEST(TestBuiltInIntegral, TestUnsignedLongLong)
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

TEST(TestBuiltInFloatingPoint, TestFloat)
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

TEST(TestBuiltInFloatingPoint, TestDouble)
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

TEST(TestBuiltInFloatingPoint, TestLongDouble)
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
