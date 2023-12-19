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

TEST(TestLibrary, TestFactory)
{
    auto type = rew::registry.find("TObject");
    {
        auto factory = type->reflection->factory.find("std::shared_ptr<void>, std::shared_ptr<TObject>");
        ASSERT("shared_ptr-factory-null", factory != nullptr);

        auto instance = factory->call({ std::make_shared<TObject>() });
        auto ptr = type->ptr(instance); // return static_cast<void*>(std::any_cast<std::shared_ptr<void>&>(instance).get())
        EXPECT("shared_ptr-ptr", ptr != nullptr);
    }
    {
        auto factory = type->reflection->factory.find("void*(TObject*)");
        ASSERT("void*-factory-null", factory != nullptr);

        auto instance = factory->call({ new TObject });
        auto ptr = type->ptr(instance); // return static_cast<void*>(std::any_cast<void*&>(instance))
        EXPECT("void*-ptr", ptr != nullptr);
    }
    {
        auto factory = type->reflection->factory.find("TObject*, TObject*");
        ASSERT("type*-factory-null", factory != nullptr);

        auto instance = factory->call({ new TObject });
        auto ptr = type->ptr(instance); // return static_cast<void*>(std::any_cast<TObject*&>(instance))
        EXPECT("type*-ptr", ptr != nullptr);
    }
    {
        auto factory = type->reflection->factory.find("int()");
        ASSERT("int-factory-null", factory != nullptr);

        auto instance = factory->call({});
        auto ptr = type->ptr(instance); // return static_cast<void*>(std::addressof(std::any_cast<int&>(instance)))
        EXPECT("int-ptr", ptr != nullptr); // but obtained pointer is not refered to TObject type
    }
}
