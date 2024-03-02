#include <RewTestingBase.hpp>

#include <Rew/BuiltIn/array.hpp>

TEST_SPACE()
{

struct Struct {};

void Foo(const Struct*) {}

} // TEST_SPACE

REFLECTABLE_DECLARATION(Struct)
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE(Struct)
    FREE_FUNCTION(Foo)
REFLECTABLE_INIT()

TEST(TestLibrary, Test)
{
    rew::global.find("");
    // rew::reflectable<std::array<int, 8>>();

    // auto type = rew::global.find("std::array<int, 8>");

    // auto factory = type->reflection->factory.find("std::array<int, 8>()");

    // auto lhs = factory->call({});
    // auto rhs = factory->call({});

    // auto comparator = type->reflection->function.find("std::operator==")->all.begin()->second;

    // std::any context;
    // auto result = comparator.call(context, { type->context(lhs), type->context(rhs) });

    // EXPECT("comparation", std::any_cast<bool>(result) == true);
}
