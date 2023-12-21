#include "RewTestingBase.hpp"

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
    FACTORY(std::string)
REFLECTABLE_INIT()

REFLECTABLE(void*)
REFLECTABLE_INIT()

REFLECTABLE(int)
    FACTORY(int)
    FACTORY(int, int)
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
    FACTORY(EColor, EColor)
    FACTORY(EColor)
    FACTORY(EColor, int)
REFLECTABLE_INIT()

struct TObject : TBase
{
    TObject(int var, void* data);
    TObject() = default;

    double Foo(int i, const std::string& s) const;
    int Var = 0;

    REFLECTABLE_ACCESS()

    private:
    EColor Color = EColor::Green;
};

REFLECTABLE(TObject)
    PROPERTY(Color)
    PROPERTY(Var)
    FUNCTION(Foo)
    FACTORY(TObject, int, void*)
    FACTORY(TObject)
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

    auto factory = reflection->factory.find("TObject, int, void*");
    auto object = factory->call({ 256, (void*)nullptr });
    auto object_ptr = type->ptr(object);

    std::any result;

    auto function = reflection->function.find("Foo");

    auto parent = reflection->parent.find("TBase");
    auto base_ptr = parent->cast(object_ptr);
    auto base_reflection = parent->type->reflection;
    auto data_property = base_reflection->property.find("Data");
    auto str = data_property->type->ref(data_property->ptr(base_ptr));

    function->call(object_ptr, result, { 128, str });

    println(std::any_cast<double>(result));

    auto property = reflection->property.find("Var");

    property->set(object_ptr, 321);
    property->get(object_ptr, result);

    println(std::any_cast<int>(result));

    println(reflection->name);

    auto meta = reflection->meta.find("Hash");
    result = *meta;
    println(std::any_cast<int>(result));

    for (auto& [name, meta] : base_reflection->property.all)
    {
        std::any value;
        meta.get(base_ptr, value);
    }

    auto base_function = base_reflection->function.find("Boo");
    base_function->call(nullptr, result, {});

    custom_visitor_t visitor{object_ptr};
    type->evaluate(visitor);
}
