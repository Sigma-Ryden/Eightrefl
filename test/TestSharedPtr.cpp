#include <RewTestingBase.hpp>

#include <Rew/BuiltIn/shared_ptr.hpp>

TEST(TestBuiltin, TestSharedPtr)
{
    rew::reflectable<std::shared_ptr<int>>();

    auto type = rew::global.find("std::shared_ptr<int>");

    ASSERT("type", type != nullptr);
    EXPECT("type-name", type->name == "std::shared_ptr<int>");
    EXPECT("type-size", type->size == sizeof(std::shared_ptr<int>));
    EXPECT("type-context", type->context != nullptr);

    auto reflection = type->reflection;

    ASSERT("reflection", reflection != nullptr);
    EXPECT("reflection-name", reflection->name == "std::shared_ptr<int>");

    EXPECT("factory-R()", reflection->factory.find("std::shared_ptr<int>()") != nullptr);

    #ifndef REW_CORE_MINIMAL
    EXPECT("factory-R(std::nullptr_t)", reflection->factory.find("std::shared_ptr<int>(std::nullptr_t)") != nullptr);
    #endif // REW_CORE_MINIMAL

    EXPECT("factory-R(element_type*)", reflection->factory.find("std::shared_ptr<int>(int*)") != nullptr);
    EXPECT("factory-R(R const&)", reflection->factory.find("std::shared_ptr<int>(std::shared_ptr<int> const&)") != nullptr);

    EXPECT("function-operator=", reflection->function.find("operator=") != nullptr);
    EXPECT("function-reset", reflection->function.find("reset") != nullptr);

    #ifndef REW_CORE_MINIMAL
//  EXPECT("function-swap", reflection->function.find("swap") != nullptr);
    #endif // REW_CORE_MINIMAL

    EXPECT("function-get", reflection->function.find("get") != nullptr);
    EXPECT("function-operator*", reflection->function.find("operator*") != nullptr);

    #ifndef REW_CORE_MINIMAL
    EXPECT("function-operator->", reflection->function.find("operator->") != nullptr);
    EXPECT("function-use_count", reflection->function.find("use_count") != nullptr);
    #endif // REW_CORE_MINIMAL

    EXPECT("function-operator bool", reflection->function.find("operator bool") != nullptr);
}
