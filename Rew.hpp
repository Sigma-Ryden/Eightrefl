// TODO:
// split
// remove dublicates code
// add name function for types
// add more infromative throw msg
// fix nameconv
// add support of C++11
#ifndef REW_HPP
#define REW_HPP

#include <functional>
#include <map>
#include <vector>
#include <any>

#include <type_traits>

// TODO:
// replace reflection_registry_impl_t to normal name and hide to class reflection_core_t
// add REFLECTABLE_ACCESS() macro for non-public registration
#define CORE_REFLECTABLE(reflection_core, ...)                                                          \
    template <> struct rew::detail::reflection_registry_impl_t<__VA_ARGS__> {                           \
        using type = __VA_ARGS__;                                                                       \
        inline static const auto reflection = &reflection_core;                                         \
        inline static const auto name = #__VA_ARGS__;                                                   \
        struct call_t {                                                                                 \
            template <class RegisterType> call_t(RegisterType&& registry) {

#define REFLECTABLE(...) CORE_REFLECTABLE(::rew::reflection, __VA_ARGS__)

#define PROPERTY(...)                                                                                   \
    registry.template property<decltype(&type::__VA_ARGS__)>({                                          \
        #__VA_ARGS__,                                                                                   \
        property_get_handler(&type::__VA_ARGS__),                                                       \
        property_set_handler(&type::__VA_ARGS__)                                                        \
    });

#define FUNCTION(...)                                                                                   \
    registry.template function<decltype(&type::__VA_ARGS__)>({                                          \
        #__VA_ARGS__,                                                                                   \
        function_is_static(&type::__VA_ARGS__),                                                         \
        function_call_handler(&type::__VA_ARGS__)                                                       \
    });

#define PARENT(...)                                                                                     \
    registry.template parent<__VA_ARGS__, type>({                                                       \
        #__VA_ARGS__,                                                                                   \
        parent_get_handler<__VA_ARGS__, type>(),                                                        \
        parent_reflection_handler(#__VA_ARGS__, reflection)                                             \
    });

#define FACTORY(...)                                                                                    \
    registry.template factory<__VA_ARGS__>({                                                            \
        #__VA_ARGS__,                                                                                   \
        factory_call_handler<__VA_ARGS__>()                                                             \
    });

#define META(name, ...)                                                                                 \
    registry.meta(name, __VA_ARGS__);

#define REFLECTABLE_INIT()                                                                              \
            }                                                                                           \
        };                                                                                              \
        private:                                                                                        \
        inline static auto _ = call_t(reflection_register_t{                                            \
            reflection->add<type>(name)->reflection                                                     \
        });                                                                                             \
    };

#define REFLECTABLE_REGISTRY(index, ...)                                                                \
    namespace rew {                                                                                     \
        template <> struct register_trait<index> {                                                      \
            using type = __VA_ARGS__;                                                                   \
            inline static auto _ = register_base_trait::rtti_table.emplace(                             \
                typeid(type).hash_code(), index                                                         \
            ).second;                                                                                   \
        };                                                                                              \
    }

namespace rew
{

namespace detail
{

template <typename, typename enable = void>
struct is_complete : std::false_type {};

template <typename T>
struct is_complete<T, std::void_t<decltype(sizeof(T))>> : std::true_type {};

template <class ClassType>
class reflection_registry_impl_t;

} // namespace detail

class property_t
{
public:
    struct meta_t
    {
        const char *const name = nullptr;

        const std::function<void(void *const, std::any&)> get = nullptr;
        const std::function<void(void *const, const std::any&)> set = nullptr;
        // TODO:
        // add type_t and reg for primitives
    };

    std::map<const char*, meta_t> all;

public:
    meta_t* find(const char* name)
    {
        auto it = all.find(name);
        return it != all.end() ? &it->second : nullptr;
    }

    bool add(const char* name, const meta_t& meta)
    {
        return all.emplace(name, meta).second;
    }

    bool remove(const char* name)
    {
        return all.erase(name)>0;
    }
};

class function_t
{
public:
    struct meta_t
    {
        const char *const name = nullptr;
        const bool is_static = false;

        const std::function<void(void *const, std::any&, std::initializer_list<std::any>)> call = nullptr;
        // TODO:
        // add arguments count, arguments types
    };

    std::map<const char*, meta_t> all;

public:
    meta_t* find(const char* name)
    {
        auto it = all.find(name);
        return it != all.end() ? &it->second : nullptr;
    }

    bool add(const char* name, const meta_t& meta)
    {
        return all.emplace(name, meta).second;
    }

    bool remove(const char* name)
    {
        return all.erase(name)>0;
    }
};

struct reflection_t;

struct register_t
{
    virtual ~register_t() = default;
};

struct type_t
{
    const char *const name = nullptr;

    reflection_t *const reflection = nullptr;
    const std::function<void(register_t&)> registry = nullptr;
};

class parent_t
{
public:
    struct meta_t
    {
        const char *const name = nullptr;

        const std::function<void*(void *const)> get = nullptr;
        const std::function<type_t*(void)> type = nullptr;
    };

    std::map<const char*, meta_t> all;

public:
    meta_t* find(const char* name)
    {
        auto it = all.find(name);
        return it != all.end() ? &it->second : nullptr;
    }

    bool add(const char* name, const meta_t& meta)
    {
        return all.emplace(name, meta).second;
    }

    bool remove(const char* name)
    {
        return all.erase(name)>0;
    }
};

class factory_t
{
public:
    struct meta_t
    {
        const char *const name = nullptr;

        const std::function<void*(std::initializer_list<std::any>)> call = nullptr;
        // TODO:
        // add arguments count, arguments type
    };

    std::map<const char*, meta_t> all;

public:
    meta_t* find(const char* name)
    {
        auto it = all.find(name);
        return it != all.end() ? &it->second : nullptr;
    }

    bool add(const char* name, const meta_t& meta)
    {
        return all.emplace(name, meta).second;
    }

    bool remove(const char* name)
    {
        return all.erase(name)>0;
    }
};

// TODO:
// improve meta class for better registration info
class meta_t
{
public:
    std::map<const char*, std::any> all;

public:
    std::any* find(const char* name)
    {
        auto it = all.find(name);
        return it != all.end() ? &it->second : nullptr;
    }

    bool add(const char* name, const std::any& data)
    {
        return all.emplace(name, data).second;
    }

    bool remove(const char* name)
    {
        return all.erase(name)>0;
    }
};

struct reflection_t
{
    const char *const name = nullptr;

    parent_t parent;
    property_t property;
    function_t function;
    factory_t factory;
    meta_t meta;
};

struct reflection_register_t : register_t
{
    reflection_register_t(reflection_t* reflection) : reflection(reflection) {}

    template <typename PropertyType>
    void property(const rew::property_t::meta_t& meta)
    {
        reflection->property.add(meta.name, meta);
    }

    template <typename FunctionType>
    void function(const rew::function_t::meta_t& meta)
    {
        reflection->function.add(meta.name, meta);
    }

    template <class BaseClassType, class DerivedClassType>
    void parent(const rew::parent_t::meta_t& meta)
    {
        reflection->parent.add(meta.name, meta);
    }

    template <class OtherClassType, typename... ArgumentTypes>
    void factory(const rew::factory_t::meta_t& meta)
    {
        reflection->factory.add(meta.name, meta);
    }

    template <typename MetaType>
    void meta(const char* name, const MetaType& data)
    {
        reflection->meta.add(name, data);
    }

    reflection_t* reflection = nullptr;
};


struct register_base_trait
{
    inline static std::map<std::size_t, std::size_t> rtti_table;
};

template <int Key>
struct register_trait
{
    using type = void;
};

struct polymorphic_register_t
{
    template <class ReflectableType>
    static void call(register_t& registry)
    {
        auto key = register_base_trait::rtti_table.at(typeid(registry).hash_code());
        try_call<ReflectableType>(registry, key);
    }

    template <class ReflectableType, std::size_t Key = 0>
    static void try_call(register_t& registry, std::size_t key)
    {
        if constexpr (Key < 8)
        {
            using derived_register_t = typename register_trait<Key>::type;
            if constexpr (not std::is_same_v<derived_register_t, void>)
            {
                using call_t = typename detail::reflection_registry_impl_t<ReflectableType>::call_t;
                if (Key == key)
                {
                    call_t(dynamic_cast<derived_register_t&>(registry));
                    return;
                }
            }
            try_call<ReflectableType, Key + 1>(registry, key);
        }
        else
        {
            throw "bad registry";
        }
    }
};

class reflection_core_t
{
public:
    std::map<const char*, type_t> all;

public:
    ~reflection_core_t()
    {
        for (auto& [name, meta] : all) delete meta.reflection;
    }

public:
    type_t* find(const char* name)
    {
        auto it = all.find(name);
        return it != all.end() ? &it->second : nullptr;
    }

    template <class ClassType>
    type_t* find()
    {
        return get(detail::reflection_registry_impl_t<ClassType>::name);
    }

    template <class ClassType>
    type_t* add(const char* name)
    {
        auto reflection = new reflection_t{ name };

        auto registry = [](register_t& registry)
        {
            polymorphic_register_t::call<ClassType>(registry);
        };

        auto [it, success] = all.emplace(name, type_t{ name, reflection, registry });
        return &it->second;
    }
};

reflection_core_t reflection;

template <class ClassType, typename PropertyType>
auto property_get_handler(PropertyType ClassType::* property)
{
    return [property](void *const self, std::any& result)
    {
        result = static_cast<ClassType*>(self)->*property;
    };
}

template <class ClassType, typename PropertyType>
auto property_set_handler(PropertyType ClassType::* property)
{
    return [property](void *const self, const std::any& value)
    {
        static_cast<ClassType*>(self)->*property = std::any_cast<const PropertyType&>(value);
    };
}

template <class ClassType, typename ReturnType, typename... ArgumentTypes, std::size_t... I>
auto function_call_handler_impl(ReturnType (ClassType::* function)(ArgumentTypes...), std::index_sequence<I...>)
{
    return [function](void *const self, std::any& result, std::initializer_list<std::any> arguments)
    {
        result = (static_cast<ClassType*>(self)->*function)(std::any_cast<ArgumentTypes>(arguments.begin()[I])...);
    };
}

template <class ClassType, typename... ArgumentTypes, std::size_t... I>
auto function_call_handler_impl(void (ClassType::* function)(ArgumentTypes...), std::index_sequence<I...>)
{
    return [function](void *const self, std::any& result, std::initializer_list<std::any> arguments)
    {
        (static_cast<ClassType*>(self)->*function)(std::any_cast<ArgumentTypes>(arguments.begin()[I])...);
        result.reset();
    };
}

template <typename ReturnType, typename... ArgumentTypes, std::size_t... I>
auto function_call_handler_impl(ReturnType (*function)(ArgumentTypes...), std::index_sequence<I...>)
{
    return [function](void *const self, std::any& result, std::initializer_list<std::any> arguments)
    {
        result = function(std::any_cast<ArgumentTypes>(arguments.begin()[I])...);
    };
}

template <typename... ArgumentTypes, std::size_t... I>
auto function_call_handler_impl(void (*function)(ArgumentTypes...), std::index_sequence<I...>)
{
    return [function](void *const self, std::any& result, std::initializer_list<std::any> arguments)
    {
        function(std::any_cast<ArgumentTypes>(arguments.begin()[I])...);
        result.reset();
    };
}

template <class ClassType, typename ReturnType, typename... ArgumentTypes>
auto function_call_handler(ReturnType (ClassType::* function)(ArgumentTypes...))
{
    return function_call_handler_impl(function, std::index_sequence_for<ArgumentTypes...>{});
}

template <typename ReturnType, typename... ArgumentTypes>
auto function_call_handler(ReturnType (*function)(ArgumentTypes...))
{
    return function_call_handler_impl(function, std::index_sequence_for<ArgumentTypes...>{});
}

template <class ClassType, typename ReturnType, typename... ArgumentTypes>
auto function_is_static(ReturnType (ClassType::* function)(ArgumentTypes...))
{
    return false;
}

template <typename ReturnType, typename... ArgumentTypes>
auto function_is_static(ReturnType (*function)(ArgumentTypes...))
{
    return true;
}

template <class BaseClassType, class DerivedClassType>
auto parent_get_handler()
{
    return [](void *const self)
    {
        return static_cast<BaseClassType*>(static_cast<DerivedClassType*>(self));
    };
}

auto parent_reflection_handler(const char* name, reflection_core_t* reflection)
{
    return [name, reflection](void)
    {
        return reflection->find(name);
    };
}

template <class OtherClassType, typename... ArgumentTypes, std::size_t... I>
auto factory_call_handler_impl(std::index_sequence<I...>)
{
    return [](std::initializer_list<std::any> arguments) -> void*
    {
        return new OtherClassType(std::any_cast<ArgumentTypes>(arguments.begin()[I])...);
    };
}

template <class OtherClassType, typename... ArgumentTypes>
auto factory_call_handler()
{
    return factory_call_handler_impl<OtherClassType, ArgumentTypes...>(std::index_sequence_for<ArgumentTypes...>{});
}

} // namespace rew

#endif // REW_HPP

REFLECTABLE_REGISTRY(0, rew::reflection_register_t)
