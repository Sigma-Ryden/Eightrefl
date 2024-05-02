#include <RewTestingBase.hpp>

TEST(TestLibrary, TestConst)
{
    EXPECT("type", rew::global.find("int const") == nullptr);
    EXPECT("type-name", rew::meta::reflectable_traits<const int>::name() == "int const");
}

TEST(TestLibrary, TestRef)
{
    EXPECT("type", rew::global.find<int&>() == nullptr);
    EXPECT("type-name", rew::meta::reflectable_traits<int&>::name() == "int&");
}

TEST(TestLibrary, TestConstRef)
{
    EXPECT("type", rew::global.find<const int&>() == nullptr);
    EXPECT("type-name", rew::meta::reflectable_traits<const int&>::name() == "int const&");
}

TEST(TestLibrary, TestRawFunction)
{
    EXPECT("type-name", rew::meta::reflectable_traits<void() const>::name() == "void() const");
}

TEST_SPACE()
{

struct TestFieldPropertyStruct
{
    int FieldProperty = 0;
    int const ReadonlyFieldProperty = 0;
    int& ReferenceFieldProperty;
    static int StaticFieldProperty;
    static int const StaticReadonlyFieldProperty;
};

int TestFieldPropertyStruct::StaticFieldProperty = 0;
int const TestFieldPropertyStruct::StaticReadonlyFieldProperty = 0;

int FreeFieldProperty = 0;
int const FreeReadonlyFieldProperty = 0;
int& FreeReferenceFieldProperty = FreeFieldProperty;

} // TEST_SPACE

REFLECTABLE_DECLARATION(TestFieldPropertyStruct)
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE(TestFieldPropertyStruct)
    PROPERTY(FieldProperty)
    PROPERTY(ReadonlyFieldProperty)
//  PROPERTY(ReferenceFieldProperty) // unsupported!
    PROPERTY(StaticFieldProperty)
    PROPERTY(StaticReadonlyFieldProperty)
    FREE_PROPERTY(FreeFieldProperty)
    FREE_PROPERTY(FreeReadonlyFieldProperty)
    FREE_PROPERTY(FreeReferenceFieldProperty)
REFLECTABLE_INIT()

TEST(TestLibrary, TestFieldProperty)
{
    auto type = rew::global.find("TestFieldPropertyStruct");

    ASSERT("type", type != nullptr);

    auto reflection = type->reflection;

    ASSERT("reflection", reflection != nullptr);

    auto field_property = reflection->property.find("FieldProperty");

    ASSERT("property-field_property", field_property != nullptr);
    EXPECT("property-field_property-get", field_property->get != nullptr);
    EXPECT("property-field_property-set", field_property->set != nullptr);
    EXPECT("property-field_property-context", field_property->context != nullptr);

    auto readonly_field_property = reflection->property.find("ReadonlyFieldProperty");

    ASSERT("property-readonly_field_property", readonly_field_property != nullptr);
    EXPECT("property-readonly_field_property-get", readonly_field_property->get != nullptr);
    EXPECT("property-readonly_field_property-set", readonly_field_property->set == nullptr);
    EXPECT("property-readonly_field_property-context", readonly_field_property->context != nullptr);

    auto static_field_property = reflection->property.find("StaticFieldProperty");

    ASSERT("property-static_field_property", static_field_property != nullptr);
    EXPECT("property-static_field_property-get", static_field_property->get != nullptr);
    EXPECT("property-static_field_property-set", static_field_property->set != nullptr);
    EXPECT("property-static_field_property-context", static_field_property->context != nullptr);

    auto static_readonly_field_property = reflection->property.find("StaticReadonlyFieldProperty");

    ASSERT("property-static_readonly_field_property", static_readonly_field_property != nullptr);
    EXPECT("property-static_readonly_field_property-get", static_readonly_field_property->get != nullptr);
    EXPECT("property-static_readonly_field_property-set", static_readonly_field_property->set == nullptr);
    EXPECT("property-static_readonly_field_property-context", static_readonly_field_property->context != nullptr);

    auto free_field_property = reflection->property.find("FreeFieldProperty");

    ASSERT("property-free_field_property", free_field_property != nullptr);
    EXPECT("property-free_field_property-get", free_field_property->get != nullptr);
    EXPECT("property-free_field_property-set", free_field_property->set != nullptr);
    EXPECT("property-free_field_property-context", free_field_property->context != nullptr);

    auto free_readonly_field_property = reflection->property.find("FreeReadonlyFieldProperty");

    ASSERT("property-free_readonly_field_property", free_readonly_field_property != nullptr);
    EXPECT("property-free_readonly_field_property-get", free_readonly_field_property->get != nullptr);
    EXPECT("property-free_readonly_field_property-set", free_readonly_field_property->set == nullptr);
    EXPECT("property-free_readonly_field_property-context", free_readonly_field_property->context != nullptr);

    auto free_reference_field_property = reflection->property.find("FreeReferenceFieldProperty");

    ASSERT("property-free_reference_field_property", free_reference_field_property != nullptr);
    EXPECT("property-free_reference_field_property-get", free_reference_field_property->get != nullptr);
    EXPECT("property-free_reference_field_property-set", free_reference_field_property->set != nullptr);
    EXPECT("property-free_reference_field_property-context", free_reference_field_property->context != nullptr);
}

TEST_SPACE()
{

struct TestFunctionPropertyStruct : TestFieldPropertyStruct
{
    int& FunctionPropertyWithContext() { return FieldProperty; }
    void FunctionPropertyWithContext(int& value) { FieldProperty = value; }
    int const& FunctionPropertyWithConstContext() { return FieldProperty; }
    void FunctionPropertyWithConstContext(int const& value) { FieldProperty = value; }
    int const& ConstFunctionPropertyWithConstContext() const { return FieldProperty; }
    void ConstFunctionPropertyWithConstContext(int const& value) { FieldProperty = value; }
    int FunctionPropertyNoContext() { return 0; }
    void FunctionPropertyNoContext(int) {}
    int ConstFunctionPropertyNoContext() const { return 0; }
    void ConstFunctionPropertyNoContext(int) {}

    int& ReadonlyFunctionPropertyWithContext() { return FieldProperty; }
    int const& ReadonlyFunctionPropertyWithConstContext() { return FieldProperty; }
    int const& ReadonlyConstFunctionPropertyWithConstContext() const { return FieldProperty; }
    int ReadonlyFunctionPropertyNoContext() { return 0; }
    int ReadonlyConstFunctionPropertyNoContext() const { return 0; }

    static int& StaticFunctionPropertyWithContext() { return StaticFieldProperty; }
    static void StaticFunctionPropertyWithContext(int& value) { StaticFieldProperty = value; }
    static int const& StaticFunctionPropertyWithConstContext() { return StaticFieldProperty; }
    static void StaticFunctionPropertyWithConstContext(int const& value) { StaticFieldProperty = value; }
    static int StaticFunctionPropertyNoContext() { return 0; }
    static void StaticFunctionPropertyNoContext(int) {}

    static int& StaticReadonlyFunctionPropertyWithContext() { return StaticFieldProperty; }
    static int const& StaticReadonlyFunctionPropertyWithConstContext() { return StaticFieldProperty; }
    static int StaticReadonlyFunctionPropertyNoContext() { return 0; }
};

int& FreeFunctionPropertyWithContext() { return FreeFieldProperty; }
void FreeFunctionPropertyWithContext(int& value) { FreeFieldProperty = value; }
int const& FreeFunctionPropertyWithConstContext() { return FreeFieldProperty; }
void FreeFunctionPropertyWithConstContext(int const& value) { FreeFieldProperty = value; }
int FreeFunctionPropertyNoContext() { return 0; }
void FreeFunctionPropertyNoContext(int) {}

int& FreeReadonlyFunctionPropertyWithContext() { return FreeFieldProperty; }
int const& FreeReadonlyFunctionPropertyWithConstContext() { return FreeFieldProperty; }
int FreeReadonlyFunctionPropertyNoContext() { return 0; }

} // TEST_SPACE

REFLECTABLE_DECLARATION(TestFunctionPropertyStruct)
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE(TestFunctionPropertyStruct)
    PROPERTY(FunctionPropertyWithContext)
    PROPERTY(FunctionPropertyWithConstContext)
    PROPERTY(ConstFunctionPropertyWithConstContext)
    PROPERTY(FunctionPropertyNoContext)
    PROPERTY(ConstFunctionPropertyNoContext)

    PROPERTY(ReadonlyFunctionPropertyWithContext)
    PROPERTY(ReadonlyFunctionPropertyWithConstContext)
    PROPERTY(ReadonlyConstFunctionPropertyWithConstContext)
    PROPERTY(ReadonlyFunctionPropertyNoContext)
    PROPERTY(ReadonlyConstFunctionPropertyNoContext)

    PROPERTY(StaticFunctionPropertyWithContext)
    PROPERTY(StaticFunctionPropertyWithConstContext)
    PROPERTY(StaticFunctionPropertyNoContext)

    PROPERTY(StaticReadonlyFunctionPropertyWithContext)
    PROPERTY(StaticReadonlyFunctionPropertyWithConstContext)
    PROPERTY(StaticReadonlyFunctionPropertyNoContext)

    FREE_PROPERTY(FreeFunctionPropertyWithContext)
    FREE_PROPERTY(FreeFunctionPropertyWithConstContext)
    FREE_PROPERTY(FreeFunctionPropertyNoContext)

    FREE_PROPERTY(FreeReadonlyFunctionPropertyWithContext)
    FREE_PROPERTY(FreeReadonlyFunctionPropertyWithConstContext)
    FREE_PROPERTY(FreeReadonlyFunctionPropertyNoContext)
REFLECTABLE_INIT()

TEST(TestLibrary, TestFunctionProperty)
{
    auto type = rew::global.find("TestFunctionPropertyStruct");

    ASSERT("type", type != nullptr);

    auto reflection = type->reflection;

    ASSERT("reflection", reflection != nullptr);

    auto function_property_with_context = reflection->property.find("FunctionPropertyWithContext");

    ASSERT("property-function_property_with_context", function_property_with_context != nullptr);
    EXPECT("property-function_property_with_context-get", function_property_with_context->get != nullptr);
    EXPECT("property-function_property_with_context-set", function_property_with_context->set != nullptr);
    EXPECT("property-function_property_with_context-context", function_property_with_context->context != nullptr);

    auto function_property_with_const_context = reflection->property.find("FunctionPropertyWithConstContext");

    ASSERT("property-function_property_with_const_context", function_property_with_const_context != nullptr);
    EXPECT("property-function_property_with_const_context-get", function_property_with_const_context->get != nullptr);
    EXPECT("property-function_property_with_const_context-set", function_property_with_const_context->set != nullptr);
    EXPECT("property-function_property_with_const_context-context", function_property_with_const_context->context != nullptr);

    auto const_function_property_with_const_context = reflection->property.find("ConstFunctionPropertyWithConstContext");

    ASSERT("property-const_function_property_with_const_context", const_function_property_with_const_context != nullptr);
    EXPECT("property-const_function_property_with_const_context-get", const_function_property_with_const_context->get != nullptr);
    EXPECT("property-const_function_property_with_const_context-set", const_function_property_with_const_context->set != nullptr);
    EXPECT("property-const_function_property_with_const_context-context", const_function_property_with_const_context->context != nullptr);

    auto function_property_no_context = reflection->property.find("FunctionPropertyNoContext");

    ASSERT("property-function_property_no_context", function_property_no_context != nullptr);
    EXPECT("property-function_property_no_context-get", function_property_no_context->get != nullptr);
    EXPECT("property-function_property_no_context-set", function_property_no_context->set != nullptr);
    EXPECT("property-function_property_no_context-context", function_property_no_context->context == nullptr);

    auto const_function_property_no_context = reflection->property.find("ConstFunctionPropertyNoContext");

    ASSERT("property-const_function_property_no_context", const_function_property_no_context != nullptr);
    EXPECT("property-const_function_property_no_context-get", const_function_property_no_context->get != nullptr);
    EXPECT("property-const_function_property_no_context-set", const_function_property_no_context->set != nullptr);
    EXPECT("property-const_function_property_no_context-context", const_function_property_no_context->context == nullptr);


    auto readonly_function_property_with_context = reflection->property.find("ReadonlyFunctionPropertyWithContext");

    ASSERT("property-readonly_function_property_with_context", readonly_function_property_with_context != nullptr);
    EXPECT("property-readonly_function_property_with_context-get", readonly_function_property_with_context->get != nullptr);
    EXPECT("property-readonly_function_property_with_context-set", readonly_function_property_with_context->set == nullptr);
    EXPECT("property-readonly_function_property_with_context-context", readonly_function_property_with_context->context != nullptr);

    auto readonly_function_property_with_const_context = reflection->property.find("ReadonlyFunctionPropertyWithConstContext");

    ASSERT("property-readonly_function_property_with_const_context", readonly_function_property_with_const_context != nullptr);
    EXPECT("property-readonly_function_property_with_const_context-get", readonly_function_property_with_const_context->get != nullptr);
    EXPECT("property-readonly_function_property_with_const_context-set", readonly_function_property_with_const_context->set == nullptr);
    EXPECT("property-readonly_function_property_with_const_context-context", readonly_function_property_with_const_context->context != nullptr);

    auto readonly_const_function_property_with_const_context = reflection->property.find("ReadonlyConstFunctionPropertyWithConstContext");

    ASSERT("property-readonly_const_function_property_with_const_context", readonly_const_function_property_with_const_context != nullptr);
    EXPECT("property-readonly_const_function_property_with_const_context-get", readonly_const_function_property_with_const_context->get != nullptr);
    EXPECT("property-readonly_const_function_property_with_const_context-set", readonly_const_function_property_with_const_context->set == nullptr);
    EXPECT("property-readonly_const_function_property_with_const_context-context", readonly_const_function_property_with_const_context->context != nullptr);

    auto readonly_function_property_no_context = reflection->property.find("ReadonlyFunctionPropertyNoContext");

    ASSERT("property-readonly_function_property_no_context", readonly_function_property_no_context != nullptr);
    EXPECT("property-readonly_function_property_no_context-get", readonly_function_property_no_context->get != nullptr);
    EXPECT("property-readonly_function_property_no_context-set", readonly_function_property_no_context->set == nullptr);
    EXPECT("property-readonly_function_property_no_context-context", readonly_function_property_no_context->context == nullptr);

    auto readonly_const_function_property_no_context = reflection->property.find("ReadonlyConstFunctionPropertyNoContext");

    ASSERT("property-readonly_const_function_property_no_context", readonly_const_function_property_no_context != nullptr);
    EXPECT("property-readonly_const_function_property_no_context-get", readonly_const_function_property_no_context->get != nullptr);
    EXPECT("property-readonly_const_function_property_no_context-set", readonly_const_function_property_no_context->set == nullptr);
    EXPECT("property-readonly_const_function_property_no_context-context", readonly_const_function_property_no_context->context == nullptr);


    auto static_function_property_with_context = reflection->property.find("StaticFunctionPropertyWithContext");

    ASSERT("property-static_function_property_with_context", static_function_property_with_context != nullptr);
    EXPECT("property-static_function_property_with_context-get", static_function_property_with_context->get != nullptr);
    EXPECT("property-static_function_property_with_context-set", static_function_property_with_context->set != nullptr);
    EXPECT("property-static_function_property_with_context-context", static_function_property_with_context->context != nullptr);

    auto static_function_property_with_const_context = reflection->property.find("StaticFunctionPropertyWithConstContext");

    ASSERT("property-static_function_property_with_const_context", static_function_property_with_const_context != nullptr);
    EXPECT("property-static_function_property_with_const_context-get", static_function_property_with_const_context->get != nullptr);
    EXPECT("property-static_function_property_with_const_context-set", static_function_property_with_const_context->set != nullptr);
    EXPECT("property-static_function_property_with_const_context-context", static_function_property_with_const_context->context != nullptr);

    auto static_function_property_no_context = reflection->property.find("StaticFunctionPropertyNoContext");

    ASSERT("property-static_function_property_no_context", static_function_property_no_context != nullptr);
    EXPECT("property-static_function_property_no_context-get", static_function_property_no_context->get != nullptr);
    EXPECT("property-static_function_property_no_context-set", static_function_property_no_context->set != nullptr);
    EXPECT("property-static_function_property_no_context-context", static_function_property_no_context->context == nullptr);


    auto static_readonly_function_property_with_context = reflection->property.find("StaticReadonlyFunctionPropertyWithContext");

    ASSERT("property-static_readonly_function_property_with_context", static_readonly_function_property_with_context != nullptr);
    EXPECT("property-static_readonly_function_property_with_context-get", static_readonly_function_property_with_context->get != nullptr);
    EXPECT("property-static_readonly_function_property_with_context-set", static_readonly_function_property_with_context->set == nullptr);
    EXPECT("property-static_readonly_function_property_with_context-context", static_readonly_function_property_with_context->context != nullptr);

    auto static_readonly_function_property_with_const_context = reflection->property.find("StaticReadonlyFunctionPropertyWithConstContext");

    ASSERT("property-static_readonly_function_property_with_const_context", static_readonly_function_property_with_const_context != nullptr);
    EXPECT("property-static_readonly_function_property_with_const_context-get", static_readonly_function_property_with_const_context->get != nullptr);
    EXPECT("property-static_readonly_function_property_with_const_context-set", static_readonly_function_property_with_const_context->set == nullptr);
    EXPECT("property-static_readonly_function_property_with_const_context-context", static_readonly_function_property_with_const_context->context != nullptr);

    auto static_readonly_function_property_no_context = reflection->property.find("StaticReadonlyFunctionPropertyNoContext");

    ASSERT("property-static_readonly_function_property_no_context", static_readonly_function_property_no_context != nullptr);
    EXPECT("property-static_readonly_function_property_no_context-get", static_readonly_function_property_no_context->get != nullptr);
    EXPECT("property-static_readonly_function_property_no_context-set", static_readonly_function_property_no_context->set == nullptr);
    EXPECT("property-static_readonly_function_property_no_context-context", static_readonly_function_property_no_context->context == nullptr);


    auto free_function_property_with_context = reflection->property.find("FreeFunctionPropertyWithContext");

    ASSERT("property-free_function_property_with_context", free_function_property_with_context != nullptr);
    EXPECT("property-free_function_property_with_context-get", free_function_property_with_context->get != nullptr);
    EXPECT("property-free_function_property_with_context-set", free_function_property_with_context->set != nullptr);
    EXPECT("property-free_function_property_with_context-context", free_function_property_with_context->context != nullptr);

    auto free_function_property_with_const_context = reflection->property.find("FreeFunctionPropertyWithConstContext");

    ASSERT("property-free_function_property_with_const_context", free_function_property_with_const_context != nullptr);
    EXPECT("property-free_function_property_with_const_context-get", free_function_property_with_const_context->get != nullptr);
    EXPECT("property-free_function_property_with_const_context-set", free_function_property_with_const_context->set != nullptr);
    EXPECT("property-free_function_property_with_const_context-context", free_function_property_with_const_context->context != nullptr);

    auto free_function_property_no_context = reflection->property.find("FreeFunctionPropertyNoContext");

    ASSERT("property-free_function_property_no_context", free_function_property_no_context != nullptr);
    EXPECT("property-free_function_property_no_context-get", free_function_property_no_context->get != nullptr);
    EXPECT("property-free_function_property_no_context-set", free_function_property_no_context->set != nullptr);
    EXPECT("property-free_function_property_no_context-context", free_function_property_no_context->context == nullptr);


    auto free_readonly_function_property_with_context = reflection->property.find("FreeReadonlyFunctionPropertyWithContext");

    ASSERT("property-free_readonly_function_property_with_context", free_readonly_function_property_with_context != nullptr);
    EXPECT("property-free_readonly_function_property_with_context-get", free_readonly_function_property_with_context->get != nullptr);
    EXPECT("property-free_readonly_function_property_with_context-set", free_readonly_function_property_with_context->set == nullptr);
    EXPECT("property-free_readonly_function_property_with_context-context", free_readonly_function_property_with_context->context != nullptr);

    auto free_readonly_function_property_with_const_context = reflection->property.find("FreeReadonlyFunctionPropertyWithConstContext");

    ASSERT("property-free_readonly_function_property_with_const_context", free_readonly_function_property_with_const_context != nullptr);
    EXPECT("property-free_readonly_function_property_with_const_context-get", free_readonly_function_property_with_const_context->get != nullptr);
    EXPECT("property-free_readonly_function_property_with_const_context-set", free_readonly_function_property_with_const_context->set == nullptr);
    EXPECT("property-free_readonly_function_property_with_const_context-context", free_readonly_function_property_with_const_context->context != nullptr);

    auto free_readonly_function_property_no_context = reflection->property.find("FreeReadonlyFunctionPropertyNoContext");

    ASSERT("property-free_readonly_function_property_no_context", free_readonly_function_property_no_context != nullptr);
    EXPECT("property-free_readonly_function_property_no_context-get", free_readonly_function_property_no_context->get != nullptr);
    EXPECT("property-free_readonly_function_property_no_context-set", free_readonly_function_property_no_context->set == nullptr);
    EXPECT("property-free_readonly_function_property_no_context-context", free_readonly_function_property_no_context->context == nullptr);
}
