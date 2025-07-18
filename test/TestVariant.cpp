#include <EightreflTestingBase.hpp>

#include <Eightrefl/Standard/variant.hpp>

TEST(TestBuiltin, TestVariant)
{
    eightrefl::reflectable<std::variant<int, float, bool>>();

    auto type = eightrefl::standard()->find("std::variant<int, float, bool>");

    ASSERT("type", type != nullptr);
    EXPECT("type-name", type->name == "std::variant<int, float, bool>");
    EXPECT("type-size", type->size == sizeof(std::variant<int, float, bool>));
    EXPECT("type-context", type->context != nullptr);

    auto reflection = type->reflection;

    ASSERT("reflection", reflection != nullptr);
    EXPECT("reflection-name", reflection->name == "std::variant<int, float, bool>");

    EXPECT("factory-R()", reflection->factory.find("std::variant<int, float, bool>()") != nullptr);
    EXPECT("factory-R(R const&)", reflection->factory.find("std::variant<int, float, bool>(std::variant<int, float, bool> const&)") != nullptr);

    EXPECT("function-operator=", reflection->function.find("operator=") != nullptr);
//  EXPECT("function-index", reflection->function.find("index") != nullptr);

    #ifdef EIGHTREFL_FULLY_ENABLE
//  EXPECT("function-valueless_by_exception", reflection->function.find("valueless_by_exception") != nullptr);
    EXPECT("function-swap", reflection->function.find("swap") != nullptr);
    #endif // EIGHTREFL_FULLY_ENABLE
}
