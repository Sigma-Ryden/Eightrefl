#include <RewTestingBase.hpp>

//TEST_SPACE()
//{

enum TestEnumType { Blue, Green, Red, Alpha };

//} // TEST_SPACE

REFLECTABLE_DECLARATION(TestEnumType)
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE(TestEnumType)
    META("Blue", TestEnumType::Blue)
    META("Green", TestEnumType::Green)
//  META("Red", TestEnumType::Red) // skip for test
    META("Alpha", Alpha) // since TestEnumType is no scoped enum, we can ommit 'TestEnum::'
REFLECTABLE_INIT()

TEST(TestCommon, TestEnum)
{
    auto type = rew::global.find("TestEnumType");

    ASSERT("type", type != nullptr);

    auto reflection = type->reflection;

    ASSERT("reflection", reflection != nullptr);

    EXPECT("meta-1", reflection->meta.find("Blue") != nullptr);
    EXPECT("meta-2", reflection->meta.find("Green") != nullptr);
    EXPECT("meta-3", reflection->meta.find("Alpha") != nullptr);
}


TEST_SPACE()
{

enum class TestScopedEnumType { Up, Down, Size };

} // TEST_SPACE

REFLECTABLE_DECLARATION(TestScopedEnumType)
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE(TestScopedEnumType)
    META("Up", TestScopedEnumType::Up)
    META("Down", TestScopedEnumType::Down)
REFLECTABLE_INIT()

TEST(TestCommon, TestScopedEnum)
{
    auto type = rew::global.find("TestScopedEnumType");

    ASSERT("type", type != nullptr);

    auto reflection = type->reflection;

    ASSERT("reflection", reflection != nullptr);

    EXPECT("meta-1", reflection->meta.find("Up") != nullptr);
    EXPECT("meta-2", reflection->meta.find("Down") != nullptr);
}


struct TestEnumAsStringInjection : rew::injectable_t
{
    rew::type_t* xxtype = nullptr;
    std::vector<std::string> xxenum_as_string;

    TestEnumAsStringInjection(rew::type_t* type) : xxtype(type)
    {
    }
    
    ~TestEnumAsStringInjection()
    {
        xxtype->reflection->meta.add("EnumAsString", xxenum_as_string);
    }

    template <typename ReflectableType>
    void type(rew::type_t& type)
    {
        xxenum_as_string.resize((std::size_t)ReflectableType::Size);
    }

    template <typename ReflectableType, typename MetaType>
    void meta(std::string const& name, std::any& meta)
    {
        auto enumerator = (std::size_t)std::any_cast<MetaType&>(meta);
        if (enumerator < (std::size_t)ReflectableType::Size)
        {
            xxenum_as_string[enumerator] = name;
        }
    }
};

REFLECTABLE_DECLARATION(TestEnumAsStringInjection)
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE(TestEnumAsStringInjection)
REFLECTABLE_INIT()

TEST(TestCommon, TestEnumAsString)
{
    auto type = rew::global.find("TestScopedEnumType");

    ASSERT("type", type != nullptr);

    auto reflection = type->reflection;

    ASSERT("reflection", reflection != nullptr);

    rew::find_or_add_injection<TestScopedEnumType, TestEnumAsStringInjection>(type);

    auto injection = type->injection.find("TestEnumAsStringInjection");

    ASSERT("injection", injection != nullptr);

    {
        auto enum_as_string = TestEnumAsStringInjection(type);
        injection->call(enum_as_string);
    }

    auto& enum_as_string = std::any_cast<std::vector<std::string>&>
    (
        *type->reflection->meta.find("EnumAsString")
    );

    EXPECT("enum_as_string", enum_as_string[(std::size_t)TestScopedEnumType::Down] == "Down");
}
