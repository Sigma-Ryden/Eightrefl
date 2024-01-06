#include "RewTestingBase.hpp"

#include <string>
#include <memory>

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

template <typename T, typename Allocator>
struct rew::meta::reflectable_name_t<std::vector<T, Allocator>>
{
    static std::string get() { return "std::vector<" + reflectable_name_t<T>::get() + ">"; }
};

CONDITIONAL_REFLECTABLE(is_std_vector<T>::value)
REFLECTABLE_INIT()

struct FSomeData
{
    std::vector<int*> data;

    void Foo() {}
};

template <typename>
struct is_std_shared_ptr : std::false_type {};

template <typename T>
struct is_std_shared_ptr<std::shared_ptr<T>> : std::true_type {};

template <typename T>
struct rew::meta::reflectable_name_t<std::shared_ptr<T>>
{
    static std::string get() { return "std::shared_ptr<" + reflectable_name_t<T>::get() + ">"; }
};

CONDITIONAL_REFLECTABLE(is_std_shared_ptr<T>::value)
    FUNCTION(get)
REFLECTABLE_INIT()

REFLECTABLE(FSomeData)
    PROPERTY(data)
    FUNCTION(Foo)
    FACTORY(std::shared_ptr<FSomeData>(std::shared_ptr<FSomeData>))
REFLECTABLE_INIT()

TEST(TestLibrary, Test)
{
    auto fsome_data_type = rew::global.find("FSomeData");

    auto factory = fsome_data_type->reflection->factory.find("std::shared_ptr<FSomeData>(std::shared_ptr<FSomeData>)");
    auto fsome_data_shared_ptr = factory->call({ std::make_shared<FSomeData>() });

    auto shared_ptr_type = rew::global.find(fsome_data_shared_ptr);
    auto shared_ptr_context = shared_ptr_type->context(fsome_data_shared_ptr);
    auto get_function = shared_ptr_type->reflection->function.find("get");
    auto fsome_data_context = get_function->call(shared_ptr_context, {});

    auto function = fsome_data_type->reflection->function.find("Foo");
    function->call(fsome_data_context, {});
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
