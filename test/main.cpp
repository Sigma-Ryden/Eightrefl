#include <Rew/Core.hpp>

#include <AutoTesting/Core.hpp>

#include <iostream>

#define println(...) std::cout << #__VA_ARGS__ << ' ' << __VA_ARGS__ << '\n';

struct custom_visitor_t : rew::visitor_t
{
    void* object = nullptr;

    custom_visitor_t(void* object) : object(object) {}

    template <typename ReflectableType>
    void type(rew::type_t& type)
    {
        println(type.name);
    }

    template <typename ReflectableType, typename PropertyType>
    void property(rew::property_meta_t& meta)
    {
        std::any var;
        meta.get(object, var);

        println(meta.name);
    }

    template <typename ReflectableType, typename FunctionType>
    void function(rew::function_meta_t& meta)
    {
        println(meta.name);
    }

    template <typename ReflectableType, typename ParentReflectableType>
    void parent(rew::parent_meta_t& meta)
    {
        println(meta.name);
    }

    template <typename ReflectableType, typename FunctionType>
    void factory(rew::factory_meta_t& meta)
    {
        println(meta.name);
    }

    template <typename ReflectableType, typename MetaType>
    void meta(const std::string& name, std::any& meta)
    {
        println(name);
    }
};

REFLECTABLE_VISITOR_REGISTRY(1, custom_visitor_t)

REFLECTABLE(std::string)
FACTORY()
REFLECTABLE_INIT()

REFLECTABLE(void*)
REFLECTABLE_INIT()

REFLECTABLE(int)
FACTORY()
FACTORY(int)
REFLECTABLE_INIT()

struct TBase
{
    void* meta = nullptr;
    std::string data_ = "abcd";

    static void Boo();
    static void Boo(int);

    const std::string& Data() const { return data_; }
    void Data(const std::string& data) { data_ = data; }
};

REFLECTABLE(TBase)
    PROPERTY(Data)
    FUNCTION(Boo)
REFLECTABLE_INIT()

void TBase::Boo() { println("TBase::Boo()"); }

void TBase::Boo(int) { println("TBase::Boo(int)"); }

enum class EColor
{
    Red,
    Green,
    Blue
};

REFLECTABLE(EColor)
FACTORY()
FACTORY(EColor)
FACTORY(int)
REFLECTABLE_INIT()

struct TObject : TBase
{
    TObject(int var, void* data);
    TObject() = default;

    double Foo(int i, const std::string& s) const;
    int Var = 0;
    EColor Color = EColor::Green;
};

REFLECTABLE(TObject)
    PROPERTY(Color)
    PROPERTY(Var)
    FUNCTION(Foo)
    FACTORY(int, void*)
    FACTORY(void)
    PARENT(TBase)
    META("Hash", 5678)
REFLECTABLE_INIT()

TObject::TObject(int var, void* data) : Var(var) {}

double TObject::Foo(int i, const std::string& s) const
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

    auto parent = reflection->parent.find("TBase");
    auto base = (TBase*)parent->ptr(object);
    auto base_reflection = parent->type->reflection;
    auto data_property = base_reflection->property.find("Data");
    auto str = data_property->type->ref(data_property->ptr(base));

    function->call(object, result, { 128, str });

    println(std::any_cast<double>(result));

    auto property = reflection->property.find("Var");

    property->set(object, 321);
    property->get(object, result);

    println(std::any_cast<int>(result));

    println(reflection->name);

    auto meta = reflection->meta.find("Hash");
    result = *meta;
    println(std::any_cast<int>(result));

    for (auto& [name, meta] : base_reflection->property.all)
    {
        std::any value;
        meta.get(base, value);
    }

    auto base_function = base_reflection->function.find("Boo");
    base_function->call(nullptr, result, {});

    custom_visitor_t visitor{object};
    type->evaluate(visitor);
}

TEST(TestLibrary, TestBuiltin)
{
    using reflectbale_type = int;

    static std::string s_name = "int";
    static reflectbale_type s_instance_value{ 456 };

    auto type = rew::registry.find(s_name);
    ASSERT("type-null", type != nullptr);
    EXPECT("type-name", type->name == s_name);

    ASSERT("type-evaluate-null", type->evaluate != nullptr);

    ASSERT("type-reflection-null", type->reflection != nullptr);

    auto default_factory = type->reflection->factory.find("");
    ASSERT("type-reflection-factory-default-null", default_factory != nullptr);
    ASSERT("type-reflection-factory-default-args_count", default_factory->arg_count == 0);
    
    auto default_instance = default_factory->call({});
    ASSERT("type-reflection-factory-default-instance-null", default_instance != nullptr);

    auto default_instance_value = *static_cast<reflectbale_type*>(default_instance);
    EXPECT("type-reflection-property-value-get-result-default-value", default_instance_value == reflectbale_type{});

    auto factory = type->reflection->factory.find(s_name);
    ASSERT("type-reflection-factory-null", factory != nullptr);
    ASSERT("type-reflection-factory-args_count", factory->arg_count == 1);

    auto initialized_instance = factory->call({ s_instance_value });
    ASSERT("type-reflection-factory-instance-null", initialized_instance != nullptr);
    
    auto initialized_instance_value = *static_cast<reflectbale_type*>(initialized_instance);
    EXPECT("type-reflection-property-value-get-result-value", initialized_instance_value == s_instance_value);
}

TEST(TestLibrary, TestEnum)
{
    auto type = rew::registry.find("EColor");
}

int main()
{
    TRY_CATCH(EXECUTE_ALL());
    TESTING_STAT();
    return 0;
}
