#include <Rew/Core.hpp>

#include <AutoTesting/Core.hpp>

#include <iostream>

#define println(...) std::cout << #__VA_ARGS__ << ' ' << __VA_ARGS__ << '\n';

struct custom_visitor_t : rew::visitor_t
{
    std::any object;

    custom_visitor_t(const std::any& object) : object(object) {}

    template <typename ReflectableType>
    void type(const rew::type_t& type)
    {
        println(type.name);
    }

    template <typename ReflectableType, typename PropertyType>
    void property(const rew::property_meta_t& meta)
    {
        std::any var;
        meta.get(object, var);

        println(meta.name);
        //println(std::any_cast<PropertyType&>(var));
    }

    template <typename ReflectableType, typename FunctionType>
    void function(const rew::function_meta_t& meta)
    {
        println(meta.name);
    }

    template <typename ReflectableType, typename ParentReflectableType>
    void parent(const rew::parent_meta_t& meta)
    {
        println(meta.name);
    }

    template <typename ReflectableType, typename FunctionType>
    void factory(const rew::factory_meta_t& meta)
    {
        println(meta.name);
    }

    template <typename ReflectableType, typename MetaType>
    void meta(const std::string& name, const MetaType& data)
    {
        println(name);
    }
};

REFLECTABLE_VISITOR_REGISTRY(1, custom_visitor_t)

struct TBase
{
    std::string data_ = "abcd";

    static void Boo();
    static void Boo(int);

    const std::string& Data() { return data_; }
    void Data(const std::string& data) { data_ = data; }
};

REFLECTABLE(TBase)
    PROPERTY(Data)
    FUNCTION(Boo)
REFLECTABLE_INIT()

void TBase::Boo() { println("TBase::Boo()"); }

void TBase::Boo(int) { println("TBase::Boo(int)"); }

struct TObject : TBase
{
    TObject(int var, void* data);
    TObject() = default;

    double Foo(int i, const std::string& s);
    int Var = 0;
};

REFLECTABLE(TObject)
    PROPERTY(Var)
    FUNCTION(Foo)
    FACTORY(int, void*)
    FACTORY(void)
    PARENT(TBase)
    META("Hash", 5678)
REFLECTABLE_INIT()

TObject::TObject(int var, void* data) : Var(var) {}

double TObject::Foo(int i, const std::string& s)
{
    println(Var);
    println(i);
    println(s);
    return 3.14;
}

TEST(TestDemo, TestExample)
{
    auto type = rew::registry.find("TObject");
    auto reflection = type->reflection;

    auto factory = reflection->factory.find("int, void*");
    auto object = factory->call({ 256, (void*)nullptr });

    std::any result;

    auto function = reflection->function.find("Foo");
    function->call(object, result, { 128, std::string("Tom") });

    println(std::any_cast<double>(result));

    auto property = reflection->property.find("Var");

    property->set(object, 321);
    property->get(object, result);

    println(std::any_cast<int>(result));

    println(reflection->name);

    auto meta = reflection->meta.find("Hash");
    result = *meta;
    println(std::any_cast<int>(result));

    auto parent = reflection->parent.find("TBase");
    auto base = parent->get(object);
    auto base_reflection = parent->type->reflection;

    for (auto& [name, meta] : base_reflection->property.all)
    {
        std::any value;
        meta.get(base, value);
    }

    auto base_function = base_reflection->function.find("Boo");
    std::any null;
    base_function->call(null, result, {});

    custom_visitor_t visitor{std::any_cast<TObject*>(object)};
    type->evaluate(visitor);
}

BUILTIN_REFLECTABLE(int)
REFLECTABLE_INIT()

TEST(TestLibrary, TestBuiltin)
{
    using reflectbale_type = int;

    static std::string s_name = "int";

    auto type = rew::registry.find(s_name);
    
    ASSERT("type-null", type != nullptr);
    EXPECT("type-name", type->name == s_name);

    ASSERT("type-evaluate-null", type->evaluate != nullptr);
    
    ASSERT("type-reflection-null", type->reflection != nullptr);

    auto default_factory = type->reflection->factory.find("");
    
    ASSERT("type-reflection-factory-default-null", default_factory != nullptr);
    ASSERT("type-reflection-factory-default-args_count", default_factory->args_count == 0);
    
    auto default_instance = default_factory->call({});

    ASSERT("type-reflection-factory-default-instance-null", default_instance.has_value());
    
    auto value = type->reflection->property.find("value");
    
    ASSERT("type-reflection-property-value-null", value != nullptr);
    
    ASSERT("type-reflection-property-value-get", value->get != nullptr);
    
    std::any default_result;
    value->get(default_instance, default_result);
    
    ASSERT("type-reflection-property-value-get-result-default-null", default_result.has_value());
    
    auto default_value_address = std::any_cast<reflectbale_type>(&default_result);
    
    ASSERT("type-reflection-property-value-get-result-default-cast", default_value_address != nullptr);
    EXPECT("type-reflection-property-value-get-result-default-value", *default_value_address == reflectbale_type{});

    static reflectbale_type s_set_value{ 123 };
        
    ASSERT("type-reflection-property-value-set", value->set != nullptr);
    
    value->set(default_instance, s_set_value);
    
    std::any refreshed_result;
    value->get(default_instance, refreshed_result);
        
    auto set_value_address = std::any_cast<reflectbale_type>(&refreshed_result);
    
    EXPECT("type-reflection-property-value-get-result-default-set-value", *set_value_address == s_set_value);
    
    static reflectbale_type s_instance_value{ 456 };

    auto factory = type->reflection->factory.find(s_name);
    
    ASSERT("type-reflection-factory-null", factory != nullptr);
    ASSERT("type-reflection-factory-args_count", factory->args_count == 1);

    auto instance = factory->call({ s_instance_value });
    
    ASSERT("type-reflection-factory-instance-null", instance.has_value());
    
    std::any initiate_result;
    value->get(instance, initiate_result);
    
    ASSERT("type-reflection-property-value-get-result-null", initiate_result.has_value());
    auto instance_value_address = std::any_cast<reflectbale_type>(&initiate_result);
        
    ASSERT("type-reflection-property-value-get-result-cast", instance_value_address != nullptr);
    EXPECT("type-reflection-property-value-get-result-value", *instance_value_address == s_instance_value);
}

int main()
{
    TRY_CATCH(EXECUTE_ALL());
    TESTING_STAT();
    return 0;
}
