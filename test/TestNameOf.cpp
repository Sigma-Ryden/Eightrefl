#include <RewTestingBase.hpp>

TEST(TestLibrary, TestNameOf)
{
    EXPECT("const-type", rew::meta::reflectable_traits<const int>::name() == "int const");
    EXPECT("reference-type", rew::meta::reflectable_traits<int&>::name() == "int&");
    EXPECT("const_reference-type", rew::meta::reflectable_traits<const int&>::name() == "int const&");
    EXPECT("function-type", rew::meta::reflectable_traits<void(int, float)>::name() == "void(int, float)");
    EXPECT("const-function-type", rew::meta::reflectable_traits<void(int, float) const>::name() == "void(int, float) const");
    EXPECT("reference-function-type", rew::meta::reflectable_traits<void(int, float)&>::name() == "void(int, float)&");
    EXPECT("const_reference-function-type", rew::meta::reflectable_traits<void(int, float) const&>::name() == "void(int, float) const&");
}
