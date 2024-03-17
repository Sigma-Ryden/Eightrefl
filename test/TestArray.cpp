#include <RewTestingBase.hpp>

#include <Rew/BuiltIn/array.hpp>

TEST(TestBuiltin, TestArray)
{
    rew::reflectable<std::array<int, 8>>();

    auto type = rew::global.find("std::array<int, 8>");

    auto factory = type->reflection->factory.find("std::array<int, 8>()");

    auto lhs = factory->call({});
    auto rhs = factory->call({});

    auto compare = type->reflection->function.find("std::operator==")->all.begin()->second;

    auto result = compare.call(nullptr, { type->context(lhs), type->context(rhs) });

    EXPECT("compare", std::any_cast<bool>(result) == true);
}
