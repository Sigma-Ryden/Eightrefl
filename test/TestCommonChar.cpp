#include <RewTestingBase.hpp>

TEST(TestCommonChar, TestChar)
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

TEST(TestCommonChar, TestWChar)
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
TEST(TestCommonChar, TestChar8)
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

TEST(TestCommonChar, TestChar16)
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

TEST(TestCommonChar, TestChar32)
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
