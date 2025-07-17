#include <EightreflTestingBase.hpp>

#include <Eightrefl/BuiltIn/priority_queue.hpp>

TEST(TestBuiltin, TestPriorityQueue)
{
    eightrefl::reflectable<std::priority_queue<int>>();

    auto type = eightrefl::global.find("std::priority_queue<int>");

    ASSERT("type", type != nullptr);
    EXPECT("type-name", type->name == "std::priority_queue<int>");
    EXPECT("type-size", type->size == sizeof(std::priority_queue<int>));
    EXPECT("type-context", type->context != nullptr);

    auto reflection = type->reflection;

    ASSERT("reflection", reflection != nullptr);
    EXPECT("reflection-name", reflection->name == "std::priority_queue<int>");

    EXPECT("factory-R()", reflection->factory.find("std::priority_queue<int>()") != nullptr);

    #ifdef EIGHTREFL_FULLY_ENABLE
    EXPECT("factory-R(value_compare const&, container_type const&)", reflection->factory.find("std::priority_queue<int>(std::less<int> const&, std::vector<int> const&)") != nullptr);
    EXPECT("factory-R(value_compare const&)", reflection->factory.find("std::priority_queue<int>(std::less<int> const&)") != nullptr);
    #endif // EIGHTREFL_FULLY_ENABLE

    EXPECT("factory-R(R const&)", reflection->factory.find("std::priority_queue<int>(std::priority_queue<int> const&)") != nullptr);

    EXPECT("function-operator=", reflection->function.find("operator=") != nullptr);
    EXPECT("function-top", reflection->function.find("top") != nullptr);
    EXPECT("function-empty", reflection->function.find("empty") != nullptr);
    EXPECT("function-size", reflection->function.find("size") != nullptr);
    EXPECT("function-push", reflection->function.find("push") != nullptr);
    EXPECT("function-pop", reflection->function.find("pop") != nullptr);

    #ifdef EIGHTREFL_FULLY_ENABLE
    EXPECT("function-swap", reflection->function.find("swap") != nullptr);
    #endif // EIGHTREFL_FULLY_ENABLE
}
