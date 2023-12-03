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
    auto reflection = rew::reflection.get("TObject");

    auto object = reflection->factory.call("TObject");
    std::any result;

    reflection->function.call("Foo", object, result, 128, std::string("Tom"));

    println(std::any_cast<double>(result));

    reflection->property.set("Var", object, 321);
    reflection->property.get("Var", object, result);

    println(std::any_cast<int>(result));

    println(reflection->meta.name);

    result = reflection->meta.get("Hash");
    println(std::any_cast<int>(result));

    auto base = reflection->parent.get("TBase", object);
    auto base_reflection = reflection->parent.reflection("TBase");

    for (auto& [name, get] : base_reflection->property.itable)
    {
        std::any value;
        get(base, value);
    }

    base_reflection->function.call("Boo", nullptr, result);

    return 0;
}
