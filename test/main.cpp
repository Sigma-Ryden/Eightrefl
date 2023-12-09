#include <Rew/Core.hpp>

#include <iostream>

#define println(...) std::cout << #__VA_ARGS__ << ' ' << __VA_ARGS__ << '\n';

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
    double Foo(int i, const std::string& s);
    int Var = 0;
};

REFLECTABLE(TObject)
    PROPERTY(Var)
    FUNCTION(Foo)
    FACTORY(TObject)
    PARENT(TBase)
    META("Hash", 5678)
REFLECTABLE_INIT()

double TObject::Foo(int i, const std::string& s)
{
    println(i);
    println(s);
    return 3.14;
}

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
    custom_visitor_t(void* object) : Object(object) {}

    template <typename PropertyType>
    void property(const rew::property_t::meta_t& meta)
    {
        using var_t = typename property_traits<PropertyType>::var_t;
        std::any var;
        meta.get(Object, var);
        std::cout << "property: " << meta.name << " value: " << std::any_cast<var_t>(var) << '\n';
    }

    template <typename FunctionType>
    void function(const rew::function_t::meta_t& meta)
    {
        std::cout << std::boolalpha;
        std::cout << "function: " << meta.name << " is_static: " << meta.is_static << '\n';
        std::cout << std::noboolalpha;
    }

    template <class BaseClassType, class DerivedClassType>
    void parent(const rew::parent_t::meta_t& meta)
    {
         std::cout <<"parent: " << meta.name << '\n';
    }

    template <class OtherClassType, typename... ArgumentTypes>
    void factory(const rew::factory_t::meta_t& meta)
    {
    }

    template <typename MetaType>
    void meta(const char* name, const MetaType& data)
    {
         std::cout << "meta: "<< name << " value: " << data << '\n';
    }

    void* Object = nullptr;
};

REFLECTABLE_VISITOR_REGISTRY(1, custom_visitor_t)

int main()
{
    auto type = rew::registry.find("TObject");
    auto reflection = type->reflection;

    auto factory = reflection->factory.find("TObject");
    auto object = factory->call({});

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
    auto base_type = rew::registry.find(parent->name);
    auto base_reflection = base_type->reflection;

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
