#include "RewTestingBase.hpp"

#include <string>
#include <memory>

REFLECTABLE(int)
    FACTORY(int())
    FACTORY(int(int))
REFLECTABLE_INIT()

TEMPLATE_REFLECTABLE((template <typename T>), (T*), (NAMEOF(T)+"*"))
    FACTORY(T*)
REFLECTABLE_INIT()

TEMPLATE_REFLECTABLE((template <typename T, typename Allocator>), (std::vector<T, Allocator>), ("std::vector<"+NAMEOF(T)+">"))
REFLECTABLE_INIT()

template <typename T>
struct FSomeDataBase
{
    T* i;
};

TEMPLATE_REFLECTABLE((template <typename T>), (FSomeDataBase<T>), ("FSomeDataBase<"+NAMEOF(T)+">"))
REFLECTABLE_INIT()

template <typename T>
struct FSomeData : FSomeDataBase<T>
{
    std::vector<T*> data;

    void Foo(T) {}
};

TEMPLATE_REFLECTABLE((template <typename T>), (std::shared_ptr<T>), ("std::shared_ptr<"+NAMEOF(T)+">"))
    //FUNCTION(swap)
    FUNCTION(get)
    FUNCTION(operator*)
    FUNCTION(operator->)
    //FUNCTION(reset)
    FUNCTION(use_count)
    FUNCTION(operator bool)
REFLECTABLE_INIT()

TEMPLATE_REFLECTABLE((template <typename T>), (FSomeData<T>), ("FSomeData<"+NAMEOF(T)+">"))
    PARENT(FSomeDataBase<T>)
    PROPERTY(data)
    PROPERTY(i)
    FUNCTION(Foo, T)
    FACTORY(std::shared_ptr<FSomeData<T>>(std::shared_ptr<FSomeData<T>>))
REFLECTABLE_INIT()

struct X {
    void f() {}
    template <typename T> void f(T) {}
};

TEST(TestLibrary, Test)
{
    //void(std::shared_ptr<int>::*__p)() = &std::shared_ptr<int>::reset;
    //::rew::utility::overload<>::of(&std::shared_ptr<int>::reset);
    ::rew::utility::overload<>::of(&X::f);
    rew::reflectable<FSomeData<void*>>();
    auto fsome_data_type = rew::global.find("FSomeData<void*>");
    auto fsome_data_base_type = fsome_data_type->reflection->parent.find("FSomeDataBase<void*>");

    auto factory = fsome_data_type->reflection->factory.find("std::shared_ptr<FSomeData<void*>>(std::shared_ptr<FSomeData<void*>>)");
    auto fsome_data_shared_ptr = factory->call({ std::make_shared<FSomeData<void*>>() });

    auto shared_ptr_type = rew::global.find(fsome_data_shared_ptr);
    auto shared_ptr_context = shared_ptr_type->context(fsome_data_shared_ptr);
    auto get_function = shared_ptr_type->reflection->function.find("get()");
    auto fsome_data_context = get_function->call(shared_ptr_context, {});

    auto function = fsome_data_type->reflection->function.find("Foo(void*)");
    function->call(fsome_data_context, {(void*)nullptr});
    auto fsome_data_property = fsome_data_type->reflection->property.find("i");
    std::any result;
    fsome_data_property->get(fsome_data_context, result);
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
