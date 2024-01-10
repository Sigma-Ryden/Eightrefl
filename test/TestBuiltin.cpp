#include "RewTestingBase.hpp"

#include <string>
#include <memory>

REFLECTABLE_DECLARATION(bool)
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE(bool)
    FACTORY(T())
    FACTORY(T(T))
REFLECTABLE_INIT()

REFLECTABLE_DECLARATION(char)
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE(char)
    FACTORY(T())
    FACTORY(T(T))
REFLECTABLE_INIT()

REFLECTABLE_DECLARATION(short)
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE(short)
    FACTORY(T())
    FACTORY(T(T))
REFLECTABLE_INIT()

REFLECTABLE_DECLARATION(int)
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE(int)
    FACTORY(T())
    FACTORY(T(T))
REFLECTABLE_INIT()

REFLECTABLE_DECLARATION(float)
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE(float)
    FACTORY(T())
    FACTORY(T(T))
REFLECTABLE_INIT()

REFLECTABLE_DECLARATION(double)
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE(double)
    FACTORY(T())
    FACTORY(T(T))
REFLECTABLE_INIT()

REFLECTABLE_DECLARATION(std::size_t)
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE(std::size_t)
    FACTORY(T())
    FACTORY(T(T))
REFLECTABLE_INIT()

REFLECTABLE_DECLARATION(std::ptrdiff_t)
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE(std::ptrdiff_t)
    FACTORY(T())
    FACTORY(T(T))
REFLECTABLE_INIT()

CONDITIONAL_REFLECTABLE_DECLARATION(std::is_pointer_v<T>)
    REFLECTABLE_NAME(NAMEOF(std::remove_pointer_t<T>)+"*")
REFLECTABLE_DECLARATION_INIT()

CONDITIONAL_REFLECTABLE(std::is_pointer_v<T>)
    FACTORY(T())
    FACTORY(T(T))
REFLECTABLE_INIT()

CONDITIONAL_REFLECTABLE_DECLARATION(std::is_lvalue_reference_v<T>)
    REFLECTABLE_NAME(NAMEOF(std::remove_reference_t<T>)+"&")
REFLECTABLE_DECLARATION_INIT()

CONDITIONAL_REFLECTABLE(std::is_lvalue_reference_v<T>)
REFLECTABLE_INIT()

CONDITIONAL_REFLECTABLE_DECLARATION(std::is_rvalue_reference_v<T>)
    REFLECTABLE_NAME(NAMEOF(std::remove_reference_t<T>)+"&&")
REFLECTABLE_DECLARATION_INIT()

CONDITIONAL_REFLECTABLE(std::is_rvalue_reference_v<T>)
REFLECTABLE_INIT()

CONDITIONAL_REFLECTABLE_DECLARATION(std::is_const_v<T>)
    REFLECTABLE_NAME(NAMEOF(std::remove_const_t<T>)+" const")
REFLECTABLE_DECLARATION_INIT()

CONDITIONAL_REFLECTABLE(std::is_const_v<T>)
    FACTORY(T())
    FACTORY(T(T))
REFLECTABLE_INIT()

template <typename T> struct is_std_initializer_list : std::false_type {};
template <typename T> struct is_std_initializer_list<std::initializer_list<T>> : std::true_type {};

CONDITIONAL_REFLECTABLE_DECLARATION(is_std_initializer_list<T>::value)
    REFLECTABLE_NAME("std::initializer_list<"+NAMEOF(typename T::value_type)+">")
REFLECTABLE_DECLARATION_INIT()

CONDITIONAL_REFLECTABLE(is_std_initializer_list<T>::value)
    FACTORY(T())
    FUNCTION(begin)
    FUNCTION(end)
    READONLY_FUNCTION(size)
REFLECTABLE_INIT()

template <typename T> struct is_std_allocator : std::false_type {};
template <typename T> struct is_std_allocator<std::allocator<T>> : std::true_type {};

CONDITIONAL_REFLECTABLE_DECLARATION(is_std_allocator<T>::value)
    REFLECTABLE_NAME("std::allocator<"+NAMEOF(typename T::value_type)+">")
REFLECTABLE_DECLARATION_INIT()

CONDITIONAL_REFLECTABLE(is_std_allocator<T>::value)
    FACTORY(T())
    FACTORY(T(T const&))
    //FUNCTION(allocate, typename T::value_type* (T::*)(std::size_t))
    FUNCTION(deallocate, void (T::*)(typename T::value_type*, std::size_t))
REFLECTABLE_INIT()

template <typename T> struct is_std_vector : std::false_type {};
template <typename T, typename Allocator> struct is_std_vector<std::vector<T, Allocator>> : std::true_type {};

CONDITIONAL_REFLECTABLE_DECLARATION(is_std_vector<T>::value)
    REFLECTABLE_NAME("std::vector<"+NAMEOF(typename T::value_type)+", "+NAMEOF(typename T::allocator_type)+">")
REFLECTABLE_DECLARATION_INIT()

CONDITIONAL_REFLECTABLE(is_std_vector<T>::value)
    FACTORY(T(T const&))
    FACTORY(T())
    FACTORY(T(typename T::size_type))
    FACTORY(T(typename T::size_type, typename T::value_type const&))
    FACTORY(T(T const&))
    FACTORY(T(std::initializer_list<typename T::value_type>))
    FUNCTION(operator=, T& (T::*)(T const&))
    FUNCTION(operator=, T& (T::*)(std::initializer_list<typename T::value_type>))
    FUNCTION(assign, void (T::*)(typename T::size_type, typename T::value_type const&))
    FUNCTION(assign, void (T::*)(std::initializer_list<typename T::value_type>))
    READONLY_FUNCTION(get_allocator, typename T::allocator_type (T::*)() const)
    READONLY_FUNCTION(at)
    READONLY_FUNCTION(operator[])
    READONLY_FUNCTION(front)
    READONLY_FUNCTION(back)
    READONLY_FUNCTION(data)
    READONLY_FUNCTION(begin)
    READONLY_FUNCTION(cbegin)
    READONLY_FUNCTION(end)
    READONLY_FUNCTION(cend)
    READONLY_FUNCTION(rbegin)
    READONLY_FUNCTION(crbegin)
    READONLY_FUNCTION(rend)
    READONLY_FUNCTION(crend)
    READONLY_FUNCTION(empty)
    READONLY_FUNCTION(size)
    READONLY_FUNCTION(max_size)
    READONLY_FUNCTION(capacity)
    FUNCTION(at)
    FUNCTION(operator[])
    FUNCTION(front)
    FUNCTION(back)
    FUNCTION(data)
    FUNCTION(begin)
    FUNCTION(end)
    FUNCTION(rbegin)
    FUNCTION(rend)
    FUNCTION(reserve)
    FUNCTION(shrink_to_fit)
    FUNCTION(clear)
    FUNCTION(erase, typename T::iterator (T::*)(typename T::const_iterator))
    FUNCTION(erase, typename T::iterator (T::*)(typename T::const_iterator, typename T::const_iterator))
    FUNCTION(push_back, void (T::*)(typename T::value_type const&))
    FUNCTION(pop_back)
    FUNCTION(resize, void (T::*)(typename T::size_type))
    FUNCTION(resize, void (T::*)(typename T::size_type, typename T::value_type const&))
    FUNCTION(pop_back)
REFLECTABLE_INIT()

template <typename T>
struct FSomeDataBase
{
    T* i;
};

//TEMPLATE_REFLECTABLE((template <typename T>), (FSomeDataBase<T>), ("FSomeDataBase<"+NAMEOF(T)+">"))
//REFLECTABLE_INIT()

template <typename T>
struct FSomeData : FSomeDataBase<T>
{
    std::vector<T*> data;

    void Foo(const T *const&) {}
    void Goo(int, float) {}

};

template <typename T> struct is_std_shared_ptr : std::false_type {};
template <typename T> struct is_std_shared_ptr<std::shared_ptr<T>> : std::true_type {};

CONDITIONAL_REFLECTABLE_DECLARATION(is_std_shared_ptr<T>::value)
    REFLECTABLE_NAME("std::shared_ptr<"+NAMEOF(typename T::element_type)+">")
REFLECTABLE_DECLARATION_INIT()

CONDITIONAL_REFLECTABLE(is_std_shared_ptr<T>::value)
    FACTORY(T())
    FACTORY(T(std::nullptr_t))
    FACTORY(T(T const&))
    FUNCTION(swap)
    READONLY_FUNCTION(get)
    READONLY_FUNCTION(operator*)
    READONLY_FUNCTION(operator->)
    FUNCTION(reset, void (T::*)())
    READONLY_FUNCTION(use_count)
    READONLY_FUNCTION(operator bool)
REFLECTABLE_INIT()

REFLECTABLE(std::shared_ptr<void>)
    FACTORY(T())
    FACTORY(T(std::nullptr_t))
    FACTORY(T(T const&))
    FUNCTION(swap)
    READONLY_FUNCTION(get)
    READONLY_FUNCTION(operator->)
    FUNCTION(reset, void (T::*)())
    READONLY_FUNCTION(use_count)
    READONLY_FUNCTION(operator bool)
REFLECTABLE_INIT()

//TEMPLATE_REFLECTABLE((template <typename T>), (FSomeData<T>), ("FSomeData<"+NAMEOF(T)+">"))
//    PARENT(FSomeDataBase<T>)
//    PROPERTY(data)
//    PROPERTY(i)
//    FUNCTION(Foo)
//    FUNCTION(Goo, int, float)
//    FACTORY(std::shared_ptr<FSomeData<T>>(std::shared_ptr<FSomeData<T>>))
//REFLECTABLE_INIT()

TEST(TestLibrary, Test)
{
    //std::vector<int>::allocator_type (std::vector<int>::*__f)() const = &std::vector<int>::get_allocator;
    //void(std::shared_ptr<int>::*__p)() = &std::shared_ptr<int>::reset;
    //::rew::utility::overload<>::of(&std::shared_ptr<int>::reset);

    // TODO: add static assert inside reflectable
    //rew::reflectable<FSomeData<void*>>();
    rew::reflectable<std::allocator<int>>();
    rew::reflectable<std::vector<int>>();
    rew::reflectable<std::shared_ptr<void>>();
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
