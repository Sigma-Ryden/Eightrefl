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


// TODO: add tests for implicit add to registry through other types
TEST_SPACE()
{

struct TestCustomRegistryStruct {};

rew::registry_t CustomRegistry;

} // TEST_SPACE

// REFLECTABLE_DECLARATION use CUSTOM_REFLECTABLE_DECLARATION with
// REFLECTABLE_REGISTRY(&rew::global)
// REFLECTABLE_NAME(typename)
CUSTOM_REFLECTABLE_DECLARATION(TestCustomRegistryStruct)
    REFLECTABLE_REGISTRY(&CustomRegistry)
    REFLECTABLE_NAME("TestCustomRegistryStruct")
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE(TestCustomRegistryStruct)
REFLECTABLE_INIT()

TEST(TestLibrary, TestCustomRegistry)
{
    auto registry = rew::meta::reflectable_traits<TestCustomRegistryStruct>::registry();

    ASSERT("registry", registry == &CustomRegistry);

    auto type = CustomRegistry.find("TestCustomRegistryStruct");

    ASSERT("type", type != nullptr);

    auto reflection = type->reflection;

    EXPECT("reflection", reflection != nullptr);
}


TEST_SPACE()
{

struct TestTypedPropertyStruct
{
    int& FunctionProperty() { return Property; }
    void FunctionProperty(int& value) { Property = value; }

    int& OtherFunctionProperty() { return Property; }
    int const& OtherFunctionProperty() const { return Property; }
    void OtherFunctionProperty(int& value) { Property = value; }

    template <typename T> T TemplateFunctionProperty() { return T(); }

    int Property = 0;
    static constexpr int ConstProperty = 0;
};

} // TEST_SPACE

REFLECTABLE_DECLARATION(TestTypedPropertyStruct)
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE(TestTypedPropertyStruct)
    PROPERTY(FunctionProperty, int&())
    PROPERTY(OtherFunctionProperty, int const&() const)
    PROPERTY(TemplateFunctionProperty<char>, char())
    PROPERTY(Property, int)
    PROPERTY(ConstProperty, int const)
REFLECTABLE_INIT()

TEST(TestLibrary, TestTypedProperty)
{
    auto type = rew::global.find("TestTypedPropertyStruct");

    ASSERT("type", type != nullptr);

    auto reflection = type->reflection;

    ASSERT("reflection", reflection != nullptr);

    auto function_property = reflection->property.find("FunctionProperty");

    ASSERT("function_property", function_property != nullptr);
    EXPECT("function_property-get", function_property->get != nullptr);
    EXPECT("function_property-set", function_property->set != nullptr);
    EXPECT("function_property-context", function_property->context != nullptr);

    auto other_function_property = reflection->property.find("OtherFunctionProperty");

    ASSERT("other_function_property", other_function_property != nullptr);
    EXPECT("other_function_property-get", other_function_property->get != nullptr);
    EXPECT("other_function_property-set", other_function_property->set == nullptr);
    EXPECT("other_function_property-context", other_function_property->context != nullptr);

    auto template_function_property = reflection->property.find("TemplateFunctionProperty<char>");

    ASSERT("template_function_property", template_function_property != nullptr);
    EXPECT("template_function_property-get", template_function_property->get != nullptr);
    EXPECT("template_function_property-set", template_function_property->set == nullptr);
    EXPECT("template_function_property-context", template_function_property->context == nullptr);

    auto property = reflection->property.find("Property");

    ASSERT("property", property != nullptr);
    EXPECT("property-get", property->get != nullptr);
    EXPECT("property-set", property->set != nullptr);
    EXPECT("property-context", property->context != nullptr);

    auto const_property = reflection->property.find("ConstProperty");

    ASSERT("const_property", const_property != nullptr);
    EXPECT("const_property-get", const_property->get != nullptr);
    EXPECT("const_property-set", const_property->set == nullptr);
    EXPECT("const_property-context", const_property->context != nullptr);
}


TEST_SPACE()
{

struct TestNamedPropertyStruct
{
    bool IsActivated() const { return bIsActivated; }

    void Activate(bool yes)
    {
        bIsActivatedBuffer = bIsActivated;
        bIsActivated = yes;
    }

    bool bIsActivated = false;
    bool bIsActivatedBuffer = false;
};

} // TEST_SPACE

REFLECTABLE_DECLARATION(TestNamedPropertyStruct)
REFLECTABLE_DECLARATION_INIT()

// without macro using we can put function get and variable set to one or inverse,
// just if u want max flexibility, please, use no macro version
REFLECTABLE(TestNamedPropertyStruct)
    NAMED_PROPERTY("IsActivated", IsActivated, Activate)
    NAMED_PROPERTY("Flag", bIsActivated, bIsActivatedBuffer)
REFLECTABLE_INIT()

TEST(TestLibrary, TestNamedProperty)
{
    auto type = rew::global.find("TestNamedPropertyStruct");

    ASSERT("type", type != nullptr);

    auto reflection = type->reflection;

    ASSERT("reflection", reflection != nullptr);

    auto function_property = reflection->property.find("IsActivated");

    ASSERT("function_property", function_property != nullptr);
    ASSERT("function_property-get", function_property->get != nullptr);
    ASSERT("function_property-set", function_property->set != nullptr);
    EXPECT("function_property-context", function_property->context == nullptr);

    auto [function_property_get_ptr, function_property_set_ptr] = function_property->pointer;

    EXPECT("function_property-get-pointer",
        std::any_cast<bool(TestNamedPropertyStruct::*)() const>(function_property_get_ptr) ==
        &TestNamedPropertyStruct::IsActivated
    );
    EXPECT("function_property-set-pointer",
        std::any_cast<void(TestNamedPropertyStruct::*)(bool)>(function_property_set_ptr) ==
        &TestNamedPropertyStruct::Activate
    );

    auto property = reflection->property.find("Flag");

    ASSERT("property", property != nullptr);
    ASSERT("property-get", property->get != nullptr);
    ASSERT("function_property-set", function_property->set != nullptr);
    EXPECT("property-context", property->context != nullptr);

    auto [property_get_ptr, property_set_ptr] = property->pointer;

    EXPECT("property-get-pointer",
        std::any_cast<bool TestNamedPropertyStruct::*>(property_get_ptr) ==
        &TestNamedPropertyStruct::bIsActivated
    );
    EXPECT("property-set-pointer",
        std::any_cast<bool TestNamedPropertyStruct::*>(property_set_ptr) ==
        &TestNamedPropertyStruct::bIsActivatedBuffer
    );
}