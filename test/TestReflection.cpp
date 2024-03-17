#include <RewTestingBase.hpp>

#include <Rew/BuiltIn/string.hpp>
#include <Rew/BuiltIn/any.hpp>

TEMPLATE_REFLECTABLE_DECLARATION((template <typename MetaType>), rew::attribute_t<MetaType>)
    REFLECTABLE_NAME("rew::attribute_t<" + NAMEOF(MetaType) + ">")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE((template <typename MetaType>), rew::attribute_t<MetaType>)
REFLECTABLE_INIT()

REFLECTABLE_DECLARATION(rew::parent_t)
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE_DECLARATION(rew::function_t)
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE_DECLARATION(rew::factory_t)
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE_DECLARATION(rew::property_t)
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE_DECLARATION(rew::reflection_t)
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE(rew::reflection_t)
    PROPERTY(name)
    PROPERTY(parent)
    PROPERTY(function)
    PROPERTY(factory)
    PROPERTY(property)
    PROPERTY(meta)
REFLECTABLE_INIT()

REFLECTABLE_DECLARATION(rew::type_t)
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE(rew::type_t)
REFLECTABLE_INIT()

TEST(TestReflection, TestSelfReflection)
{
}


struct TestReflectionProperty
{
    int f() { return 0; }
    void f(int) {}

    const int g() { return 0; }

    const int i = 0;

    int j = 0;
};

REFLECTABLE_DECLARATION(TestReflectionProperty)
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE(TestReflectionProperty)
    PROPERTY(f)
    PROPERTY(g)
    PROPERTY(i)
    PROPERTY(j)
REFLECTABLE_INIT()

TEST(TestReflection, TestProperty)
{
}
