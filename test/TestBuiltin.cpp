#include "RewTestingBase.hpp"

#include <string>

REFLECTABLE(int)
    FACTORY(int())
    FACTORY(int(int))
REFLECTABLE_INIT()

//REFLECTABLE(void*)
//REFLECTABLE_INIT()

REFLECTABLE_DECLARATION(void*)

template <typename>
struct name_t;

template <>
struct name_t<int>
{
    static std::string value() { return"int"; }
};

template <>
struct name_t<void*>
{
    static std::string value() { return"void*"; }
};

template <typename T, typename Allocator>
struct name_t<std::vector<T, Allocator>>
{
    static std::string value() {
        return "std::vector<" + name_t<T>::value() + ">";
    }
};

template <typename>
struct is_std_vector : std::false_type {};

template <typename T, typename Allocator>
struct is_std_vector<std::vector<T, Allocator>> : std::true_type {};

CONDITIONAL_REFLECTABLE(is_std_vector<T>::value)
REFLECTABLE_INIT()

struct FSomeData
{
    std::vector<void*> data;
};

REFLECTABLE(FSomeData)
    (void)::rew_reflection_registry_t<decltype(rew::utility::property_value(&FSomeData::data))>{};
    PROPERTY(data)
REFLECTABLE_INIT()

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
