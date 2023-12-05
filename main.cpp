#include "Rew.hpp"

#include <iostream>

#define println(...) std::cout << '\t' << #__VA_ARGS__ << ' ' << __VA_ARGS__ << '\n';

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
    PARENT(TBase)
    META("Hash", 5678)
REFLECTABLE_INIT()

double TObject::Foo(int i, const std::string& s)
{
    println(i);
    println(s);
    return 3.14;
}

int main()
{
    auto reflection = rew::reflection.find("TObject");
    //rew::reflection.eval("TObject");

    auto factory = reflection->factory.find("TObject");
    auto object = factory->call();

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
    auto base_reflection = parent->reflection();

    for (auto& [name, meta] : base_reflection->property.all)
    {
        std::any value;
        meta.get(base, value);
    }

    auto base_function = base_reflection->function.find("Boo");
    base_function->call(nullptr, result, {});

    return 0;
}
