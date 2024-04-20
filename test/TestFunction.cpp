#include <RewTestingBase.hpp>

#include <Rew/BuiltIn/function.hpp>

TEST(TestBuiltin, TestFunction)
{
    rew::reflectable<std::function<void()>>();

    auto type = rew::global.find("std::function<void()>");

    ASSERT("type", type != nullptr);
    EXPECT("type-name", type->name == "std::function<void()>");
    EXPECT("type-size", type->size == sizeof(std::function<void()>));
    EXPECT("type-context", type->context != nullptr);

    auto reflection = type->reflection;

    ASSERT("reflection", reflection != nullptr);
    EXPECT("reflection-name", reflection->name == "std::function<void()>");

    EXPECT("factory-R()", reflection->factory.find("std::function<void()>()") != nullptr);
    EXPECT("factory-R(std::nullptr_t)", reflection->factory.find("std::function<void()>(std::nullptr_t)") != nullptr);
    EXPECT("factory-R(R const&)", reflection->factory.find("std::function<void()>(std::function<void()> const&)") != nullptr);

    EXPECT("function-operator=", reflection->function.find("operator=") != nullptr);

    #ifndef REW_CORE_MINIMAL
    EXPECT("function-swap", reflection->function.find("swap") != nullptr);
    #endif // REW_CORE_MINIMAL

    EXPECT("function-operator bool", reflection->function.find("operator bool") != nullptr);
    EXPECT("function-operator()", reflection->function.find("operator()") != nullptr);

    #ifndef REW_CORE_MINIMAL
    EXPECT("function-target_type", reflection->function.find("target_type") != nullptr);
    #endif // REW_CORE_MINIMAL
}
