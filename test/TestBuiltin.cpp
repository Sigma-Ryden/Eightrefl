#include <RewTestingBase.hpp>

#include <Rew/BuiltIn/reference_wrapper.hpp>

REFLECTABLE_USING(int32, int)

REFLECTABLE_DECLARATION(int32)
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE(int32)
REFLECTABLE_INIT()

TEST(TestLibrary, Test)
{
    rew::reflectable<std::reference_wrapper<int32>>();
    auto type = rew::global.find("std::reference_wrapper<int32>");
    (void)type;
}
