#include <RewTestingBase.hpp>

#include "SerializableVisitor.hpp"

TEST_SPACE()
{

struct TBaseData
{
    int x = 0;
};

struct TData : TBaseData
{
    int y = 0;
};

} // TEST_SPACE

BUILTIN_REFLECTABLE(int)
REFLECTABLE_INIT()

REFLECTABLE(TBaseData)
    PROPERTY(x)
REFLECTABLE_INIT()

REFLECTABLE(TData)
    PARENT(TBaseData)
    PROPERTY(y)
REFLECTABLE_INIT()

TEST(TestLibrary, TestSerialization)
{
    serializable_visitor_t serializable;
    for (auto& [name, type] : rew::global.all) type->evaluate(serializable);

    static int s_x = 123;
    static int s_y = 456;

    std::vector<unsigned char> storage;
    {
        TData d;
        d.x = s_x;
        d.y = s_y;

        auto ar = sf::oarchive(storage);
        ar & d;
    }
    {
        TData d;

        auto ar = sf::iarchive(storage);
        ar & d;

        EXPECT("data", d.x == s_x && d.y == s_y);
    }
}
