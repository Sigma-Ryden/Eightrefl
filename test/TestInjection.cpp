#include <RewTestingBase.hpp>

TEST_SPACE()
{

struct TestInjectionStruct {};

} // TEST_SPACE

REFLECTABLE_DECLARATION(TestInjectionStruct)
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE(TestInjectionStruct)
REFLECTABLE_INIT()


template <typename T>
struct ToString
{
    std::string Call()
    {
        return "name: " + rew::nameof<T>();
    }
};

struct ToStringInjection : rew::injectable_t
{
    template <typename ReflectableType>
    void type(rew::type_t& type)
    {
        type.reflection->meta.add("ToString", ToString<ReflectableType>());
    }
};

// will reflect injection to all types
// for correct registry u must add injection before all library includes
REFLECTABLE_INJECTION_DECLARATION(0, ToStringInjection)
REFLECTABLE_DECLARATION_INIT()

TEST(TestLibrary, TestDefaultInjection)
{
    auto type = rew::global.find("TestInjectionStruct");

    ASSERT("type", type != nullptr);

    auto reflection = type->reflection;

    ASSERT("reflection", reflection != nullptr);
    EXPECT("before_injection", reflection->meta.find("ToString") == nullptr);
    
    auto injection = type->injection.find("ToStringInjection");

    ASSERT("injection", injection != nullptr);

    ToStringInjection to_string;
    injection->call(to_string);

    auto meta = std::any_cast<ToString<TestInjectionStruct>>(reflection->meta.find("ToString"));

    EXPECT("after_injection", meta != nullptr && meta->Call() == "name: TestInjectionStruct");
}



struct VirusInjection : rew::injectable_t
{
    template <typename ReflectableType, typename FunctionType>
    void factory(rew::factory_t& factory)
    {
        factory.meta.add("IsDefaultConstructible", factory.arguments.size() == 0);
    }
};

REFLECTABLE_DECLARATION(VirusInjection)
REFLECTABLE_DECLARATION_INIT()

TEST(TestLibrary, TestDynamicInjection)
{
    auto type = rew::global.find("int");

    ASSERT("type", type != nullptr);

    auto reflection = type->reflection;

    ASSERT("reflection", reflection != nullptr);

    //EXPECT("default-injection", type->injection.find("ToStringInjection") != nullptr);
    ASSERT("before_add-injection", type->injection.find("VirusInjection") == nullptr);
    
    rew::find_or_add_injection<int, VirusInjection>(type);
 
    auto injection = type->injection.find("VirusInjection");

    ASSERT("after_add-injection", injection != nullptr);

    {
        auto default_constructible = reflection->factory.find("int()")->meta.find("IsDefaultConstructible");
        auto no_default_constructible = reflection->factory.find("int(int)")->meta.find("IsDefaultConstructible");

        EXPECT("before_injection", default_constructible == nullptr && no_default_constructible == nullptr);
    }

    VirusInjection virus;
    injection->call(virus);

    {
        auto default_constructible = reflection->factory.find("int()")->meta.find("IsDefaultConstructible");
        auto no_default_constructible = reflection->factory.find("int(int)")->meta.find("IsDefaultConstructible");

        EXPECT
        (
            "after_injection",
            default_constructible != nullptr &&
            no_default_constructible != nullptr &&
            std::any_cast<bool>(*default_constructible) == true &&
            std::any_cast<bool>(*no_default_constructible) == false
        );
    }
}