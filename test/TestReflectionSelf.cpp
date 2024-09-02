#include <RewTestingBase.hpp>

#include <TestReflectionSelf/TestAttribute.hpp>

TEST(TestReflectionSelf, TestAttribute)
{
    rew::reflectable<rew::attribute_t<int>>();

    auto type = rew::global.find("rew::attribute_t<int>");

    ASSERT("type", type != nullptr);
    EXPECT("type-name", type->name == "rew::attribute_t<int>");
    EXPECT("type-size", type->size == sizeof(rew::attribute_t<int>));
    EXPECT("type-context", type->context != nullptr);

    auto reflection = type->reflection;

    ASSERT("reflection", reflection != nullptr);
    EXPECT("reflection-name", reflection->name == "rew::attribute_t<int>");

    EXPECT("function-find", reflection->function.find("find") != nullptr);
    EXPECT("function-add", reflection->function.find("add") != nullptr);
    EXPECT("property-all", reflection->property.find("all") != nullptr);
}

TEST(TestReflectionSelf, TestParent)
{
    auto type = rew::global.find("rew::parent_t");

    ASSERT("type", type != nullptr);
    EXPECT("type-name", type->name == "rew::parent_t");
    EXPECT("type-size", type->size == sizeof(rew::parent_t));
    EXPECT("type-context", type->context != nullptr);

    auto reflection = type->reflection;

    ASSERT("reflection", reflection != nullptr);
    EXPECT("reflection-name", reflection->name == "rew::parent_t");

    EXPECT("property-name", reflection->property.find("name") != nullptr);
    EXPECT("property-type", reflection->property.find("type") != nullptr);
    EXPECT("property-cast", reflection->property.find("cast") != nullptr);
    EXPECT("property-meta", reflection->property.find("meta") != nullptr);
}

TEST(TestReflectionSelf, TestFactory)
{
    auto type = rew::global.find("rew::factory_t");

    ASSERT("type", type != nullptr);
    EXPECT("type-name", type->name == "rew::factory_t");
    EXPECT("type-size", type->size == sizeof(rew::factory_t));
    EXPECT("type-context", type->context != nullptr);

    auto reflection = type->reflection;

    ASSERT("reflection", reflection != nullptr);
    EXPECT("reflection-name", reflection->name == "rew::factory_t");

    EXPECT("property-name", reflection->property.find("name") != nullptr);
    EXPECT("property-call", reflection->property.find("call") != nullptr);
    EXPECT("property-arguments", reflection->property.find("arguments") != nullptr);
    EXPECT("property-meta", reflection->property.find("meta") != nullptr);
}

TEST(TestReflectionSelf, TestFunction)
{
    auto type = rew::global.find("rew::function_t");

    ASSERT("type", type != nullptr);
    EXPECT("type-name", type->name == "rew::function_t");
    EXPECT("type-size", type->size == sizeof(rew::function_t));
    EXPECT("type-context", type->context != nullptr);

    auto reflection = type->reflection;

    ASSERT("reflection", reflection != nullptr);
    EXPECT("reflection-name", reflection->name == "rew::function_t");

    EXPECT("property-name", reflection->property.find("name") != nullptr);
    EXPECT("property-call", reflection->property.find("call") != nullptr);
    EXPECT("property-arguments", reflection->property.find("arguments") != nullptr);
    EXPECT("property-result", reflection->property.find("result") != nullptr);
    EXPECT("property-pointer", reflection->property.find("pointer") != nullptr);
    EXPECT("property-meta", reflection->property.find("meta") != nullptr);
}

TEST(TestReflectionSelf, TestProperty)
{
    auto type = rew::global.find("rew::property_t");

    ASSERT("type", type != nullptr);
    EXPECT("type-name", type->name == "rew::property_t");
    EXPECT("type-size", type->size == sizeof(rew::property_t));
    EXPECT("type-context", type->context != nullptr);

    auto reflection = type->reflection;

    ASSERT("reflection", reflection != nullptr);
    EXPECT("reflection-name", reflection->name == "rew::property_t");

    EXPECT("property-name", reflection->property.find("name") != nullptr);
    EXPECT("property-type", reflection->property.find("type") != nullptr);
    EXPECT("property-get", reflection->property.find("get") != nullptr);
    EXPECT("property-set", reflection->property.find("set") != nullptr);
    EXPECT("property-context", reflection->property.find("context") != nullptr);
    EXPECT("property-meta", reflection->property.find("meta") != nullptr);
}

TEST(TestReflectionSelf, TestMeta)
{
    auto type = rew::global.find("rew::meta_t");

    ASSERT("type", type != nullptr);
    EXPECT("type-name", type->name == "rew::meta_t");
    EXPECT("type-size", type->size == sizeof(rew::meta_t));
    EXPECT("type-context", type->context != nullptr);

    auto reflection = type->reflection;

    ASSERT("reflection", reflection != nullptr);
    EXPECT("reflection-name", reflection->name == "rew::meta_t");

    EXPECT("property-name", reflection->property.find("name") != nullptr);
    EXPECT("property-value", reflection->property.find("value") != nullptr);
    EXPECT("property-meta", reflection->property.find("meta") != nullptr);
}

TEST(TestReflectionSelf, TestReflection)
{
    auto type = rew::global.find("rew::reflection_t");

    ASSERT("type", type != nullptr);
    EXPECT("type-name", type->name == "rew::reflection_t");
    EXPECT("type-size", type->size == sizeof(rew::reflection_t));
    EXPECT("type-context", type->context != nullptr);

    auto reflection = type->reflection;

    ASSERT("reflection", reflection != nullptr);
    EXPECT("reflection-name", reflection->name == "rew::reflection_t");

    EXPECT("property-name", reflection->property.find("name") != nullptr);
    EXPECT("property-parent", reflection->property.find("parent") != nullptr);
    EXPECT("property-function", reflection->property.find("function") != nullptr);
    EXPECT("property-factory", reflection->property.find("factory") != nullptr);
    EXPECT("property-property", reflection->property.find("property") != nullptr);
    EXPECT("property-meta", reflection->property.find("meta") != nullptr);
}

TEST(TestReflectionSelf, TestInjection)
{
    auto type = rew::global.find("rew::injection_t");

    ASSERT("type", type != nullptr);
    EXPECT("type-name", type->name == "rew::injection_t");
    EXPECT("type-size", type->size == sizeof(rew::injection_t));
    EXPECT("type-context", type->context != nullptr);

    auto reflection = type->reflection;

    ASSERT("reflection", reflection != nullptr);
    EXPECT("reflection-name", reflection->name == "rew::injection_t");

    EXPECT("property-name", reflection->property.find("name") != nullptr);
    EXPECT("property-call", reflection->property.find("call") != nullptr);
}

TEST(TestReflectionSelf, TestType)
{
    auto type = rew::global.find("rew::type_t");

    ASSERT("type", type != nullptr);
    EXPECT("type-name", type->name == "rew::type_t");
    EXPECT("type-size", type->size == sizeof(rew::type_t));
    EXPECT("type-context", type->context != nullptr);

    auto reflection = type->reflection;

    ASSERT("reflection", reflection != nullptr);
    EXPECT("reflection-name", reflection->name == "rew::type_t");

    EXPECT("property-name", reflection->property.find("name") != nullptr);
    EXPECT("property-reflection", reflection->property.find("reflection") != nullptr);
    EXPECT("property-size", reflection->property.find("size") != nullptr);
    EXPECT("property-context", reflection->property.find("context") != nullptr);
    EXPECT("property-injection", reflection->property.find("injection") != nullptr);
}

TEST(TestReflectionSelf, TestRegistry)
{
    auto type = rew::global.find("rew::registry_t");

    ASSERT("type", type != nullptr);
    EXPECT("type-name", type->name == "rew::registry_t");
    EXPECT("type-size", type->size == sizeof(rew::registry_t));
    EXPECT("type-context", type->context != nullptr);

    auto reflection = type->reflection;

    ASSERT("reflection", reflection != nullptr);
    EXPECT("reflection-name", reflection->name == "rew::registry_t");

    EXPECT("factory-R()", reflection->factory.find("rew::registry_t()") != nullptr);
    EXPECT("function-find", reflection->function.find("find") != nullptr);
    EXPECT("property-all", reflection->property.find("all") != nullptr);
    EXPECT("property-rtti_all", reflection->property.find("rtti_all") != nullptr);
}
