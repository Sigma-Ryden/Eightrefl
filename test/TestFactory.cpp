#include "RewTestingBase.hpp"

#include <memory>

TEST_SPACE()
{

struct TObject {};

} // TEST_SPACE

REFLECTABLE(TObject)
    FACTORY(std::shared_ptr<void>, std::shared_ptr<TObject>)
    FACTORY(void*(TObject*)) // same as FACTORY(void*, TObject*), but searching by "void*(TObject*)"
    FACTORY(TObject*, TObject*)
    FACTORY(int())
REFLECTABLE_INIT()

REFLECTABLE(int)
REFLECTABLE_INIT()

REFLECTABLE(void*)
REFLECTABLE_INIT()

REFLECTABLE(TObject*)
REFLECTABLE_INIT()

REFLECTABLE(std::shared_ptr<void>)
REFLECTABLE_INIT()

TEST(TestLibrary, TestFactory)
{
    auto type = rew::registry.find("TObject");
    {
        auto factory = type->reflection->factory.find("std::shared_ptr<void>, std::shared_ptr<TObject>");
        ASSERT("shared_ptr-factory-null", factory != nullptr);

        auto instance = factory->call({ std::make_shared<TObject>() });

        // same as static_cast<void*>(std::any_cast<std::shared_ptr<void>&>(instance).get())
        auto ptr = rew::registry.find(instance)->context(instance);
        EXPECT("shared_ptr-ptr", ptr != nullptr);
    }
    {
        auto factory = type->reflection->factory.find("void*(TObject*)");
        ASSERT("void*-factory-null", factory != nullptr);

        auto instance = factory->call({ new TObject });

        // same as static_cast<void*>(std::any_cast<void*&>(instance))
        auto ptr = rew::registry.find(instance)->context(instance);
        EXPECT("void*-ptr", ptr != nullptr);
    }
    {
        auto factory = type->reflection->factory.find("TObject*, TObject*");
        ASSERT("type*-factory-null", factory != nullptr);

        auto instance = factory->call({ new TObject });

        // same as static_cast<void*>(std::any_cast<TObject*&>(instance))
        auto ptr = rew::registry.find(instance)->context(instance);
        EXPECT("type*-ptr", ptr != nullptr);
    }
    {
        auto factory = type->reflection->factory.find("int()");
        ASSERT("int-factory-null", factory != nullptr);

        auto instance = factory->call({});

        // same as static_cast<void*>(std::addressof(std::any_cast<int&>(instance)))
        // but obtained pointer is not refered to TObject type
        auto ptr = rew::registry.find(instance)->context(instance);
        EXPECT("int-ptr", ptr != nullptr);
    }
}
