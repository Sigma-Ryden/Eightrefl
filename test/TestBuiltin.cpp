#include "RewTestingBase.hpp"

#include <string>

template <typename>
struct is_std_vector : std::false_type {};

template <typename T, typename Allocator>
struct is_std_vector<std::vector<T, Allocator>> : std::true_type {};

REFLECTABLE(int)
    FACTORY(int())
    FACTORY(int(int))
REFLECTABLE_INIT()

//REFLECTABLE(void*)
//REFLECTABLE_INIT()

REFLECTABLE_DECLARATION(void*)

namespace rew::meta
{

template <typename T, typename Allocator>
struct reflectable_name_t<std::vector<T, Allocator>>
{
    static std::string get() { return "std::vector<" + reflectable_name_t<T>::get() + ">"; }
};

} // namespace rew::meta

CONDITIONAL_REFLECTABLE(is_std_vector<T>::value)
REFLECTABLE_INIT()

struct FSomeData
{
    std::vector<int*> data;

    void Foo() {}
};

REFLECTABLE(FSomeData)
    PROPERTY(data)
    FUNCTION(Foo)
    FACTORY(std::shared_ptr<FSomeData>(std::shared_ptr<FSomeData>))
REFLECTABLE_INIT()

TEST(TestLibrary, Test)
{
    auto type = rew::global.find("FSomeData");

    auto factory = type->reflection->factory.find("std::shared_ptr<FSomeData>(std::shared_ptr<FSomeData>)");
    auto object = factory->call({ std::make_shared<FSomeData>() });

    auto sh_type = rew::global.find(object.type());
    // TODO: ...
    auto context = type->context(object);

    auto function = type->reflection->function.find("Foo");
    std::any result;
    function->call(context, result, {});
}

TEST(TestLibrary, TestBuiltin)
{
    using reflectbale_type = int;

    static std::string s_name = "int";
    static reflectbale_type s_instance_value{ 456 };

    auto type = rew::global.find(s_name);
    ASSERT("type-null", type != nullptr);
    EXPECT("type-name", type->name == s_name);

    ASSERT("type-evaluate-null", type->evaluate != nullptr);

    ASSERT("type-reflection-null", type->reflection != nullptr);

    auto default_factory = type->reflection->factory.find("int()");
    ASSERT("type-reflection-factory-default-null", default_factory != nullptr);
    ASSERT("type-reflection-factory-default-args_count", default_factory->arg_count == 0);

    auto default_instance = default_factory->call({});
    ASSERT("type-reflection-factory-default-instance-null", default_instance.has_value());

    auto default_instance_value = std::any_cast<reflectbale_type>(default_instance);
    EXPECT("type-reflection-property-value-get-result-default-value", default_instance_value == reflectbale_type{});

    auto factory = type->reflection->factory.find("int(int)");
    ASSERT("type-reflection-factory-null", factory != nullptr);
    ASSERT("type-reflection-factory-args_count", factory->arg_count == 1);

    auto initialized_instance = factory->call({ s_instance_value });
    ASSERT("type-reflection-factory-instance-null", initialized_instance.has_value());

    auto initialized_instance_value = std::any_cast<reflectbale_type>(initialized_instance);
    EXPECT("type-reflection-property-value-get-result-value", initialized_instance_value == s_instance_value);
}
