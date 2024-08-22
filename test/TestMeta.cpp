#include <RewTestingBase.hpp>

#include <vector> // vector
#include <string> // string

TEST_SPACE()
{

struct TestParentMetaStruct {};

struct TestMetaStruct : TestParentMetaStruct
{
    TestMetaStruct() {}
    static int Function() { return 0; }
    void Function(double value) {}
    float Property = 0.f;
    int Constant = 0;

    inline static char* TestDescriptor = nullptr;
};

int TestMetaStructPropertyCast(float) { return 0; }

namespace TestMetaStructFlags
{

enum Type : long { Abstract, Internal, Serializable };

} // TestMetaStructFlags

struct TestMetaStructConfigs
{
    bool const IsVisible = false;
    float LifeTime = -1,f;
    char const* Name = "UNKNOWN";
};

std::string sMetaName = "Descriptors";
void* TestDescriptorReadOnly = nullptr;

struct FunctionHandlerStruct
{
    void operator()(double) {}

    bool IsAsync = true;
};

FunctionHandlerStruct FunctionHandler{ false };

} // TEST_SPACE

REFLECTABLE_DECLARATION(TestParentMetaStruct)
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE(TestParentMetaStruct)
REFLECTABLE_INIT()

REFLECTABLE_DECLARATION(TestMetaStruct)
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE(TestMetaStruct)
    META("DisplayName", "Meta Struct")
    META("Configs", TestMetaStructConfigs{ true, 100.f })
    META(sMetaName, std::vector<void*>{ &R::TestDescriptor, &TestDescriptorReadOnly })

    PARENT(TestParentMetaStruct) META("Hidden", true)

    FACTORY(R()) META("ConstructionType", std::string("PostLoad"))

    FUNCTION(Function, int()) META("MinValue", -1) META("MaxValue", 10) META("static", true)
    FUNCTION(Function, void(double)) META("Handler", FunctionHandler)

    PROPERTY(Property) META("Cast", &TestMetaStructPropertyCast)
    META("Flags", TestMetaStructFlags::Serializable | TestMetaStructFlags::Internal)

    PROPERTY(Constant) META("Mutable", true)
REFLECTABLE_INIT()

TEST(TestLibrary, TestMeta)
{
    auto type = rew::global.find("TestMetaStruct");

    ASSERT("type", type != nullptr);

    auto reflection = type->reflection;

    ASSERT("reflection", reflection != nullptr);

    {
        auto meta0 = std::any_cast<char const*>(reflection->meta.find("DisplayName"));

        ASSERT("reflection-meta0", meta0 != nullptr);
        EXPECT("reflection-meta0-value", std::string(*meta0) == "Meta Struct");
    }
    {
        auto meta1 = std::any_cast<TestMetaStructConfigs>(reflection->meta.find("Configs"));

        ASSERT("reflection-meta1", meta1 != nullptr);
        EXPECT("reflection-meta1-value", meta1->IsVisible == true && meta1->LifeTime == 100.f);
    }
    {
        auto meta2 = std::any_cast<std::vector<void*>>(reflection->meta.find(sMetaName));

        ASSERT("reflection-meta2", meta2 != nullptr);
        EXPECT("reflection-meta2-value",
            meta2->size() == 2 && (*meta2)[0] == &TestMetaStruct::TestDescriptor && (*meta2)[1] == &TestDescriptorReadOnly);
    }

    auto parent = reflection->parent.find("TestParentMetaStruct");

    ASSERT("parent", parent != nullptr);

    {
        auto meta0 = std::any_cast<bool>(parent->meta.find("Hidden"));

        ASSERT("parent-meta0", meta0 != nullptr);
        EXPECT("parent-meta0-value", *meta0 == true);
    }

    auto factory = reflection->factory.find("TestMetaStruct()");

    ASSERT("factory", factory != nullptr);

    {
        auto meta0 = std::any_cast<std::string>(factory->meta.find("ConstructionType"));

        ASSERT("factory-meta0", meta0 != nullptr);
        EXPECT("factory-meta0-value", *meta0 == "PostLoad");
    }

    auto overloads = reflection->function.find("Function");

    ASSERT("overloads", overloads != nullptr);

    auto function0 = overloads->find("int()");

    ASSERT("function0", function0 != nullptr);

    {
        auto meta0 = std::any_cast<int>(function0->meta.find("MinValue"));

        ASSERT("function0-meta0", meta0 != nullptr);
        EXPECT("function0-meta0-value", *meta0 == -1);
    }
    {
        auto meta1 = std::any_cast<int>(function0->meta.find("MaxValue"));

        ASSERT("function0-meta1", meta1 != nullptr);
        EXPECT("function0-meta1-value", *meta1 == 10);
    }
    {
        auto meta2 = std::any_cast<bool>(function0->meta.find("static"));

        ASSERT("function0-meta2", meta2 != nullptr);
        EXPECT("function0-meta2-value", *meta2 == true);
    }

    auto function1 = overloads->find("void(double)");

    ASSERT("function1", function1 != nullptr);

    {
        auto meta0 = std::any_cast<FunctionHandlerStruct>(function1->meta.find("Handler"));

        ASSERT("function1-meta0", meta0 != nullptr);
        EXPECT("function1-meta0-value", meta0->IsAsync == FunctionHandler.IsAsync);
    }

    auto property0 = reflection->property.find("Property");

    ASSERT("property0", property0 != nullptr);

    {
        using meta0_type = decltype(&TestMetaStructPropertyCast);

        auto meta0 = std::any_cast<meta0_type>(property0->meta.find("Cast"));

        ASSERT("property0-meta0", meta0 != nullptr);
        EXPECT("property0-meta0-value", *meta0 == &TestMetaStructPropertyCast);
    }
    {
        auto meta1 = std::any_cast<long>(property0->meta.find("Flags"));

        ASSERT("property0-meta1", meta1 != nullptr);
        EXPECT("property0-meta1-value", *meta1 == TestMetaStructFlags::Serializable | TestMetaStructFlags::Internal);
    }

    auto property1 = reflection->property.find("Constant");

    ASSERT("property1", property1 != nullptr);

    {
        auto meta0 = std::any_cast<bool>(property1->meta.find("Mutable"));

        ASSERT("meta0", meta0 != nullptr);
        EXPECT("Mutable", *meta0 == true);
    }
}
