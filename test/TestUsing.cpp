#include <RewTestingBase.hpp>

TEST_SPACE()
{

template <typename T>
struct TestContainerWithIteratorUsing
{
    struct Iterator {};

    void Insert(Iterator position, T value) {}

    Iterator* Head = nullptr;
};

} // TEST_SPACE

// will not compile!
// TEMPLATE_REFLECTABLE_DECLARATION(template <typename T>, TestContainerWithIteratorUsing<T>::Iterator)
//     REFLECTABLE_NAME(rew::nameof<TestContainerWithIteratorUsing<T>>() + "::Iterator")
// REFLECTABLE_DECLARATION_INIT()

// TEMPLATE_REFLECTABLE(template <typename T>, TestContainerWithIteratorUsing<T>::Iterator)
// REFLECTABLE_INIT()

TEMPLATE_REFLECTABLE_USING
(
    template <typename T>, TestContainerWithIteratorUsing_Iterator,
    TestContainerWithIteratorUsing_Iterator<T>, typename TestContainerWithIteratorUsing<T>::Iterator
)

TEMPLATE_REFLECTABLE_DECLARATION(template <typename T>, TestContainerWithIteratorUsing_Iterator<T>)
    REFLECTABLE_NAME(rew::nameof<TestContainerWithIteratorUsing<T>>() + "::Iterator")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE(template <typename T>, TestContainerWithIteratorUsing_Iterator<T>)
REFLECTABLE_INIT()

TEMPLATE_REFLECTABLE_DECLARATION(template <typename T>, TestContainerWithIteratorUsing<T>)
    REFLECTABLE_NAME("TestContainerWithIteratorUsing<" + rew::nameof<T>() + ">")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE(template <typename T>, TestContainerWithIteratorUsing<T>)
    FUNCTION(Insert, void(TestContainerWithIteratorUsing_Iterator<T>, T))
    PROPERTY(Head, TestContainerWithIteratorUsing_Iterator<T>*)
REFLECTABLE_INIT()

TEST(TestLibrary, TestUsing)
{
    rew::reflectable<TestContainerWithIteratorUsing<int>>();

    auto type = rew::global.find("TestContainerWithIteratorUsing<int>");

    ASSERT("type", type != nullptr);

    auto reflection = type->reflection;

    ASSERT("reflection", reflection != nullptr);

    auto function = reflection->function.find("Insert");

    ASSERT("function", function != nullptr);
    EXPECT("function-type", function->find("void(TestContainerWithIteratorUsing<int>::Iterator, int)") != nullptr);

    auto property = reflection->property.find("Head");

    ASSERT("property", property != nullptr);
    ASSERT("property-type", property->type != nullptr);
    EXPECT("property-type-name", property->type->name == "TestContainerWithIteratorUsing<int>::Iterator*");
}
