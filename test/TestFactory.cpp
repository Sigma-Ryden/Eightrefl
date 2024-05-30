#include <RewTestingBase.hpp>

TEST_SPACE()
{

struct TestFactoryStruct {};

struct TestCustomFactoryStruct
{
    TestCustomFactoryStruct()
    {
        result = new TestFactoryStruct();
    }

    void* result = nullptr;
};

} // TEST_SPACE

REFLECTABLE_DECLARATION(TestCustomFactoryStruct)
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE_DECLARATION(TestFactoryStruct)
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE(TestFactoryStruct)
    FACTORY(R())
    FACTORY(TestCustomFactoryStruct())
REFLECTABLE_INIT()

TEST(TestLibrary, TestFactoryStruct)
{
    auto type = rew::global.find("TestFactoryStruct");

    ASSERT("type", type != nullptr);

    auto reflection = type->reflection;

    ASSERT("reflection", reflection != nullptr);

    EXPECT("factory", reflection->factory.find("TestFactoryStruct()") != nullptr);

    auto custom = reflection->factory.find("TestCustomFactoryStruct()");

    EXPECT("custom-factory", custom != nullptr);

    auto object = custom->call({});
    auto object_ptr = std::any_cast<TestCustomFactoryStruct>(&object);

    EXPECT("custom-factory-object", object_ptr != nullptr && object_ptr->result);
}
