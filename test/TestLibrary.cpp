#include <RewTestingBase.hpp>

TEST_SPACE()
{

struct TestReflectableDeclaratonOnlyStruct {};
struct TestReflectableDeclaratonOnlyWithUsingStruct {};

struct TestReflectableStruct
{
    void Foo(TestReflectableDeclaratonOnlyWithUsingStruct) {}
};

} // TEST_SPACE

REFLECTABLE_DECLARATION(TestReflectableDeclaratonOnlyStruct)
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE_DECLARATION(TestReflectableDeclaratonOnlyWithUsingStruct)
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE_DECLARATION(TestReflectableStruct)
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE(TestReflectableStruct)
    FUNCTION(Foo) // will implicitly add TestReflectableDeclaratonOnlyWithUsingStruct to registry
REFLECTABLE_INIT()

TEST(TestLibrary, TestReflectableRegistry)
{
    {
        // will not compile, since requires REFLECTABLE(...)
        //rew::reflectable<TestReflectableDeclaratonOnlyStruct>();

        auto type = rew::global.find("TestReflectableDeclaratonOnlyStruct");

        ASSERT("type", type == nullptr);
    }
    {
        // will explicitly add type to registry, no requires REFLECTABLE(...), since can add incomplete types
        rew::find_or_add_type<TestReflectableDeclaratonOnlyStruct>();

        auto type = rew::global.find("TestReflectableDeclaratonOnlyStruct");

        ASSERT("type", type != nullptr);
        EXPECT("reflection", type->reflection != nullptr);
    }
    {
        auto type = rew::global.find("TestReflectableDeclaratonOnlyWithUsingStruct");

        ASSERT("type", type != nullptr);
        EXPECT("reflection", type->reflection != nullptr);
    }
    {
        auto type = rew::global.find("TestReflectableStruct");

        ASSERT("type", type != nullptr);
        EXPECT("reflection", type->reflection != nullptr);
    }
}

TEST_SPACE()
{

struct TestDeparenStruct
{
    void Function() {}
    void FunctionWithParentheses(float) {}

    template <typename... Args>
    void Template() {}
};

} // TEST_SPACE

REFLECTABLE_DECLARATION(TestDeparenStruct)
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE(TestDeparenStruct)
    FUNCTION(Function)
    FUNCTION((FunctionWithParentheses))
    FUNCTION(Template<int>, void())
    FUNCTION((Template<float, bool>), void())
REFLECTABLE_INIT()

TEST(TestLibrary, TestDeparen)
{
    auto type = rew::global.find("TestDeparenStruct");

    ASSERT("type", type != nullptr);

    auto reflection = type->reflection;

    ASSERT("reflection", reflection != nullptr);

    EXPECT("function", reflection->function.find("Function") != nullptr);
    EXPECT("function_with_parentheses", reflection->function.find("FunctionWithParentheses") != nullptr);
    EXPECT("template", reflection->function.find("Template<int>") != nullptr);
    EXPECT("template_with_parentheses", reflection->function.find("Template<float, bool>") != nullptr);
}
