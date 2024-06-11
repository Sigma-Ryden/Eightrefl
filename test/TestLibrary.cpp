#include <RewTestingBase.hpp>

TEST_SPACE()
{

struct TestReflectableDeclaratonOnlyStruct {};
struct TestReflectableDeclaratonOnlyWithUsingStruct {};

struct TestReflectableStruct
{
    void Foo(TestReflectableDeclaratonOnlyWithUsingStruct) {}
};

struct TestReflectableWithoutDeclarationStruct {};

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

// will not compile, since rrquires REFLECTABLE_DECLARATION(...), that contains reflectable_traits
//REFLECTABLE(TestReflectableWithoutDeclarationStruct)
//REFLECTABLE_INIT()

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


TEST_SPACE()
{

struct TestParentTopBaseStruct {};
struct TestParentBaseStruct : TestParentTopBaseStruct {};

struct TestParentInterface
{
    virtual void Foo() = 0;
};

struct TestParentDerivedStruct : TestParentBaseStruct, TestParentInterface
{
    void Foo() override {}
};

} // TEST_SPACE

REFLECTABLE_DECLARATION(TestParentTopBaseStruct)
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE(TestParentTopBaseStruct)
REFLECTABLE_INIT()

REFLECTABLE_DECLARATION(TestParentBaseStruct)
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE(TestParentBaseStruct)
    PARENT(TestParentTopBaseStruct)
REFLECTABLE_INIT()

REFLECTABLE_DECLARATION(TestParentInterface)
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE(TestParentInterface)
    FUNCTION(Foo)
REFLECTABLE_INIT()

REFLECTABLE_DECLARATION(TestParentDerivedStruct)
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE(TestParentDerivedStruct)
    PARENT(TestParentBaseStruct)
    PARENT(TestParentInterface)
    FUNCTION(Foo)
REFLECTABLE_INIT()

TEST(TestLibrary, TestParent)
{
    auto type = rew::global.find("TestParentDerivedStruct");

    ASSERT("type", type != nullptr);

    auto reflection = type->reflection;

    ASSERT("reflection", reflection != nullptr);

    auto base = reflection->parent.find("TestParentBaseStruct");

    ASSERT("base", base != nullptr);
    EXPECT("base-type", base->type == rew::global.find(base->name));

    auto base_base = base->type->reflection->parent.find("TestParentTopBaseStruct");

    ASSERT("base-base", base_base != nullptr);
    EXPECT("base-base-type", base_base->type == rew::global.find(base_base->name));
    EXPECT("non_directed_base", reflection->parent.find(base_base->name) == nullptr);

    auto interface = reflection->parent.find("TestParentInterface");

    ASSERT("interface", interface != nullptr);
    EXPECT("interface-type", interface->type == rew::global.find(interface->name));
}


TEST_SPACE()
{

struct TestMetaStruct {};

} // TEST_SPACE

REFLECTABLE_DECLARATION(TestMetaStruct)
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE(TestMetaStruct)
REFLECTABLE_INIT()

TEST(TestLibrary, TestMeta)
{
    auto type = rew::global.find("TestMetaStruct");

    ASSERT("type", type != nullptr);

    auto reflection = type->reflection;

    ASSERT("reflection", reflection != nullptr);
}

// TODO: add test custom reflectable registry
// TODO: add test reflectable registry reallocation
