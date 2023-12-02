#ifndef REW_HPP
#define REW_HPP

#include <functional>
#include <map>
#include <any>
#include <string>
#include <vector>

//#define REW_UNSAFE_ENABLE

#define REFLECTABLE(...)                                                                                \
    template <>                                                                                         \
    class rew::detail::reflection_registry_implementation_t<__VA_ARGS__> {                              \
        using type = __VA_ARGS__;                                                                       \
        inline static const auto name = #__VA_ARGS__;                                                   \
        inline static const auto instance = reflection.add<type>(name)

#define PROPERTY(...) .add_property(#__VA_ARGS__, &type::__VA_ARGS__)
#define FUNCTION(...) .add_function(#__VA_ARGS__, &type::__VA_ARGS__)
#define PARENT(...) .add_parent<__VA_ARGS__>(#__VA_ARGS__)
#define FACTORY(...) .add_factory<__VA_ARGS__>(#__VA_ARGS__)

#define REFLECTABLE_INIT() .add_factory(name).add_meta(name); };

namespace rew
{

struct reflectable_t { virtual ~reflectable_t() = default; };

namespace detail
{

template <class ClassType>
class reflection_implementation_t;

class reflection_t;

template <class ClassType>
class reflection_registry_implementation_t;

class reflection_registry_t
{
public:
    std::map<const char*, reflection_t> table;

public:
    reflection_t& get(const char* name)
    {
        return table.at(name);
    }

    template <class ClassType>
    detail::reflection_implementation_t<ClassType>& add(const char* name)
    {
        return *std::launder(reinterpret_cast<detail::reflection_implementation_t<ClassType>*>(&table[name]));
    }
};

} // namespace detail

inline static detail::reflection_registry_t reflection;

namespace detail
{

class property_t
{
public:
    std::map<const char*, std::function<void(void *const, std::any&)>> itable;
    std::map<const char*, std::function<void(void *const, const std::any&)>> otable;

public:
    void get(const char* name, void *const self, std::any& result)
    {
        itable.at(name)(self, result);
    }

    template <typename ValueType>
    void set(const char* name, void *const self, ValueType&& value)
    {
        otable.at(name)(self, value);
    }
};

class function_t
{
public:
#ifdef REW_UNSAFE_ENABLE
    std::map<const char*, std::function<void(void *const, std::any&, std::initializer_list<void*>)>> table;
#else
    std::map<const char*, std::function<void(void *const, std::any&, std::initializer_list<std::any>)>> table;
#endif
public:
    template <typename... ArgumentTypes>
    void call(const char* name, void *const self, std::any& result, ArgumentTypes&&... arguments)
    {
    #ifdef REW_UNSAFE_ENABLE
        table.at(name)(self, result, { std::addressof(arguments)... });
    #else
        table.at(name)(self, result, { std::forward<ArgumentTypes>(arguments)... });
    #endif
    }
};

class parent_t
{
public:
    std::map<const char*, std::function<void*(void *const)>> table;
    std::map<const char*, std::function<reflection_t&(void)>> registry;

public:
    void* get(const char* name, void *const self)
    {
        return table.at(name)(self);
    }

    reflection_t& reflection(const char* name)
    {
        return registry.at(name)();
    }
};

class factory_t
{
public:
    std::map<const char*, std::function<reflectable_t*(void)>> table;

public:
    reflectable_t* call(const char* name)
    {
        return table.at(name)();
    }
};

class meta_t
{
public:
    std::string name;
};

struct reflection_t
{
    property_t property;
    function_t function;
    parent_t parent;
    factory_t factory;
    meta_t meta;
};

template <class ClassType>
class reflection_implementation_t : reflection_t
{
public:
    template <typename PropertyType>
    reflection_implementation_t& add_property(const char* name, PropertyType ClassType::* property)
    {
        this->property.itable[name] = [property](void *const self, std::any& result)
        {
            result = static_cast<ClassType*>(self)->*property;
        };

        this->property.otable[name] = [property](void *const self, const std::any& value)
        {
            static_cast<ClassType*>(self)->*property = std::any_cast<const PropertyType&>(value);
        };

        return *this;
    }

    template <typename ReturnType, typename... ArgumentTypes>
    reflection_implementation_t& add_function(const char* name, ReturnType (ClassType::* function)(ArgumentTypes...))
    {
        return add_function_implementation(name, function, std::make_index_sequence<sizeof...(ArgumentTypes)>{});
    }

private:
    template <typename ReturnType, typename... ArgumentTypes, std::size_t... I>
    reflection_implementation_t& add_function_implementation(const char* name, ReturnType (ClassType::* function)(ArgumentTypes...), std::index_sequence<I...>)
    {
    #ifdef REW_UNSAFE_ENABLE
        this->function.table[name] = [function](void *const self, std::any& result, std::initializer_list<void*> arguments)
        {
            result = (static_cast<ClassType*>(self)->*function)(*static_cast<std::decay_t<ArgumentTypes>*>(arguments.begin()[I])...);
        };
    #else
        this->function.table[name] = [function](void *const self, std::any& result, std::initializer_list<std::any> arguments)
        {
            result = (static_cast<ClassType*>(self)->*function)(std::any_cast<ArgumentTypes>(arguments.begin()[I])...);
        };
    #endif
        return *this;
    }

    template <typename ReturnType, typename... ArgumentTypes, std::size_t... I>
    reflection_implementation_t& add_function_implementation(const char* name, void (ClassType::* function)(ArgumentTypes...), std::index_sequence<I...>)
    {
        this->function.table[name] = [function](void *const self, std::any& result, const std::vector<std::any>& arguments)
        {
            (static_cast<ClassType*>(self)->*function)(std::any_cast<ArgumentTypes>(arguments[I])...);
        };

        return *this;
    }

public:
    template <class ParentClassType>
    reflection_implementation_t& add_parent(const char* name)
    {
        this->parent.table[name] = [](void *const self)
        {
            return static_cast<ParentClassType*>(static_cast<ClassType*>(self));
        };

        this->parent.registry[name] = [name](void) -> reflection_t&
        {
            return rew::reflection.get(name);
        };

        return *this;
    }

    template <class OtherClassType = ClassType>
    reflection_implementation_t& add_factory(const char* name)
    {
        this->factory.table[name] = [](void)
        {
            return static_cast<reflectable_t*>(new OtherClassType);
        };
        return *this;
    }

    reflection_implementation_t& add_meta(const char* name)
    {
        this->meta.name = name;
        return *this;
    }
};

} // namespace detail

} // namespace rew

#endif // REW_HPP
