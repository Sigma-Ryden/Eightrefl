#include <RewTestingBase.hpp>

#include <Rew/BuiltIn/string.hpp>
#include <Rew/BuiltIn/any.hpp>
#include <Rew/BuiltIn/function.hpp>
#include <Rew/BuiltIn/vector.hpp>
#include <Rew/BuiltIn/unordered_map.hpp>
#include <Rew/BuiltIn/typeindex.hpp>

TEMPLATE_REFLECTABLE_DECLARATION((template <typename MetaType>), rew::attribute_t<MetaType>)
    REFLECTABLE_NAME("rew::attribute_t<" + NAMEOF(MetaType) + ">")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE((template <typename MetaType>), rew::attribute_t<MetaType>)
   PROPERTY(all)
REFLECTABLE_INIT()

REFLECTABLE_DECLARATION(rew::type_t)
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE_DECLARATION(rew::parent_t)
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE(rew::parent_t)
    PROPERTY(name)
    PROPERTY(type)
    PROPERTY(cast)
    PROPERTY(meta)
REFLECTABLE_INIT()

REFLECTABLE_DECLARATION(rew::function_t)
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE(rew::function_t)
    PROPERTY(name)
    PROPERTY(call)
    PROPERTY(arguments)
    PROPERTY(result)
    PROPERTY(pointer)
    PROPERTY(meta)
REFLECTABLE_INIT()

REFLECTABLE_DECLARATION(rew::factory_t)
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE(rew::factory_t)
    PROPERTY(name)
    PROPERTY(call)
    PROPERTY(arguments)
    PROPERTY(meta)
REFLECTABLE_INIT()

REFLECTABLE_DECLARATION(rew::property_t)
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE(rew::property_t)
    PROPERTY(name)
    PROPERTY(type)
    PROPERTY(get)
    PROPERTY(set)
    PROPERTY(context)
    PROPERTY(meta)
REFLECTABLE_INIT()

REFLECTABLE_DECLARATION(rew::reflection_t)
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE(rew::reflection_t)
    PROPERTY(name)
    PROPERTY(parent)
    PROPERTY(function)
    PROPERTY(factory)
    PROPERTY(property)
    PROPERTY(meta)
REFLECTABLE_INIT()

REFLECTABLE_DECLARATION(rew::injection_t)
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE(rew::type_t)
    PROPERTY(name)
    PROPERTY(reflection)
    PROPERTY(size)
    PROPERTY(context)
    PROPERTY(injection)
REFLECTABLE_INIT()

REFLECTABLE_DECLARATION(rew::registry_t)
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE(rew::registry_t)
    FACTORY(rew::registry_t())
    FUNCTION(find, rew::type_t*(const std::string&))
    FUNCTION(find, rew::type_t*(std::type_index))
    FUNCTION(find, rew::type_t*(std::any&))
    PROPERTY(all)
    PROPERTY(rtti_all)
REFLECTABLE_INIT()

TEST(TestReflection, TestSelfReflection)
{
    rew::reflectable<std::vector<int>>();
    auto type = rew::global.find(typeid(std::vector<int>::iterator));
    (void)type;
    //static_assert(is_std_vector_iterator(std::vector<int>::iterator{}), "");
    //rew::reflectable<std::unordered_map<int, int>>();
    //using dirty_reflectable_type = std::pair<std_iterator<std::vector<int>>, bool>;

    //using reflectable_traits = rew::meta::reflectable_traits<dirty_reflectable_type>;
    //using reflectable_type = typename meta::reflectable_using<dirty_reflectable_type>::R;
    //auto xxx = reflectable_traits::name();

    //rew::find_or_add_type<dirty_reflectable_type>();
    //using R = Struct<Iterator<Array<bool>>>;
    //rew::reflectable<R>();
    //auto type = rew::global.find("Struct<Array<bool>::Iterator>");
    // auto factory = type->reflection->factory.find("Struct<Array<bool>::Iterator>()");
    // auto obj = factory->call({});
    // auto context = type->context(obj);
    // auto function = type->reflection->function.find("Foo")->all.begin()->second;
    // function.call(context, { Array<bool>::Iterator{} });

    //using type = __ArrayIterator<Array<bool>, Array<bool>::Iterator>::type;
    //using type = refl<std::vector<int>::iterator>;
}

struct TestReflectionProperty
{
    int f() { return 0; }
    void f(int) {}

    const int g() { return 0; }

    const int i = 0;

    int j = 0;
};

REFLECTABLE_DECLARATION(TestReflectionProperty)
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE(TestReflectionProperty)
    PROPERTY(f)
    PROPERTY(g)
    PROPERTY(i)
    PROPERTY(j)
REFLECTABLE_INIT()

TEST(TestReflection, TestProperty)
{
}
