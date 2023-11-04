#include <cstdarg>
#include <functional>
#include <any>
#include <map>
#include <string>

#define RREQUIRE(...) typename std::enable_if<bool(__VA_ARGS__), int>::type = 0

struct Instantiable { virtual ~Instantiable() = default; };

template <class ClassType>
struct ReflectionRegistry;

template <class ClassType>
struct Reflection
{
    template <typename T>
    using PureType = typename std::remove_reference<T>::type;

    struct Property
    {
        static void get(const std::string& name, ClassType *const self, std::any& result)
        {
            imeta().at(name)(self, result);
        }

        template <typename ValueType>
        static void set(const std::string& name, ClassType *const self, ValueType&& value)
        {
            ometa().at(name)(self, value);
        }

        static decltype(auto) imeta()
        {
            static std::map<std::string, std::function<void(ClassType *const, std::any&)>> data;
            return (data);
        }

        static decltype(auto) ometa()
        {
            static std::map<std::string, std::function<void(ClassType *const, const std::any&)>> data;
            return (data);
        }
    };

    struct Function
    {
        template <typename... ArgumentTypes>
        static void call(const std::string& name, ClassType *const self, std::any& result, ArgumentTypes&&... arguments)
        {
            call_implementation(name, self, result, sizeof...(ArgumentTypes), std::addressof(arguments)...);
        }

        static decltype(auto) meta()
        {
            static std::map<std::string, std::function<void(ClassType *const, std::any&, std::va_list)>> data;
            return (data);
        }

    private:
        static void call_implementation(const std::string& name, ClassType *const self, std::any& result, int count, ...)
        {
            std::va_list arguments;

            va_start(arguments, count);
            meta()[name](self, result, arguments);
            va_end(arguments);
        }
    };

    struct Parent
    {
        static void get(const std::string& name, ClassType *const self, std::any& result)
        {
            meta().at(name)(self, result);
        }

        static decltype(auto) meta()
        {
            static std::map<std::string, void(*)(ClassType *const, std::any&)> data;
            return (data);
        }
    };

    struct Factory
    {
        static Instantiable* call(const std::string& name)
        {
            return meta().at(name)();
        }

        static decltype(auto) meta()
        {
            static std::map<std::string, Instantiable*(*)()> data;
            return (data);
        }
    };

    struct Meta
    {
        static std::string name()
        {
            return ReflectionRegistry<ClassType>::name;
        }
    };

    template <typename PropertyType>
    Reflection& add_property(const std::string& name, PropertyType ClassType::* property)
    {
        Property::imeta()[name] = [property](ClassType *const self, std::any& result)
        {
            result = self->*property;
        };

        Property::ometa()[name] = [property](ClassType *const self, const std::any& value)
        {
            self->*property = std::any_cast<const PropertyType&>(value);
        };

        return *this;
    }

    template <typename ReturnType, typename... ArgumentTypes>
    Reflection& add_function(const std::string& name, ReturnType (ClassType::* function)(ArgumentTypes...))
    {
        Function::meta()[name] = [function](ClassType *const self, std::any& result, std::va_list arguments)
        {
            [function]<std::size_t... I>(ClassType *const self, std::any& result, std::va_list arguments, std::index_sequence<I...>) {
                result = (self->*function)(*extended_va_arg<I, PureType<ArgumentTypes>...>(arguments)...);
            }(self, result, arguments, std::make_index_sequence<sizeof...(ArgumentTypes)>{});
        };

        return *this;
    }

    template <typename... ArgumentTypes>
    Reflection& add_function(const std::string& name, void (ClassType::* function)(ArgumentTypes...))
    {
        Function::meta()[name] = [function](ClassType *const self, std::any& result, std::va_list arguments)
        {
            [function]<std::size_t... I>(ClassType *const self, std::va_list arguments, std::index_sequence<I...>) {
                (self->*function)(*extended_va_arg<I, PureType<ArgumentTypes>...>(arguments)...);
            }(self, arguments, std::make_index_sequence<sizeof...(ArgumentTypes)>{});
        };

        return *this;
    }

    template <class ParentClassType,
              RREQUIRE(std::is_base_of<ParentClassType, ClassType>::value)>
    Reflection& add_parent(const std::string& name)
    {
        Parent::meta()[name] = [](ClassType *const self, std::any& result)
        {
            result = static_cast<ParentClassType*>(self);
        };

        return *this;
    }

    template <class OtherClassType = ClassType>
    Reflection& add_factory(const std::string& name)
    {
        Factory::meta()[name] = [](void)
        {
            return static_cast<Instantiable*>(new OtherClassType);
        };
        return *this;
    }

private:
    template <std::size_t ArgumentTypeIndex, typename... ArgumentTypes>
    static auto extended_va_arg(std::va_list list) {
        for (std::size_t i = 0; i < ArgumentTypeIndex; ++i) (void)va_arg(list, void*); // need for skip

        using type = typename std::tuple_element<ArgumentTypeIndex, std::tuple<ArgumentTypes...>>::type;
        return va_arg(list, type*);
    }
};

#define REFLECTABLE(...) \
template <> struct ReflectionRegistry<__VA_ARGS__> { \
    using type = __VA_ARGS__; \
    inline static const auto name = #__VA_ARGS__; \
    inline static const auto reflection = Reflection<type>()

#define PROPERTY(...) .add_property(#__VA_ARGS__, &type::__VA_ARGS__)
#define FUNCTION(...) .add_function(#__VA_ARGS__, &type::__VA_ARGS__)
#define PARENT(...) .add_parent<__VA_ARGS__>(#__VA_ARGS__)
#define FACTORY(...) .add_factory<__VA_ARGS__>(#__VA_ARGS__)

#define REFLECTABLE_INIT() \
.add_factory(name); };


#include <iostream>
#define println(...) std::cout << '\t' << #__VA_ARGS__ << ' ' << __VA_ARGS__ << '\n';

struct TBase : Instantiable
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

double TObject::Foo(int i, const std::string& s)
{
    println(i);
    println(s);
    return 3.14;
}

int main()
{
    TObject object;

    using TObjectType = Reflection<TObject>;
    using TObjectProperty = TObjectType::Property;
    using TObjectFunction = TObjectType::Function;
    using TObjectParent = TObjectType::Parent;
    using TObjectFactory = TObjectType::Factory;

    std::string s = "Tom";
    std::any result;

    TObjectFunction::call("Foo", &object, result, 128, s);
    println(std::any_cast<double>(result));

    object.Var = 123;
    TObjectProperty::get("Var", &object, result);
    println(std::any_cast<int>(result));

    TObjectProperty::set("Var", &object, 321);
    println(object.Var);

    TObjectParent::get("TBase", &object, result);

    auto base = *std::any_cast<TBase*>(result);

    for(auto& [name, get]:TObjectProperty::imeta())
    {
        std::any value;
        get(&object, value);
    }

    TObjectFactory::call("TObject");

    println(TObjectType::Meta::name());

    return 0;
}
