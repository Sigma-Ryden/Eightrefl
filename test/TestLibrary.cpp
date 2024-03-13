#include <RewTestingBase.hpp>

TEST(TestLibrary, TestConst)
{
    EXPECT("type", rew::global.find("int const") == nullptr);
    EXPECT("type-name", rew::meta::reflectable_traits<const int>::name() == "int const");
}

TEST(TestLibrary, TestRef)
{
    EXPECT("type", rew::global.find<int&>() == nullptr);
    EXPECT("type-name", rew::meta::reflectable_traits<int&>::name() == "int&");
}

TEST(TestLibrary, TestConstRef)
{
    EXPECT("type", rew::global.find<const int&>() == nullptr);
    EXPECT("type-name", rew::meta::reflectable_traits<const int&>::name() == "int const&");
}

TEST(TestLibrary, TestRawFunction)
{
    EXPECT("type-name", rew::meta::reflectable_traits<void() const>::name() == "void() const");
}

#include <Rew/BuiltIn/array.hpp>

TEST(TestLibrary, TestSimple)
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
