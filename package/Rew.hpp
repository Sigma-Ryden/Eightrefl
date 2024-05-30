#ifndef REW_HPP
#define REW_HPP

#include <cstddef> // size_t

#include <string> // string
#include <unordered_map> // unordered_map

#include <typeindex> // type_index

#include <any> // any

#include <functional> // function

namespace rew
{

template <class MetaType>
struct attribute_t
{
    MetaType* find(std::string const& name)
    {
        auto it = all.find(name);
        return it != all.end() ? &it->second : nullptr;
    }

    MetaType& add(std::string const& name, MetaType const& meta)
    {
        return all.emplace(name, meta).first->second;
    }

    bool remove(std::string const& name)
    {
        return all.erase(name)>0;
    }

    std::unordered_map<std::string, MetaType> all;
};

} // namespace rew

// .meta(name, expr)
#define META(name_str, ...)                                                                             \
    {                                                                                                   \
        auto __meta = rew::find_or_add_meta(__reflection, name_str, __VA_ARGS__);                       \
        injection.template meta<CleanR, decltype(__VA_ARGS__)>(name_str, *__meta);                      \
    }

// .parent<R, type>()
#define PARENT(...)                                                                                     \
    {                                                                                                   \
        auto __meta = rew::find_or_add_parent<CleanR, __VA_ARGS__>(__reflection);                       \
        injection.template parent<CleanR, __VA_ARGS__>(*__meta);                                        \
    }

namespace rew
{

struct type_t;

struct parent_t
{
    std::string const name;
    type_t *const type = nullptr;
    std::function<std::any(std::any const& child_context)> const cast = nullptr;
    attribute_t<std::any> meta;
};

template <typename ReflectableType, typename ParentReflectableType>
auto handler_parent_cast()
{
    return [](std::any const& child_context) -> std::any
    {
        return static_cast<ParentReflectableType*>(std::any_cast<ReflectableType*>(child_context));
    };
}

} // namespace rew

#include <vector> // vector

#include <memory> // addressof

#include <type_traits> // conjunction, disjunction, false_type, true_type, void_t

#include <utility> // pair

namespace rew
{

namespace meta
{

template <typename... Bn> using all = std::conjunction<Bn...>;
template <typename... Bn> using one = std::disjunction<Bn...>;

template <typename T, typename enable = void> struct is_complete : std::false_type {};
template <typename T> struct is_complete<T, std::void_t<decltype(sizeof(T))>> : std::true_type {};

template <typename T, typename enable = void>
struct reflectable_traits;

template <typename T, typename enable = void>
struct reflectable_using
{
    using R = T;
};

struct dummy_type {};

namespace detail
{

template <typename T>
struct is_inheritable : all<std::is_class<T>, std::negation<std::is_final<T>>> {};

} // namespace detail

template <typename T>
struct reflectable_using_base : std::conditional_t<detail::is_inheritable<T>::value, T, dummy_type> {};

template <typename T> using clean = typename reflectable_using<T>::R;

template <typename T>
struct to_reflectable_reference
{
    using type = std::remove_const_t<std::remove_reference_t<T>>*;
};

template <typename T>
using to_reflectable_reference_t = typename to_reflectable_reference<T>::type;

template <typename T>
struct to_reflectable_pointer
{
    using type = std::remove_const_t<std::remove_pointer_t<T>>*;
};

template <typename T>
using to_reflectable_pointer_t = typename to_reflectable_pointer<T>::type;

template <typename T>
struct to_reflectable_object
{
    using type = std::remove_const_t<T>;
};

template <typename T>
using to_reflectable_object_t = typename to_reflectable_object<T>::type;

namespace detail
{

template <typename T, typename enable = void> struct is_lazy : std::false_type {};
template <typename T> struct is_lazy<T, std::void_t<decltype(&reflectable_traits<T>::lazy)>> : std::true_type {};

template <typename T, typename enable = void> struct is_builtin : std::false_type {};
template <typename T> struct is_builtin<T, std::void_t<decltype(&reflectable_traits<T>::biiltin)>> : std::true_type {};

} // namespace detail

template <typename T, typename enable = void> struct is_reflectable : std::false_type {};
template <typename T> struct is_reflectable<T, std::void_t<decltype(&reflectable_traits<T>::registry)>> : std::true_type {};

template <typename T> struct is_lazy_reflectable : all<is_reflectable<T>, detail::is_lazy<T>> {};

template <typename T> struct is_builtin_reflectable : all<is_reflectable<T>, detail::is_builtin<T>> {};
template <typename T> struct is_custom_reflectable : all<is_reflectable<T>, std::negation<detail::is_builtin<T>>> {};

template <typename PropertyType>
struct property_traits
{
    using type = PropertyType;
};

template <typename ReflectableType, typename PropertyType>
struct property_traits<PropertyType (ReflectableType::*)(void) const>
    : property_traits<PropertyType> {};

template <typename ReflectableType, typename PropertyType>
struct property_traits<PropertyType (ReflectableType::*)(void) const&>
    : property_traits<PropertyType> {};

template <typename ReflectableType, typename PropertyType>
struct property_traits<PropertyType (ReflectableType::*)(void)>
    : property_traits<PropertyType> {};

template <typename ReflectableType, typename PropertyType>
struct property_traits<PropertyType (ReflectableType::*)(void)&>
    : property_traits<PropertyType> {};

template <typename ReflectableType, typename PropertyType>
struct property_traits<void (ReflectableType::*)(PropertyType)>
    : property_traits<PropertyType> {};

template <typename ReflectableType, typename PropertyType>
struct property_traits<void (ReflectableType::*)(PropertyType)&>
    : property_traits<PropertyType> {};

template <typename PropertyType>
struct property_traits<PropertyType (*)(void)>
    : property_traits<PropertyType> {};

template <typename PropertyType>
struct property_traits<PropertyType() const>
    : property_traits<PropertyType> {};

template <typename PropertyType>
struct property_traits<PropertyType()>
    : property_traits<PropertyType> {};

template <typename PropertyType>
struct property_traits<void(PropertyType)>
    : property_traits<PropertyType> {};

template <typename ReflectableType, typename PropertyType>
struct property_traits<PropertyType ReflectableType::*>
    : property_traits<PropertyType> {};

template <typename>
struct function_traits;

template <typename ReturnType, typename... ArgumentTypes>
struct function_traits<ReturnType(ArgumentTypes...) const>
{
    using dirty_type = ReturnType(ArgumentTypes...) const;
    using dirty_pointer = ReturnType(*)(ArgumentTypes...);

    using type = clean<ReturnType>(clean<ArgumentTypes>...) const;
    using pointer = clean<ReturnType>(*)(clean<ArgumentTypes>...);
};

template <typename ReturnType, typename... ArgumentTypes>
struct function_traits<ReturnType(ArgumentTypes...) const&>
{
    using dirty_type = ReturnType(ArgumentTypes...) const&;
    using dirty_pointer = ReturnType(*)(ArgumentTypes...);

    using type = clean<ReturnType>(clean<ArgumentTypes>...) const&;
    using pointer = clean<ReturnType>(*)(clean<ArgumentTypes>...);
};

template <typename ReturnType, typename... ArgumentTypes>
struct function_traits<ReturnType(ArgumentTypes...)>
{
    using dirty_type = ReturnType(ArgumentTypes...);
    using dirty_pointer = ReturnType(*)(ArgumentTypes...);

    using type = clean<ReturnType>(clean<ArgumentTypes>...);
    using pointer = clean<ReturnType>(*)(clean<ArgumentTypes>...);
};

template <typename ReturnType, typename... ArgumentTypes>
struct function_traits<ReturnType(ArgumentTypes...)&>
{
    using dirty_type = ReturnType(ArgumentTypes...)&;
    using dirty_pointer = ReturnType(*)(ArgumentTypes...);

    using type = clean<ReturnType>(clean<ArgumentTypes>...)&;
    using pointer = clean<ReturnType>(*)(clean<ArgumentTypes>...);
};

template <typename ReturnType, typename... ArgumentTypes>
struct function_traits<ReturnType(*)(ArgumentTypes...)>
    : function_traits<ReturnType(ArgumentTypes...)> {};

template <class ClassType, typename ReturnType, typename... ArgumentTypes>
struct function_traits<ReturnType (ClassType::*)(ArgumentTypes...) const>
    : function_traits<ReturnType(ArgumentTypes...) const> {};

template <class ClassType, typename ReturnType, typename... ArgumentTypes>
struct function_traits<ReturnType (ClassType::*)(ArgumentTypes...) const&>
    : function_traits<ReturnType(ArgumentTypes...) const&> {};

template <class ClassType, typename ReturnType, typename... ArgumentTypes>
struct function_traits<ReturnType (ClassType::*)(ArgumentTypes...)>
    : function_traits<ReturnType(ArgumentTypes...)> {};

template <class ClassType, typename ReturnType, typename... ArgumentTypes>
struct function_traits<ReturnType (ClassType::*)(ArgumentTypes...)&>
    : function_traits<ReturnType(ArgumentTypes...)&> {};

namespace detail
{

template <typename ReflectableType, typename ParentReflectableType, typename ReturnType, typename... ArgumentTypes>
constexpr auto function_ptr_impl(ReturnType (ParentReflectableType::* function)(ArgumentTypes...) const)
{
    struct __inner : protected ReflectableType
    {
        static constexpr auto get(ReturnType (ParentReflectableType::* function)(ArgumentTypes...) const)
        {
            return static_cast<ReturnType (ReflectableType::*)(ArgumentTypes...) const>(function);
        }
    };
    return __inner::get(function);
}

template <typename ReflectableType, typename ParentReflectableType, typename ReturnType, typename... ArgumentTypes>
constexpr auto function_ptr_impl(ReturnType (ParentReflectableType::* function)(ArgumentTypes...) const&)
{
    struct __inner : protected ReflectableType
    {
        static constexpr auto get(ReturnType (ParentReflectableType::* function)(ArgumentTypes...) const&)
        {
            return static_cast<ReturnType (ReflectableType::*)(ArgumentTypes...) const&>(function);
        }
    };
    return __inner::get(function);
}

template <typename ReflectableType, typename ParentReflectableType, typename ReturnType, typename... ArgumentTypes>
constexpr auto function_ptr_impl(ReturnType (ParentReflectableType::* function)(ArgumentTypes...))
{
    struct __inner : protected ReflectableType
    {
        static constexpr auto get(ReturnType (ParentReflectableType::* function)(ArgumentTypes...))
        {
            return static_cast<ReturnType (ReflectableType::*)(ArgumentTypes...)>(function);
        }
    };
    return __inner::get(function);
}

template <typename ReflectableType, typename ParentReflectableType, typename ReturnType, typename... ArgumentTypes>
constexpr auto function_ptr_impl(ReturnType (ParentReflectableType::* function)(ArgumentTypes...)&)
{
    struct __inner : protected ReflectableType
    {
        static constexpr auto get(ReturnType (ParentReflectableType::* function)(ArgumentTypes...)&)
        {
            return static_cast<ReturnType (ReflectableType::*)(ArgumentTypes...)&>(function);
        }
    };
    return __inner::get(function);
}

template <typename ReflectableType, typename ReturnType, typename... ArgumentTypes>
constexpr auto function_ptr_impl(ReturnType (*function)(ArgumentTypes...))
{
    return function;
}

template <typename ReflectableType, typename ParentReflectableType, typename ValueType>
constexpr auto property_ptr_impl(ValueType ParentReflectableType::* property)
{
    struct __inner : protected ReflectableType
    {
        static constexpr auto get(ValueType ParentReflectableType::* property)
        {
            return static_cast<ValueType ReflectableType::*>(property);
        }
    };
    return __inner::get(property);
}

} // namespace detail

template <typename...>
struct access_traits;

template <>
struct access_traits<>
{
    template <typename...>
    struct property
    {
        template <typename PropertyType>
        static constexpr auto of(PropertyType (*get)(void), void (*set)(PropertyType))
        {
            return std::make_pair(get, set);
        }

        template <typename PropertyType>
        static constexpr auto of(PropertyType* get, PropertyType* set)
        {
            return std::make_pair(get, set);
        }
    };

    template <typename PropertyType>
    struct property<PropertyType()>
    {
        static constexpr auto of(clean<PropertyType> (*get)(void), void (*set)(clean<PropertyType>))
        {
            return std::make_pair(get, set);
        }
    };

    template <typename PropertyType>
    struct property<PropertyType>
    {
        static constexpr auto of(clean<PropertyType>* get, clean<PropertyType>* set)
        {
            return std::make_pair(get, set);
        }
    };

    template <typename...>
    struct function
    {
        template <typename ReturnType, typename... ArgumentTypes>
        static constexpr auto of(ReturnType (*function)(ArgumentTypes...)) { return function; }
    };

    template <typename ReturnType, typename... ArgumentTypes>
    struct function<ReturnType(ArgumentTypes...)>
    {
        static constexpr auto of(clean<ReturnType> (*function)(clean<ArgumentTypes>...)) { return function; }

        template <typename OtherReturnType, typename... OtherArgumentTypes>
        static constexpr auto of(OtherReturnType (*function)(OtherArgumentTypes...)) { return function; }
    };
};

template <class ClassType>
struct access_traits<ClassType>
{
    template <typename PropertyGetterType, typename PropertySetterType>
    static constexpr auto ppack(PropertyGetterType get, PropertySetterType set)
    {
        return std::make_pair(detail::property_ptr_impl<ClassType>(get), detail::property_ptr_impl<ClassType>(set));
    }

    template <typename PropertyGetterType, typename PropertySetterType>
    static constexpr auto fpack(PropertyGetterType get, PropertySetterType set)
    {
        return std::make_pair(detail::function_ptr_impl<ClassType>(get), detail::function_ptr_impl<ClassType>(set));
    }

    template <typename FunctionType>
    static constexpr auto fpack(FunctionType function)
    {
        return detail::function_ptr_impl<ClassType>(function);
    }

    template <typename...>
    struct property
    {
        template <typename ParentClassType, typename PropertyType>
        static constexpr auto of(PropertyType ParentClassType::* get, PropertyType ParentClassType::* set)
        {
            return ppack(get, set);
        }

        template <typename ParentClassType, typename PropertyType>
        static constexpr auto of(PropertyType (ParentClassType::* get)(void) const, void (ParentClassType::* set)(PropertyType))
        {
            return fpack(get, set);
        }

        template <typename ParentClassType, typename PropertyType>
        static constexpr auto of(PropertyType (ParentClassType::* get)(void) const, void (ParentClassType::* set)(PropertyType)&)
        {
            return fpack(get, set);
        }

        template <typename ParentClassType, typename PropertyType>
        static constexpr auto of(PropertyType (ParentClassType::* get)(void) const&, void (ParentClassType::* set)(PropertyType))
        {
            return fpack(get, set);
        }

        template <typename ParentClassType, typename PropertyType>
        static constexpr auto of(PropertyType (ParentClassType::* get)(void) const&, void (ParentClassType::* set)(PropertyType)&)
        {
            return fpack(get, set);
        }

        template <typename ParentClassType, typename PropertyType>
        static constexpr auto of(PropertyType (ParentClassType::* get)(void), void (ParentClassType::* set)(PropertyType))
        {
            return fpack(get, set);
        }

        template <typename ParentClassType, typename PropertyType>
        static constexpr auto of(PropertyType (ParentClassType::* get)(void), void (ParentClassType::* set)(PropertyType)&)
        {
            return fpack(get, set);
        }

        template <typename ParentClassType, typename PropertyType>
        static constexpr auto of(PropertyType (ParentClassType::* get)(void)&, void (ParentClassType::* set)(PropertyType))
        {
            return fpack(get, set);
        }

        template <typename ParentClassType, typename PropertyType>
        static constexpr auto of(PropertyType (ParentClassType::* get)(void)&, void (ParentClassType::* set)(PropertyType)&)
        {
            return fpack(get, set);
        }

        template <typename PropertyType>
        static constexpr auto of(PropertyType (*get)(void), void (*set)(PropertyType)) { return std::make_pair(get, set); }

        template <typename PropertyType>
        static constexpr auto of(PropertyType* get, PropertyType* set) { return std::make_pair(get, set); }
    };

    template <typename DirtyPropertyType>
    struct property<DirtyPropertyType() const>
    {
        using PropertyType = clean<DirtyPropertyType>;

        template <typename ParentClassType>
        static constexpr auto of(PropertyType (ParentClassType::* get)(void) const, void (ParentClassType::* set)(PropertyType))
        {
            return fpack(get, set);
        }

        template <typename ParentClassType>
        static constexpr auto of(PropertyType (ParentClassType::* get)(void) const, void (ParentClassType::* set)(PropertyType)&)
        {
            return fpack(get, set);
        }
    };

    template <typename DirtyPropertyType>
    struct property<DirtyPropertyType() const&>
    {
        using PropertyType = clean<DirtyPropertyType>;

        template <typename ParentClassType>
        static constexpr auto of(PropertyType (ParentClassType::* get)(void) const&, void (ParentClassType::* set)(PropertyType))
        {
            return fpack(get, set);
        }

        template <typename ParentClassType>
        static constexpr auto of(PropertyType (ParentClassType::* get)(void) const&, void (ParentClassType::* set)(PropertyType)&)
        {
            return fpack(get, set);
        }
    };

    template <typename DirtyPropertyType>
    struct property<DirtyPropertyType()>
    {
        using PropertyType = clean<DirtyPropertyType>;

        template <typename ParentClassType>
        static constexpr auto of(PropertyType (ParentClassType::* get)(void), void (ParentClassType::* set)(PropertyType))
        {
            return fpack(get, set);
        }

        template <typename ParentClassType>
        static constexpr auto of(PropertyType (ParentClassType::* get)(void), void (ParentClassType::* set)(PropertyType)&)
        {
            return fpack(get, set);
        }

        static constexpr auto of(PropertyType (*get)(void), void (*set)(PropertyType))
        {
            return std::make_pair(get, set);
        }
    };

    template <typename DirtyPropertyType>
    struct property<DirtyPropertyType()&>
    {
        using PropertyType = clean<DirtyPropertyType>;

        template <typename ParentClassType>
        static constexpr auto of(PropertyType (ParentClassType::* get)(void)&, void (ParentClassType::* set)(PropertyType))
        {
            return fpack(get, set);
        }

        template <typename ParentClassType>
        static constexpr auto of(PropertyType (ParentClassType::* get)(void)&, void (ParentClassType::* set)(PropertyType)&)
        {
            return fpack(get, set);
        }
    };

    template <typename DirtyPropertyType>
    struct property<DirtyPropertyType>
    {
        using PropertyType = clean<DirtyPropertyType>;

        template <typename ParentClassType>
        static constexpr auto of(PropertyType ParentClassType::* get, PropertyType ParentClassType::* set) { return ppack(get, set); }

        static constexpr auto of(PropertyType* get, PropertyType* set) { return std::make_pair(get, set); }
    };

    template <typename...>
    struct function
    {
        template <typename ReturnType, typename... ArgumentTypes>
        static constexpr auto of(ReturnType (ClassType::* function)(ArgumentTypes...) const) { return function; }
        template <class ParentClassType, typename ReturnType, typename... ArgumentTypes>
        static constexpr auto of(ReturnType (ParentClassType::* function)(ArgumentTypes...) const) { return fpack(function); }

        template <typename ReturnType, typename... ArgumentTypes>
        static constexpr auto of(ReturnType (ClassType::* function)(ArgumentTypes...) const&) { return function; }
        template <class ParentClassType, typename ReturnType, typename... ArgumentTypes>
        static constexpr auto of(ReturnType (ParentClassType::* function)(ArgumentTypes...) const&) { return fpack(function); }

        template <typename ReturnType, typename... ArgumentTypes>
        static constexpr auto of(ReturnType (ClassType::* function)(ArgumentTypes...)) { return function; }
        template <class ParentClassType, typename ReturnType, typename... ArgumentTypes>
        static constexpr auto of(ReturnType (ParentClassType::* function)(ArgumentTypes...)) { return fpack(function); }

        template <typename ReturnType, typename... ArgumentTypes>
        static constexpr auto of(ReturnType (ClassType::* function)(ArgumentTypes...)&) { return function; }
        template <class ParentClassType, typename ReturnType, typename... ArgumentTypes>
        static constexpr auto of(ReturnType (ParentClassType::* function)(ArgumentTypes...)&) { return fpack(function); }

        template <typename ReturnType, typename... ArgumentTypes>
        static constexpr auto of(ReturnType (*function)(ArgumentTypes...)) { return function; }
    };

    template <typename DirtyReturnType, typename... DirtyArgumentTypes>
    struct function<DirtyReturnType(DirtyArgumentTypes...) const>
    {
        static constexpr auto of(clean<DirtyReturnType> (ClassType::* function)(clean<DirtyArgumentTypes>...) const) { return function; }

        template <class ParentClassType>
        static constexpr auto of(clean<DirtyReturnType> (ParentClassType::* function)(clean<DirtyArgumentTypes>...) const)
        {
            return fpack(function);
        }
    };

    template <typename DirtyReturnType, typename... DirtyArgumentTypes>
    struct function<DirtyReturnType(DirtyArgumentTypes...) const&>
    {
        static constexpr auto of(clean<DirtyReturnType> (ClassType::* function)(clean<DirtyArgumentTypes>...) const&) { return function; }

        template <class ParentClassType>
        static constexpr auto of(clean<DirtyReturnType> (ParentClassType::* function)(clean<DirtyArgumentTypes>...) const&)
        {
            return fpack(function);
        }
    };

    template <typename DirtyReturnType, typename... DirtyArgumentTypes>
    struct function<DirtyReturnType(DirtyArgumentTypes...)>
    {
        static constexpr auto of(clean<DirtyReturnType> (ClassType::* function)(clean<DirtyArgumentTypes>...)) { return function; }

        template <class ParentClassType>
        static constexpr auto of(clean<DirtyReturnType> (ParentClassType::* function)(clean<DirtyArgumentTypes>...))
        {
            return fpack(function);
        }

        static constexpr auto of(clean<DirtyReturnType> (*function)(clean<DirtyArgumentTypes>...)) { return function; }
    };

    template <typename DirtyReturnType, typename... DirtyArgumentTypes>
    struct function<DirtyReturnType(DirtyArgumentTypes...)&>
    {
        static constexpr auto of(clean<DirtyReturnType> (ClassType::* function)(clean<DirtyArgumentTypes>...)&) { return function; }

        template <class ParentClassType>
        static constexpr auto of(clean<DirtyReturnType> (ParentClassType::* function)(clean<DirtyArgumentTypes>...)&)
        {
            return fpack(function);
        }
    };
};

} // namespace meta

} // namespace rew

namespace rew
{

inline namespace utility
{

template <typename ValueType>
ValueType forward(std::any const& object)
{
    if constexpr (std::is_reference_v<ValueType>)
    {
        return *std::any_cast<meta::to_reflectable_reference_t<ValueType>>(object);
    }
    else if constexpr (std::is_pointer_v<ValueType>)
    {
        return std::any_cast<meta::to_reflectable_pointer_t<ValueType>>(object);
    }
    else
    {
        return std::any_cast<meta::to_reflectable_object_t<ValueType>>(object);
    }
}

template <typename ValueType>
std::any backward(ValueType&& result)
{
    if constexpr (std::is_reference_v<ValueType>)
    {
        return const_cast<meta::to_reflectable_reference_t<ValueType>>(std::addressof(result));
    }
    else if constexpr (std::is_pointer_v<ValueType>)
    {
        return const_cast<meta::to_reflectable_pointer_t<ValueType>>(result);
    }
    else
    {
        return result;
    }
}

} // inline namespace utility

} // namespace rew

#define __REW_DEPAREN(arg) __REW_DEPAREN_IMPL(__REW_UATE arg)
#define __REW_UATE(...) __REW_UATE __VA_ARGS__
#define __REW_DEPAREN_IMPL(...) __REW_DEPAREN_IMPL_(__VA_ARGS__)
#define __REW_DEPAREN_IMPL_(...) __REW_EVAL ## __VA_ARGS__
#define __REW_EVAL__REW_UATE

#define __REW_TO_STRING(...) __REW_TO_STRING_IMPL(__REW_DEPAREN(__VA_ARGS__))
#define __REW_TO_STRING_IMPL(...) __REW_TO_STRING_IMPL_(__VA_ARGS__)
#define __REW_TO_STRING_IMPL_(...) #__VA_ARGS__

// .function<R, signature>(name, &R::func)
#define NAMED_FUNCTION(name_str, name, ...)                                                             \
    {                                                                                                   \
        using __access_traits = rew::meta::access_traits<CleanR>;                                       \
        auto __ptr = __access_traits::template function<__VA_ARGS__>::of(&CleanR::__REW_DEPAREN(name)); \
        auto __meta = rew::find_or_add_function<__VA_ARGS__>(__reflection, name_str, __ptr);            \
        injection.template function<CleanR, decltype(__ptr)>(*__meta);                                  \
    }

#define FUNCTION(name, ...) NAMED_FUNCTION(__REW_TO_STRING(name), name, __VA_ARGS__)

// .function<signature>(name, &func)
#define NAMED_FREE_FUNCTION(name_str, name, ...)                                                        \
    {                                                                                                   \
        using __access_traits = rew::meta::access_traits<>;                                             \
        auto __ptr = __access_traits::template function<__VA_ARGS__>::of(&__REW_DEPAREN(name));         \
        auto __meta = rew::find_or_add_function<__VA_ARGS__>(__reflection, name_str, __ptr);            \
        injection.template function<CleanR, decltype(__ptr)>(*__meta);                                  \
    }

#define FREE_FUNCTION(name, ...) NAMED_FREE_FUNCTION(__REW_TO_STRING(name), name, __VA_ARGS__)

namespace rew
{

struct type_t;

struct function_t
{
    std::string const name;
    std::function<std::any(std::any const& context, std::vector<std::any> const& args)> const call = nullptr;
    std::vector<type_t*> const arguments;
    type_t *const result = nullptr;
    std::any const pointer;
    attribute_t<std::any> meta;
};

namespace detail
{

template <typename ReflectableType, typename ReturnType, typename... ArgumentTypes,
          typename FunctionType, std::size_t... I>
auto handler_member_function_call_impl(FunctionType function, std::index_sequence<I...>)
{
    return [function](std::any const& context, std::vector<std::any> const& arguments) -> std::any
    {
        auto reflectable = std::any_cast<ReflectableType*>(context);
        if constexpr (std::is_void_v<ReturnType>)
        {
            (reflectable->*function)(utility::forward<ArgumentTypes>(arguments[I])...);
            return {};
        }
        else
        {
            return utility::backward
            (
                (reflectable->*function)(utility::forward<ArgumentTypes>(arguments[I])...)
            );
        }
    };
}

template <typename ReturnType, typename... ArgumentTypes, std::size_t... I>
auto handler_free_function_call_impl(ReturnType (*function)(ArgumentTypes...), std::index_sequence<I...>)
{
    return [function](std::any const&, std::vector<std::any> const& arguments) -> std::any
    {
        if constexpr (std::is_void_v<ReturnType>)
        {
            function(utility::forward<ArgumentTypes>(arguments[I])...);
            return {};
        }
        else
        {
            return utility::backward
            (
                function(utility::forward<ArgumentTypes>(arguments[I])...)
            );
        }
    };
}

} // namespace detail

template <typename ReflectableType, typename ReturnType, typename... ArgumentTypes>
auto handler_function_call(ReturnType (ReflectableType::* function)(ArgumentTypes...) const)
{
    return detail::handler_member_function_call_impl<ReflectableType, ReturnType, ArgumentTypes...>
    (
        function, std::index_sequence_for<ArgumentTypes...>{}
    );
}

template <typename ReflectableType, typename ReturnType, typename... ArgumentTypes>
auto handler_function_call(ReturnType (ReflectableType::* function)(ArgumentTypes...) const&)
{
    return detail::handler_member_function_call_impl<ReflectableType, ReturnType, ArgumentTypes...>
    (
        function, std::index_sequence_for<ArgumentTypes...>{}
    );
}

template <typename ReflectableType, typename ReturnType, typename... ArgumentTypes>
auto handler_function_call(ReturnType (ReflectableType::* function)(ArgumentTypes...))
{
    return detail::handler_member_function_call_impl<ReflectableType, ReturnType, ArgumentTypes...>
    (
        function, std::index_sequence_for<ArgumentTypes...>{}
    );
}

template <typename ReflectableType, typename ReturnType, typename... ArgumentTypes>
auto handler_function_call(ReturnType (ReflectableType::* function)(ArgumentTypes...)&)
{
    return detail::handler_member_function_call_impl<ReflectableType, ReturnType, ArgumentTypes...>
    (
        function, std::index_sequence_for<ArgumentTypes...>{}
    );
}

template <typename ReturnType, typename... ArgumentTypes>
auto handler_function_call(ReturnType (*function)(ArgumentTypes...))
{
    return detail::handler_free_function_call_impl(function, std::index_sequence_for<ArgumentTypes...>{});
}

} // namespace rew

// .factory<signature>()
#define FACTORY(...)                                                                                    \
    {                                                                                                   \
        using __traits = rew::meta::function_traits<__VA_ARGS__>;                                       \
        auto __meta = rew::find_or_add_factory<typename __traits::dirty_pointer>(__reflection);         \
        injection.template factory<CleanR, typename __traits::pointer>(*__meta);                        \
    }

namespace rew
{

struct type_t;

struct factory_t
{
    std::string const name;
    std::function<std::any(std::vector<std::any> const& args)> const call = nullptr;
    std::vector<type_t*> const arguments;
    type_t *const result = nullptr;
    attribute_t<std::any> meta;
};

namespace detail
{

template <typename ReflectableType, typename... ArgumentTypes, std::size_t... I>
auto handler_factory_call_impl(std::index_sequence<I...>)
{
    return [](std::vector<std::any> const& arguments) -> std::any
    {
        if constexpr (std::is_aggregate_v<ReflectableType>)
        {
            return ReflectableType{ utility::forward<ArgumentTypes>(arguments[I])... };
        }
        else
        {
            return ReflectableType( utility::forward<ArgumentTypes>(arguments[I])... );
        }
    };
}

} // namespace detail

template <typename ReflectableType, typename... ArgumentTypes>
auto handler_factory_call(ReflectableType (*)(ArgumentTypes...))
{
    return detail::handler_factory_call_impl<ReflectableType, ArgumentTypes...>
    (
        std::index_sequence_for<ArgumentTypes...>{}
    );
}

} // namespace rew

// .property<R,type>(name, &R::get, &R::set)
#define NAMED_PROPERTY(name_str, get, set, ...)                                                         \
    {                                                                                                   \
        using __access = rew::meta::access_traits<CleanR>;                                              \
        auto [__get, __set] = __access::template property<__VA_ARGS__>::of(                             \
            &CleanR::__REW_DEPAREN(get), &CleanR::__REW_DEPAREN(set)                                    \
        );                                                                                              \
        auto __meta = rew::find_or_add_property<__VA_ARGS__>(__reflection, name_str, __get, __set);     \
        injection.template property<CleanR, decltype(__get), decltype(__set)>(*__meta);                 \
    }

#define PROPERTY(name, ...) NAMED_PROPERTY(__REW_TO_STRING(name), name, name, __VA_ARGS__)

// .property<type>(name, &get, &set)
#define NAMED_FREE_PROPERTY(name_str, get, set, ...)                                                    \
    {                                                                                                   \
        using __access = rew::meta::access_traits<>;                                                    \
        auto [__get, __set] = __access::template property<__VA_ARGS__>::of(                             \
            &__REW_DEPAREN(get), &__REW_DEPAREN(set)                                                    \
        );                                                                                              \
        auto __meta = rew::find_or_add_property<__VA_ARGS__>(__reflection, name_str, __get, __set);     \
        injection.template property<CleanR, decltype(__get), decltype(__set)>(*__meta);                 \
    }

#define FREE_PROPERTY(name, ...) NAMED_FREE_PROPERTY(__REW_TO_STRING(name), name, name, __VA_ARGS__)

namespace rew
{

struct type_t;

struct property_t
{
    std::string const name;
    type_t *const type = nullptr;
    std::function<void(std::any const& context, std::any& result)> const get = nullptr;
    std::function<void(std::any const& context, std::any const& value)> const set = nullptr;
    std::function<std::any(std::any const& outer_context)> const context = nullptr;
    std::pair<std::any, std::any> const pointer;
    attribute_t<std::any> meta;
};

namespace detail
{

template <typename ReflectableType, typename GetterType>
auto handler_property_get_impl(GetterType getter)
{
    return [getter](std::any const& context, std::any& value)
    {
        using type = typename meta::property_traits<GetterType>::type;

        value = utility::backward<type>
        (
            (std::any_cast<ReflectableType*>(context)->*getter)()
        );
    };
}

} // namespace detail

template <typename ReflectableType, typename PropertyType>
auto handler_property_get(PropertyType ReflectableType::* property)
{
    return [property](std::any const& context, std::any& result)
    {
        result = utility::forward<PropertyType>
        (
            std::any_cast<ReflectableType*>(context)->*property
        );
    };
}

template <typename ReflectableType, typename PropertyType>
auto handler_property_get(PropertyType (ReflectableType::* getter)(void) const)
{
    return detail::handler_property_get_impl<ReflectableType>(getter);
}

template <typename ReflectableType, typename PropertyType>
auto handler_property_get(PropertyType (ReflectableType::* getter)(void) const&)
{
    return detail::handler_property_get_impl<ReflectableType>(getter);
}

template <typename ReflectableType, typename PropertyType>
auto handler_property_get(PropertyType (ReflectableType::* getter)(void))
{
    return detail::handler_property_get_impl<ReflectableType>(getter);
}

template <typename ReflectableType, typename PropertyType>
auto handler_property_get(PropertyType (ReflectableType::* getter)(void)&)
{
    return detail::handler_property_get_impl<ReflectableType>(getter);
}

template <typename PropertyType>
auto handler_property_get(PropertyType* property)
{
    return [property](std::any const&, std::any& result)
    {
        result = utility::forward<PropertyType>(*property);
    };
}

template <typename PropertyType>
auto handler_property_get(PropertyType (*getter)(void))
{
    return [getter](std::any const&, std::any& result)
    {
        result = utility::backward<PropertyType>(getter());
    };
}

namespace detail
{

template <typename ReflectableType, typename SetterType>
auto handler_property_set_impl(SetterType setter)
{
    return [setter](std::any const& context, std::any const& value)
    {
        using type = typename meta::property_traits<SetterType>::type;

        (std::any_cast<ReflectableType*>(context)->*setter)(utility::forward<type>(value));
    };
}

} // namespace detail

template <typename ReflectableType, typename PropertyType>
auto handler_property_set(PropertyType ReflectableType::* property)
{
    return [property](std::any const& context, std::any const& value)
    {
        std::any_cast<ReflectableType*>(context)->*property = utility::forward<PropertyType>(value);
    };
}

template <typename ReflectableType, typename PropertyType>
auto handler_property_set(PropertyType const ReflectableType::* property)
{
    return nullptr;
}

template <typename ReflectableType, typename PropertyType>
auto handler_property_set(void (ReflectableType::* setter)(PropertyType))
{
    return detail::handler_property_set_impl<ReflectableType>(setter);
}

template <typename ReflectableType, typename PropertyType>
auto handler_property_set(void (ReflectableType::* setter)(PropertyType)&)
{
    return detail::handler_property_set_impl<ReflectableType>(setter);
}

template <typename PropertyType>
auto handler_property_set(PropertyType* property)
{
    return [property](std::any const&, std::any const& value)
    {
        *property = utility::forward<PropertyType>(value);
    };
}

template <typename PropertyType>
auto handler_property_set(PropertyType const* property)
{
    return nullptr;
}

template <typename PropertyType>
auto handler_property_set(void (*setter)(PropertyType))
{
    return [setter](std::any const&, std::any const& value)
    {
        setter(utility::forward<PropertyType>(value));
    };
}

template <typename ReflectableType, typename PropertyType>
auto handler_property_set(PropertyType (ReflectableType::* getter)(void) const)
{
    return nullptr;
}

template <typename ReflectableType, typename PropertyType>
auto handler_property_set(PropertyType (ReflectableType::* getter)(void) const&)
{
    return nullptr;
}

template <typename ReflectableType, typename PropertyType>
auto handler_property_set(PropertyType (ReflectableType::* getter)(void))
{
    return nullptr;
}

template <typename ReflectableType, typename PropertyType>
auto handler_property_set(PropertyType (ReflectableType::* getter)(void)&)
{
    return nullptr;
}

template <typename PropertyType>
auto handler_property_set(PropertyType (*getter)(void))
{
    return nullptr;
}

namespace detail
{

template <typename ReflectableType, typename GetterType>
auto handler_property_context_impl(GetterType getter)
{
    using type = typename meta::property_traits<GetterType>::type;
    if constexpr (std::is_reference_v<type>)
    {
        return [getter](std::any const& outer_context) -> std::any
        {
            return const_cast<meta::to_reflectable_reference_t<type>>
            (
                std::addressof((std::any_cast<ReflectableType*>(outer_context)->*getter)())
            );
        };
    }
    else
    {
        return nullptr;
    }
}

} // namespace detail

template <typename ReflectableType, typename PropertyType>
auto handler_property_context(PropertyType ReflectableType::* property)
{
    return [property](std::any const& outer_context) -> std::any
    {
        return const_cast<meta::to_reflectable_object_t<PropertyType>*>
        (
            std::addressof(std::any_cast<ReflectableType*>(outer_context)->*property)
        );
    };
}

template <typename ReflectableType, typename PropertyType>
auto handler_property_context(PropertyType (ReflectableType::* getter)(void) const)
{
    return detail::handler_property_context_impl<ReflectableType>(getter);
}

template <typename ReflectableType, typename PropertyType>
auto handler_property_context(PropertyType (ReflectableType::* getter)(void) const&)
{
    return detail::handler_property_context_impl<ReflectableType>(getter);
}

template <typename ReflectableType, typename PropertyType>
auto handler_property_context(PropertyType (ReflectableType::* getter)(void))
{
    return detail::handler_property_context_impl<ReflectableType>(getter);
}

template <typename ReflectableType, typename PropertyType>
auto handler_property_context(PropertyType (ReflectableType::* getter)(void)&)
{
    return detail::handler_property_context_impl<ReflectableType>(getter);
}

template <typename PropertyType>
auto handler_property_context(PropertyType* property)
{
    return [property](std::any const&) -> std::any
    {
        return const_cast<meta::to_reflectable_object_t<PropertyType>*>(property);
    };
}

template <typename PropertyType>
auto handler_property_context(PropertyType (*getter)(void))
{
    if constexpr (std::is_reference_v<PropertyType>)
    {
        return [getter](std::any const&) -> std::any
        {
            return const_cast<meta::to_reflectable_reference_t<PropertyType>>
            (
                std::addressof(getter())
            );
        };
    }
    else
    {
        return nullptr;
    }
}

template <typename GetterType, typename SetterType>
constexpr auto property_pointer(GetterType get, SetterType set)
{
    return std::make_pair(get, set);
}

template <typename ReflectableType, typename PropertyType>
constexpr auto property_pointer(PropertyType const ReflectableType::* get, PropertyType const ReflectableType::* set)
{
    return std::make_pair(get, std::any{});
}

template <typename ReflectableType, typename PropertyType>
constexpr auto property_pointer(PropertyType (ReflectableType::* get)(void) const, PropertyType (ReflectableType::* set)(void) const)
{
    return std::make_pair(get, std::any{});
}

template <typename ReflectableType, typename PropertyType>
constexpr auto property_pointer(PropertyType (ReflectableType::* get)(void) const&, PropertyType (ReflectableType::* set)(void) const&)
{
    return std::make_pair(get, std::any{});
}

template <typename ReflectableType, typename PropertyType>
constexpr auto property_pointer(PropertyType (ReflectableType::* get)(void), PropertyType (ReflectableType::* set)(void))
{
    return std::make_pair(get, std::any{});
}

template <typename ReflectableType, typename PropertyType>
constexpr auto property_pointer(PropertyType (ReflectableType::* get)(void)&, PropertyType (ReflectableType::* set)(void)&)
{
    return std::make_pair(get, std::any{});
}

template <typename PropertyType>
constexpr auto property_pointer(PropertyType (*get)(void), PropertyType (*set)(void))
{
    return std::make_pair(get, std::any{});
}

template <typename PropertyType>
constexpr auto property_pointer(PropertyType const* get, PropertyType const* set)
{
    return std::make_pair(get, std::any{});
}

} // namespace rew

template <typename ReflectableType, typename enable = void>
struct __rew;

namespace rew
{

struct reflection_t
{
    std::string const name;

    attribute_t<parent_t> parent;
    attribute_t<attribute_t<function_t>> function;
    attribute_t<factory_t> factory;
    attribute_t<property_t> property;
    attribute_t<std::any> meta;
};

} // namespace rew

namespace rew
{

struct type_t;
struct parent_t;
struct factory_t;
struct function_t;
struct property_t;

struct injectable_t
{
    virtual ~injectable_t() = default;

    template <typename ReflectableType>
    void type(rew::type_t& type) {}

    template <typename ReflectableType, typename ParentReflectableType>
    void parent(rew::parent_t& parent) {}

    template <typename ReflectableType, typename FunctionType>
    void factory(rew::factory_t& factory) {}

    template <typename ReflectableType, typename FunctionType>
    void function(rew::function_t& function) {}

    template <typename ReflectableType, typename GetterType, typename SetterType>
    void property(rew::property_t& property) {}

    template <typename ReflectableType, typename MetaType>
    void meta(std::string const& name, std::any& meta) {}
};

namespace meta
{

template <std::size_t InjectionIndex> struct injection_traits;

} // namespace meta

struct injection_t
{
    std::string const name;
    std::function<void(injectable_t& injection)> const call = nullptr;
};

template <typename ReflectionType, class InjectionType>
auto handler_injection_call()
{
    return [](injectable_t& injection)
    {
        ::__rew<ReflectionType>::evaluate(dynamic_cast<InjectionType&>(injection));
    };
}

} // namespace rew

namespace rew
{

struct reflection_t;

struct type_t
{
    std::string const name;
    reflection_t *const reflection = nullptr;
    std::size_t const size = 0;
    std::function<std::any(std::any& object)> const context = nullptr;

    attribute_t<injection_t> injection;
};

} // namespace rew

#define REW_REGISTRY_RESERVE_SIZE std::size_t(4096)

namespace rew
{

class registry_t
{
public:
    std::unordered_map<std::string, type_t*> all;
    std::unordered_map<std::type_index, type_t*> rtti_all;

public:
    registry_t()
    {
        all.reserve(REW_REGISTRY_RESERVE_SIZE);
    }

    ~registry_t()
    {
        for (auto& [name, meta] : all)
        {
            delete meta->reflection;
            delete meta;
        }
    }

    type_t* find(std::string const& name) const
    {
        auto it = all.find(name);
        return it != all.end() ? it->second : nullptr;
    }

    type_t* find(std::type_index typeindex) const
    {
        auto it = rtti_all.find(typeindex);
        return it != rtti_all.end() ? it->second : nullptr;
    }

    type_t* find(std::any& object) const
    {
        return find(object.type());
    }

    template <typename ReflectableType>
    type_t* find() const
    {
        return find(meta::reflectable_traits<ReflectableType>::name());
    }

private:
    template <typename ReflectableType>
    static auto context()
    {
        return [](std::any& object)
        {
            return std::addressof(std::any_cast<ReflectableType&>(object));
        };
    }

    template <typename ReflectableType>
    static auto size()
    {
        return sizeof(ReflectableType);
    }

public:
    template <typename ReflectableType, typename DirtyReflectableType = ReflectableType>
    type_t* add(std::string const& name)
    {
        auto& type = all[name];
        if (type != nullptr) return type;

        type = new type_t
        {
            name,
            new reflection_t { name },
            size<ReflectableType>(),
            context<ReflectableType>()
        };

        auto& rtti_type = rtti_all[typeid(ReflectableType)];
        if (rtti_type == nullptr) rtti_type = type;

        if constexpr (!std::is_same_v<ReflectableType, DirtyReflectableType>)
        {
            rtti_all.emplace(typeid(DirtyReflectableType), type);
        }

        return type;
    }
};

template <>
inline auto registry_t::context<std::any>()
{
    return [](std::any& object)
    {
        return std::addressof(object);
    };
}

template <>
inline auto registry_t::context<void>()
{
    return nullptr;
}

template <>
inline auto registry_t::size<void>()
{
    return std::size_t(0);
}

inline registry_t global;

} // namespace rew

#define CUSTOM_TEMPLATE_REFLECTABLE_DECLARATION(template_header, ...)                                   \
    namespace rew { namespace meta {                                                                    \
        __REW_DEPAREN(template_header) struct reflectable_traits<__VA_ARGS__> {                         \
            using R = typename rew::meta::reflectable_using<__VA_ARGS__>::R;

#define CUSTOM_CONDITIONAL_REFLECTABLE_DECLARATION(...)                                                 \
    namespace rew { namespace meta {                                                                    \
        template <typename DirtyR> struct reflectable_traits<DirtyR, std::enable_if_t<__VA_ARGS__>> {   \
            using R = typename rew::meta::reflectable_using<DirtyR>::R;                                 \

#define CUSTOM_REFLECTABLE_DECLARATION(...)                                                             \
    namespace rew { namespace meta {                                                                    \
        template <> struct reflectable_traits<__VA_ARGS__> {                                            \
            using R = typename rew::meta::reflectable_using<__VA_ARGS__>::R;

#define TEMPLATE_REFLECTABLE_DECLARATION(template_header, ...)                                          \
    CUSTOM_TEMPLATE_REFLECTABLE_DECLARATION(template_header, __VA_ARGS__)                               \
        LAZY_REFLECTABLE()                                                                              \
        REFLECTABLE_REGISTRY(&rew::global)

#define CONDITIONAL_REFLECTABLE_DECLARATION(...)                                                        \
    CUSTOM_CONDITIONAL_REFLECTABLE_DECLARATION(__VA_ARGS__)                                             \
        LAZY_REFLECTABLE()                                                                              \
        REFLECTABLE_REGISTRY(&rew::global)

#define REFLECTABLE_DECLARATION(...)                                                                    \
    CUSTOM_REFLECTABLE_DECLARATION(__VA_ARGS__)                                                         \
        REFLECTABLE_REGISTRY(&rew::global)                                                              \
        REFLECTABLE_NAME(#__VA_ARGS__)

#define REFLECTABLE_REGISTRY(...)  static auto registry() { return __VA_ARGS__; }
#define REFLECTABLE_NAME(...) static std::string name() { return __VA_ARGS__; }
#define LAZY_REFLECTABLE(...) static auto lazy() { __VA_ARGS__ }
#define BUILTIN_REFLECTABLE(...) static auto builtin() { __VA_ARGS__ }

#define REFLECTABLE_DECLARATION_INIT(...)                                                               \
        };                                                                                              \
    }}

#define __REW_REFLECTABLE_BODY()                                                                        \
    template <class InjectionType>                                                                      \
    static void evaluate(InjectionType&& injection) {                                                   \
        auto __type = rew::find_or_add_type<R>();                                                       \
        auto __reflection = __type->reflection; (void)__reflection;                                     \
        injection.template type<R>(*__type);

#define TEMPLATE_REFLECTABLE(template_header, ...)                                                      \
    __REW_DEPAREN(template_header) struct __rew<__VA_ARGS__> {                                          \
        using R = __VA_ARGS__;                                                                          \
        using CleanR = typename rew::meta::reflectable_traits<R>::R;                                    \
        __REW_REFLECTABLE_BODY()

#define CONDITIONAL_REFLECTABLE(...)                                                                    \
    template <typename R> struct __rew<R, std::enable_if_t<__VA_ARGS__>> {                              \
        using CleanR = typename rew::meta::reflectable_traits<R>::R;                                    \
        __REW_REFLECTABLE_BODY()

#define REFLECTABLE(...)                                                                                \
    template <> struct __rew<__VA_ARGS__> {                                                             \
        using R = __VA_ARGS__;                                                                          \
        using CleanR = typename rew::meta::reflectable_traits<R>::R;                                    \
        __REW_REFLECTABLE_BODY()

#define REFLECTABLE_INIT(...)                                                                           \
            rew::add_default_injection_set<R>(__type);                                                  \
        }                                                                                               \
    private:                                                                                            \
        inline static auto __autogenerated = (evaluate(rew::injectable_t{}), true);                     \
    };

#define REFLECTABLE_INJECTION_DECLARATION(injection_index, ...)                                         \
    namespace rew { namespace meta {                                                                    \
        template <> struct injection_traits<injection_index> { using type = __VA_ARGS__; };             \
    }}                                                                                                  \
    RAW_REFLECTABLE_DECLARATION(__VA_ARGS__)                                                            \
        REFLECTABLE_NAME(#__VA_ARGS__)

#define TEMPLATE_REFLECTABLE_CLEAN(template_header, reflectable_clean, ...)                             \
   namespace rew { namespace meta {                                                                     \
        __REW_DEPAREN(template_header) struct reflectable_using<__REW_DEPAREN(reflectable_clean)> {     \
            using R = __VA_ARGS__;                                                                      \
        };                                                                                              \
    }}

#define REFLECTABLE_CLEAN(reflectable_clean, ...)                                                       \
    namespace rew { namespace meta {                                                                    \
        template <> struct reflectable_using<reflectable_clean> {                                       \
            using R = __VA_ARGS__;                                                                      \
        };                                                                                              \
    }}

#define TEMPLATE_REFLECTABLE_USING(template_header, reflectable_using, reflectable_using_full, ...)     \
    __REW_DEPAREN(template_header)                                                                      \
    struct reflectable_using : rew::meta::reflectable_using_base<__VA_ARGS__> {};                       \
    TEMPLATE_REFLECTABLE_CLEAN(template_header, reflectable_using_full, __VA_ARGS__)

#define REFLECTABLE_USING(reflectable_using, ...)                                                       \
    struct reflectable_using : rew::meta::reflectable_using_base<__VA_ARGS__> {};                       \
    REFLECTABLE_CLEAN(reflectable_using, __VA_ARGS__)

#define REFLECTABLE_ACCESS(...) template <typename, typename> friend struct __rew;

namespace rew
{

template <typename ReflectableType>
auto const nameof()
{
    return meta::reflectable_traits<ReflectableType>::name();
}

template <typename ReflectableType>
using cleanof = meta::clean<ReflectableType>;

template <typename ReflectableType>
void reflectable()
{
    static auto locked = false;
    if (!locked)
    {
        locked = true;
        ::__rew<ReflectableType>::evaluate(injectable_t{});
    }
}

template <typename ReflectableType>
ReflectableType&& reflectable(ReflectableType&& object)
{
    reflectable<std::decay_t<ReflectableType>>();
    return std::forward<ReflectableType>(object);
}

template <typename DirtyReflectableType>
type_t* find_or_add_type()
{
    using dirty_reflectable_type = typename std::conditional_t
    <
        std::is_reference_v<DirtyReflectableType>,
        meta::to_reflectable_reference<DirtyReflectableType>,
        std::conditional_t
        <
            std::is_pointer_v<DirtyReflectableType>,
            meta::to_reflectable_pointer<DirtyReflectableType>,
            meta::to_reflectable_object<DirtyReflectableType>
        >
    >::type;

    using reflectable_type = typename meta::reflectable_using<dirty_reflectable_type>::R;
    using reflectable_traits = meta::reflectable_traits<dirty_reflectable_type>;

    if constexpr (meta::is_lazy_reflectable<dirty_reflectable_type>::value)
    {
        reflectable<dirty_reflectable_type>();
    }

    auto __name = reflectable_traits::name();
    auto __registry = reflectable_traits::registry();

    auto __type = __registry->all[__name];
    if (__type == nullptr)
    {
        __type = __registry->template add<reflectable_type, dirty_reflectable_type>(__name);
    }

    return __type;
}

template <typename ReflectableType, typename ParentReflectableType>
parent_t* find_or_add_parent(reflection_t* reflection)
{
    using reflectable_traits = meta::reflectable_traits<ParentReflectableType>;
    static_assert(std::is_base_of_v<ParentReflectableType, ReflectableType>);

    auto __name = reflectable_traits::name();

    auto __meta = reflection->parent.find(__name);
    if (__meta == nullptr) __meta = &reflection->parent.add
    (
        __name,
        {
            __name,
            find_or_add_type<ParentReflectableType>(),
            handler_parent_cast<ReflectableType, ParentReflectableType>()
        }
    );

    return __meta;
}

namespace detail
{

template <typename... ArgumentTypes, typename ReturnType>
auto function_argument_types(ReturnType (*unused)(ArgumentTypes...))
{
    return std::vector<type_t*>({ find_or_add_type<ArgumentTypes>()... });
}

template <typename... ArgumentTypes, typename ReturnType>
auto function_return_type(ReturnType (*unused)(ArgumentTypes...))
{
    return find_or_add_type<ReturnType>();
}

} // namespace detail

template <typename DirtyFactoryType>
factory_t* find_or_add_factory(reflection_t* reflection)
{
    using function_traits = meta::function_traits<DirtyFactoryType>;
    using dirty_type = typename function_traits::dirty_type;
    using dirty_pointer = typename function_traits::dirty_pointer;
    using pointer = typename function_traits::pointer;

    auto __name = meta::reflectable_traits<dirty_type>::name();

    auto __meta = reflection->factory.find(__name);
    if (__meta == nullptr) __meta = &reflection->factory.add
    (
        __name,
        {
            __name,
            handler_factory_call(pointer{}),
            detail::function_argument_types(dirty_pointer{}),
            detail::function_return_type(dirty_pointer{})
        }
    );

    return __meta;
}

template <typename DirtyFunctionType = void, typename FunctionType>
function_t* find_or_add_function(reflection_t* reflection, std::string const& name, FunctionType ptr)
{
    using function_traits = meta::function_traits
    <
        std::conditional_t<std::is_void_v<DirtyFunctionType>, FunctionType, DirtyFunctionType>
    >;

    using dirty_type = typename function_traits::dirty_type;
    using dirty_pointer = typename function_traits::dirty_pointer;

    auto __function = reflection->function.find(name);
    if (__function == nullptr) __function = &reflection->function.add(name, {});

    auto __overload = meta::reflectable_traits<dirty_type>::name();

    auto __meta = __function->find(__overload);
    if (__meta == nullptr) __meta = &__function->add
    (
        __overload,
        {
            __overload,
            handler_function_call(ptr),
            detail::function_argument_types(dirty_pointer{}),
            detail::function_return_type(dirty_pointer{}),
            ptr
        }
    );

    return __meta;
}

template <typename DirtyPropertyType = void, typename GetterType, typename SetterType>
property_t* find_or_add_property(reflection_t* reflection, std::string const& name,
                                 GetterType getter, SetterType setter)
{
    using property_traits = meta::property_traits
    <
        std::conditional_t<std::is_void_v<DirtyPropertyType>, GetterType, DirtyPropertyType>
    >;

    using type = typename property_traits::type;

    auto __meta = reflection->property.find(name);
    if (__meta == nullptr) __meta = &reflection->property.add
    (
        name,
        {
            name,
            find_or_add_type<type>(),
            handler_property_get(getter),
            handler_property_set(setter),
            handler_property_context(getter),
            property_pointer(getter, setter)
        }
    );

    return __meta;
}

template <typename MetaType>
std::any* find_or_add_meta(reflection_t* reflection, std::string const& name, const MetaType& data)
{
    auto __meta = reflection->meta.find(name);
    if (__meta == nullptr) __meta = &reflection->meta.add(name, data);

    return __meta;
}

template <typename ReflectableType, class InjectionType>
injection_t* find_or_add_injection(type_t* type)
{
    using reflectable_injection_traits = meta::reflectable_traits<InjectionType>;

    auto __name = reflectable_injection_traits::name();

    auto __meta = type->injection.find(__name);
    if (__meta == nullptr) __meta = &type->injection.add
    (
        __name,
        {
            __name,
            handler_injection_call<ReflectableType, InjectionType>()
        }
    );

    return __meta;
}

template <typename ReflectableType, std::size_t CurrentKey = 0, std::size_t MaxKey = 4>
void add_default_injection_set(type_t* type)
{
    if constexpr (meta::is_complete<meta::injection_traits<CurrentKey>>::value)
    {
        using injection_traits = meta::injection_traits<CurrentKey>;

        find_or_add_injection<ReflectableType, typename injection_traits::type>(type);
        if constexpr (CurrentKey < MaxKey)
        {
            add_default_injection_set<ReflectableType, CurrentKey + 1>(type);
        }
    }
}

} // namespace rew

// incomplete type
REFLECTABLE_DECLARATION(void)
    BUILTIN_REFLECTABLE()
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE(void)
REFLECTABLE_INIT()

// pointer type
TEMPLATE_REFLECTABLE_CLEAN(template <typename ElementType>, ElementType*, rew::cleanof<ElementType>*)

TEMPLATE_REFLECTABLE_DECLARATION(template <typename ElementType>, ElementType*)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME(rew::nameof<ElementType>() + "*")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE(template <typename ElementType>, ElementType*)
    FACTORY(R())
    FACTORY(R(R))
REFLECTABLE_INIT()

// nullptr type
REFLECTABLE_DECLARATION(std::nullptr_t)
    BUILTIN_REFLECTABLE()
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE(std::nullptr_t)
REFLECTABLE_INIT()

// qualified types
TEMPLATE_REFLECTABLE_CLEAN(template <typename ElementType>, ElementType&, rew::cleanof<ElementType>&)

TEMPLATE_REFLECTABLE_DECLARATION(template <typename ElementType>, ElementType&)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME(rew::nameof<ElementType>() + "&")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE_CLEAN(template <typename ElementType>, ElementType const, rew::cleanof<ElementType> const)

TEMPLATE_REFLECTABLE_DECLARATION(template <typename ElementType>, ElementType const)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME(rew::nameof<ElementType>() + " const")
REFLECTABLE_DECLARATION_INIT()
// ~ qualified types

// static array type
TEMPLATE_REFLECTABLE_CLEAN((template <typename ElementType, std::size_t N>), ElementType[N], rew::cleanof<ElementType>[N])

TEMPLATE_REFLECTABLE_DECLARATION((template <typename ElementType, std::size_t N>), ElementType[N])
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME(rew::nameof<ElementType>() + "[" + std::to_string(N) + "]")
REFLECTABLE_DECLARATION_INIT()

// static array pointer type
TEMPLATE_REFLECTABLE_DECLARATION((template <typename ElementType, std::size_t N>), ElementType(*)[N])
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME("std::type_identity_t<" + rew::nameof<ElementType[N]>() + ">*")
REFLECTABLE_DECLARATION_INIT()

// static array reference type
TEMPLATE_REFLECTABLE_DECLARATION((template <typename ElementType, std::size_t N>), ElementType(&)[N])
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME("std::type_identity_t<" + rew::nameof<ElementType[N]>() + ">&")
REFLECTABLE_DECLARATION_INIT()

// function types
TEMPLATE_REFLECTABLE_CLEAN
(
    (template <typename ReturnType, typename... ArgumentTypes>),
    (ReturnType(ArgumentTypes...)), rew::cleanof<ReturnType>(rew::cleanof<ArgumentTypes>...)
)

TEMPLATE_REFLECTABLE_DECLARATION(template <typename ReturnType>, ReturnType())
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME(rew::nameof<ReturnType>() + "()")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE_DECLARATION
(
    (template <typename ReturnType, typename ArgumentType, typename... ArgumentTypes>),
    ReturnType(ArgumentType, ArgumentTypes...)
)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME(rew::nameof<ReturnType>() + "(" + ( rew::nameof<ArgumentType>() + ... + (", " + rew::nameof<ArgumentTypes>()) ) + ")")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE_CLEAN
(
    (template <typename ReturnType, typename... ArgumentTypes>),
    (ReturnType(ArgumentTypes...)&), rew::cleanof<ReturnType(ArgumentTypes...)>&
)

TEMPLATE_REFLECTABLE_DECLARATION
(
    (template <typename ReturnType, typename... ArgumentTypes>),
    ReturnType(ArgumentTypes...)&
)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME(rew::nameof<ReturnType(ArgumentTypes...)>() + "&")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE_CLEAN
(
    (template <typename ReturnType, typename... ArgumentTypes>),
    (ReturnType(ArgumentTypes...) const), rew::cleanof<ReturnType(ArgumentTypes...)> const
)

TEMPLATE_REFLECTABLE_DECLARATION
(
    (template <typename ReturnType, typename... ArgumentTypes>),
    ReturnType(ArgumentTypes...) const
)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME(rew::nameof<ReturnType(ArgumentTypes...)>() + " const")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE_CLEAN
(
    (template <typename ReturnType, typename... ArgumentTypes>),
    (ReturnType(ArgumentTypes...) const&), rew::cleanof<ReturnType(ArgumentTypes...)> const&
)

TEMPLATE_REFLECTABLE_DECLARATION
(
    (template <typename ReturnType, typename... ArgumentTypes>),
    ReturnType(ArgumentTypes...) const&
)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME(rew::nameof<ReturnType(ArgumentTypes...)>() + " const&")
REFLECTABLE_DECLARATION_INIT()
// ~ function types

// function pointer type
TEMPLATE_REFLECTABLE_DECLARATION
(
    (template <typename ReturnType, typename... ArgumentTypes>), ReturnType(*)(ArgumentTypes...)
)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME("std::type_identity_t<" + rew::nameof<ReturnType(ArgumentTypes...)>() + ">*")
REFLECTABLE_DECLARATION_INIT()

// function reference type
TEMPLATE_REFLECTABLE_DECLARATION
(
    (template <typename ReturnType, typename... ArgumentTypes>), ReturnType(&)(ArgumentTypes...)
)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME("std::type_identity_t<" + rew::nameof<ReturnType(ArgumentTypes...)>() + ">&")
REFLECTABLE_DECLARATION_INIT()

// aliasing
REFLECTABLE_USING(std_size_t, std::size_t)

CUSTOM_REFLECTABLE_DECLARATION(std_size_t)
    REFLECTABLE_REGISTRY(&rew::global)
    REFLECTABLE_NAME("std::size_t")
    BUILTIN_REFLECTABLE()
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE(std_size_t)
    FACTORY(R())
    FACTORY(R(R))
REFLECTABLE_INIT()

REFLECTABLE_USING(std_ptrdiff_t, std::ptrdiff_t)

CUSTOM_REFLECTABLE_DECLARATION(std_ptrdiff_t)
    REFLECTABLE_REGISTRY(&rew::global)
    REFLECTABLE_NAME("std::ptrdiff_t")
    BUILTIN_REFLECTABLE()
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE(std_ptrdiff_t)
    FACTORY(R())
    FACTORY(R(R))
REFLECTABLE_INIT()
// ~ aliasing

// boolean type
REFLECTABLE_DECLARATION(bool)
    BUILTIN_REFLECTABLE()
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE(bool)
    FACTORY(R())
    FACTORY(R(R))
REFLECTABLE_INIT()
// ~ boolean type

// char types
REFLECTABLE_DECLARATION(char)
    BUILTIN_REFLECTABLE()
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE(char)
    FACTORY(R())
    FACTORY(R(R))
REFLECTABLE_INIT()

REFLECTABLE_DECLARATION(wchar_t)
    BUILTIN_REFLECTABLE()
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE(wchar_t)
    FACTORY(R())
    FACTORY(R(R))
REFLECTABLE_INIT()

#if __cplusplus >= 202002L
REFLECTABLE_DECLARATION(char8_t)
    BUILTIN_REFLECTABLE()
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE(char8_t)
    FACTORY(R())
    FACTORY(R(R))
REFLECTABLE_INIT()
#endif // if

REFLECTABLE_DECLARATION(char16_t)
    BUILTIN_REFLECTABLE()
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE(char16_t)
    FACTORY(R())
    FACTORY(R(R))
REFLECTABLE_INIT()

REFLECTABLE_DECLARATION(char32_t)
    BUILTIN_REFLECTABLE()
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE(char32_t)
    FACTORY(R())
    FACTORY(R(R))
REFLECTABLE_INIT()
// ~ char types

// integral types
REFLECTABLE_DECLARATION(signed char)
    BUILTIN_REFLECTABLE()
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE(signed char)
    FACTORY(R())
    FACTORY(R(R))
REFLECTABLE_INIT()

REFLECTABLE_DECLARATION(unsigned char)
    BUILTIN_REFLECTABLE()
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE(unsigned char)
    FACTORY(R())
    FACTORY(R(R))
REFLECTABLE_INIT()

REFLECTABLE_DECLARATION(int)
    BUILTIN_REFLECTABLE()
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE(int)
    FACTORY(R())
    FACTORY(R(R))
REFLECTABLE_INIT()

REFLECTABLE_DECLARATION(unsigned int)
    BUILTIN_REFLECTABLE()
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE(unsigned int)
    FACTORY(R())
    FACTORY(R(R))
REFLECTABLE_INIT()

REFLECTABLE_DECLARATION(long)
    BUILTIN_REFLECTABLE()
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE(long)
    FACTORY(R())
    FACTORY(R(R))
REFLECTABLE_INIT()

REFLECTABLE_DECLARATION(unsigned long)
    BUILTIN_REFLECTABLE()
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE(unsigned long)
    FACTORY(R())
    FACTORY(R(R))
REFLECTABLE_INIT()

REFLECTABLE_DECLARATION(long long)
    BUILTIN_REFLECTABLE()
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE(long long)
    FACTORY(R())
    FACTORY(R(R))
REFLECTABLE_INIT()

REFLECTABLE_DECLARATION(unsigned long long)
    BUILTIN_REFLECTABLE()
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE(unsigned long long)
    FACTORY(R())
    FACTORY(R(R))
REFLECTABLE_INIT()
// ~ integral types

// floating point types
REFLECTABLE_DECLARATION(float)
    BUILTIN_REFLECTABLE()
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE(float)
    FACTORY(R())
    FACTORY(R(R))
REFLECTABLE_INIT()

REFLECTABLE_DECLARATION(double)
    BUILTIN_REFLECTABLE()
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE(double)
    FACTORY(R())
    FACTORY(R(R))
REFLECTABLE_INIT()

REFLECTABLE_DECLARATION(long double)
    BUILTIN_REFLECTABLE()
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE(long double)
    FACTORY(R())
    FACTORY(R(R))
REFLECTABLE_INIT()
// ~ floating point types

#ifndef REW_CORE_MINIMAL
// as function return type

#include <typeinfo> // type_info

REFLECTABLE_DECLARATION(std::type_info)
    BUILTIN_REFLECTABLE()
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE(std::type_info)
    FUNCTION(operator==)

    #ifndef REW_CORE_MINIMAL
    FUNCTION(before)
    #endif // REW_CRE_MINIMAL

    FUNCTION(hash_code)
    FUNCTION(name)
REFLECTABLE_INIT()

#endif // REW_CORE_MINIMAL

TEMPLATE_REFLECTABLE_CLEAN
(
    (template <typename ReturnType, typename... ArgumentTypes>),
    (std::function<ReturnType(ArgumentTypes...)>), std::function<rew::cleanof<ReturnType(ArgumentTypes...)>>
)

TEMPLATE_REFLECTABLE_DECLARATION
(
    (template <typename ReturnType, typename... ArgumentTypes>),
    std::function<ReturnType(ArgumentTypes...)>
)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME("std::function<" + rew::nameof<ReturnType(ArgumentTypes...)>() + ">")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE
(
    (template <typename ReturnType, typename... ArgumentTypes>),
    std::function<ReturnType(ArgumentTypes...)>
)
    FACTORY(R())
    FACTORY(R(std::nullptr_t))
    FACTORY(R(R const&))
    FUNCTION(operator=, R&(R const&))
    FUNCTION(operator=, R&(std::nullptr_t))

    #ifndef REW_CORE_MINIMAL
    FUNCTION(swap)
    #endif // REW_CORE_MINIMAL

    FUNCTION(operator bool)
    FUNCTION(operator())

    #ifndef REW_CORE_MINIMAL
    FUNCTION(target_type)
    #endif // REW_CORE_MINIMAL
REFLECTABLE_INIT()

TEMPLATE_REFLECTABLE_CLEAN
(
    template <typename ValueType>,
    std::allocator<ValueType>, std::allocator<rew::cleanof<ValueType>>
)

TEMPLATE_REFLECTABLE_DECLARATION(template <typename ValueType>, std::allocator<ValueType>)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME("std::allocator<" + rew::nameof<ValueType>() + ">")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE(template <typename ValueType>, std::allocator<ValueType>)
    FACTORY(R())
    FACTORY(R(R const&))
REFLECTABLE_INIT()

// default allocator for vector

#ifndef REW_CORE_MINIMAL
// as function argument type

#include <initializer_list> // initializer_list

TEMPLATE_REFLECTABLE_CLEAN
(
    template <typename ValueType>,
    std::initializer_list<ValueType>, std::initializer_list<rew::cleanof<ValueType>>
)

TEMPLATE_REFLECTABLE_DECLARATION(template <typename ValueType>, std::initializer_list<ValueType>)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME("std::initializer_list<" + rew::nameof<ValueType>() + ">")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE(template <typename ValueType>, std::initializer_list<ValueType>)
    FACTORY(R())
    FACTORY(R(R const&))
    FUNCTION(operator=, R&(R const&))
    
    #ifndef REW_CORE_MINIMAL
    FUNCTION(begin)
    FUNCTION(end)
    #endif // REW_CORE_MINIMAL

    FUNCTION(size)
REFLECTABLE_INIT()

// as function argument type

TEMPLATE_REFLECTABLE_USING
(
    template <class StdContainerType>, std_const_iterator,
    std_const_iterator<StdContainerType>, typename rew::cleanof<StdContainerType>::const_iterator
)

TEMPLATE_REFLECTABLE_DECLARATION(template <class StdContainerType>, std_const_iterator<StdContainerType>)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME(rew::nameof<StdContainerType>() + "::const_iterator")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE(template <class StdContainerType>, std_const_iterator<StdContainerType>)
REFLECTABLE_INIT()

TEMPLATE_REFLECTABLE_USING
(
    template <class StdContainerType>, std_iterator,
    std_iterator<StdContainerType>, typename rew::cleanof<StdContainerType>::iterator
)

TEMPLATE_REFLECTABLE_DECLARATION(template <class StdContainerType>, std_iterator<StdContainerType>)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME(rew::nameof<StdContainerType>() + "::iterator")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE(template <class StdContainerType>, std_iterator<StdContainerType>)
REFLECTABLE_INIT()

TEMPLATE_REFLECTABLE_USING
(
    template <class StdContainerType>, std_const_reverse_iterator,
    std_const_reverse_iterator<StdContainerType>, typename rew::cleanof<StdContainerType>::const_reverse_iterator
)

TEMPLATE_REFLECTABLE_DECLARATION(template <class StdContainerType>, std_const_reverse_iterator<StdContainerType>)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME(rew::nameof<StdContainerType>() + "::const_reverse_iterator")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE(template <class StdContainerType>, std_const_reverse_iterator<StdContainerType>)
REFLECTABLE_INIT()

TEMPLATE_REFLECTABLE_USING
(
    template <class StdContainerType>, std_reverse_iterator,
    std_reverse_iterator<StdContainerType>, typename rew::cleanof<StdContainerType>::reverse_iterator
)

TEMPLATE_REFLECTABLE_DECLARATION(template <class StdContainerType>, std_reverse_iterator<StdContainerType>)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME(rew::nameof<StdContainerType>() + "::reverse_iterator")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE(template <class StdContainerType>, std_reverse_iterator<StdContainerType>)
REFLECTABLE_INIT()

TEMPLATE_REFLECTABLE_USING
(
    template <class StdContainerType>, std_local_iterator,
    std_local_iterator<StdContainerType>, typename rew::cleanof<StdContainerType>::local_iterator
)

TEMPLATE_REFLECTABLE_DECLARATION(template <class StdContainerType>, std_local_iterator<StdContainerType>)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME(rew::nameof<StdContainerType>() + "::local_iterator")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE(template <class StdContainerType>, std_local_iterator<StdContainerType>)
REFLECTABLE_INIT()

TEMPLATE_REFLECTABLE_USING
(
    template <class StdContainerType>, std_const_local_iterator,
    std_const_local_iterator<StdContainerType>, typename rew::cleanof<StdContainerType>::const_local_iterator
)

TEMPLATE_REFLECTABLE_DECLARATION(template <class StdContainerType>, std_const_local_iterator<StdContainerType>)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME(rew::nameof<StdContainerType>() + "::const_local_iterator")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE(template <class StdContainerType>, std_const_local_iterator<StdContainerType>)
REFLECTABLE_INIT()

#endif // REW_CORE_MINIMAL

TEMPLATE_REFLECTABLE_CLEAN
(
    (template <typename ValueType, typename AllocatorType>),
    (std::vector<ValueType, AllocatorType>), std::vector<rew::cleanof<ValueType>, rew::cleanof<AllocatorType>>
)

TEMPLATE_REFLECTABLE_DECLARATION(template <typename ValueType>, std::vector<ValueType>)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME("std::vector<" + rew::nameof<ValueType>() + ">")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE_DECLARATION
(
    (template <typename ValueType, typename AllocatorType>), std::vector<ValueType, AllocatorType>
)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME("std::vector<" + rew::nameof<ValueType>() + ", " + rew::nameof<AllocatorType>() + ">")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE
(
    (template <typename ValueType, typename AllocatorType>), std::vector<ValueType, AllocatorType>
)
    FACTORY(R())

    #ifndef REW_CORE_MINIMAL
    FACTORY(R(typename R::allocator_type const&))
    FACTORY(R(typename R::size_type, typename R::const_reference, typename R::allocator_type const&))
    FACTORY(R(typename R::size_type, typename R::const_reference))
    FACTORY(R(typename R::size_type, typename R::allocator_type const&))
    FACTORY(R(typename R::size_type))
    FACTORY(R(std_const_iterator<R>, std_const_iterator<R>, typename R::allocator_type const&))
    FACTORY(R(std_const_iterator<R>, std_const_iterator<R>))
    FACTORY(R(R const&,  typename R::allocator_type const&))
    #endif // REW_CORE_MINIMAL

    FACTORY(R(R const&))

    #ifndef REW_CORE_MINIMAL
    FACTORY(R(std::initializer_list<typename R::value_type>, typename R::allocator_type const&))
    FACTORY(R(std::initializer_list<typename R::value_type>))
    #endif // REW_CORE_MINIMAL

    FUNCTION(operator=, R&(R const&))

    #ifndef REW_CORE_MINIMAL
    FUNCTION(operator=, R&(std::initializer_list<typename R::value_type>))
    FUNCTION(assign, void(typename R::size_type, typename R::const_reference))
    FUNCTION(assign, void(std_const_iterator<R>, std_const_iterator<R>))
    FUNCTION(assign, void(std::initializer_list<typename R::value_type>))
    FUNCTION(get_allocator)
    FUNCTION(at, typename R::const_reference(typename R::size_type) const)
    FUNCTION(at, typename R::reference(typename R::size_type))
    #endif // REW_CORE_MINIMAL

    FUNCTION(operator[], typename R::const_reference(typename R::size_type) const)
    FUNCTION(operator[], typename R::reference(typename R::size_type))

    #ifndef REW_CORE_MINIMAL
    FUNCTION(front, typename R::const_reference() const)
    FUNCTION(front, typename R::reference())
    FUNCTION(back, typename R::const_reference() const)
    FUNCTION(back, typename R::reference())
    #endif // REW_CORE_MINIMAL

    FUNCTION(data, typename R::const_pointer() const)
    FUNCTION(data, typename R::pointer())

    #ifndef REW_CORE_MINIMAL
    FUNCTION(begin, std_const_iterator<R>() const)
    FUNCTION(begin, std_iterator<R>())
    FUNCTION(cbegin, std_const_iterator<R>() const)
    FUNCTION(end, std_const_iterator<R>() const)
    FUNCTION(end, std_iterator<R>())
    FUNCTION(cend, std_const_iterator<R>() const)
    FUNCTION(rbegin, std_const_reverse_iterator<R>() const)
    FUNCTION(rbegin, std_reverse_iterator<R>())
    FUNCTION(crbegin, std_const_reverse_iterator<R>() const)
    FUNCTION(rend, std_const_reverse_iterator<R>() const)
    FUNCTION(rend, std_reverse_iterator<R>())
    FUNCTION(crend, std_const_reverse_iterator<R>() const)
    #endif // REW_CORE_MINIMAL

    FUNCTION(empty)
    FUNCTION(size)

    #ifndef REW_CORE_MINIMAL
    FUNCTION(max_size)
    #endif // REW_CORE_MINIMAL

    FUNCTION(capacity)
    FUNCTION(reserve)

    #ifndef REW_CORE_MINIMAL
    FUNCTION(shrink_to_fit)
    #endif // REW_CORE_MINIMAL

    FUNCTION(clear)

    #ifndef REW_CORE_MINIMAL
    FUNCTION(insert, std_iterator<R>(std_const_iterator<R>, typename R::const_reference))
    FUNCTION(insert, std_iterator<R>(std_const_iterator<R>, typename R::size_type, typename R::const_reference))
    FUNCTION(insert, std_iterator<R>(std_const_iterator<R>, std_const_iterator<R>, std_const_iterator<R>))
    FUNCTION(insert, std_iterator<R>(std_const_iterator<R>, std::initializer_list<typename R::value_type>))
    FUNCTION(erase, std_iterator<R>(std_const_iterator<R>))
    FUNCTION(erase, std_iterator<R>(std_const_iterator<R>, std_const_iterator<R>))
    #endif // REW_CORE_MINIMAL

    FUNCTION(push_back, void(typename R::const_reference))
    FUNCTION(pop_back)
    FUNCTION(resize, void(typename R::size_type))

    #ifndef REW_CORE_MINIMAL
    FUNCTION(resize, void(typename R::size_type, typename R::const_reference))
    FUNCTION(swap)
    #endif // REW_CORE_MINIMAL
REFLECTABLE_INIT()

TEMPLATE_REFLECTABLE_USING
(
    template <class StdContainer>, std_vectorbool_reference,
    std_vectorbool_reference<StdContainer>, typename rew::cleanof<StdContainer>::reference
)

TEMPLATE_REFLECTABLE_DECLARATION(template <class StdContainer>, std_vectorbool_reference<StdContainer>)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME(rew::nameof<StdContainer>() + "::reference")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE(template <class StdContainer>, std_vectorbool_reference<StdContainer>)
    FUNCTION(operator=, R&(bool))

    #ifndef REW_CORE_MINIMAL
    FUNCTION(operator=, R&(R const&))
    #endif // REW_CORE_MINIMAL

    FUNCTION(operator bool)
    FUNCTION(flip)
REFLECTABLE_INIT()

TEMPLATE_REFLECTABLE(template <typename AllocatorType>, std::vector<bool, AllocatorType>)
    FACTORY(R())

    #ifndef REW_CORE_MINIMAL
    FACTORY(R(typename R::allocator_type const&))
    FACTORY(R(typename R::size_type, typename R::const_reference, typename R::allocator_type const&))
    FACTORY(R(typename R::size_type, typename R::const_reference))
    FACTORY(R(typename R::size_type, typename R::allocator_type const&))
    FACTORY(R(typename R::size_type))
    FACTORY(R(std_const_iterator<R>, std_const_iterator<R>, typename R::allocator_type const&))
    FACTORY(R(std_const_iterator<R>, std_const_iterator<R>))
    FACTORY(R(R const&,  typename R::allocator_type const&))
    #endif // REW_CORE_MINIMAL

    FACTORY(R(R const&))

    #ifndef REW_CORE_MINIMAL
    FACTORY(R(std::initializer_list<typename R::value_type>, typename R::allocator_type const&))
    FACTORY(R(std::initializer_list<typename R::value_type>))
    #endif // REW_CORE_MINIMAL

    FUNCTION(operator=, R&(R const&))

    #ifndef REW_CORE_MINIMAL
    FUNCTION(operator=, R&(std::initializer_list<typename R::value_type>))
    FUNCTION(assign, void(typename R::size_type, typename R::value_type const&))
    FUNCTION(assign, void(std_const_iterator<R>, std_const_iterator<R>))
    FUNCTION(assign, void(std::initializer_list<typename R::value_type>))
    FUNCTION(get_allocator)
    FUNCTION(at, typename R::const_reference(typename R::size_type) const)
    FUNCTION(at, std_vectorbool_reference<R>(typename R::size_type))
    #endif // REW_CORE_MINIMAL

    FUNCTION(operator[], typename R::const_reference(typename R::size_type) const)
    FUNCTION(operator[], std_vectorbool_reference<R>(typename R::size_type))

    #ifndef REW_CORE_MINIMAL
    FUNCTION(front, typename R::const_reference() const)
    FUNCTION(front, std_vectorbool_reference<R>())
    FUNCTION(back, typename R::const_reference() const)
    FUNCTION(back, std_vectorbool_reference<R>())
    #endif // REW_CORE_MINIMAL

    #ifndef REW_CORE_MINIMAL
    FUNCTION(begin, std_const_iterator<R>() const)
    FUNCTION(begin, std_iterator<R>())
    FUNCTION(cbegin, std_const_iterator<R>() const)
    FUNCTION(end, std_const_iterator<R>() const)
    FUNCTION(end, std_iterator<R>())
    FUNCTION(cend, std_const_iterator<R>() const)
    FUNCTION(rbegin, std_const_reverse_iterator<R>() const)
    FUNCTION(rbegin, std_reverse_iterator<R>())
    FUNCTION(crbegin, std_const_reverse_iterator<R>() const)
    FUNCTION(rend, std_const_reverse_iterator<R>() const)
    FUNCTION(rend, std_reverse_iterator<R>())
    FUNCTION(crend, std_const_reverse_iterator<R>() const)
    #endif // REW_CORE_MINIMAL

    FUNCTION(empty)
    FUNCTION(size)

    #ifndef REW_CORE_MINIMAL
    FUNCTION(max_size)
    #endif // REW_CORE_MINIMAL

    FUNCTION(capacity)
    FUNCTION(reserve)

    #ifndef REW_CORE_MINIMAL
    FUNCTION(shrink_to_fit)
    #endif // REW_CORE_MINIMAL

    FUNCTION(clear)

    #ifndef REW_CORE_MINIMAL
    FUNCTION(insert, std_iterator<R>(std_const_iterator<R>, typename R::value_type const&))
    FUNCTION(insert, std_iterator<R>(std_const_iterator<R>, typename R::size_type, typename R::value_type const&))
    FUNCTION(insert, std_iterator<R>(std_const_iterator<R>, std_const_iterator<R>, std_const_iterator<R>))
    FUNCTION(insert, std_iterator<R>(std_const_iterator<R>, std::initializer_list<typename R::value_type>))
    FUNCTION(erase, std_iterator<R>(std_const_iterator<R>))
    FUNCTION(erase, std_iterator<R>(std_const_iterator<R>, std_const_iterator<R>))
    #endif // REW_CORE_MINIMAL

    FUNCTION(push_back, void(typename R::const_reference))
    FUNCTION(pop_back)
//  FUNCTION(resize, void(typename R::size_type))

    #ifndef REW_CORE_MINIMAL
    FUNCTION(resize, void(typename R::size_type, typename R::const_reference))
    FUNCTION(swap, void(R&))
    FUNCTION(swap, void(std_vectorbool_reference<R>, std_vectorbool_reference<R>))
    FUNCTION(flip)
    #endif // REW_CORE_MINIMAL
REFLECTABLE_INIT()

#include <array> // array

#ifndef REW_CORE_MINIMAL
// as function argument type
#endif // REW_CORE_MINIMAL

TEMPLATE_REFLECTABLE_CLEAN
(
    (template <typename ValueType, std::size_t ArraySize>),
    (std::array<ValueType, ArraySize>), std::array<rew::cleanof<ValueType>, ArraySize>
)

TEMPLATE_REFLECTABLE_DECLARATION((template <typename ValueType, std::size_t ArraySize>), std::array<ValueType, ArraySize>)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME("std::array<" + rew::nameof<ValueType>() + ", " + std::to_string(ArraySize) + ">")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE((template <typename ValueType, std::size_t ArraySize>), std::array<ValueType, ArraySize>)
    FACTORY(R())
    FACTORY(R(R const&))
    FUNCTION(operator=, R&(R const&))

    #ifndef REW_CORE_MINIMAL
    FUNCTION(at, typename R::const_reference(typename R::size_type) const)
    FUNCTION(at, typename R::reference(typename R::size_type))
    #endif // REW_CORE_MINIMAL

    FUNCTION(operator[], typename R::const_reference(typename R::size_type) const)
    FUNCTION(operator[], typename R::reference(typename R::size_type))

    #ifndef REW_CORE_MINIMAL
    FUNCTION(front, typename R::const_reference() const)
    FUNCTION(front, typename R::reference())
    FUNCTION(back, typename R::const_reference() const)
    FUNCTION(back, typename R::reference())
    #endif // REW_CORE_MINIMAL

    FUNCTION(data, typename R::const_pointer() const)
    FUNCTION(data, typename R::pointer())

    #ifndef REW_CORE_MINIMAL
    FUNCTION(begin, std_const_iterator<R>() const)
    FUNCTION(begin, std_iterator<R>())
    FUNCTION(cbegin, std_const_iterator<R>() const)
    FUNCTION(end, std_const_iterator<R>() const)
    FUNCTION(end, std_iterator<R>())
    FUNCTION(cend, std_const_iterator<R>() const)
    FUNCTION(rbegin, std_const_reverse_iterator<R>() const)
    FUNCTION(rbegin, std_reverse_iterator<R>())
    FUNCTION(crbegin, std_const_reverse_iterator<R>() const)
    FUNCTION(rend, std_const_reverse_iterator<R>() const)
    FUNCTION(rend, std_reverse_iterator<R>())
    FUNCTION(crend, std_const_reverse_iterator<R>() const)
    #endif // REW_CORE_MINIMAL

    FUNCTION(empty)
    FUNCTION(size)

    #ifndef REW_CORE_MINIMAL
    FUNCTION(max_size)
    FUNCTION(fill)
    FUNCTION(swap)
    #endif // REW_CORE_MINIMAL

    //FREE_FUNCTION(std::operator==, bool(R const&, R const&))
REFLECTABLE_INIT()

// default allocator for basic_string

#ifndef REW_CORE_MINIMAL
// as function argument type
#endif // REW_CORE_MINIMAL

// as traits type

TEMPLATE_REFLECTABLE_CLEAN
(
    template <typename CharType>,
    std::char_traits<CharType>, std::char_traits<rew::cleanof<CharType>>
)

TEMPLATE_REFLECTABLE_DECLARATION(template <typename CharType>, std::char_traits<CharType>)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME("std::char_traits<" + rew::nameof<CharType>() + ">")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE(template <typename CharType>, std::char_traits<CharType>)
    #ifndef REW_CORE_MINIMAL
    FUNCTION(assign, void(typename R::char_type&, typename R::char_type const&))
    FUNCTION(assign, typename R::char_type*(typename R::char_type*, std::size_t, typename R::char_type))
    FUNCTION(eq)
    FUNCTION(lt)
    FUNCTION(move)
    FUNCTION(copy)
    FUNCTION(compare)
    FUNCTION(length)
    FUNCTION(find)
    FUNCTION(to_char_type)
    FUNCTION(to_int_type)
    FUNCTION(eq_int_type)
    FUNCTION(eof)
    FUNCTION(not_eof)
    #endif // REW_CORE_MINIMAL
REFLECTABLE_INIT()

TEMPLATE_REFLECTABLE_CLEAN
(
    (template <typename CharType, typename Traits, typename AllocatorType>),
    (std::basic_string<CharType, Traits, AllocatorType>),
    std::basic_string<rew::cleanof<CharType>, rew::cleanof<Traits>, rew::cleanof<AllocatorType>>
)

TEMPLATE_REFLECTABLE_DECLARATION(template <>, std::string)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME("std::string")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE_DECLARATION(template <>, std::wstring)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME("std::wstring")
REFLECTABLE_DECLARATION_INIT()

#if __cplusplus >= 202002L
TEMPLATE_REFLECTABLE_DECLARATION(template <>, std::u8string)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME("std::u8string")
REFLECTABLE_DECLARATION_INIT()
#endif // if

TEMPLATE_REFLECTABLE_DECLARATION(template <>, std::u16string)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME("std::u16string")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE_DECLARATION(template <>, std::u32string)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME("std::u32string")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE_DECLARATION(template <typename CharType>, std::basic_string<CharType>)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME("std::basic_string<" + rew::nameof<CharType>() + ">")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE_DECLARATION((template <typename CharType, typename Traits>), std::basic_string<CharType, Traits>)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME("std::basic_string<" + rew::nameof<CharType>() + ", " + rew::nameof<Traits>() + ">")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE_DECLARATION
(
    (template <typename CharType, typename Traits, typename AllocatorType>),
    std::basic_string<CharType, Traits, AllocatorType>
)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME("std::basic_string<" + rew::nameof<CharType>() + ", " + rew::nameof<Traits>() + ", " + rew::nameof<AllocatorType>() + ">")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE
(
    (template <typename CharType, typename Traits, typename AllocatorType>),
    std::basic_string<CharType, Traits, AllocatorType>
)
    FACTORY(R())

    #ifndef REW_CORE_MINIMAL
    FACTORY(R(typename R::allocator_type const&))
    FACTORY(R(typename R::size_type, typename R::value_type, typename R::allocator_type const&))
    FACTORY(R(typename R::size_type, typename R::value_type))
    FACTORY(R(R const&, typename R::size_type, typename R::size_type, typename R::allocator_type const&))
    FACTORY(R(R const&, typename R::size_type, typename R::size_type))
    FACTORY(R(typename R::value_type const*, typename R::size_type, typename R::allocator_type const&))
    FACTORY(R(typename R::value_type const*, typename R::size_type))
    FACTORY(R(typename R::value_type const*, typename R::allocator_type const&))
    FACTORY(R(typename R::value_type const*))
    FACTORY(R(std_const_iterator<R>, std_const_iterator<R>, typename R::allocator_type const&))
    FACTORY(R(std_const_iterator<R>, std_const_iterator<R>))
    FACTORY(R(R const&, typename R::allocator_type const&))
    #endif // REW_CORE_MINIMAL

    FACTORY(R(R const&))

    #ifndef REW_CORE_MINIMAL
    FACTORY(R(std::initializer_list<typename R::value_type>, typename R::allocator_type const&))
    FACTORY(R(std::initializer_list<typename R::value_type>))
    #endif // REW_CORE_MINIMAL

    FUNCTION(operator=, R&(R const&))

    #ifndef REW_CORE_MINIMAL
    FUNCTION(operator=, R&(typename R::value_type const*))
    FUNCTION(operator=, R&(typename R::value_type))
    FUNCTION(operator=, R&(std::initializer_list<typename R::value_type>))
    FUNCTION(assign, R&(typename R::size_type, typename R::value_type))
    FUNCTION(assign, R&(R const&))
    FUNCTION(assign, R&(R const&, typename R::size_type, typename R::size_type))
    FUNCTION(assign, R&(typename R::value_type const*, typename R::size_type))
    FUNCTION(assign, R&(typename R::value_type const*))
    FUNCTION(assign, R&(std_const_iterator<R>, std_const_iterator<R>))
    FUNCTION(assign, R&(std::initializer_list<typename R::value_type>))
    FUNCTION(get_allocator)
    FUNCTION(at, typename R::reference(typename R::size_type))
    FUNCTION(at, typename R::const_reference(typename R::size_type) const)
    #endif // REW_CORE_MINIMAL

    FUNCTION(operator[], typename R::reference(typename R::size_type))
    FUNCTION(operator[], typename R::const_reference(typename R::size_type) const)

    #ifndef REW_CORE_MINIMAL
    FUNCTION(front, typename R::reference())
    FUNCTION(front, typename R::const_reference() const)
    FUNCTION(back, typename R::reference())
    FUNCTION(back, typename R::const_reference() const)
    #endif // REW_CORE_MINIMAL

    FUNCTION(data, typename R::value_type const*() const)

#if __cplusplus >= 201703L
    FUNCTION(data, typename R::value_type*())
#endif // if

    #ifndef REW_CORE_MINIMAL
    FUNCTION(c_str)
    FUNCTION(begin, std_const_iterator<R>() const)
    FUNCTION(begin, std_iterator<R>())
    FUNCTION(cbegin, std_const_iterator<R>() const)
    FUNCTION(end, std_const_iterator<R>() const)
    FUNCTION(end, std_iterator<R>())
    FUNCTION(cend, std_const_iterator<R>() const)
    FUNCTION(rbegin, std_const_reverse_iterator<R>() const)
    FUNCTION(rbegin, std_reverse_iterator<R>())
    FUNCTION(crbegin, std_const_reverse_iterator<R>() const)
    FUNCTION(rend, std_const_reverse_iterator<R>() const)
    FUNCTION(rend, std_reverse_iterator<R>())
    FUNCTION(crend, std_const_reverse_iterator<R>() const)
    #endif // REW_CORE_MINIMAL

    FUNCTION(empty)
    FUNCTION(size)

    #ifndef REW_CORE_MINIMAL
    FUNCTION(length)
    FUNCTION(max_size)
    #endif // REW_CORE_MINIMAL

    FUNCTION(reserve, void(typename R::size_type))
    FUNCTION(capacity)

    #ifndef REW_CORE_MINIMAL
    FUNCTION(shrink_to_fit)
    #endif // REW_CORE_MINIMAL

    FUNCTION(clear)

    #ifndef REW_CORE_MINIMAL
    FUNCTION(insert, R&(typename R::size_type, typename R::size_type, typename R::value_type))
    FUNCTION(insert, R&(typename R::size_type, typename R::value_type const*))
    FUNCTION(insert, R&(typename R::size_type, typename R::value_type const*, typename R::size_type))
    FUNCTION(insert, R&(typename R::size_type, R const&))
    FUNCTION(insert, R&(typename R::size_type, R const&, typename R::size_type, typename R::size_type))
    FUNCTION(insert, std_iterator<R>(std_const_iterator<R>, typename R::value_type))
    FUNCTION(insert, std_iterator<R>(std_const_iterator<R>, typename R::size_type, typename R::value_type))
    FUNCTION(insert, std_iterator<R>(std_const_iterator<R>, std::initializer_list<typename R::value_type>))
    FUNCTION(erase, R&(typename R::size_type, typename R::size_type))
    FUNCTION(erase, std_iterator<R>(std_const_iterator<R>))
    FUNCTION(erase, std_iterator<R>(std_const_iterator<R>, std_const_iterator<R>))
    #endif // REW_CORE_MINIMAL

    FUNCTION(push_back)
    FUNCTION(pop_back)

    #ifndef REW_CORE_MINIMAL
    FUNCTION(append, R&(typename R::size_type, typename R::value_type))
    FUNCTION(append, R&(R const&))
    FUNCTION(append, R&(R const&, typename R::size_type, typename R::size_type))
    FUNCTION(append, R&(typename R::value_type const*, typename R::size_type))
    FUNCTION(append, R&(typename R::value_type const*))
    FUNCTION(append, R&(std_const_iterator<R>, std_const_iterator<R>))
    FUNCTION(append, R&(std::initializer_list<typename R::value_type>))
    FUNCTION(operator+=, R&(R const&))
    FUNCTION(operator+=, R&(typename R::value_type))
    FUNCTION(operator+=, R&(typename R::value_type const*))
    FUNCTION(operator+=, R&(std::initializer_list<typename R::value_type>))
    #endif // REW_CORE_MINIMAL

    FUNCTION(replace, R&(typename R::size_type, typename R::size_type, R const&))

    #ifndef REW_CORE_MINIMAL
    FUNCTION(replace, R&(std_const_iterator<R>, std_const_iterator<R>, R const&))
    FUNCTION(replace, R&(typename R::size_type, typename R::size_type, R const&, typename R::size_type, typename R::size_type))
    FUNCTION(replace, R&(typename R::size_type, typename R::size_type, typename R::value_type const*, typename R::size_type))
    FUNCTION(replace, R&(std_const_iterator<R>, std_const_iterator<R>, typename R::value_type const*, typename R::size_type))
    FUNCTION(replace, R&(typename R::size_type, typename R::size_type, typename R::value_type const*))
    FUNCTION(replace, R&(std_const_iterator<R>, std_const_iterator<R>, typename R::value_type const*))
    FUNCTION(replace, R&(typename R::size_type, typename R::size_type, typename R::size_type, typename R::value_type))
    FUNCTION(replace, R&(std_const_iterator<R>, std_const_iterator<R>, typename R::size_type, typename R::value_type))
    FUNCTION(replace, R&(std_const_iterator<R>, std_const_iterator<R>, std_const_iterator<R>, std_const_iterator<R>))
    FUNCTION(replace, R&(std_const_iterator<R>, std_const_iterator<R>, std::initializer_list<typename R::value_type>))
    FUNCTION(copy)
    #endif // REW_CORE_MINIMAL

    FUNCTION(resize, void(typename R::size_type))

    #ifndef REW_CORE_MINIMAL
    FUNCTION(resize, void(typename R::size_type, typename R::value_type))
    FUNCTION(swap)
    #endif // REW_CORE_MINIMAL
    
    FUNCTION(find, typename R::size_type(R const&, typename R::size_type) const)

    #ifndef REW_CORE_MINIMAL
    FUNCTION(find, typename R::size_type(typename R::value_type const*, typename R::size_type, typename R::size_type) const)
    FUNCTION(find, typename R::size_type(typename R::value_type const*, typename R::size_type) const)
    FUNCTION(find, typename R::size_type(typename R::value_type, typename R::size_type) const)
    FUNCTION(rfind, typename R::size_type(R const&, typename R::size_type) const)
    FUNCTION(rfind, typename R::size_type(typename R::value_type const*, typename R::size_type, typename R::size_type) const)
    FUNCTION(rfind, typename R::size_type(typename R::value_type const*, typename R::size_type) const)
    FUNCTION(rfind, typename R::size_type(typename R::value_type, typename R::size_type) const)
    FUNCTION(find_first_of, typename R::size_type(R const&, typename R::size_type) const)
    FUNCTION(find_first_of, typename R::size_type(typename R::value_type const*, typename R::size_type, typename R::size_type) const)
    FUNCTION(find_first_of, typename R::size_type(typename R::value_type const*, typename R::size_type) const)
    FUNCTION(find_first_of, typename R::size_type(typename R::value_type, typename R::size_type) const)
    FUNCTION(find_first_not_of, typename R::size_type(R const&, typename R::size_type) const)
    FUNCTION(find_first_not_of, typename R::size_type(typename R::value_type const*, typename R::size_type, typename R::size_type) const)
    FUNCTION(find_first_not_of, typename R::size_type(typename R::value_type const*, typename R::size_type) const)
    FUNCTION(find_first_not_of, typename R::size_type(typename R::value_type, typename R::size_type) const)
    FUNCTION(find_last_of, typename R::size_type(R const&, typename R::size_type) const)
    FUNCTION(find_last_of, typename R::size_type(typename R::value_type const*, typename R::size_type, typename R::size_type) const)
    FUNCTION(find_last_of, typename R::size_type(typename R::value_type const*, typename R::size_type) const)
    FUNCTION(find_last_of, typename R::size_type(typename R::value_type, typename R::size_type) const)
    FUNCTION(find_last_not_of, typename R::size_type(R const&, typename R::size_type) const)
    FUNCTION(find_last_not_of, typename R::size_type(typename R::value_type const*, typename R::size_type, typename R::size_type) const)
    FUNCTION(find_last_not_of, typename R::size_type(typename R::value_type const*, typename R::size_type) const)
    FUNCTION(find_last_not_of, typename R::size_type(typename R::value_type, typename R::size_type) const)
    #endif // REW_CORE_MINIMAL

    FUNCTION(compare, int(R const&) const)

    #ifndef REW_CORE_MINIMAL
    FUNCTION(compare, int(typename R::size_type, typename R::size_type, R const&) const)
    FUNCTION(compare, int(typename R::size_type, typename R::size_type, R const&, typename R::size_type, typename R::size_type) const)
    FUNCTION(compare, int(typename R::value_type const*) const)
    FUNCTION(compare, int(typename R::size_type, typename R::size_type, typename R::value_type const*) const)
    FUNCTION(compare, int(typename R::size_type, typename R::size_type, typename R::value_type const*, typename R::size_type) const)
    #endif // REW_CORE_MINIMAL

#if __cplusplus >= 202002L
    #ifndef REW_CORE_MINIMAL
    FUNCTION(starts_with, bool(typename R::value_type) const)
    FUNCTION(starts_with, bool(typename R::value_type const*) const)
    FUNCTION(ends_with, bool(typename R::value_type) const)
    FUNCTION(ends_with, bool(typename R::value_type const*) const)
    #endif // REW_CORE_MINIMAL
#endif // if

    FUNCTION(substr, R(typename R::size_type, typename R::size_type) const)
REFLECTABLE_INIT()

TEMPLATE_REFLECTABLE_CLEAN
(
    (template <typename FirstType, typename SecondType>),
    (std::pair<FirstType, SecondType>), std::pair<rew::cleanof<FirstType>, rew::cleanof<SecondType>>
)

TEMPLATE_REFLECTABLE_DECLARATION((template <typename FirstType, typename SecondType>), std::pair<FirstType, SecondType>)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME("std::pair<" + rew::nameof<FirstType>() + ", " + rew::nameof<SecondType>() + ">")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE((template <typename FirstType, typename SecondType>), std::pair<FirstType, SecondType>)
    FACTORY(R())

    #ifndef REW_CORE_MINIMAL
    FACTORY(R(typename R::first_type const&, typename R::second_type const&))
    FACTORY(R(R const&))
    FUNCTION(operator=, R&(R const&))
    FUNCTION(swap, void(R&))
    #endif // REW_CORE_MINIMAL

    PROPERTY(first, typename R::first_type)
    PROPERTY(second, typename R::second_type)
REFLECTABLE_INIT()

#include <tuple> // tuple

TEMPLATE_REFLECTABLE_CLEAN
(
    (template <typename... ArgumentTypes>),
    (std::tuple<ArgumentTypes...>), std::tuple<rew::cleanof<ArgumentTypes>...>
)

TEMPLATE_REFLECTABLE_DECLARATION(template <>, std::tuple<>)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME("std::tuple<>")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE_DECLARATION
(
    (template <typename ArgumentType, typename... ArgumentTypes>),
    std::tuple<ArgumentType, ArgumentTypes...>
)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME("std::tuple<" + ( rew::nameof<ArgumentType>() + ... + (", " + rew::nameof<ArgumentTypes>()) ) + ">")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE((template <typename... ArgumentTypes>), std::tuple<ArgumentTypes...>)
    FACTORY(R())
    FACTORY(R(ArgumentTypes...))
    FACTORY(R(R const&))
    FUNCTION(operator=, R&(R const&))

    #ifndef REW_CORE_MINIMAL
    FUNCTION(swap)
    #endif // REW_CORE_MINIMAL
REFLECTABLE_INIT()

#include <list> // list

// default allocator for list

#ifndef REW_CORE_MINIMAL
// as function argument type
#endif // REW_CORE_MINIMAL

TEMPLATE_REFLECTABLE_CLEAN
(
    (template <typename ValueType, typename AllocatorType>),
    (std::list<ValueType, AllocatorType>), std::list<rew::cleanof<ValueType>, rew::cleanof<AllocatorType>>
)

TEMPLATE_REFLECTABLE_DECLARATION(template <typename ValueType>, std::list<ValueType>)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME("std::list<" + rew::nameof<ValueType>() + ">")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE_DECLARATION((template <typename ValueType, typename AllocatorType>), std::list<ValueType, AllocatorType>)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME("std::list<" + rew::nameof<ValueType>() + ", " + rew::nameof<AllocatorType>() + ">")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE((template <typename ValueType, typename AllocatorType>), std::list<ValueType, AllocatorType>)
    FACTORY(R())

    #ifndef REW_CORE_MIMIMAL
    FACTORY(R(typename R::allocator_type const&))
    FACTORY(R(typename R::size_type, typename R::const_reference, typename R::allocator_type const&))
    FACTORY(R(typename R::size_type, typename R::const_reference))
    FACTORY(R(typename R::size_type, typename R::allocator_type const&))
    FACTORY(R(typename R::size_type))
    FACTORY(R(std_const_iterator<R>, std_const_iterator<R>, typename R::allocator_type const&))
    FACTORY(R(std_const_iterator<R>, std_const_iterator<R>))
    FACTORY(R(R const&, typename R::allocator_type const&))
    #endif // REW_CORE_MINIMAL

    FACTORY(R(R const&))

    #ifndef REW_CORE_MINIMAL
    FACTORY(R(std::initializer_list<typename R::value_type>, typename R::allocator_type const&))
    FACTORY(R(std::initializer_list<typename R::value_type>))
    #endif // REW_CORE_MINIMAL

    FUNCTION(operator=, R&(R const&))

    #ifndef REW_CORE_MINIMAL
    FUNCTION(operator=, R&(std::initializer_list<typename R::value_type>))
    FUNCTION(assign, void(typename R::size_type, typename R::const_reference))
    FUNCTION(assign, void(std_const_iterator<R>, std_const_iterator<R>))
    FUNCTION(assign, void(std::initializer_list<typename R::value_type>))
    FUNCTION(get_allocator)
    FUNCTION(front, typename R::const_reference() const)
    FUNCTION(front, typename R::reference())
    FUNCTION(back, typename R::const_reference() const)
    FUNCTION(back, typename R::reference())
    FUNCTION(begin, std_const_iterator<R>() const)
    FUNCTION(begin, std_iterator<R>())
    FUNCTION(cbegin, std_const_iterator<R>() const)
    FUNCTION(end, std_const_iterator<R>() const)
    FUNCTION(end, std_iterator<R>())
    FUNCTION(cend, std_const_iterator<R>() const)
    FUNCTION(rbegin, std_const_reverse_iterator<R>() const)
    FUNCTION(rbegin, std_reverse_iterator<R>())
    FUNCTION(crbegin, std_const_reverse_iterator<R>() const)
    FUNCTION(rend, std_const_reverse_iterator<R>() const)
    FUNCTION(rend, std_reverse_iterator<R>())
    FUNCTION(crend, std_const_reverse_iterator<R>() const)
    #endif // REW_CORE_MINIMAL

    FUNCTION(empty)
    FUNCTION(size)

    #ifndef REW_CORE_MINIMAL
    FUNCTION(max_size)
    #endif // REW_CORE_MINIMAL

    FUNCTION(clear)

    #ifndef REW_CORE_MINIMAL
    FUNCTION(insert, std_iterator<R>(std_const_iterator<R>, typename R::const_reference))
    FUNCTION(insert, std_iterator<R>(std_const_iterator<R>, typename R::size_type, typename R::const_reference))
    FUNCTION(insert, std_iterator<R>(std_const_iterator<R>, std_const_iterator<R>, std_const_iterator<R>))
    FUNCTION(insert, std_iterator<R>(std_const_iterator<R>, std::initializer_list<typename R::value_type>))
    FUNCTION(erase, std_iterator<R>(std_const_iterator<R>))
    FUNCTION(erase, std_iterator<R>(std_const_iterator<R>, std_const_iterator<R>))
    #endif // REW_CORE_MINIMAL

    FUNCTION(push_back, void(typename R::const_reference))
    FUNCTION(push_front, void(typename R::const_reference))
    FUNCTION(pop_back)
    FUNCTION(pop_front)
    FUNCTION(resize, void(typename R::size_type))

    #ifndef REW_CORE_MINIMAL
    FUNCTION(resize, void(typename R::size_type, typename R::const_reference))
    FUNCTION(swap)
    #endif // REW_CORE_MINIMAL

    FUNCTION(merge, void(R&))

    #ifndef REW_CORE_MINIMAL
    FUNCTION(merge, void(R&, std::function<bool(typename R::const_reference, typename R::const_reference)>))
    FUNCTION(splice, void(std_const_iterator<R>, R&))
    FUNCTION(splice, void(std_const_iterator<R>, R&, std_const_iterator<R>))
    FUNCTION(splice, void(std_const_iterator<R>, R&, std_const_iterator<R>, std_const_iterator<R>))
    #endif // REW_CORE_MINIMAL

#if __cplusplus < 202002L
    FUNCTION(remove, void(typename R::const_reference))

    #ifndef REW_CORE_MINIMAL
    FUNCTION(remove_if, void(std::function<bool(typename R::const_reference)>))
    #endif // REW_CORE_MINIMAL

    FUNCTION(unique, void())

    #ifndef REW_CORE_MINIMAL
    FUNCTION(unique, void(std::function<bool(typename R::const_reference, typename R::const_reference)>))
    #endif // REW_CORE_MINIMAL
#else
    FUNCTION(remove, typename R::size_type(typename R::const_reference))

    #ifndef REW_CORE_MINIMAL
    FUNCTION(remove_if, typename R::size_type(std::function<bool(typename R::const_reference)>))
    #endif // REW_CORE_MINIMAL

    FUNCTION(unique, typename R::size_type())

    #ifndef REW_CORE_MINIMAL
    FUNCTION(unique, typename R::size_type(std::function<bool(typename R::const_reference, typename R::const_reference)>))
    #endif // REW_CORE_MINIMAL
#endif // if

    FUNCTION(reverse)
    FUNCTION(sort, void())

    #ifndef REW_CORE_MINIMAL
    FUNCTION(sort, void(std::function<bool(typename R::const_reference, typename R::const_reference)>))
    #endif // REW_CORE_MINIMAL
REFLECTABLE_INIT()

#include <forward_list> // forward_list

// default allocator for forward_list

#ifndef REW_CORE_MINIMAL
// as function argument type
#endif // REW_CORE_MINIMAL

TEMPLATE_REFLECTABLE_CLEAN
(
    (template <typename ValueType, typename AllocatorType>),
    (std::forward_list<ValueType, AllocatorType>), std::forward_list<rew::cleanof<ValueType>, rew::cleanof<AllocatorType>>
)

TEMPLATE_REFLECTABLE_DECLARATION(template <typename ValueType>, std::forward_list<ValueType>)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME("std::forward_list<" + rew::nameof<ValueType>() + ">")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE_DECLARATION
(
    (template <typename ValueType, typename AllocatorType>), std::forward_list<ValueType, AllocatorType>
)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME("std::forward_list<" + rew::nameof<ValueType>() + ", " + rew::nameof<AllocatorType>() + ">")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE((template <typename ValueType, typename AllocatorType>), std::forward_list<ValueType, AllocatorType>)
    FACTORY(R())

    #ifndef REW_CORE_MINIMAL
    FACTORY(R(typename R::allocator_type const&))
    FACTORY(R(typename R::size_type, typename R::const_reference, typename R::allocator_type const&))
    FACTORY(R(typename R::size_type, typename R::const_reference))
    FACTORY(R(typename R::size_type, typename R::allocator_type const&))
    FACTORY(R(typename R::size_type))
    FACTORY(R(std_const_iterator<R>, std_const_iterator<R>, typename R::allocator_type const&))
    FACTORY(R(std_const_iterator<R>, std_const_iterator<R>))
    FACTORY(R(R const&, typename R::allocator_type const&))
    #endif // REW_CORE_MINIMAL

    FACTORY(R(R const&))

    #ifndef REW_CORE_MINIMAL
    FACTORY(R(std::initializer_list<typename R::value_type>, typename R::allocator_type const&))
    FACTORY(R(std::initializer_list<typename R::value_type>))
    #endif // REW_CORE_MINIMAL

    FUNCTION(operator=, R&(R const&))

    #ifndef REW_CORE_MINIMAL
    FUNCTION(operator=, R&(std::initializer_list<typename R::value_type>))
    FUNCTION(assign, void(typename R::size_type, typename R::const_reference))
    FUNCTION(assign, void(std_const_iterator<R>, std_const_iterator<R>))
    FUNCTION(assign, void(std::initializer_list<typename R::value_type>))
    FUNCTION(get_allocator)
    FUNCTION(front, typename R::const_reference() const)
    FUNCTION(front, typename R::reference())
    FUNCTION(before_begin, std_const_iterator<R>() const)
    FUNCTION(before_begin, std_iterator<R>())
    FUNCTION(cbefore_begin, std_const_iterator<R>() const)
    FUNCTION(begin, std_const_iterator<R>() const)
    FUNCTION(begin, std_iterator<R>())
    FUNCTION(cbegin, std_const_iterator<R>() const)
    FUNCTION(end, std_const_iterator<R>() const)
    FUNCTION(end, std_iterator<R>())
    FUNCTION(cend, std_const_iterator<R>() const)
    #endif // REW_CORE_MINIMAL

    FUNCTION(empty)

    #ifndef REW_CORE_MINIMAL
    FUNCTION(max_size)
    #endif // REW_CORE_MINIMAL

    FUNCTION(clear)

    #ifndef REW_CORE_MINIMAL
    FUNCTION(insert_after, std_iterator<R>(std_const_iterator<R>, typename R::const_reference))
    FUNCTION(insert_after, std_iterator<R>(std_const_iterator<R>, typename R::size_type, typename R::const_reference))
    FUNCTION(insert_after, std_iterator<R>(std_const_iterator<R>, std_const_iterator<R>, std_const_iterator<R>))
    FUNCTION(insert_after, std_iterator<R>(std_const_iterator<R>, std::initializer_list<typename R::value_type>))
    FUNCTION(erase_after, std_iterator<R>(std_const_iterator<R>))
    FUNCTION(erase_after, std_iterator<R>(std_const_iterator<R>, std_const_iterator<R>))
    #endif // REW_CORE_MINIMAL

    FUNCTION(push_front, void(typename R::const_reference))
    FUNCTION(pop_front)
    FUNCTION(resize, void(typename R::size_type))

    #ifndef REW_CORE_MINIMAL
    FUNCTION(resize, void(typename R::size_type, typename R::const_reference))
    FUNCTION(swap)
    #endif // REW_CORE_MINIMAL

    FUNCTION(merge, void(R&))

    #ifndef REW_CORE_MINIMAL
    FUNCTION(merge, void(R&, std::function<bool(typename R::const_reference, typename R::const_reference)>))
    FUNCTION(splice_after, void(std_const_iterator<R>, R&))
    FUNCTION(splice_after, void(std_const_iterator<R>, R&, std_const_iterator<R>))
    FUNCTION(splice_after, void(std_const_iterator<R>, R&, std_const_iterator<R>, std_const_iterator<R>))
    #endif // REW_CORE_MINIMAL

#if __cplusplus < 202002L
    FUNCTION(remove, void(typename R::const_reference))

    #ifndef REW_CORE_MINIMAL
    FUNCTION(remove_if, void(std::function<bool(typename R::const_reference)>))
    #endif // REW_CORE_MINIMAL

    FUNCTION(unique, void())

    #ifndef REW_CORE_MINIMAL
    FUNCTION(unique, void(std::function<bool(typename R::const_reference, typename R::const_reference)>))
    #endif // REW_CORE_MINIMAL
#else
    FUNCTION(remove, typename R::size_type(typename R::const_reference))

    #ifndef REW_CORE_MINIMAL
    FUNCTION(remove_if, typename R::size_type(std::function<bool(typename R::const_reference)>))
    #endif // REW_CORE_MINIMAL

    FUNCTION(unique, typename R::size_type())

    #ifndef REW_CORE_MINIMAL
    FUNCTION(unique, typename R::size_type(std::function<bool(typename R::const_reference, typename R::const_reference)>))
    #endif // REW_CORE_MINIMAL
#endif // if

    FUNCTION(reverse)
    FUNCTION(sort, void())

    #ifndef REW_CORE_MINIMAL
    FUNCTION(sort, void(std::function<bool(typename R::const_reference, typename R::const_reference)>))
    #endif // REW_CORE_MINIMAL
REFLECTABLE_INIT()

#include <set> // set, multiset

// default allocator for set, multiset

#ifndef REW_CORE_MINIMAL
// as function argument/result type
#endif // REW_CORE_MINIMAL

//less - as comparator

TEMPLATE_REFLECTABLE_CLEAN
(
    template <typename KeyType>,
    std::hash<KeyType>, std::hash<rew::cleanof<KeyType>>
)

TEMPLATE_REFLECTABLE_DECLARATION(template <typename KeyType>, std::hash<KeyType>)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME("std::hash<" + rew::nameof<KeyType>() + ">")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE(template <typename KeyType>, std::hash<KeyType>)
    #ifndef REW_CORE_MINIMAL
    FACTORY(R())
    FACTORY(R(R const&))
    FUNCTION(operator())
    #endif // REW_CORE_MINIMAL
REFLECTABLE_INIT()

TEMPLATE_REFLECTABLE_CLEAN(template <typename T>, std::equal_to<T>, std::equal_to<rew::cleanof<T>>)

TEMPLATE_REFLECTABLE_DECLARATION(template <typename T>, std::equal_to<T>)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME("std::equal_to<" + rew::nameof<T>() + ">")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE(template <typename T>, std::equal_to<T>)
REFLECTABLE_INIT()

TEMPLATE_REFLECTABLE_CLEAN(template <typename T>, std::less<T>, std::less<rew::cleanof<T>>)

TEMPLATE_REFLECTABLE_DECLARATION(template <typename T>, std::less<T>)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME("std::less<" + rew::nameof<T>() + ">")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE(template <typename T>, std::less<T>)
REFLECTABLE_INIT()

TEMPLATE_REFLECTABLE_CLEAN(template <typename T>, std::greater<T>, std::greater<rew::cleanof<T>>)

TEMPLATE_REFLECTABLE_DECLARATION(template <typename T>, std::greater<T>)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME("std::greater<" + rew::nameof<T>() + ">")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE(template <typename T>, std::greater<T>)
REFLECTABLE_INIT()

TEMPLATE_REFLECTABLE_CLEAN
(
    (template <typename KeyType, typename Comparator, typename AllocatorType>),
    (std::set<KeyType, Comparator, AllocatorType>), std::set<rew::cleanof<KeyType>, rew::cleanof<Comparator>, rew::cleanof<AllocatorType>>
)

TEMPLATE_REFLECTABLE_DECLARATION(template <typename KeyType>, std::set<KeyType>)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME("std::set<" + rew::nameof<KeyType>() + ">")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE_DECLARATION
(
    (template <typename KeyType, typename Comparator>),
    std::set<KeyType, Comparator>
)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME("std::set<" + rew::nameof<KeyType>() + ", " + rew::nameof<Comparator>() + ">")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE_DECLARATION
(
    (template <typename KeyType, typename Comparator, typename AllocatorType>),
    std::set<KeyType, Comparator, AllocatorType>
)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME
    (
        "std::set<" + rew::nameof<KeyType>() + ", " + rew::nameof<Comparator>() + ", " + rew::nameof<AllocatorType>() + ">"
    )
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE_CLEAN
(
    (template <typename KeyType, typename Comparator, typename AllocatorType>),
    (std::multiset<KeyType, Comparator, AllocatorType>),
    std::multiset<rew::cleanof<KeyType>, rew::cleanof<Comparator>, rew::cleanof<AllocatorType>>
)

TEMPLATE_REFLECTABLE_DECLARATION(template <typename KeyType>, std::multiset<KeyType>)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME("std::multiset<" + rew::nameof<KeyType>() + ">")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE_DECLARATION
(
    (template <typename KeyType, typename Comparator>),
    std::multiset<KeyType, Comparator>
)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME("std::multiset<" + rew::nameof<KeyType>() + ", " + rew::nameof<Comparator>() + ">")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE_DECLARATION
(
    (template <typename KeyType, typename Comparator, typename AllocatorType>),
    std::multiset<KeyType, Comparator, AllocatorType>
)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME
    (
        "std::multiset<" + rew::nameof<KeyType>() + ", " + rew::nameof<Comparator>() + ", " + rew::nameof<AllocatorType>() + ">"
    )
REFLECTABLE_DECLARATION_INIT()

template <typename> struct __rew_is_any_std_ordered_set : std::false_type {};

template <typename KeyType, typename Comparator, typename AllocatorType>
struct __rew_is_any_std_ordered_set<std::set<KeyType, Comparator, AllocatorType>> : std::true_type {};

template <typename KeyType, typename Comparator, typename AllocatorType>
struct __rew_is_any_std_ordered_set<std::multiset<KeyType, Comparator, AllocatorType>> : std::true_type {};

CONDITIONAL_REFLECTABLE(__rew_is_any_std_ordered_set<R>::value)
    FACTORY(R())

    #ifndef REW_CORE_MINIMAL
    FACTORY(R(typename R::key_compare const&, typename R::allocator_type const&))
    FACTORY(R(typename R::key_compare const&))
    FACTORY(R(typename R::allocator_type const&))
    FACTORY(R(std_const_iterator<R>, std_const_iterator<R>, typename R::key_compare const&, typename R::allocator_type const&))
    FACTORY(R(std_const_iterator<R>, std_const_iterator<R>, typename R::allocator_type const&))
    #endif // REW_CORE_MINIMAL

    FACTORY(R(R const&))

    #ifndef REW_CORE_MINIMAL
    FACTORY(R(R const&, typename R::allocator_type const&))
    FACTORY(R(std::initializer_list<typename R::value_type>, typename R::key_compare const&, typename R::allocator_type const&))
    FACTORY(R(std::initializer_list<typename R::value_type>))
    #endif // REW_CORE_MINIMAL

    FUNCTION(operator=, R&(R const&))

    #ifndef REW_CORE_MINIMAL
    FUNCTION(operator=, R&(std::initializer_list<typename R::value_type>))
    FUNCTION(get_allocator)
    FUNCTION(begin, std_const_iterator<R>() const)
    FUNCTION(cbegin, std_const_iterator<R>() const)
    FUNCTION(end, std_const_iterator<R>() const)
    FUNCTION(cend, std_const_iterator<R>() const)
    FUNCTION(rbegin, std_const_reverse_iterator<R>() const)
    FUNCTION(crbegin, std_const_reverse_iterator<R>() const)
    FUNCTION(rend, std_const_reverse_iterator<R>() const)
    FUNCTION(crend, std_const_reverse_iterator<R>() const)
    #endif // REW_CORE_MINIMAL

    FUNCTION(empty)
    FUNCTION(size)

    #ifndef REW_CORE_MINIMAL
    FUNCTION(max_size)
    #endif // REW_CORE_MINIMAL

    FUNCTION(clear)

    #ifndef REW_CORE_MINIMAL
    FUNCTION(insert, std::pair<std_iterator<R>, bool>(typename R::const_reference))
    FUNCTION(insert, std_iterator<R>(std_iterator<R>, typename R::const_reference))
    FUNCTION(insert, void(std_const_iterator<R>, std_const_iterator<R>))
    FUNCTION(insert, void(std::initializer_list<typename R::value_type>))
    FUNCTION(erase, std_iterator<R>(std_iterator<R>))
    FUNCTION(erase, std_iterator<R>(std_const_iterator<R>))
    FUNCTION(erase, std_iterator<R>(std_const_iterator<R>, std_const_iterator<R>))
    FUNCTION(swap)
//  FUNCTION(extract, typename R::node_type(std_const_iterator<R>))
//  FUNCTION(extract, typename R::node_type(typename R::key_type const&))
    #endif // REW_CORE_MINIMAL

    FUNCTION(merge, void(R&))
    FUNCTION(count, typename R::size_type(typename R::key_type const&) const)

    #ifndef REW_CORE_MINIMAL
    FUNCTION(find, std_iterator<R>(typename R::key_type const&))
    FUNCTION(find, std_const_iterator<R>(typename R::key_type const&) const)
    #endif // REW_CORE_MINIMAL

#if __cplusplus >= 202002L
    FUNCTION(contains, bool(typename R::key_type const&) const)
#endif // if

    #ifndef REW_CORE_MINIMAL
    FUNCTION(equal_range, std::pair<std_iterator<R>, std_iterator<R>>(typename R::key_type const&))
    FUNCTION(equal_range, std::pair<std_const_iterator<R>, std_const_iterator<R>>(typename R::key_type const&) const)
    FUNCTION(lower_bound, std_iterator<R>(typename R::key_type const&))
    FUNCTION(lower_bound, std_const_iterator<R>(typename R::key_type const&) const)
    FUNCTION(upper_bound, std_iterator<R>(typename R::key_type const&))
    FUNCTION(upper_bound, std_const_iterator<R>(typename R::key_type const&) const)
    FUNCTION(key_comp)
//  FUNCTION(value_comp)
    #endif // REW_CORE_MINIMAL
REFLECTABLE_INIT()

#include <unordered_set> // unordered_set, unordered_multiset

// default allocator for unordered_set, unordered_multiset

#ifndef REW_CORE_MINIMAL
// as function argument/result type
#endif // REW_CORE_MINIMAL

// hash - as hasher
// equal_to - as comparator

TEMPLATE_REFLECTABLE_CLEAN
(
    (template <typename KeyType, typename Hasher, typename Comparator, typename AllocatorType>),
    (std::unordered_set<KeyType, Hasher, Comparator, AllocatorType>),
    std::unordered_set<rew::cleanof<KeyType>, rew::cleanof<Hasher>, rew::cleanof<Comparator>, rew::cleanof<AllocatorType>>
)

TEMPLATE_REFLECTABLE_DECLARATION(template <typename KeyType>, std::unordered_set<KeyType>)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME("std::unordered_set<" + rew::nameof<KeyType>() + ">")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE_DECLARATION
(
    (template <typename KeyType, typename Hasher>),
    std::unordered_set<KeyType, Hasher>
)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME("std::unordered_set<" + rew::nameof<KeyType>() + ", " + rew::nameof<Hasher>() + ">")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE_DECLARATION
(
    (template <typename KeyType, typename Hasher, typename Comparator>),
    std::unordered_set<KeyType, Hasher, Comparator>
)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME
    (
        "std::unordered_set<" + rew::nameof<KeyType>() + ", " + rew::nameof<Comparator>() + ", " + rew::nameof<Hasher>() + ">"
    )
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE_DECLARATION
(
    (template <typename KeyType, typename Hasher, typename Comparator, typename AllocatorType>),
    std::unordered_set<KeyType, Hasher, Comparator, AllocatorType>
)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME
    (
        "std::unordered_set<" + rew::nameof<KeyType>() + ", " + rew::nameof<Hasher>() + ", "
                              + rew::nameof<Comparator>() + ", " + rew::nameof<AllocatorType>() + ">"
    )
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE_CLEAN
(
    (template <typename KeyType, typename Hasher, typename Comparator, typename AllocatorType>),
    (std::unordered_multiset<KeyType, Hasher, Comparator, AllocatorType>),
    std::unordered_multiset<rew::cleanof<KeyType>, rew::cleanof<Hasher>, rew::cleanof<Comparator>, rew::cleanof<AllocatorType>>
)

TEMPLATE_REFLECTABLE_DECLARATION(template <typename KeyType>, std::unordered_multiset<KeyType>)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME("std::unordered_multiset<" + rew::nameof<KeyType>() + ">")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE_DECLARATION
(
    (template <typename KeyType, typename Hasher>),
    std::unordered_multiset<KeyType, Hasher>
)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME("std::unordered_set<" + rew::nameof<KeyType>() + ", " + rew::nameof<Hasher>() + ">")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE_DECLARATION
(
    (template <typename KeyType, typename Hasher, typename Comparator>),
    std::unordered_multiset<KeyType, Hasher, Comparator>
)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME
    (
        "std::unordered_multiset<" + rew::nameof<KeyType>() + ", " + rew::nameof<Comparator>() + ", " + rew::nameof<Hasher>() + ">"
    )
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE_DECLARATION
(
    (template <typename KeyType, typename Hasher, typename Comparator, typename AllocatorType>),
    std::unordered_multiset<KeyType, Hasher, Comparator, AllocatorType>
)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME
    (
        "std::unordered_multiset<" + rew::nameof<KeyType>() + ", " + rew::nameof<Hasher>() + ", "
                                   + rew::nameof<Comparator>() + ", " + rew::nameof<AllocatorType>() + ">"
    )
REFLECTABLE_DECLARATION_INIT()

template <typename> struct __rew_is_any_std_unordered_set : std::false_type {};

template <typename KeyType, typename Hasher, typename Comparator, typename AllocatorType>
struct __rew_is_any_std_unordered_set<std::unordered_set<KeyType, Hasher, Comparator, AllocatorType>> : std::true_type {};

template <typename KeyType, typename Hasher, typename Comparator, typename AllocatorType>
struct __rew_is_any_std_unordered_set<std::unordered_multiset<KeyType, Hasher, Comparator, AllocatorType>> : std::true_type {};

CONDITIONAL_REFLECTABLE(__rew_is_any_std_unordered_set<R>::value)
    FACTORY(R())

    #ifndef REW_CORE_MINIMAL
    FACTORY(R(typename R::size_type, typename R::hasher const&, typename R::key_equal const&, typename R::allocator_type const&))
    FACTORY(R(typename R::size_type, typename R::hasher const&, typename R::key_equal const&))
    FACTORY(R(typename R::size_type, typename R::hasher const&))
    FACTORY(R(typename R::size_type))
    FACTORY(R(typename R::size_type, typename R::allocator_type const&))
    FACTORY(R(typename R::size_type, typename R::hasher const&, typename R::allocator_type const&))
    FACTORY(R(std_const_iterator<R>, std_const_iterator<R>, typename R::size_type, typename R::hasher const&, typename R::key_equal const&, typename R::allocator_type const&))
    FACTORY(R(std_const_iterator<R>, std_const_iterator<R>, typename R::size_type, typename R::hasher const&, typename R::key_equal const&))
    FACTORY(R(std_const_iterator<R>, std_const_iterator<R>, typename R::size_type, typename R::hasher const&))
    FACTORY(R(std_const_iterator<R>, std_const_iterator<R>, typename R::size_type))
    FACTORY(R(std_const_iterator<R>, std_const_iterator<R>, typename R::size_type, typename R::allocator_type const&))
    FACTORY(R(std_const_iterator<R>, std_const_iterator<R>, typename R::size_type, typename R::hasher const&, typename R::allocator_type const&))
    FACTORY(R(std::initializer_list<typename R::value_type>, typename R::size_type, typename R::hasher const&, typename R::key_equal const&, typename R::allocator_type const&))
    FACTORY(R(std::initializer_list<typename R::value_type>, typename R::size_type, typename R::hasher const&, typename R::key_equal const&))
    FACTORY(R(std::initializer_list<typename R::value_type>, typename R::size_type, typename R::hasher const&))
    FACTORY(R(std::initializer_list<typename R::value_type>, typename R::size_type))
    FACTORY(R(std::initializer_list<typename R::value_type>, typename R::size_type, typename R::hasher const&, typename R::allocator_type const&))
    #endif // REW_CORE_MINIMAL

    FACTORY(R(R const&))
    FUNCTION(operator=, R&(R const&))

    #ifndef REW_CORE_MINIMAL
    FUNCTION(get_allocator)
    FUNCTION(begin, std_const_iterator<R>() const)
    FUNCTION(begin, std_iterator<R>())
    FUNCTION(cbegin, std_const_iterator<R>() const)
    FUNCTION(end, std_const_iterator<R>() const)
    FUNCTION(end, std_iterator<R>())
    FUNCTION(cend, std_const_iterator<R>() const)
    #endif // REW_CORE_MINIMAL

    FUNCTION(empty)
    FUNCTION(size)

    #ifndef REW_CORE_MINIMAL
    FUNCTION(max_size)
    #endif // REW_CORE_MINIMAL

    FUNCTION(clear)

    #ifndef REW_CORE_MINIMAL
    FUNCTION(insert, std::pair<std_iterator<R>, bool>(typename R::const_reference))
    FUNCTION(insert, std_iterator<R>(std_const_iterator<R>, typename R::const_reference))
    FUNCTION(insert, void(std_const_iterator<R>, std_const_iterator<R>))
    FUNCTION(insert, void(std::initializer_list<typename R::value_type>))
    FUNCTION(erase, std_iterator<R>(std_iterator<R>))
    FUNCTION(erase, std_iterator<R>(std_const_iterator<R>))
    FUNCTION(erase, std_iterator<R>(std_const_iterator<R>, std_const_iterator<R>))
    FUNCTION(swap)
//  FUNCTION(extract, typename R::node_type(std_const_iterator<R>))
//  FUNCTION(extract, typename R::node_type(typename R::key_type const&))
    #endif // REW_CORE_MINIMAL

    FUNCTION(merge, void(R&))
    FUNCTION(count, typename R::size_type(typename R::key_type const&) const)

    #ifndef REW_CORE_MINIMAL
    FUNCTION(find, std_iterator<R>(typename R::key_type const&))
    FUNCTION(find, std_const_iterator<R>(typename R::key_type const&) const)
    #endif // REW_CORE_MINIMAL

#if __cplusplus >= 202002L
    FUNCTION(contains, bool(typename R::key_type const&) const)
#endif // if

    #ifndef REW_CORE_MINIMAL
    FUNCTION(equal_range, std::pair<std_iterator<R>, std_iterator<R>>(typename R::key_type const&))
    FUNCTION(equal_range, std::pair<std_const_iterator<R>, std_const_iterator<R>>(typename R::key_type const&) const)
    FUNCTION(begin, std_const_local_iterator<R>(typename R::size_type) const)
    FUNCTION(begin, std_local_iterator<R>(typename R::size_type))
    FUNCTION(cbegin, std_const_local_iterator<R>(typename R::size_type) const)
    FUNCTION(end, std_const_local_iterator<R>(typename R::size_type) const)
    FUNCTION(end, std_local_iterator<R>(typename R::size_type))
    FUNCTION(cend, std_const_local_iterator<R>(typename R::size_type) const)
    FUNCTION(bucket_count)
    FUNCTION(max_bucket_count)
    FUNCTION(bucket_size)
    FUNCTION(bucket, typename R::size_type(typename R::key_type const&) const)
    FUNCTION(load_factor)
    FUNCTION(max_load_factor, float() const)
    FUNCTION(max_load_factor, void(float))
    #endif // REW_CORE_MINIMAL

    FUNCTION(rehash)
    FUNCTION(reserve)

    #ifndef REW_CORE_MINIMAL
    FUNCTION(hash_function)
    FUNCTION(key_eq)
    #endif // REW_CORE_MINIMAL
REFLECTABLE_INIT()

TEMPLATE_REFLECTABLE_CLEAN
(
    (template <typename ElementType, typename DeleterType>),
    (std::unique_ptr<ElementType, DeleterType>), std::unique_ptr<rew::cleanof<ElementType>, rew::cleanof<DeleterType>>
)

TEMPLATE_REFLECTABLE_DECLARATION(template <typename ElementType>, std::default_delete<ElementType>)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME("std::default_delete<" + rew::nameof<ElementType>() + ">")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE(template <typename ElementType>, std::default_delete<ElementType>)
REFLECTABLE_INIT()

TEMPLATE_REFLECTABLE_DECLARATION(template <typename ElementType>, std::unique_ptr<ElementType>)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME("std::unique_ptr<" + rew::nameof<ElementType>() + ">")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE_DECLARATION
(
    (template <typename ElementType, typename DeleterType>),
    std::unique_ptr<ElementType, DeleterType>
)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME("std::unique_ptr<" + rew::nameof<ElementType>() + ", " + rew::nameof<DeleterType>() + ">")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE
(
    (template <typename ElementType, typename DeleterType>),
    std::unique_ptr<ElementType, DeleterType>
)
    FUNCTION(operator=, R&(std::nullptr_t))
    FUNCTION(release)
    FUNCTION(reset, void(typename R::pointer))

    #ifndef REW_CORE_MINIMAL
    FUNCTION(swap)
    #endif // REW_CORE_MINIMAL

    FUNCTION(get)

    #ifndef REW_CORE_MINIMAL
    FUNCTION(get_deleter, typename R::deleter_type&())
    FUNCTION(get_deleter, typename R::deleter_type const&() const)
    #endif // REM_CORE_MINIMAL

    FUNCTION(operator bool)
    FUNCTION(operator*)

    #ifndef REW_CORE_MINIMAL
    FUNCTION(operator->)
    #endif // REW_CORE_MINIMAL
REFLECTABLE_INIT()

TEMPLATE_REFLECTABLE_CLEAN
(
    template <typename ElementType>,
    std::shared_ptr<ElementType>, std::shared_ptr<rew::cleanof<ElementType>>
)

TEMPLATE_REFLECTABLE_DECLARATION(template <typename ElementType>, std::shared_ptr<ElementType>)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME("std::shared_ptr<" + rew::nameof<ElementType>() + ">")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE(template <typename ElementType>, std::shared_ptr<ElementType>)
    FACTORY(R())

    #ifndef REW_CORE_MINIMAL
    FACTORY(R(std::nullptr_t))
    #endif // REW_CORE_MINIMAL

    FACTORY(R(typename R::element_type*))
    FACTORY(R(R const&))
    FUNCTION(operator=, R&(R const&))
    FUNCTION(reset, void())

    #ifndef REW_CORE_MINIMAL
    FUNCTION(reset, void(typename R::element_type*))
//  FUNCTION(swap, void(R&))
    #endif // REW_CORE_MINIMAL

    FUNCTION(get)
    FUNCTION(operator*)

    #ifndef REW_CORE_MINIMAL
    FUNCTION(operator->)
    FUNCTION(use_count)
    #endif // REW_CORE_MINIMAL

    FUNCTION(operator bool)
REFLECTABLE_INIT()

// as function argument/return type

TEMPLATE_REFLECTABLE_CLEAN
(
    template <typename ElementType>,
    std::weak_ptr<ElementType>, std::weak_ptr<rew::cleanof<ElementType>>
)

TEMPLATE_REFLECTABLE_DECLARATION(template <typename ElementType>, std::weak_ptr<ElementType>)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME("std::weak_ptr<" + rew::nameof<ElementType>() + ">")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE(template <typename ElementType>, std::weak_ptr<ElementType>)
    FACTORY(R())
    FACTORY(R(R const&))
    FACTORY(R(std::shared_ptr<typename R::element_type> const&))
    FUNCTION(operator=, R&(R const&))
    FUNCTION(operator=, R&(std::shared_ptr<typename R::element_type> const&))
    FUNCTION(reset)

    #ifndef REW_CORE_MINIMAL
//  FUNCTION(swap)
    FUNCTION(use_count)
    #endif // REW_CORE_MINIMAL

    FUNCTION(expired)
    FUNCTION(lock)

    #ifndef REW_CORE_MINIMAL
//  FUNCTION(owner_before, bool(R const&) const)
//  FUNCTION(owner_before, bool(std::shared_ptr<typename R::element_type> const&) const)
    #endif // REW_CORE_MINIMAL
REFLECTABLE_INIT()

#include <map> // map, multimap

// default allocator for map, multimap

#ifndef REW_CORE_MINIMAL
// as function argument type
#endif // REW_CORE_MINIMAL

// as value type

//less - as comparator

TEMPLATE_REFLECTABLE_CLEAN
(
    (template <typename KeyType, typename ValueType, typename Comparator, typename AllocatorType>),
    (std::map<KeyType, ValueType, Comparator, AllocatorType>),
    std::map<rew::cleanof<KeyType>, rew::cleanof<ValueType>, rew::cleanof<Comparator>, rew::cleanof<AllocatorType>>
)

TEMPLATE_REFLECTABLE_DECLARATION((template <typename KeyType, typename ValueType>), std::map<KeyType, ValueType>)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME( "std::map<" + rew::nameof<KeyType>() + ", " + rew::nameof<ValueType>() + ">")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE_DECLARATION
(
    (template <typename KeyType, typename ValueType, typename Comparator>),
    std::map<KeyType, ValueType, Comparator>
)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME
    (
        "std::map<" + rew::nameof<KeyType>() + ", " + rew::nameof<ValueType>() + ", " + rew::nameof<Comparator>() + ">"
    )
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE_DECLARATION
(
    (template <typename KeyType, typename ValueType, typename Comparator, typename AllocatorType>),
    std::map<KeyType, ValueType, Comparator, AllocatorType>
)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME
    (
        "std::map<" + rew::nameof<KeyType>() + ", " + rew::nameof<ValueType>() + ", "
                    + rew::nameof<Comparator>() + ", " + rew::nameof<AllocatorType>() + ">"
    )
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE_CLEAN
(
    (template <typename KeyType, typename ValueType, typename Comparator, typename AllocatorType>),
    (std::multimap<KeyType, ValueType, Comparator, AllocatorType>),
    std::multimap<rew::cleanof<KeyType>, rew::cleanof<ValueType>, rew::cleanof<Comparator>, rew::cleanof<AllocatorType>>
)

TEMPLATE_REFLECTABLE_DECLARATION((template <typename KeyType, typename ValueType>), std::multimap<KeyType, ValueType>)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME( "std::multimap<" + rew::nameof<KeyType>() + ", " + rew::nameof<ValueType>() + ">")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE_DECLARATION
(
    (template <typename KeyType, typename ValueType, typename Comparator>),
    std::multimap<KeyType, ValueType, Comparator>
)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME
    (
        "std::multimap<" + rew::nameof<KeyType>() + ", " + rew::nameof<ValueType>() + ", " + rew::nameof<Comparator>() + ">"
    )
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE_DECLARATION
(
    (template <typename KeyType, typename ValueType, typename Comparator, typename AllocatorType>),
    std::multimap<KeyType, ValueType, Comparator, AllocatorType>
)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME
    (
        "std::multimap<" + rew::nameof<KeyType>() + ", " + rew::nameof<ValueType>() + ", "
                         + rew::nameof<Comparator>() + ", " + rew::nameof<AllocatorType>() + ">"
    )
REFLECTABLE_DECLARATION_INIT()

template <typename> struct __rew_is_any_std_ordered_map : std::false_type {};

template <typename KeyType, typename ValueType, typename Comparator, typename AllocatorType>
struct __rew_is_any_std_ordered_map<std::map<KeyType, ValueType, Comparator, AllocatorType>> : std::true_type {};

template <typename KeyType, typename ValueType, typename Comparator, typename AllocatorType>
struct __rew_is_any_std_ordered_map<std::multimap<KeyType, ValueType, Comparator, AllocatorType>> : std::true_type {};

CONDITIONAL_REFLECTABLE(__rew_is_any_std_ordered_map<R>::value)
    FACTORY(R())

    #ifndef REW_CORE_MINIMAL
    FACTORY(R(typename R::key_compare const&, typename R::allocator_type const&))
    FACTORY(R(typename R::key_compare const&))
    FACTORY(R(typename R::allocator_type const&))
    FACTORY(R(std_const_iterator<R>, std_const_iterator<R>, typename R::key_compare const&, typename R::allocator_type const&))
    FACTORY(R(std_const_iterator<R>, std_const_iterator<R>, typename R::allocator_type const&))
    #endif // REW_CORE_MINIMAL
 
    FACTORY(R(R const&))

    #ifndef REW_CORE_MINIMAL
    FACTORY(R(R const&, typename R::allocator_type const&))
    FACTORY(R(std::initializer_list<typename R::value_type>, typename R::key_compare const&, typename R::allocator_type const&))
    FACTORY(R(std::initializer_list<typename R::value_type>))
    #endif // REW_CORE_MINIMAL

    FUNCTION(operator=, R&(R const&))

    #ifndef REW_CORE_MINIMAL
    FUNCTION(operator=, R&(std::initializer_list<typename R::value_type>))
    FUNCTION(get_allocator)
    FUNCTION(at, typename R::mapped_type&(typename R::key_type const&))
    #endif // REW_CORE_MINIMAL

    FUNCTION(at, typename R::mapped_type const&(typename R::key_type const&) const)
    FUNCTION(operator[], typename R::mapped_type&(typename R::key_type const&))

    #ifndef REW_CORE_MINIMAL
    FUNCTION(begin, std_const_iterator<R>() const)
    FUNCTION(begin, std_iterator<R>())
    FUNCTION(cbegin, std_const_iterator<R>() const)
    FUNCTION(end, std_const_iterator<R>() const)
    FUNCTION(end, std_iterator<R>())
    FUNCTION(cend, std_const_iterator<R>() const)
    FUNCTION(rbegin, std_const_reverse_iterator<R>() const)
    FUNCTION(rbegin, std_reverse_iterator<R>())
    FUNCTION(crbegin, std_const_reverse_iterator<R>() const)
    FUNCTION(rend, std_const_reverse_iterator<R>() const)
    FUNCTION(rend, std_reverse_iterator<R>())
    FUNCTION(crend, std_const_reverse_iterator<R>() const)
    #endif // REW_CORE_MINIMAL

    FUNCTION(empty)
    FUNCTION(size)

    #ifndef REW_CORE_MINIMAL
    FUNCTION(max_size)
    #endif // REW_CORE_MINIMAL

    FUNCTION(clear)

    #ifndef REW_CORE_MINIMAL
    FUNCTION(insert, std::pair<std_iterator<R>, bool>(typename R::const_reference))
    FUNCTION(insert, std_iterator<R>(std_const_iterator<R>, typename R::const_reference))
    FUNCTION(insert, void(std_const_iterator<R>, std_const_iterator<R>))
    FUNCTION(insert, void(std::initializer_list<typename R::value_type>))
    FUNCTION(erase, std_iterator<R>(std_iterator<R>))
    FUNCTION(erase, std_iterator<R>(std_const_iterator<R>))
    FUNCTION(erase, std_iterator<R>(std_const_iterator<R>, std_const_iterator<R>))
    FUNCTION(swap)
//  FUNCTION(extract, typename R::node_type(std_const_iterator<R>))
//  FUNCTION(extract, typename R::node_type(typename R::key_type const&))
    #endif // REW_CORE_MINIMAL

    FUNCTION(merge, void(R&))
    FUNCTION(count, typename R::size_type(typename R::key_type const&) const)

    #ifndef REW_CORE_MINIMAL
    FUNCTION(find, std_iterator<R>(typename R::key_type const&))
    FUNCTION(find, std_const_iterator<R>(typename R::key_type const&) const)
    #endif // REW_CORE_MINIMAL

#if __cplusplus >= 202002L
    FUNCTION(contains, bool(typename R::key_type const&) const)
#endif // if

    #ifndef REW_CORE_MINIMAL
    FUNCTION(equal_range, std::pair<std_iterator<R>, std_iterator<R>>(typename R::key_type const&))
    FUNCTION(equal_range, std::pair<std_const_iterator<R>, std_const_iterator<R>>(typename R::key_type const&) const)
    FUNCTION(lower_bound, std_iterator<R>(typename R::key_type const&))
    FUNCTION(lower_bound, std_const_iterator<R>(typename R::key_type const&) const)
    FUNCTION(upper_bound, std_iterator<R>(typename R::key_type const&))
    FUNCTION(upper_bound, std_const_iterator<R>(typename R::key_type const&) const)
    FUNCTION(key_comp)
//  FUNCTION(value_comp)
    #endif // REW_CORE_MINIMAL
REFLECTABLE_INIT()

// default allocator for unordered_map, unordered_multimap

#ifndef REW_CORE_MININAL
// as function argument type

// as value type
#endif // REW_CORE_MININAL

// hash - as hasher
// equal_to - as comparator

TEMPLATE_REFLECTABLE_CLEAN
(
    (template <typename KeyType, typename ValueType, typename Hasher, typename Comparator, typename AllocatorType>),
    (std::unordered_map<KeyType, ValueType, Hasher, Comparator, AllocatorType>),
    std::unordered_map<rew::cleanof<KeyType>, rew::cleanof<ValueType>, rew::cleanof<Hasher>, rew::cleanof<Comparator>, rew::cleanof<AllocatorType>>
)

TEMPLATE_REFLECTABLE_DECLARATION((template <typename KeyType, typename ValueType>), std::unordered_map<KeyType, ValueType>)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME("std::unordered_map<" + rew::nameof<KeyType>() + ", " + rew::nameof<ValueType>() + ">")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE_DECLARATION
(
    (template <typename KeyType, typename ValueType, typename Hasher>),
    std::unordered_map<KeyType, ValueType, Hasher>
)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME("std::unordered_map<" + rew::nameof<KeyType>() + ", " + rew::nameof<ValueType>() + ", " + rew::nameof<Hasher>() + ">")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE_DECLARATION
(
    (template <typename KeyType, typename ValueType, typename Hasher, typename Comparator>),
    std::unordered_map<KeyType, ValueType, Hasher, Comparator>
)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME
    (
        "std::unordered_map<" + rew::nameof<KeyType>() + ", " + rew::nameof<ValueType>() + ", "
                              + rew::nameof<Comparator>() + ", " + rew::nameof<Hasher>() + ">"
    )
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE_DECLARATION
(
    (template <typename KeyType, typename ValueType, typename Hasher, typename Comparator, typename AllocatorType>),
    std::unordered_map<KeyType, ValueType, Hasher, Comparator, AllocatorType>
)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME
    (
        "std::unordered_map<" + rew::nameof<KeyType>() + ", " + rew::nameof<ValueType>() + ", " + rew::nameof<Hasher>() + ", "
                              + rew::nameof<Comparator>() + ", " + rew::nameof<AllocatorType>() + ">"
    )
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE_CLEAN
(
    (template <typename KeyType, typename ValueType, typename Hasher, typename Comparator, typename AllocatorType>),
    (std::unordered_multimap<KeyType, ValueType, Hasher, Comparator, AllocatorType>),
    std::unordered_multimap<rew::cleanof<KeyType>, rew::cleanof<ValueType>, rew::cleanof<Hasher>, rew::cleanof<Comparator>, rew::cleanof<AllocatorType>>
)

TEMPLATE_REFLECTABLE_DECLARATION((template <typename KeyType, typename ValueType>), std::unordered_multimap<KeyType, ValueType>)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME("std::unordered_multimap<" + rew::nameof<KeyType>() + ", " + rew::nameof<ValueType>() + ">")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE_DECLARATION
(
    (template <typename KeyType, typename ValueType, typename Hasher>),
    std::unordered_multimap<KeyType, ValueType, Hasher>
)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME("std::unordered_multimap<" + rew::nameof<KeyType>() + ", " + rew::nameof<ValueType>() + ", " + rew::nameof<Hasher>() + ">")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE_DECLARATION
(
    (template <typename KeyType, typename ValueType, typename Hasher, typename Comparator>),
    std::unordered_multimap<KeyType, ValueType, Hasher, Comparator>
)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME
    (
        "std::unordered_multimap<" + rew::nameof<KeyType>() + ", " + rew::nameof<ValueType>() + ", "
                                   + rew::nameof<Comparator>() + ", " + rew::nameof<Hasher>() + ">"
    )
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE_DECLARATION
(
    (template <typename KeyType, typename ValueType, typename Hasher, typename Comparator, typename AllocatorType>),
    std::unordered_multimap<KeyType, ValueType, Hasher, Comparator, AllocatorType>
)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME
    (
        "std::unordered_multimap<" + rew::nameof<KeyType>() + ", " + rew::nameof<ValueType>() + ", " + rew::nameof<Hasher>() + ", "
                                   + rew::nameof<Comparator>() + ", " + rew::nameof<AllocatorType>() + ">"
    )
REFLECTABLE_DECLARATION_INIT()

template <typename> struct __rew_is_any_std_unordered_map : std::false_type {};

template <typename KeyType, typename Hasher, typename Comparator, typename AllocatorType>
struct __rew_is_any_std_unordered_map<std::unordered_map<KeyType, Hasher, Comparator, AllocatorType>> : std::true_type {};

template <typename KeyType, typename Hasher, typename Comparator, typename AllocatorType>
struct __rew_is_any_std_unordered_map<std::unordered_multimap<KeyType, Hasher, Comparator, AllocatorType>> : std::true_type {};

CONDITIONAL_REFLECTABLE(__rew_is_any_std_unordered_map<R>::value)
    FACTORY(R())

    #ifndef REW_CORE_MINIMAL
    FACTORY(R(typename R::size_type, typename R::hasher const&, typename R::key_equal const&, typename R::allocator_type const&))
    FACTORY(R(typename R::size_type, typename R::hasher const&, typename R::key_equal const&))
    FACTORY(R(typename R::size_type, typename R::hasher const&))
    FACTORY(R(typename R::size_type))
    FACTORY(R(typename R::size_type, typename R::allocator_type const&))
    FACTORY(R(typename R::size_type, typename R::hasher const&, typename R::allocator_type const&))
    FACTORY(R(std_const_iterator<R>, std_const_iterator<R>, typename R::size_type, typename R::hasher const&, typename R::key_equal const&, typename R::allocator_type const&))
    FACTORY(R(std_const_iterator<R>, std_const_iterator<R>, typename R::size_type, typename R::hasher const&, typename R::key_equal const&))
    FACTORY(R(std_const_iterator<R>, std_const_iterator<R>, typename R::size_type, typename R::hasher const&))
    FACTORY(R(std_const_iterator<R>, std_const_iterator<R>, typename R::size_type))
    FACTORY(R(std_const_iterator<R>, std_const_iterator<R>, typename R::size_type, typename R::allocator_type const&))
    FACTORY(R(std_const_iterator<R>, std_const_iterator<R>, typename R::size_type, typename R::hasher const&, typename R::allocator_type const&))
    FACTORY(R(std::initializer_list<typename R::value_type>, typename R::size_type, typename R::hasher const&, typename R::key_equal const&, typename R::allocator_type const&))
    FACTORY(R(std::initializer_list<typename R::value_type>, typename R::size_type, typename R::hasher const&, typename R::key_equal const&))
    FACTORY(R(std::initializer_list<typename R::value_type>, typename R::size_type, typename R::hasher const&))
    FACTORY(R(std::initializer_list<typename R::value_type>, typename R::size_type))
    FACTORY(R(std::initializer_list<typename R::value_type>, typename R::size_type, typename R::hasher const&, typename R::allocator_type const&))
    #endif // REW_CORE_MINIMAL

    FACTORY(R(R const&))
    FUNCTION(operator=, R&(R const&))

    #ifndef REW_CORE_MINIMAL
    FUNCTION(get_allocator)
    FUNCTION(begin, std_const_iterator<R>() const)
    FUNCTION(begin, std_iterator<R>())
    FUNCTION(cbegin, std_const_iterator<R>() const)
    FUNCTION(end, std_const_iterator<R>() const)
    FUNCTION(end, std_iterator<R>())
    FUNCTION(cend, std_const_iterator<R>() const)
    #endif // REW_CORE_MINIMAL

    FUNCTION(empty)
    FUNCTION(size)

    #ifndef REW_CORE_MINIMAL
    FUNCTION(max_size)
    #endif // REW_CORE_MININAL

    FUNCTION(clear)

    #ifndef REW_CORE_MINIMAL
    FUNCTION(insert, std::pair<std_iterator<R>, bool>(typename R::const_reference))
    FUNCTION(insert, std_iterator<R>(std_const_iterator<R>, typename R::const_reference))
    FUNCTION(insert, void(std_const_iterator<R>, std_const_iterator<R>))
    FUNCTION(insert, void(std::initializer_list<typename R::value_type>))
    FUNCTION(erase, std_iterator<R>(std_iterator<R>))
    FUNCTION(erase, std_iterator<R>(std_const_iterator<R>))
    FUNCTION(erase, std_iterator<R>(std_const_iterator<R>, std_const_iterator<R>))
    FUNCTION(swap)
//  FUNCTION(extract, typename R::node_type(std_const_iterator<R>))
//  FUNCTION(extract, typename R::node_type(typename R::key_type const&))
    #endif // REW_CORE_MINIMAL

    FUNCTION(merge, void(R&))

    #ifndef REW_CORE_MINIMAL
    FUNCTION(at, typename R::mapped_type&(typename R::key_type const&))
    #endif // REW_CORE_MINIMAL

    FUNCTION(at, typename R::mapped_type const&(typename R::key_type const&) const)
    FUNCTION(operator[], typename R::mapped_type&(typename R::key_type const&))
    FUNCTION(count, typename R::size_type(typename R::key_type const&) const)

    #ifndef REW_CORE_MINIMAL
    FUNCTION(find, std_iterator<R>(typename R::key_type const&))
    FUNCTION(find, std_const_iterator<R>(typename R::key_type const&) const)
    #endif // REW_CORE_MINIMAL

#if __cplusplus >= 202002L
    FUNCTION(contains, bool(typename R::key_type const&) const)
#endif // if

    #ifndef REW_CORE_MINIMAL
    FUNCTION(equal_range, std::pair<std_iterator<R>, std_iterator<R>>(typename R::key_type const&))
    FUNCTION(equal_range, std::pair<std_const_iterator<R>, std_const_iterator<R>>(typename R::key_type const&) const)
    FUNCTION(begin, std_const_local_iterator<R>(typename R::size_type) const)
    FUNCTION(begin, std_local_iterator<R>(typename R::size_type))
    FUNCTION(cbegin, std_const_local_iterator<R>(typename R::size_type) const)
    FUNCTION(end, std_const_local_iterator<R>(typename R::size_type) const)
    FUNCTION(end, std_local_iterator<R>(typename R::size_type))
    FUNCTION(cend, std_const_local_iterator<R>(typename R::size_type) const)
    FUNCTION(bucket_count)
    FUNCTION(max_bucket_count)
    FUNCTION(bucket_size)
    FUNCTION(bucket, typename R::size_type(typename R::key_type const&) const)
    FUNCTION(load_factor)
    FUNCTION(max_load_factor, float() const)
    FUNCTION(max_load_factor, void(float))
    #endif // REW_CORE_MINIMAL

    FUNCTION(rehash)
    FUNCTION(reserve)

    #ifndef REW_CORE_MINIMAL
    FUNCTION(hash_function)
    FUNCTION(key_eq)
    #endif // REW_CORE_MINIMAL
REFLECTABLE_INIT()

#include <deque> // deque

// default allocator for deque

#ifndef RWW_CORE_MINIMAL
// as function argument type
#endif // REW_CORE_MINIMAL

TEMPLATE_REFLECTABLE_CLEAN
(
    (template <typename ValueType, typename AllocatorType>),
    (std::deque<ValueType, AllocatorType>), std::deque<rew::cleanof<ValueType>, rew::cleanof<AllocatorType>>
)

TEMPLATE_REFLECTABLE_DECLARATION(template <typename ValueType>, std::deque<ValueType>)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME("std::deque<" + rew::nameof<ValueType>() + ">")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE_DECLARATION((template <typename ValueType, typename AllocatorType>), std::deque<ValueType, AllocatorType>)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME("std::deque<" + rew::nameof<ValueType>() + ", " + rew::nameof<AllocatorType>() + ">")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE((template <typename ValueType, typename AllocatorType>), std::deque<ValueType, AllocatorType>)
    FACTORY(R())

    #ifndef REW_CORE_MINIMAL
    FACTORY(R(typename R::allocator_type const&))
    FACTORY(R(typename R::size_type, typename R::const_reference, typename R::allocator_type const&))
    FACTORY(R(typename R::size_type, typename R::const_reference))
    FACTORY(R(typename R::size_type, typename R::allocator_type const&))
    FACTORY(R(typename R::size_type))
    FACTORY(R(std_const_iterator<R>, std_const_iterator<R>, typename R::allocator_type const&))
    FACTORY(R(std_const_iterator<R>, std_const_iterator<R>))
    FACTORY(R(R const&, typename R::allocator_type const&))
    #endif // REW_CORE_MINIMAL

    FACTORY(R(R const&))

    #ifndef REW_CORE_MINIMAL
    FACTORY(R(std::initializer_list<typename R::value_type>, typename R::allocator_type const&))
    FACTORY(R(std::initializer_list<typename R::value_type>))
    #endif // REW_CORE_MINIMAL

    FUNCTION(operator=, R&(R const&))

    #ifndef REW_CORE_MINIMAL
    FUNCTION(operator=, R&(std::initializer_list<typename R::value_type>))
    FUNCTION(assign, void(typename R::size_type, typename R::const_reference))
    FUNCTION(assign, void(std_const_iterator<R>, std_const_iterator<R>))
    FUNCTION(assign, void(std::initializer_list<typename R::value_type>))
    FUNCTION(get_allocator)
    FUNCTION(at, typename R::const_reference(typename R::size_type) const)
    FUNCTION(at, typename R::reference(typename R::size_type))
    #endif // REW_CORE_MINIMAL

    FUNCTION(operator[], typename R::const_reference(typename R::size_type) const)
    FUNCTION(operator[], typename R::reference(typename R::size_type))

    #ifndef REW_CORE_MINIMAL
    FUNCTION(front, typename R::const_reference() const)
    FUNCTION(front, typename R::reference())
    FUNCTION(back, typename R::const_reference() const)
    FUNCTION(back, typename R::reference())
    FUNCTION(begin, std_const_iterator<R>() const)
    FUNCTION(begin, std_iterator<R>())
    FUNCTION(cbegin, std_const_iterator<R>() const)
    FUNCTION(end, std_const_iterator<R>() const)
    FUNCTION(end, std_iterator<R>())
    FUNCTION(cend, std_const_iterator<R>() const)
    FUNCTION(rbegin, std_const_reverse_iterator<R>() const)
    FUNCTION(rbegin, std_reverse_iterator<R>())
    FUNCTION(crbegin, std_const_reverse_iterator<R>() const)
    FUNCTION(rend, std_const_reverse_iterator<R>() const)
    FUNCTION(rend, std_reverse_iterator<R>())
    FUNCTION(crend, std_const_reverse_iterator<R>() const)
    #endif // REW_CORE_MINIMAL

    FUNCTION(empty)
    FUNCTION(size)

    #ifndef REW_CORE_MINIMAL
    FUNCTION(max_size)
    FUNCTION(shrink_to_fit)
    #endif // REW_CORE_MINIMAL

    FUNCTION(clear)

    #ifndef REW_CORE_MINIMAL
    FUNCTION(insert, std_iterator<R>(std_const_iterator<R>, typename R::const_reference))
    FUNCTION(insert, std_iterator<R>(std_const_iterator<R>, typename R::size_type, typename R::const_reference))
    FUNCTION(insert, std_iterator<R>(std_const_iterator<R>, std_const_iterator<R>, std_const_iterator<R>))
    FUNCTION(insert, std_iterator<R>(std_const_iterator<R>, std::initializer_list<typename R::value_type>))
    FUNCTION(erase, std_iterator<R>(std_const_iterator<R>))
    FUNCTION(erase, std_iterator<R>(std_const_iterator<R>, std_const_iterator<R>))
    #endif // REW_CORE_MINIMAL

    FUNCTION(push_back, void(typename R::const_reference))
    FUNCTION(push_front, void(typename R::const_reference))
    FUNCTION(pop_back)
    FUNCTION(pop_front)
    FUNCTION(resize, void(typename R::size_type))

    #ifndef REW_CORE_MINIMAL
    FUNCTION(resize, void(typename R::size_type, typename R::const_reference))
    FUNCTION(swap)
    #endif // REW_CORE_MINIMAL
REFLECTABLE_INIT()

#include <stack> // stack

// default container for stack

TEMPLATE_REFLECTABLE_CLEAN
(
    (template <typename ValueType, class ContainerType>),
    (std::stack<ValueType, ContainerType>), std::stack<rew::cleanof<ValueType>, rew::cleanof<ContainerType>>
)

TEMPLATE_REFLECTABLE_DECLARATION(template <typename ValueType>, std::stack<ValueType>)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME("std::stack<" + rew::nameof<ValueType>() + ">")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE_DECLARATION((template <typename ValueType, class ContainerType>), std::stack<ValueType, ContainerType>)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME("std::stack<" + rew::nameof<ValueType>() + ", " + rew::nameof<ContainerType>() + ">")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE((template <typename ValueType, class ContainerType>), std::stack<ValueType, ContainerType>)
    FACTORY(R())
    FACTORY(R(typename R::container_type const&))
    FACTORY(R(R const&))
    FUNCTION(operator=, R&(R const&))
    FUNCTION(top, typename R::reference())
    FUNCTION(top, typename R::const_reference() const)
    FUNCTION(empty)
    FUNCTION(size)
    FUNCTION(push, void(typename R::const_reference))
    FUNCTION(pop)

    #ifndef REW_CORE_MINIMAL
    FUNCTION(swap)
    #endif // REW_CORE_MINIMAL
REFLECTABLE_INIT()

#include <queue> // queue

// default container for queue

TEMPLATE_REFLECTABLE_CLEAN
(
    (template <typename ValueType, class ContainerType>),
    (std::queue<ValueType, ContainerType>), std::queue<rew::cleanof<ValueType>, rew::cleanof<ContainerType>>
)

TEMPLATE_REFLECTABLE_DECLARATION(template <typename ValueType>, std::queue<ValueType>)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME("std::queue<" + rew::nameof<ValueType>() + ">")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE_DECLARATION((template <typename ValueType, class ContainerType>), std::queue<ValueType, ContainerType>)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME("std::queue<" + rew::nameof<ValueType>() + ", " + rew::nameof<ContainerType>() + ">")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE((template <typename ValueType, class ContainerType>), std::queue<ValueType, ContainerType>)
    FACTORY(R())
    FACTORY(R(typename R::container_type const&))
    FACTORY(R(R const&))
    FUNCTION(operator=, R&(R const&))
    FUNCTION(front, typename R::reference())
    FUNCTION(front, typename R::const_reference() const)
    FUNCTION(back, typename R::reference())
    FUNCTION(back, typename R::const_reference() const)
    FUNCTION(empty)
    FUNCTION(size)
    FUNCTION(push, void(typename R::const_reference))
    FUNCTION(pop)

    #ifndef REW_CORE_MINIMAL
    FUNCTION(swap)
    #endif // REW_CORE_MINIMAL
REFLECTABLE_INIT()

#include <bitset> // bitest

// as function argument tye

TEMPLATE_REFLECTABLE_USING
(
    template <class StdBitsetType>, std_bitset_reference,
    std_bitset_reference<StdBitsetType>, typename rew::cleanof<StdBitsetType>::reference
)

TEMPLATE_REFLECTABLE_DECLARATION(template <class StdBitsetType>, std_bitset_reference<StdBitsetType>)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME(rew::nameof<StdBitsetType>() + "::reference")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE(template <class StdBitsetType>, std_bitset_reference<StdBitsetType>)
    FUNCTION(operator=, R&(bool))

    #ifndef REW_CORE_MINIMAL
    FUNCTION(operator=, R&(R const&))
    #endif // REW_CORE_MINIMAL

    FUNCTION(operator bool)

    #ifndef REW_CORE_MINIMAL
    FUNCTION(operator~)
    #endif // REW_CORE_MINIMAL

    FUNCTION(flip, R&())
REFLECTABLE_INIT()

TEMPLATE_REFLECTABLE_DECLARATION(template <std::size_t BitsetSize>, std::bitset<BitsetSize>)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME("std::bitset<" + std::to_string(BitsetSize) + ">")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE(template <std::size_t BitsetSize>, std::bitset<BitsetSize>)
    FACTORY(R())
    FACTORY(R(R const&))

    #ifndef REW_CORE_MINIMAL
    FACTORY(R(unsigned long))
    #endif // REW_CORE_MINIMAL

    FACTORY(R(unsigned long long))

    #ifndef REW_CORE_MINIMAL
    FACTORY(R(std::string const&, std_size_t))
    #endif // REW_CORE_MINIMAL

    FACTORY(R(std::string const&))

    #ifndef REW_CORE_MINIMAL
    FACTORY(R(char const*, std_size_t))
    FACTORY(R(char const*))
    #endif // REW_CORE_MINIMAL

    FUNCTION(operator=, R&(R const&))
    FUNCTION(operator==)
    FUNCTION(operator[], bool(std_size_t) const)
    FUNCTION(operator[], std_bitset_reference<R>(std_size_t))

    #ifndef REW_CORE_MINIMAL
    FUNCTION(test)
    FUNCTION(all)
    FUNCTION(any)
    FUNCTION(none)
    FUNCTION(count)
    FUNCTION(size)
    FUNCTION(operator&=)
    FUNCTION(operator|=)
    FUNCTION(operator^=)
    FUNCTION(operator~)
    FUNCTION(operator<<)
    FUNCTION(operator<<=)
    FUNCTION(operator>>)
    FUNCTION(operator>>=)
    FUNCTION(set, R&())
    #endif // REW_CORE_MINIMAL

    FUNCTION(set, R&(std_size_t, bool))

    #ifndef REW_CORE_MINIMAL
    FUNCTION(reset, R&())
    #endif // REW_CORE_MINIMAL

    FUNCTION(reset, R&(std_size_t))

    #ifndef REW_CORE_MINIMAL
    FUNCTION(flip, R&())
    #endif // REW_CORE_MINIMAL

    FUNCTION(flip, R&(std_size_t))
    FUNCTION(to_string, std::string(char, char) const)

    #ifndef REW_CORE_MINIMAL
    FUNCTION(to_ulong)
    #endif // REW_CORE_MINIMAL

    FUNCTION(to_ullong)
REFLECTABLE_INIT()

#include <complex> // complex

template <typename> struct __rew_std_complex_pass_value;

template <typename ValueType>
struct __rew_std_complex_pass_value<std::complex<ValueType>>
{
    using type = ValueType const&;
};

template <> struct __rew_std_complex_pass_value<std::complex<float>> { using type = float; };
template <> struct __rew_std_complex_pass_value<std::complex<double>> { using type = double; };
template <> struct __rew_std_complex_pass_value<std::complex<long double>> { using type = long double; };

TEMPLATE_REFLECTABLE_CLEAN
(
    template <typename ValueType>,
    std::complex<ValueType>, std::complex<rew::cleanof<ValueType>>
)

TEMPLATE_REFLECTABLE_DECLARATION(template <typename ValueType>, std::complex<ValueType>)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME("std::complex<" + rew::nameof<ValueType>() + ">")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE(template <typename ValueType>, std::complex<ValueType>)
    FACTORY(R())
    FACTORY(R(typename __rew_std_complex_pass_value<R>::type, typename __rew_std_complex_pass_value<R>::type))
    FACTORY(R(R const&))
    FUNCTION(operator=, R&(R const&))

    #ifndef REW_CORE_MINIMAL
    FUNCTION(operator+=, R&(typename __rew_std_complex_pass_value<R>::type))
    #endif // REW_CORE_MINIMAL

    FUNCTION(operator+=, R&(R const&))

    #ifndef REW_CORE_MINIMAL
    FUNCTION(operator-=, R&(typename __rew_std_complex_pass_value<R>::type))
    #endif // REW_CORE_MINIMAL

    FUNCTION(operator-=, R&(R const&))

    #ifndef REW_CORE_MINIMAL
    FUNCTION(operator*=, R&(typename __rew_std_complex_pass_value<R>::type))
    #endif // REW_CORE_MINIMAL

    FUNCTION(operator*=, R&(R const&))

    #ifndef REW_CORE_MINIMAL
    FUNCTION(operator/=, R&(typename __rew_std_complex_pass_value<R>::type))
    #endif // REW_CORE_MINIMAL

    FUNCTION(operator/=, R&(R const&))
    PROPERTY(real)
    PROPERTY(imag)
REFLECTABLE_INIT()

#include <optional> // optional, nullopt_t

REFLECTABLE_DECLARATION(std::nullopt_t)
    BUILTIN_REFLECTABLE()
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE(std::nullopt_t)
REFLECTABLE_INIT()

TEMPLATE_REFLECTABLE_CLEAN
(
    template <typename ValueType>,
    std::optional<ValueType>, std::optional<rew::cleanof<ValueType>>
)

TEMPLATE_REFLECTABLE_DECLARATION(template <typename ValueType>, std::optional<ValueType>)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME("std::optional<" + rew::nameof<ValueType>() + ">")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE(template <typename ValueType>, std::optional<ValueType>)
    FACTORY(R())
    FACTORY(R(std::nullopt_t))
    FACTORY(R(R const&))
    FACTORY(R(typename R::value_type const&))
    FUNCTION(operator=, R&(std::nullopt_t))
    FUNCTION(operator=, R&(R const&))

    #ifndef REW_CORE_MINIMAL
    FUNCTION(operator->, typename R::value_type const*() const)
    FUNCTION(operator->, typename R::value_type*())
    FUNCTION(operator*, typename R::value_type const&() const&)
    FUNCTION(operator*, typename R::value_type&()&)
    FUNCTION(operator bool)
    #endif // REW_CORE_MINIMAL

    FUNCTION(has_value)
    FUNCTION(value, typename R::value_type&()&)
    FUNCTION(value, typename R::value_type const&() const&)

    #ifndef REW_CORE_MINIMAL
    FUNCTION(value_or, typename R::value_type(typename R::value_type const&) const&)
    FUNCTION(swap)
    #endif // REW_CORE_MINIMAL

    FUNCTION(reset)
    FUNCTION(emplace, typename R::value_type&(typename R::value_type const&))
REFLECTABLE_INIT()

#include <variant> // variant, monostate

REFLECTABLE_DECLARATION(std::monostate)
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE(std::monostate)
REFLECTABLE_INIT()

TEMPLATE_REFLECTABLE_CLEAN
(
    (template <typename ArgumentType, typename... ArgumentTypes>),
    (std::variant<ArgumentType, ArgumentTypes...>), std::variant<rew::cleanof<ArgumentType>, rew::cleanof<ArgumentTypes>...>
)

TEMPLATE_REFLECTABLE_DECLARATION
(
    (template <typename ArgumentType, typename... ArgumentTypes>),
    std::variant<ArgumentType, ArgumentTypes...>
)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME("std::variant<" + ( rew::nameof<ArgumentType>() + ... + (", " + rew::nameof<ArgumentTypes>()) ) + ">")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE
(
    (template <typename ArgumentType, typename... ArgumentTypes>),
    std::variant<ArgumentType, ArgumentTypes...>
)
    FACTORY(R())
    FACTORY(R(R const&))
    FUNCTION(operator=, R&(R const&))
    FUNCTION(index)

    #ifndef REW_CORE_MINIMAL
    FUNCTION(valueless_by_exception)
    FUNCTION(swap)
    #endif // REW_CORE_MINIMAL
REFLECTABLE_INIT()

// as function return type

REFLECTABLE_DECLARATION(std::any)
    BUILTIN_REFLECTABLE()
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE(std::any)
    FACTORY(R())
    FACTORY(R(R const&))
    FUNCTION(operator=, R&(R const&))
    FUNCTION(reset)

    #ifndef REW_CORE_MINIMAL
    FUNCTION(swap)
    #endif // REW_CORE_MINIMAL

    FUNCTION(has_value)
    FUNCTION(type)
REFLECTABLE_INIT()

TEMPLATE_REFLECTABLE_CLEAN
(
    template <typename ValueType>,
    std::reference_wrapper<ValueType>, std::reference_wrapper<rew::cleanof<ValueType>>
)

TEMPLATE_REFLECTABLE_DECLARATION(template <typename ValueType>, std::reference_wrapper<ValueType>)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME("std::reference_wrapper<" + rew::nameof<ValueType>() + ">")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE(template <typename ValueType>, std::reference_wrapper<ValueType>)
    FACTORY(R(ValueType&))
    FACTORY(R(R const&))
    FUNCTION(operator=, R&(R const&))

    #ifndef REW_CORE_MINIMAL
    NAMED_FUNCTION("operator " + rew::nameof<ValueType&>(), operator rew::cleanof<ValueType>&)
    #endif // REW_CORE_MINIMAL

    FUNCTION(get)
REFLECTABLE_INIT()

// as function argument type

#if __cplusplus > 201703L

#if __cplusplus > 201703L
#include <compare> // strong_ordering

REFLECTABLE_DECLARATION(std::partial_ordering)
    BUILTIN_REFLECTABLE()
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE(std::partial_ordering)
REFLECTABLE_INIT()

REFLECTABLE_DECLARATION(std::weak_ordering)
    BUILTIN_REFLECTABLE()
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE(std::weak_ordering)
REFLECTABLE_INIT()

REFLECTABLE_DECLARATION(std::strong_ordering)
    BUILTIN_REFLECTABLE()
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE(std::strong_ordering)
REFLECTABLE_INIT()
#endif // if

#endif // if

REFLECTABLE_DECLARATION(std::type_index)
    BUILTIN_REFLECTABLE()
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE(std::type_index)
    FACTORY(R(std::type_info const&))

#if __cplusplus > 201703L
    FUNCTION(operator<=>)
#else
    FUNCTION(operator==)
    FUNCTION(operator!=)

    #ifndef REW_CORE_MINIMAL
    FUNCTION(operator<)
    FUNCTION(operator<=)
    FUNCTION(operator>)
    FUNCTION(operator>=)
    #endif // REW_CORE_MINIMAL
#endif // if
    FUNCTION(hash_code)
    FUNCTION(name)
REFLECTABLE_INIT()

#endif // REW_HPP
