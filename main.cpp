#include "Rew.hpp"

#include <iostream>

struct TBase : rew::reflectable_t
{
    std::string Data = "abcd";
};

REFLECTABLE(TBase)
    PROPERTY(Data)
REFLECTABLE_INIT()

struct TObject : TBase
{
    double Foo(int, const std::string&);
    int Var = 0;
};

REFLECTABLE(TObject)
    PROPERTY(Var)
    FUNCTION(Foo)
    PARENT(TBase)
REFLECTABLE_INIT()

#define println(...) std::cout << '\t' << #__VA_ARGS__ << ' ' << __VA_ARGS__ << '\n';

double TObject::Foo(int i, const std::string& s)
{
    println(i);
    println(s);
    return 3.14;
}

int main()
{
    TObject object;
    auto reflection = rew::reflection.get("TObject");

    std::string s = "Tom";
    std::any result;

    reflection.function.call("Foo", &object, result, 128, s);
    println(std::any_cast<double>(result));

    object.Var = 123;
    reflection.property.get("Var", &object, result);
    println(std::any_cast<int>(result));

    reflection.property.set("Var", &object, 321);
    println(object.Var);

    auto base_ptr = reflection.parent.get("TBase", &object);
    auto& base_reflection = reflection.parent.reflection("TBase");

    for (auto& [name, get] : base_reflection.property.itable)
    {
        std::any value;
        get(base_ptr, value);
    }

    reflection.factory.call("TObject");

    println(reflection.meta.name);

    return 0;
}
