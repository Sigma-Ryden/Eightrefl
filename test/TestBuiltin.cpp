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

template <>
struct rew::meta::reflectable_name_t<void>
{
    static std::string get() { return "void"; }
};


//REFLECTABLE(void*)
//REFLECTABLE_INIT()

CONDITIONAL_REFLECTABLE(std::is_pointer_v<T>)
    //FACTORY(T)
REFLECTABLE_INIT()

template <typename T>
struct rew::meta::reflectable_name_t<T*>
{
    static std::string get() { return reflectable_name_t<T>::get() + "*"; }
};

template <typename T, typename Allocator>
struct rew::meta::reflectable_name_t<std::vector<T, Allocator>>
{
    static std::string get() { return "std::vector<" + reflectable_name_t<T>::get() + ">"; }
};

CONDITIONAL_REFLECTABLE(is_std_vector<T>::value)
REFLECTABLE_INIT()

template <typename T>
struct FSomeDataBase
{
    T* i;
};

template <typename>
struct is_f_base : std::false_type {};

template <typename T>
struct is_f_base<FSomeDataBase<T>> : std::true_type {};

CONDITIONAL_REFLECTABLE(is_f_base<T>::value)
REFLECTABLE_INIT()

template <typename T>
struct rew::meta::reflectable_name_t<FSomeDataBase<T>>
{
    static std::string get() { return "FSomeDataBase<" + reflectable_name_t<T>::get() + ">"; }
};

template <typename T>
struct FSomeData : FSomeDataBase<T>
{
    std::vector<T*> data;

    void Foo(T) {}
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

template <typename>
struct is_f : std::false_type {};

template <typename T>
struct is_f<FSomeData<T>> : std::true_type {};

template <typename T>
struct f_traits;

template <typename T>
struct f_traits<FSomeData<T>>
{
    using type = T;
};

CONDITIONAL_REFLECTABLE(is_f<T>::value)
    PARENT(FSomeDataBase<typename f_traits<T>::type>)
    PROPERTY(data)
    PROPERTY(i)
    FUNCTION(Foo, typename f_traits<T>::type)
    FACTORY(std::shared_ptr<T>(std::shared_ptr<T>))
REFLECTABLE_INIT()

template <typename T>
struct rew::meta::reflectable_name_t<FSomeData<T>>
{
    static std::string get() { return "FSomeData<" + reflectable_name_t<T>::get() + ">"; }
};

TEST(TestLibrary, Test)
{
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
