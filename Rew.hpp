#ifndef REW_HPP
#define REW_HPP

#include <functional>
#include <map>
#include <any>
#include <string>
#include <vector>

#define REFLECTABLE(...)                                                                                \
    template <>                                                                                         \
    struct rew::detail::reflection_registry_impl_t<__VA_ARGS__> {                                       \
        using type = __VA_ARGS__;                                                                       \
        inline static const auto name = #__VA_ARGS__;                                                   \
        inline static const auto instance = reflection.add<type>(name)

#define PROPERTY(...) ->add_property(#__VA_ARGS__, &type::__VA_ARGS__)
#define FUNCTION(...) ->add_function(#__VA_ARGS__, &type::__VA_ARGS__)
#define PARENT(...) ->add_parent<__VA_ARGS__>(#__VA_ARGS__)
#define FACTORY(...) ->add_factory<__VA_ARGS__>(#__VA_ARGS__)
#define META(name, ...) ->add_meta(name, __VA_ARGS__)

#define REFLECTABLE_INIT() ->add_factory(name)->add_default_meta(name); };

namespace rew
{

namespace detail
{

template <typename, typename enable = void>
struct is_complete : std::false_type {};

template <typename T>
struct is_complete<T, std::void_t<decltype(sizeof(T))>> : std::true_type {};

struct reflection_t;

template <class ClassType>
class reflection_impl_t;

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
    std::map<const char*, std::function<void(void *const, std::any&, std::initializer_list<std::any>)>> table;

public:
    template <typename... ArgumentTypes>
    void call(const char* name, void *const self, std::any& result, ArgumentTypes&&... arguments)
    {
        table.at(name)(self, result, { std::forward<ArgumentTypes>(arguments)... });
    }
};

class parent_t
{
public:
    std::map<const char*, std::function<void*(void *const)>> table;
    std::map<const char*, std::function<reflection_t*(void)>> registry;

public:
    void* get(const char* name, void *const self)
    {
        return table.at(name)(self);
    }

    reflection_t* reflection(const char* name)
    {
        return registry.at(name)();
    }
};

class factory_t
{
public:
    std::map<const char*, std::function<void*(void)>> table;

public:
    void* call(const char* name)
    {
        return table.at(name)();
    }
};

class meta_t
{
public:
    std::map<const char*, std::any> table;
    std::string name;

public:
    std::any get(const char* name)
    {
        return table.at(name);
    }
};

struct reflection_t
{
    property_t property;
    function_t function;
    parent_t parent;
    factory_t factory;
    meta_t meta;

    virtual ~reflection_t() = default;
};

template <class ClassType>
class reflection_registry_impl_t;

class reflection_registry_t
{
public:
    std::map<const char*, reflection_t*> table;
    std::map<const char*, reflection_t*> rtti_table;

public:
    ~reflection_registry_t()
    {
        for (auto& [name, reflection] : table) delete reflection;
    }

    reflection_t* get(const char* name)
    {
        auto it = table.find(name);
        return it != table.end() ? it->second : nullptr;
    }

    reflection_t* get(const std::any& object)
    {
        auto it = rtti_table.find(object.type().name());
        return it != rtti_table.end() ? it->second : nullptr;
    }

    template <class ClassType>
    reflection_t* get()
    {
        return get(detail::reflection_registry_impl_t<ClassType>::name);
    }

public:
    template <class ClassType>
    detail::reflection_impl_t<ClassType>* add(const char* name)
    {
        auto reflection = new detail::reflection_impl_t<ClassType>();
        table.emplace(name, reflection);
        rtti_table.emplace(typeid(ClassType).name(), reflection);
        return reflection;
    }
};

} // namespace detail

inline static detail::reflection_registry_t reflection;

namespace detail
{

template <class ClassType>
class reflection_impl_t : public reflection_t
{
public:
    template <typename PropertyType>
    reflection_impl_t* add_property(const char* name, PropertyType ClassType::* property)
    {
        this->property.itable[name] = [property](void *const self, std::any& result)
        {
            result = static_cast<ClassType*>(self)->*property;
        };

        this->property.otable[name] = [property](void *const self, const std::any& value)
        {
            static_cast<ClassType*>(self)->*property = std::any_cast<const PropertyType&>(value);
        };

        return this;
    }

    template <typename ReturnType, typename... ArgumentTypes>
    reflection_impl_t* add_function(const char* name, ReturnType (*function)(ArgumentTypes...))
    {
        return add_function_impl(name, function, std::make_index_sequence<sizeof...(ArgumentTypes)>{});
    }

    template <typename ReturnType, typename... ArgumentTypes>
    reflection_impl_t* add_function(const char* name, ReturnType (ClassType::* function)(ArgumentTypes...))
    {
        return add_function_impl(name, function, std::make_index_sequence<sizeof...(ArgumentTypes)>{});
    }

private:
    template <typename ReturnType, typename... ArgumentTypes, std::size_t... I>
    reflection_impl_t* add_function_impl(const char* name, ReturnType (ClassType::* function)(ArgumentTypes...), std::index_sequence<I...>)
    {
        this->function.table[name] = [function](void *const self, std::any& result, std::initializer_list<std::any> arguments)
        {
            result = (static_cast<ClassType*>(self)->*function)(std::any_cast<ArgumentTypes>(arguments.begin()[I])...);
        };

        return this;
    }

    template <typename... ArgumentTypes, std::size_t... I>
    reflection_impl_t* add_function_impl(const char* name, void (ClassType::* function)(ArgumentTypes...), std::index_sequence<I...>)
    {
        this->function.table[name] = [function](void *const self, std::any& result, const std::vector<std::any>& arguments)
        {
            (static_cast<ClassType*>(self)->*function)(std::any_cast<ArgumentTypes>(arguments[I])...);
        };
        return this;
    }

    template <typename ReturnType, typename... ArgumentTypes, std::size_t... I>
    reflection_impl_t* add_function_impl(const char* name, ReturnType (*function)(ArgumentTypes...), std::index_sequence<I...>)
    {
        this->function.table[name] = [function](void *const self, std::any& result, const std::vector<std::any>& arguments)
        {
            result = function(std::any_cast<ArgumentTypes>(arguments[I])...);
        };
        return this;
    }

    template <typename... ArgumentTypes, std::size_t... I>
    reflection_impl_t* add_function_impl(const char* name, void (*function)(ArgumentTypes...), std::index_sequence<I...>)
    {
        this->function.table[name] = [function](void *const self, std::any& result, const std::vector<std::any>& arguments)
        {
            function(std::any_cast<ArgumentTypes>(arguments[I])...);
        };
        return this;
    }

public:
    template <class ParentClassType>
    reflection_impl_t* add_parent(const char* name)
    {
        this->parent.table[name] = [](void *const self)
        {
            return static_cast<ParentClassType*>(static_cast<ClassType*>(self));
        };

        this->parent.registry[name] = [name](void) -> reflection_t*
        {
            return rew::reflection.get(name);
        };

        return this;
    }

    template <class OtherClassType = ClassType>
    reflection_impl_t* add_factory(const char* name)
    {
        this->factory.table[name] = [](void)
        {
            return static_cast<void*>(new OtherClassType);
        };
        return this;
    }

    reflection_impl_t* add_default_meta(const char* name)
    {
        this->meta.name = name;
        return this;
    }

    reflection_impl_t* add_meta(const char* name, const std::any& data)
    {
        this->meta.table[name] = data;
        return this;
    }
};

} // namespace detail

} // namespace rew

#endif // REW_HPP
