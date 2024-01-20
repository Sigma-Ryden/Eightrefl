#include "RewTestingBase.hpp"

#include <Rew/BuiltIn/vector.hpp>
#include <Rew/BuiltIn/shared_ptr.hpp>
#include <Rew/BuiltIn/string.hpp>

#include <string>
#include <memory>

REFLECTABLE_DECLARATION(void)
    BUILTIN_REFLECTABLE()
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE_DECLARATION(std::nullptr_t)
    BUILTIN_REFLECTABLE()
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE_DECLARATION(bool)
    BUILTIN_REFLECTABLE()
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE(bool)
    FACTORY(R())
    FACTORY(R(R))
REFLECTABLE_INIT()

REFLECTABLE_DECLARATION(char)
    BUILTIN_REFLECTABLE()
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE(char)
    FACTORY(R())
    FACTORY(R(R))
REFLECTABLE_INIT()

REFLECTABLE_DECLARATION(short)
    BUILTIN_REFLECTABLE()
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE(short)
    FACTORY(R())
    FACTORY(R(R))
REFLECTABLE_INIT()

REFLECTABLE_DECLARATION(int)
    BUILTIN_REFLECTABLE()
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE(int)
    FACTORY(R())
    FACTORY(R(R))
REFLECTABLE_INIT()

REFLECTABLE_DECLARATION(float)
    BUILTIN_REFLECTABLE()
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE(float)
    FACTORY(R())
    FACTORY(R(R))
REFLECTABLE_INIT()

REFLECTABLE_DECLARATION(double)
    BUILTIN_REFLECTABLE()
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE(double)
    FACTORY(R())
    FACTORY(R(R))
REFLECTABLE_INIT()

REFLECTABLE_DECLARATION(std::size_t)
    BUILTIN_REFLECTABLE()
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE(std::size_t)
    FACTORY(R())
    FACTORY(R(R))
REFLECTABLE_INIT()

REFLECTABLE_DECLARATION(std::ptrdiff_t)
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE(std::ptrdiff_t)
    FACTORY(R())
    FACTORY(R(R))
REFLECTABLE_INIT()

TEMPLATE_REFLECTABLE_DECLARATION((template <typename T>), (T*))
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME(NAMEOF(T)+"*")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE((template <typename T>), (T*))
    FACTORY(R())
    FACTORY(R(R))
REFLECTABLE_INIT()

TEMPLATE_REFLECTABLE_DECLARATION((template <typename T>), (T&))
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME(NAMEOF(T)+"&")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE_DECLARATION((template <typename T>), (T const))
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME(NAMEOF(T)+" const")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE((template <typename T>), (T const))
    FACTORY(R())
    FACTORY(R(R))
REFLECTABLE_INIT()

template <typename T>
struct FSomeDataBase
{
    using value_type = T;
    T* i;
};

template <typename T> struct is_fsome_data_base : std::false_type {};
template <typename T> struct is_fsome_data_base<FSomeDataBase<T>> : std::true_type {};

CONDITIONAL_REFLECTABLE_DECLARATION(is_fsome_data_base<R>::value)
    REFLECTABLE_NAME("FSomeDataBase<"+NAMEOF(typename R::value_type)+">")
REFLECTABLE_DECLARATION_INIT()

CONDITIONAL_REFLECTABLE(is_fsome_data_base<R>::value)
REFLECTABLE_INIT()

template <typename R>
struct FSomeData : FSomeDataBase<R>
{
    using value_type = R;

    std::vector<R*> data;

    void Foo(const R *const&) {}
    void Goo(int, std::string) {}
};

template <typename R> struct is_fsome_data : std::false_type {};
template <typename R> struct is_fsome_data<FSomeData<R>> : std::true_type {};

CONDITIONAL_REFLECTABLE_DECLARATION(is_fsome_data<R>::value)
    REFLECTABLE_NAME("FSomeData<"+NAMEOF(typename R::value_type)+">")
REFLECTABLE_DECLARATION_INIT()

CONDITIONAL_REFLECTABLE(is_fsome_data<R>::value)
    PARENT(FSomeDataBase<typename R::value_type>)
    PROPERTY(data)
    PROPERTY(i)
    FUNCTION(Foo)
    FUNCTION(Goo, void(int, std::string))
    FACTORY(std::shared_ptr<R>(std::shared_ptr<R>))
REFLECTABLE_INIT()

TEST(TestLibrary, Test)
{
    //std::vector<int>::allocator_type (std::vector<int>::*__f)() const = &std::vector<int>::get_allocator;
    //void(std::shared_ptr<int>::*__p)() = &std::shared_ptr<int>::reset;
    //::rew::utility::overload<>::of(&std::shared_ptr<int>::reset);
    // TODO: add static assert inside reflectable
    rew::reflectable<FSomeData<void>>();
    rew::reflectable<std::allocator<int>>();
    rew::reflectable<std::vector<int>>();
    rew::reflectable<std::shared_ptr<int>>();
    rew::reflectable<std::string>();

    auto vector_type = rew::global.find("std::vector<int, std::allocator<int>>");
    auto get_allocator_function = vector_type->reflection->function.find("get_allocator() const");

    std::vector<int> v;
    std::any vector_context = &v;
    get_allocator_function->call(vector_context, {});

    auto fsome_data_type = rew::global.find("FSomeData<void*>");

    return;
    auto fsome_data_base_type = fsome_data_type->reflection->parent.find("FSomeDataBase<void*>");

    auto factory = fsome_data_type->reflection->factory.find("std::shared_ptr<FSomeData<void*>>(std::shared_ptr<FSomeData<void*>>)");
    auto fsome_data_shared_ptr = factory->call({ std::make_shared<FSomeData<void*>>() });

    auto shared_ptr_type = rew::global.find(fsome_data_shared_ptr);
    auto shared_ptr_context = shared_ptr_type->context(fsome_data_shared_ptr);
    auto get_function = shared_ptr_type->reflection->function.find("get() const");
    auto fsome_data_context = get_function->call(shared_ptr_context, {});

    auto function = fsome_data_type->reflection->function.find("Goo(int, float)");
    function->call(fsome_data_context, {1, 1.f});
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
