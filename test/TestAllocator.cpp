#include <RewTestingBase.hpp>

#include <Rew/BuiltIn/allocator.hpp>

TEST(TestLibrary, TestAllocator)
{
    rew::reflectable<std::allocator<int>>();
    rew::global.find("");
}
