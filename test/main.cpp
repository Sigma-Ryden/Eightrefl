#include <Rew/Core.hpp>

#include <iostream>

#define println(...) std::cout << #__VA_ARGS__ << ' ' << __VA_ARGS__ << '\n';

template <typename PropertyType>
struct property_traits;

template <typename VariableType, class ClassType>
struct property_traits<VariableType ClassType::*>
{
    using var_t = VariableType;
    using class_t = ClassType;
};

struct custom_visitor_t : rew::visitor_t
{
    void* object = nullptr;

    custom_visitor_t(void* object) : object(object) {}

    template <typename ClassType>
    void type(const rew::type_t& type)
    {
        println(type.name);
    }

    template <typename PropertyType>
    void property(const rew::property_meta_t& meta)
    {
        using var_t = typename property_traits<PropertyType>::var_t;

        std::any var;
        meta.get(object, var);

        println(meta.name);
        println(std::any_cast<var_t&>(var));
    }

    template <typename FunctionType>
    void function(const rew::function_meta_t& meta)
    {
        println(meta.name);
    }

    template <class BaseClassType, class DerivedClassType>
    void parent(const rew::parent_meta_t& meta)
    {
        println(meta.name);
    }

    template <class ClassType, typename... ArgumentTypes>
    void factory(const rew::factory_meta_t& meta)
    {
        println(meta.name);
    }

    template <typename MetaType>
    void meta(const std::string& name, const MetaType& data)
    {
        println(name);
    }
};

REFLECTABLE_VISITOR_REGISTRY(1, custom_visitor_t)

struct TBase
{
    std::string Data = "abcd";

    static void Boo();
};

REFLECTABLE(TBase)
    PROPERTY(Data)
    FUNCTION(Boo)
REFLECTABLE_INIT()

void TBase::Boo()
{
    println("TBase::Boo()");
}

struct TObject : TBase
{
    TObject(int var, void* data);
    double Foo(int i, const std::string& s);
    int Var = 0;
};

REFLECTABLE(TObject)
    PROPERTY(Var)
    FUNCTION(Foo)
    FACTORY(int, void*)
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

int main()
{
    auto type = rew::registry.find("TObject");
    auto reflection = type->reflection;

    auto factory = reflection->factory.find("int, void*");
    auto object = (TObject*)factory->call({ 256, (void*)nullptr });

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
    base_function->call(nullptr, result, {});

    custom_visitor_t visitor{object};
    type->evaluate(visitor);

    return 0;
}
