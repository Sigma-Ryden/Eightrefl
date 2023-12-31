#include <RewTestingBase.hpp>

#include <iostream>

#define println(...) std::cout << #__VA_ARGS__ << ' ' << __VA_ARGS__ << '\n';

BUILTIN_REFLECTABLE(std::string)
    FACTORY(std::string)
REFLECTABLE_INIT()

BUILTIN_REFLECTABLE(void*)
REFLECTABLE_INIT()

REFLECTABLE_DECLARATION(int)

class TBase
{
public:
    void* meta = nullptr;

private:
    std::string data_ = "abcd";

public:
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

BUILTIN_REFLECTABLE(EColor)
    FACTORY(EColor, EColor)
    FACTORY(EColor)
    FACTORY(EColor, int)
REFLECTABLE_INIT()

class TObject : public TBase
{
    REFLECTABLE_ACCESS()

public:
    TObject(int var, void* data);
    TObject() = default;

    double Foo(int i, const std::string& s) const;

private:
    int Var = 0;
    EColor Color = EColor::Green;
};

REFLECTABLE(TObject)
    PROPERTY(Color)
    PROPERTY(Var)
    FUNCTION(Foo)
    FACTORY(TObject, int, void*)
    FACTORY(TObject)
    PARENT(TBase)
    META("hash", 5678)
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
    auto type = rew::global.find("TObject");
    auto reflection = type->reflection;

    auto factory = reflection->factory.find("TObject, int, void*");
    auto object = factory->call({256, (void*)nullptr});
    auto object_ptr = type->context(object);

    std::any result;

    auto function = reflection->function.find("Foo");

    auto parent = reflection->parent.find("TBase");
    auto base_ptr = parent->cast(object_ptr);
    auto base_reflection = parent->type->reflection;
    auto data_property = base_reflection->property.find("Data");
    auto str = data_property->type->ref(data_property->ptr(base_ptr));

    function->call(object_ptr, result, {128, str});

    println(std::any_cast<double>(result));

    auto property = reflection->property.find("Var");
    auto property_type = property->type;

    property->set(object_ptr, 321);
    property->get(object_ptr, result);

    println(std::any_cast<int>(result));

    println(reflection->name);

    auto meta = reflection->meta.find("hash");
    result = *meta;
    println(std::any_cast<int>(result));

    for (auto& [name, meta] : base_reflection->property.all)
    {
        std::any value;
        meta.get(base_ptr, value);
    }

    auto base_function = base_reflection->function.find("Boo");
    base_function->call(nullptr, result, {});
}
