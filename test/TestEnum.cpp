#include <RewTestingBase.hpp>

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

TEST(TestCommon, TestEnum)
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

TEST(TestCommon, TestScopedEnum)
{
    auto type = rew::global.find("TestScopedEnum");

    ASSERT("type", type != nullptr);

    auto reflection = type->reflection;

    ASSERT("reflection", reflection != nullptr);

    EXPECT("meta-1", reflection->meta.find("Up") != nullptr);
    EXPECT("meta-2", reflection->meta.find("Down") != nullptr);
    EXPECT("meta-3", reflection->meta.find("Size") != nullptr);
}
