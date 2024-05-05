#include <RewTestingBase.hpp>

TEST(TestLibrary, TestNameOf)
{
    EXPECT("const-type", rew::meta::reflectable_traits<const int>::name() == "int const");
    EXPECT("reference-type", rew::meta::reflectable_traits<int&>::name() == "int&");
    EXPECT("const_reference-type", rew::meta::reflectable_traits<const int&>::name() == "int const&");
    EXPECT("function-type", rew::meta::reflectable_traits<void(int, float)>::name() == "void(int, float)");
    EXPECT("const-function-type", rew::meta::reflectable_traits<void(int, float) const>::name() == "void(int, float) const");
    EXPECT("reference-function-type", rew::meta::reflectable_traits<void(int, float)&>::name() == "void(int, float)&");
    EXPECT("const_reference-function-type", rew::meta::reflectable_traits<void(int, float) const&>::name() == "void(int, float) const&");
}


TEST_SPACE()
{

enum TestEnum { Blue, Green, Red, Alpha };

} // TEST_SPACE

REFLECTABLE_DECLARATION(TestEnum)
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE(TestEnum)
    META("Blue", TestEnum::Blue)
    META("Green", TestEnum::Green)
//  META("Red", TestEnum::Red) // skip for test
    META("Alpha", Alpha) // since TestEnum is no scoped enum, we can ommit 'TestEnum::'
REFLECTABLE_INIT()

TEST(TestLibrary, TestEnum)
{
    auto type = rew::global.find("TestEnum");

    ASSERT("type", type != nullptr);

    auto reflection = type->reflection;

    ASSERT("reflection", reflection != nullptr);

    EXPECT("meta-1", reflection->meta.find("Blue") != nullptr);
    EXPECT("meta-2", reflection->meta.find("Green") != nullptr);
    EXPECT("meta-3", reflection->meta.find("Alpha") != nullptr);
}


TEST_SPACE()
{

enum TestScopedEnum { Up, Down, Size };

} // TEST_SPACE

REFLECTABLE_DECLARATION(TestScopedEnum)
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE(TestScopedEnum)
    META("Up", TestScopedEnum::Up)
    META("Down", TestScopedEnum::Down)
    META("Size", (int)TestScopedEnum::Size)
REFLECTABLE_INIT()

TEST(TestLibrary, TestScopedEnum)
{
    auto type = rew::global.find("TestScopedEnum");

    ASSERT("type", type != nullptr);

    auto reflection = type->reflection;

    ASSERT("reflection", reflection != nullptr);

    EXPECT("meta-1", reflection->meta.find("Up") != nullptr);
    EXPECT("meta-2", reflection->meta.find("Down") != nullptr);
    EXPECT("meta-3", reflection->meta.find("Size") != nullptr);
}


TEST_SPACE()
{

struct TestFunctionStruct
{
    void Function() {}
    void Reference()& {}
    void Const() const {}
    void ConstReference() const& {}

    void Overload()& {}
    void Overload() const& {}
    void Overload(int) {}
    void Overload(int) const {}
};

} // TEST_SPACE

REFLECTABLE_DECLARATION(TestFunctionStruct)
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE(TestFunctionStruct)
    FUNCTION(Function)
    FUNCTION(Reference)
    FUNCTION(Const)
    FUNCTION(ConstReference)
    FUNCTION(Overload, void()&)
    FUNCTION(Overload, void() const&)
    FUNCTION(Overload, void(int))
    FUNCTION(Overload, void(int) const)
REFLECTABLE_INIT()

TEST(TestLibrary, TestFunction)
{
    auto type = rew::global.find("TestFunctionStruct");

    ASSERT("type", type != nullptr);

    auto reflection = type->reflection;

    ASSERT("reflection", reflection != nullptr);

    EXPECT("function", reflection->function.find("Function") != nullptr);
    EXPECT("function-reference", reflection->function.find("Reference") != nullptr);
    EXPECT("function-const", reflection->function.find("Const") != nullptr);
    EXPECT("function-const_reference", reflection->function.find("ConstReference") != nullptr);

    auto overload = reflection->function.find("Overload");

    ASSERT("function-overload", overload != nullptr);
    EXPECT("function-overload-1", overload->all.find("void()&") != nullptr);
    EXPECT("function-overload-2", overload->all.find("void() const&") != nullptr);
    EXPECT("function-overload-3", overload->all.find("void(int)") != nullptr);
    EXPECT("function-overload-4", overload->all.find("void(int) const") != nullptr);
}


TEST_SPACE()
{

struct TestStaticFunctionStruct
{
    static void Function() {}
    static void Overload() {}
    static void Overload(int) {}
};

} // TEST_SPACE

REFLECTABLE_DECLARATION(TestStaticFunctionStruct)
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE(TestStaticFunctionStruct)
    FUNCTION(Function)
    FUNCTION(Overload, void())
    FUNCTION(Overload, void(int))
REFLECTABLE_INIT()

TEST(TestLibrary, TestStaticFunction)
{
    auto type = rew::global.find("TestStaticFunctionStruct");

    ASSERT("type", type != nullptr);

    auto reflection = type->reflection;

    ASSERT("reflection", reflection != nullptr);

    EXPECT("function", reflection->function.find("Function") != nullptr);

    auto overload = reflection->function.find("Overload");

    ASSERT("function-overload", overload != nullptr);
    EXPECT("function-overload-1", overload->all.find("void()") != nullptr);
    EXPECT("function-overload-2", overload->all.find("void(int)") != nullptr);
}


TEST_SPACE()
{

struct TestFreeFunctionStruct {};

void Function() {}
void Overload() {}
void Overload(int) {}

} // TEST_SPACE

REFLECTABLE_DECLARATION(TestFreeFunctionStruct)
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE(TestFreeFunctionStruct)
    FREE_FUNCTION(Function)
    FREE_FUNCTION(Overload, void())
    FREE_FUNCTION(Overload, void(int))
REFLECTABLE_INIT()

TEST(TestLibrary, TestFreeFunction)
{
    auto type = rew::global.find("TestFreeFunctionStruct");

    ASSERT("type", type != nullptr);

    auto reflection = type->reflection;

    ASSERT("reflection", reflection != nullptr);

    EXPECT("function", reflection->function.find("Function") != nullptr);

    auto overload = reflection->function.find("Overload");

    ASSERT("function-overload", overload != nullptr);
    EXPECT("function-overload-1", overload->all.find("void()") != nullptr);
    EXPECT("function-overload-2", overload->all.find("void(int)") != nullptr);
}


TEST_SPACE()
{

struct TestFieldPropertyStruct
{
    int Property = 0;
    int const Readonly = 0;
    int& Reference;
};

} // TEST_SPACE

REFLECTABLE_DECLARATION(TestFieldPropertyStruct)
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE(TestFieldPropertyStruct)
    PROPERTY(Property)
    PROPERTY(Readonly)
//  PROPERTY(Reference) // unsupported!
REFLECTABLE_INIT()

TEST(TestLibrary, TestFieldProperty)
{
    auto type = rew::global.find("TestFieldPropertyStruct");

    ASSERT("type", type != nullptr);

    auto reflection = type->reflection;

    ASSERT("reflection", reflection != nullptr);

    auto property = reflection->property.find("Property");

    ASSERT("property", property != nullptr);
    EXPECT("property-get", property->get != nullptr);
    EXPECT("property-set", property->set != nullptr);
    EXPECT("property-context", property->context != nullptr);

    auto readonly = reflection->property.find("Readonly");

    ASSERT("property-readonly", readonly != nullptr);
    EXPECT("property-readonly-get", readonly->get != nullptr);
    EXPECT("property-readonly-set", readonly->set == nullptr);
    EXPECT("property-readonly-context", readonly->context != nullptr);
}


TEST_SPACE()
{

struct TestStaticFieldPropertyStruct
{
    static int Property;
    static int const Readonly;
};

int TestStaticFieldPropertyStruct::Property = 0;
int const TestStaticFieldPropertyStruct::Readonly = 0;

} // TEST_SPACE

REFLECTABLE_DECLARATION(TestStaticFieldPropertyStruct)
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE(TestStaticFieldPropertyStruct)
    PROPERTY(Property)
    PROPERTY(Readonly)
REFLECTABLE_INIT()

TEST(TestLibrary, TestStaticFieldProperty)
{
    auto type = rew::global.find("TestStaticFieldPropertyStruct");

    ASSERT("type", type != nullptr);

    auto reflection = type->reflection;

    ASSERT("reflection", reflection != nullptr);

    auto property = reflection->property.find("Property");

    ASSERT("property", property != nullptr);
    EXPECT("property-get", property->get != nullptr);
    EXPECT("property-set", property->set != nullptr);
    EXPECT("property-context", property->context != nullptr);

    auto readonly = reflection->property.find("Readonly");

    ASSERT("property-readonly", readonly != nullptr);
    EXPECT("property-readonly-get", readonly->get != nullptr);
    EXPECT("property-readonly-set", readonly->set == nullptr);
    EXPECT("property-readonly-context", readonly->context != nullptr);
}


TEST_SPACE()
{

struct TestFreeFieldPropertyStruct {};

int Property = 0;
int const Readonly = 0;
int& Reference = Property;

} // TEST_SPACE

REFLECTABLE_DECLARATION(TestFreeFieldPropertyStruct)
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE(TestFreeFieldPropertyStruct)
    FREE_PROPERTY(Property)
    FREE_PROPERTY(Readonly)
    FREE_PROPERTY(Reference)
REFLECTABLE_INIT()

TEST(TestLibrary, TestFreeFieldProperty)
{
    auto type = rew::global.find("TestFreeFieldPropertyStruct");

    ASSERT("type", type != nullptr);

    auto reflection = type->reflection;

    ASSERT("reflection", reflection != nullptr);

    auto property = reflection->property.find("Property");

    ASSERT("property", property != nullptr);
    EXPECT("property-get", property->get != nullptr);
    EXPECT("property-set", property->set != nullptr);
    EXPECT("property-context", property->context != nullptr);

    auto readonly = reflection->property.find("Readonly");

    ASSERT("property-readonly", readonly != nullptr);
    EXPECT("property-readonly-get", readonly->get != nullptr);
    EXPECT("property-readonly-set", readonly->set == nullptr);
    EXPECT("property-readonly-context", readonly->context != nullptr);

    auto reference = reflection->property.find("Reference");

    ASSERT("property-reference", reference != nullptr);
    EXPECT("property-reference-get", reference->get != nullptr);
    EXPECT("property-reference-set", reference->set != nullptr);
    EXPECT("property-reference-context", reference->context != nullptr);
}


TEST_SPACE()
{

struct TestFunctionPropertyStruct : TestFieldPropertyStruct
{
    int& WithContext() { return Property; } void WithContext(int& value) { Property = value; }
    int const& WithConstContext() { return Property; } void WithConstContext(int const& value) { Property = value; }
    int const& ConstWithConstContext() const { return Property; } void ConstWithConstContext(int const& value) { Property = value; }
    int NoContext() { return 0; } void NoContext(int) {}
    int ConstNoContext() const { return 0; } void ConstNoContext(int) {}

    int& ReadonlyWithContext() { return Property; }
    int const& ReadonlyWithConstContext() { return Property; }
    int const& ReadonlyConstWithConstContext() const { return Property; }
    int ReadonlyNoContext() { return 0; }
    int ReadonlyConstNoContext() const { return 0; }
};

} // TEST_SPACE

REFLECTABLE_DECLARATION(TestFunctionPropertyStruct)
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE(TestFunctionPropertyStruct)
    PROPERTY(WithContext)
    PROPERTY(WithConstContext)
    PROPERTY(ConstWithConstContext)
    PROPERTY(NoContext)
    PROPERTY(ConstNoContext)

    PROPERTY(ReadonlyWithContext)
    PROPERTY(ReadonlyWithConstContext)
    PROPERTY(ReadonlyConstWithConstContext)
    PROPERTY(ReadonlyNoContext)
    PROPERTY(ReadonlyConstNoContext)
REFLECTABLE_INIT()

TEST(TestLibrary, TestFunctionProperty)
{
    auto type = rew::global.find("TestFunctionPropertyStruct");

    ASSERT("type", type != nullptr);

    auto reflection = type->reflection;

    ASSERT("reflection", reflection != nullptr);

    auto with_context = reflection->property.find("WithContext");

    ASSERT("property-with_context", with_context != nullptr);
    EXPECT("property-with_context-get", with_context->get != nullptr);
    EXPECT("property-with_context-set", with_context->set != nullptr);
    EXPECT("property-with_context-context", with_context->context != nullptr);

    auto with_const_context = reflection->property.find("WithConstContext");

    ASSERT("property-with_const_context", with_const_context != nullptr);
    EXPECT("property-with_const_context-get", with_const_context->get != nullptr);
    EXPECT("property-with_const_context-set", with_const_context->set != nullptr);
    EXPECT("property-with_const_context-context", with_const_context->context != nullptr);

    auto const_with_const_context = reflection->property.find("ConstWithConstContext");

    ASSERT("property-const_with_const_context", const_with_const_context != nullptr);
    EXPECT("property-const_with_const_context-get", const_with_const_context->get != nullptr);
    EXPECT("property-const_with_const_context-set", const_with_const_context->set != nullptr);
    EXPECT("property-const_with_const_context-context", const_with_const_context->context != nullptr);

    auto no_context = reflection->property.find("NoContext");

    ASSERT("property-no_context", no_context != nullptr);
    EXPECT("property-no_context-get", no_context->get != nullptr);
    EXPECT("property-no_context-set", no_context->set != nullptr);
    EXPECT("property-no_context-context", no_context->context == nullptr);

    auto const_no_context = reflection->property.find("ConstNoContext");

    ASSERT("property-const_no_context", const_no_context != nullptr);
    EXPECT("property-const_no_context-get", const_no_context->get != nullptr);
    EXPECT("property-const_no_context-set", const_no_context->set != nullptr);
    EXPECT("property-const_no_context-context", const_no_context->context == nullptr);


    auto readonly_with_context = reflection->property.find("ReadonlyWithContext");

    ASSERT("property-readonly_with_context", readonly_with_context != nullptr);
    EXPECT("property-readonly_with_context-get", readonly_with_context->get != nullptr);
    EXPECT("property-readonly_with_context-set", readonly_with_context->set == nullptr);
    EXPECT("property-readonly_with_context-context", readonly_with_context->context != nullptr);

    auto readonly_with_const_context = reflection->property.find("ReadonlyWithConstContext");

    ASSERT("property-readonly_with_const_context", readonly_with_const_context != nullptr);
    EXPECT("property-readonly_with_const_context-get", readonly_with_const_context->get != nullptr);
    EXPECT("property-readonly_with_const_context-set", readonly_with_const_context->set == nullptr);
    EXPECT("property-readonly_with_const_context-context", readonly_with_const_context->context != nullptr);

    auto readonly_const_with_const_context = reflection->property.find("ReadonlyConstWithConstContext");

    ASSERT("property-readonly_const_with_const_context", readonly_const_with_const_context != nullptr);
    EXPECT("property-readonly_const_with_const_context-get", readonly_const_with_const_context->get != nullptr);
    EXPECT("property-readonly_const_with_const_context-set", readonly_const_with_const_context->set == nullptr);
    EXPECT("property-readonly_const_with_const_context-context", readonly_const_with_const_context->context != nullptr);

    auto readonly_no_context = reflection->property.find("ReadonlyNoContext");

    ASSERT("property-readonly_no_context", readonly_no_context != nullptr);
    EXPECT("property-readonly_no_context-get", readonly_no_context->get != nullptr);
    EXPECT("property-readonly_no_context-set", readonly_no_context->set == nullptr);
    EXPECT("property-readonly_no_context-context", readonly_no_context->context == nullptr);

    auto readonly_const_no_context = reflection->property.find("ReadonlyConstNoContext");

    ASSERT("property-readonly_const_no_context", readonly_const_no_context != nullptr);
    EXPECT("property-readonly_const_no_context-get", readonly_const_no_context->get != nullptr);
    EXPECT("property-readonly_const_no_context-set", readonly_const_no_context->set == nullptr);
    EXPECT("property-readonly_const_no_context-context", readonly_const_no_context->context == nullptr);
}


TEST_SPACE()
{

struct TestStaticFunctionPropertyStruct : TestStaticFieldPropertyStruct
{
    static int& WithContext() { return Property; } static void WithContext(int& value) { Property = value; }
    static int const& WithConstContext() { return Property; } static void WithConstContext(int const& value) { Property = value; }
    static int NoContext() { return 0; } static void NoContext(int) {}

    static int& ReadonlyWithContext() { return Property; }
    static int const& ReadonlyWithConstContext() { return Property; }
    static int ReadonlyNoContext() { return 0; }
};

} // TEST_SPACE

REFLECTABLE_DECLARATION(TestStaticFunctionPropertyStruct)
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE(TestStaticFunctionPropertyStruct)
    PROPERTY(WithContext)
    PROPERTY(WithConstContext)
    PROPERTY(NoContext)

    PROPERTY(ReadonlyWithContext)
    PROPERTY(ReadonlyWithConstContext)
    PROPERTY(ReadonlyNoContext)
REFLECTABLE_INIT()

TEST(TestLibrary, TestStaticFunctionProperty)
{
    auto type = rew::global.find("TestStaticFunctionPropertyStruct");

    ASSERT("type", type != nullptr);

    auto reflection = type->reflection;

    ASSERT("reflection", reflection != nullptr);

    auto with_context = reflection->property.find("WithContext");

    ASSERT("property-with_context", with_context != nullptr);
    EXPECT("property-with_context-get", with_context->get != nullptr);
    EXPECT("property-with_context-set", with_context->set != nullptr);
    EXPECT("property-with_context-context", with_context->context != nullptr);

    auto with_const_context = reflection->property.find("WithConstContext");

    ASSERT("property-with_const_context", with_const_context != nullptr);
    EXPECT("property-with_const_context-get", with_const_context->get != nullptr);
    EXPECT("property-with_const_context-set", with_const_context->set != nullptr);
    EXPECT("property-with_const_context-context", with_const_context->context != nullptr);

    auto no_context = reflection->property.find("NoContext");

    ASSERT("property-no_context", no_context != nullptr);
    EXPECT("property-no_context-get", no_context->get != nullptr);
    EXPECT("property-no_context-set", no_context->set != nullptr);
    EXPECT("property-no_context-context", no_context->context == nullptr);


    auto readonly_with_context = reflection->property.find("ReadonlyWithContext");

    ASSERT("property-readonly_with_context", readonly_with_context != nullptr);
    EXPECT("property-readonly_with_context-get", readonly_with_context->get != nullptr);
    EXPECT("property-readonly_with_context-set", readonly_with_context->set == nullptr);
    EXPECT("property-readonly_with_context-context", readonly_with_context->context != nullptr);

    auto readonly_with_const_context = reflection->property.find("ReadonlyWithConstContext");

    ASSERT("property-readonly_with_const_context", readonly_with_const_context != nullptr);
    EXPECT("property-readonly_with_const_context-get", readonly_with_const_context->get != nullptr);
    EXPECT("property-readonly_with_const_context-set", readonly_with_const_context->set == nullptr);
    EXPECT("property-readonly_with_const_context-context", readonly_with_const_context->context != nullptr);

    auto readonly_no_context = reflection->property.find("ReadonlyNoContext");

    ASSERT("property-readonly_no_context", readonly_no_context != nullptr);
    EXPECT("property-readonly_no_context-get", readonly_no_context->get != nullptr);
    EXPECT("property-readonly_no_context-set", readonly_no_context->set == nullptr);
    EXPECT("property-readonly_no_context-context", readonly_no_context->context == nullptr);
}


TEST_SPACE()
{

struct TestFreeFunctionPropertyStruct {};

int& WithContext() { return Property; } void WithContext(int& value) { Property = value; }
int const& WithConstContext() { return Property; } void WithConstContext(int const& value) { Property = value; }
int NoContext() { return 0; } void NoContext(int) {}

int& ReadonlyWithContext() { return Property; }
int const& ReadonlyWithConstContext() { return Property; }
int ReadonlyNoContext() { return 0; }

} // TEST_SPACE

REFLECTABLE_DECLARATION(TestFreeFunctionPropertyStruct)
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE(TestFreeFunctionPropertyStruct)
    FREE_PROPERTY(WithContext)
    FREE_PROPERTY(WithConstContext)
    FREE_PROPERTY(NoContext)

    FREE_PROPERTY(ReadonlyWithContext)
    FREE_PROPERTY(ReadonlyWithConstContext)
    FREE_PROPERTY(ReadonlyNoContext)
REFLECTABLE_INIT()

TEST(TestLibrary, TestFreeFunctionProperty)
{
    auto type = rew::global.find("TestFreeFunctionPropertyStruct");

    ASSERT("type", type != nullptr);

    auto reflection = type->reflection;

    ASSERT("reflection", reflection != nullptr);

    auto with_context = reflection->property.find("WithContext");

    ASSERT("property-with_context", with_context != nullptr);
    EXPECT("property-with_context-get", with_context->get != nullptr);
    EXPECT("property-with_context-set", with_context->set != nullptr);
    EXPECT("property-with_context-context", with_context->context != nullptr);

    auto with_const_context = reflection->property.find("WithConstContext");

    ASSERT("property-with_const_context", with_const_context != nullptr);
    EXPECT("property-with_const_context-get", with_const_context->get != nullptr);
    EXPECT("property-with_const_context-set", with_const_context->set != nullptr);
    EXPECT("property-with_const_context-context", with_const_context->context != nullptr);

    auto no_context = reflection->property.find("NoContext");

    ASSERT("property-no_context", no_context != nullptr);
    EXPECT("property-no_context-get", no_context->get != nullptr);
    EXPECT("property-no_context-set", no_context->set != nullptr);
    EXPECT("property-no_context-context", no_context->context == nullptr);


    auto readonly_with_context = reflection->property.find("ReadonlyWithContext");

    ASSERT("property-readonly_with_context", readonly_with_context != nullptr);
    EXPECT("property-readonly_with_context-get", readonly_with_context->get != nullptr);
    EXPECT("property-readonly_with_context-set", readonly_with_context->set == nullptr);
    EXPECT("property-readonly_with_context-context", readonly_with_context->context != nullptr);

    auto readonly_with_const_context = reflection->property.find("ReadonlyWithConstContext");

    ASSERT("property-readonly_with_const_context", readonly_with_const_context != nullptr);
    EXPECT("property-readonly_with_const_context-get", readonly_with_const_context->get != nullptr);
    EXPECT("property-readonly_with_const_context-set", readonly_with_const_context->set == nullptr);
    EXPECT("property-readonly_with_const_context-context", readonly_with_const_context->context != nullptr);

    auto readonly_no_context = reflection->property.find("ReadonlyNoContext");

    ASSERT("property-readonly_no_context", readonly_no_context != nullptr);
    EXPECT("property-readonly_no_context-get", readonly_no_context->get != nullptr);
    EXPECT("property-readonly_no_context-set", readonly_no_context->set == nullptr);
    EXPECT("property-readonly_no_context-context", readonly_no_context->context == nullptr);
}
