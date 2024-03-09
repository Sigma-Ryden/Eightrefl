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
