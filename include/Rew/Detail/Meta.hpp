#ifndef REW_DETAIL_META_HPP
#define REW_DETAIL_META_HPP

#include <type_traits> // conjunction, disjunction, false_type, true_type, void_t

#include <utility> // pair

template <typename ReflectableType, typename enable = void>
struct xxrew_alias { using R = ReflectableType; };

template <typename ReflectableType, typename enable = void>
struct xxrew_traits;

namespace rew
{

namespace meta
{

template <typename T>
struct type_identity { using type = T; };

template <typename T>
struct inherits_t : std::conditional_t
<
    std::conjunction_v< std::is_class<T>, std::negation<std::is_final<T>> >,
    T,
    type_identity<T>
> {};

template <typename T>
struct to_reflectable_reference { using type = std::remove_const_t<std::remove_reference_t<T>>*; };

template <typename T>
struct to_reflectable_pointer { using type = std::remove_const_t<std::remove_pointer_t<T>>*; };

template <typename T>
struct to_reflectable_object { using type = std::remove_const_t<T>; };

template <typename T, typename enable = void> struct is_complete : std::false_type {};
template <typename T> struct is_complete<T, std::void_t<decltype(sizeof(T))>> : std::true_type {};

template <typename T, typename enable = void> struct is_reflectable : std::false_type {};
template <typename T> struct is_reflectable<T, std::void_t<decltype(&::xxrew_traits<T>::registry)>> : std::true_type {};

template <typename T, typename enable = void> struct is_lazy : std::false_type {};
template <typename T> struct is_lazy<T, std::void_t<decltype(&::xxrew_traits<T>::lazy)>> : std::true_type {};

template <typename T, typename enable = void> struct is_builtin : std::false_type {};
template <typename T> struct is_builtin<T, std::void_t<decltype(&::xxrew_traits<T>::biiltin)>> : std::true_type {};

template <typename PropertyType>
struct property_traits { using type = PropertyType; };

// TODO: rework!
// template <typename PropertyType>
// struct property_traits<PropertyType*>
// {
//     using type = typename std::conditional_t
//     <
//         std::is_pointer_v<PropertyType>,
//         type_identity<PropertyType>,
//         property_traits<PropertyType>
//     >::type;
// };

template <typename ReflectableType, typename PropertyType>
struct property_traits<PropertyType (ReflectableType::*)(void) const> { using type = PropertyType; };

template <typename ReflectableType, typename PropertyType>
struct property_traits<PropertyType (ReflectableType::*)(void) const&> { using type = PropertyType; };

template <typename ReflectableType, typename PropertyType>
struct property_traits<PropertyType (ReflectableType::*)(void)> { using type = PropertyType; };

template <typename ReflectableType, typename PropertyType>
struct property_traits<PropertyType (ReflectableType::*)(void)&> { using type = PropertyType; };

template <typename ReflectableType, typename PropertyType>
struct property_traits<void (ReflectableType::*)(PropertyType)> { using type = PropertyType; };

template <typename ReflectableType, typename PropertyType>
struct property_traits<void (ReflectableType::*)(PropertyType)&> { using type = PropertyType; };

template <typename PropertyType>
struct property_traits<PropertyType (*)(void)> { using type = PropertyType; };

template <typename PropertyType>
struct property_traits<PropertyType() const> { using type = PropertyType; };

template <typename PropertyType>
struct property_traits<PropertyType()> { using type = PropertyType; };

template <typename PropertyType>
struct property_traits<void(PropertyType)> { using type = PropertyType; };

template <typename ReflectableType, typename PropertyType>
struct property_traits<PropertyType ReflectableType::*> { using type = PropertyType; };

template <typename>
struct function_traits;

template <typename ReturnType, typename... ArgumentTypes>
struct function_traits<ReturnType(ArgumentTypes...) const>
{
    using dirty_type = ReturnType(ArgumentTypes...) const;
    using dirty_pointer = ReturnType(*)(ArgumentTypes...);

    using type = typename ::xxrew_alias<ReturnType>::R(typename ::xxrew_alias<ArgumentTypes>::R...) const;
    using pointer = typename ::xxrew_alias<ReturnType>::R(*)(typename ::xxrew_alias<ArgumentTypes>::R...);
};

template <typename ReturnType, typename... ArgumentTypes>
struct function_traits<ReturnType(ArgumentTypes...) const&>
{
    using dirty_type = ReturnType(ArgumentTypes...) const&;
    using dirty_pointer = ReturnType(*)(ArgumentTypes...);

    using type = typename ::xxrew_alias<ReturnType>::R(typename ::xxrew_alias<ArgumentTypes>::R...) const&;
    using pointer = typename ::xxrew_alias<ReturnType>::R(*)(typename ::xxrew_alias<ArgumentTypes>::R...);
};

template <typename ReturnType, typename... ArgumentTypes>
struct function_traits<ReturnType(ArgumentTypes...)>
{
    using dirty_type = ReturnType(ArgumentTypes...);
    using dirty_pointer = ReturnType(*)(ArgumentTypes...);

    using type = typename ::xxrew_alias<ReturnType>::R(typename ::xxrew_alias<ArgumentTypes>::R...);
    using pointer = typename ::xxrew_alias<ReturnType>::R(*)(typename ::xxrew_alias<ArgumentTypes>::R...);
};

template <typename ReturnType, typename... ArgumentTypes>
struct function_traits<ReturnType(ArgumentTypes...)&>
{
    using dirty_type = ReturnType(ArgumentTypes...)&;
    using dirty_pointer = ReturnType(*)(ArgumentTypes...);

    using type = typename ::xxrew_alias<ReturnType>::R(typename ::xxrew_alias<ArgumentTypes>::R...)&;
    using pointer = typename ::xxrew_alias<ReturnType>::R(*)(typename ::xxrew_alias<ArgumentTypes>::R...);
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
    struct xxinner : protected ReflectableType
    {
        static constexpr auto get(ReturnType (ParentReflectableType::* function)(ArgumentTypes...) const)
        {
            return static_cast<ReturnType (ReflectableType::*)(ArgumentTypes...) const>(function);
        }
    };
    return xxinner::get(function);
}

template <typename ReflectableType, typename ParentReflectableType, typename ReturnType, typename... ArgumentTypes>
constexpr auto function_ptr_impl(ReturnType (ParentReflectableType::* function)(ArgumentTypes...) const&)
{
    struct xxinner : protected ReflectableType
    {
        static constexpr auto get(ReturnType (ParentReflectableType::* function)(ArgumentTypes...) const&)
        {
            return static_cast<ReturnType (ReflectableType::*)(ArgumentTypes...) const&>(function);
        }
    };
    return xxinner::get(function);
}

template <typename ReflectableType, typename ParentReflectableType, typename ReturnType, typename... ArgumentTypes>
constexpr auto function_ptr_impl(ReturnType (ParentReflectableType::* function)(ArgumentTypes...))
{
    struct xxinner : protected ReflectableType
    {
        static constexpr auto get(ReturnType (ParentReflectableType::* function)(ArgumentTypes...))
        {
            return static_cast<ReturnType (ReflectableType::*)(ArgumentTypes...)>(function);
        }
    };
    return xxinner::get(function);
}

template <typename ReflectableType, typename ParentReflectableType, typename ReturnType, typename... ArgumentTypes>
constexpr auto function_ptr_impl(ReturnType (ParentReflectableType::* function)(ArgumentTypes...)&)
{
    struct xxinner : protected ReflectableType
    {
        static constexpr auto get(ReturnType (ParentReflectableType::* function)(ArgumentTypes...)&)
        {
            return static_cast<ReturnType (ReflectableType::*)(ArgumentTypes...)&>(function);
        }
    };
    return xxinner::get(function);
}

template <typename ReflectableType, typename ReturnType, typename... ArgumentTypes>
constexpr auto function_ptr_impl(ReturnType (*function)(ArgumentTypes...))
{
    return function;
}

template <typename ReflectableType, typename ParentReflectableType, typename ValueType>
constexpr auto property_ptr_impl(ValueType ParentReflectableType::* property)
{
    struct xxinner : protected ReflectableType
    {
        static constexpr auto get(ValueType ParentReflectableType::* property)
        {
            return static_cast<ValueType ReflectableType::*>(property);
        }
    };
    return xxinner::get(property);
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
        static constexpr auto of(typename ::xxrew_alias<PropertyType>::R (*get)(void), void (*set)(typename ::xxrew_alias<PropertyType>::R))
        {
            return std::make_pair(get, set);
        }
    };

    template <typename PropertyType>
    struct property<PropertyType>
    {
        static constexpr auto of(typename ::xxrew_alias<PropertyType>::R* get, typename ::xxrew_alias<PropertyType>::R* set)
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
        static constexpr auto of(typename ::xxrew_alias<ReturnType>::R (*function)(typename ::xxrew_alias<ArgumentTypes>::R...)) { return function; }

        template <typename OtherReturnType, typename... OtherArgumentTypes>
        static constexpr auto of(OtherReturnType (*function)(OtherArgumentTypes...)) { return function; }
    };
};

template <class ClassType>
struct access_traits<ClassType>
{
    template <typename PropertyGetterType, typename PropertySetterType>
    static constexpr auto property_data(PropertyGetterType get, PropertySetterType set)
    {
        return std::make_pair(detail::property_ptr_impl<ClassType>(get), detail::property_ptr_impl<ClassType>(set));
    }

    template <typename PropertyGetterType, typename PropertySetterType>
    static constexpr auto function_data(PropertyGetterType get, PropertySetterType set)
    {
        return std::make_pair(detail::function_ptr_impl<ClassType>(get), detail::function_ptr_impl<ClassType>(set));
    }

    template <typename FunctionType>
    static constexpr auto function_data(FunctionType function)
    {
        return detail::function_ptr_impl<ClassType>(function);
    }

    template <typename...>
    struct property
    {
        template <typename ParentClassType, typename PropertyType>
        static constexpr auto of(PropertyType ParentClassType::* get, PropertyType ParentClassType::* set)
        {
            return property_data(get, set);
        }

        template <typename ParentClassType, typename PropertyType>
        static constexpr auto of(PropertyType (ParentClassType::* get)(void) const, void (ParentClassType::* set)(PropertyType))
        {
            return function_data(get, set);
        }

        template <typename ParentClassType, typename PropertyType>
        static constexpr auto of(PropertyType (ParentClassType::* get)(void) const, void (ParentClassType::* set)(PropertyType)&)
        {
            return function_data(get, set);
        }

        template <typename ParentClassType, typename PropertyType>
        static constexpr auto of(PropertyType (ParentClassType::* get)(void) const&, void (ParentClassType::* set)(PropertyType))
        {
            return function_data(get, set);
        }

        template <typename ParentClassType, typename PropertyType>
        static constexpr auto of(PropertyType (ParentClassType::* get)(void) const&, void (ParentClassType::* set)(PropertyType)&)
        {
            return function_data(get, set);
        }

        template <typename ParentClassType, typename PropertyType>
        static constexpr auto of(PropertyType (ParentClassType::* get)(void), void (ParentClassType::* set)(PropertyType))
        {
            return function_data(get, set);
        }

        template <typename ParentClassType, typename PropertyType>
        static constexpr auto of(PropertyType (ParentClassType::* get)(void), void (ParentClassType::* set)(PropertyType)&)
        {
            return function_data(get, set);
        }

        template <typename ParentClassType, typename PropertyType>
        static constexpr auto of(PropertyType (ParentClassType::* get)(void)&, void (ParentClassType::* set)(PropertyType))
        {
            return function_data(get, set);
        }

        template <typename ParentClassType, typename PropertyType>
        static constexpr auto of(PropertyType (ParentClassType::* get)(void)&, void (ParentClassType::* set)(PropertyType)&)
        {
            return function_data(get, set);
        }

        template <typename PropertyType>
        static constexpr auto of(PropertyType (*get)(void), void (*set)(PropertyType)) { return std::make_pair(get, set); }

        template <typename PropertyType>
        static constexpr auto of(PropertyType* get, PropertyType* set) { return std::make_pair(get, set); }
    };

    template <typename DirtyPropertyType>
    struct property<DirtyPropertyType() const>
    {
        using PropertyType = typename ::xxrew_alias<DirtyPropertyType>::R;

        template <typename ParentClassType>
        static constexpr auto of(PropertyType (ParentClassType::* get)(void) const, void (ParentClassType::* set)(PropertyType))
        {
            return function_data(get, set);
        }

        template <typename ParentClassType>
        static constexpr auto of(PropertyType (ParentClassType::* get)(void) const, void (ParentClassType::* set)(PropertyType)&)
        {
            return function_data(get, set);
        }

        template <typename ParentClassType>
        static constexpr auto of(PropertyType (ParentClassType::* get)(void) const, typename type_identity<PropertyType (ParentClassType::*)(void) const>::type)
        {
            return function_data(get, get);
        }
    };

    template <typename DirtyPropertyType>
    struct property<DirtyPropertyType() const&>
    {
        using PropertyType = typename ::xxrew_alias<DirtyPropertyType>::R;

        template <typename ParentClassType>
        static constexpr auto of(PropertyType (ParentClassType::* get)(void) const&, void (ParentClassType::* set)(PropertyType))
        {
            return function_data(get, set);
        }

        template <typename ParentClassType>
        static constexpr auto of(PropertyType (ParentClassType::* get)(void) const&, void (ParentClassType::* set)(PropertyType)&)
        {
            return function_data(get, set);
        }

        template <typename ParentClassType>
        static constexpr auto of(PropertyType (ParentClassType::* get)(void) const&, typename type_identity<PropertyType (ParentClassType::*)(void) const&>::type)
        {
            return function_data(get, get);
        }
    };

    template <typename DirtyPropertyType>
    struct property<DirtyPropertyType()>
    {
        using PropertyType = typename ::xxrew_alias<DirtyPropertyType>::R;

        template <typename ParentClassType>
        static constexpr auto of(PropertyType (ParentClassType::* get)(void), void (ParentClassType::* set)(PropertyType))
        {
            return function_data(get, set);
        }

        template <typename ParentClassType>
        static constexpr auto of(PropertyType (ParentClassType::* get)(void), void (ParentClassType::* set)(PropertyType)&)
        {
            return function_data(get, set);
        }

        template <typename ParentClassType>
        static constexpr auto of(PropertyType (ParentClassType::* get)(void), typename type_identity<PropertyType (ParentClassType::*)(void)>::type)
        {
            return function_data(get, get);
        }

        static constexpr auto of(PropertyType (*get)(void), void (*set)(PropertyType))
        {
            return std::make_pair(get, set);
        }

        static constexpr auto of(PropertyType (*get)(void), typename type_identity<PropertyType (*)(void)>::type)
        {
            return std::make_pair(get, get);
        }
    };

    template <typename DirtyPropertyType>
    struct property<DirtyPropertyType()&>
    {
        using PropertyType = typename ::xxrew_alias<DirtyPropertyType>::R;

        template <typename ParentClassType>
        static constexpr auto of(PropertyType (ParentClassType::* get)(void)&, void (ParentClassType::* set)(PropertyType))
        {
            return function_data(get, set);
        }

        template <typename ParentClassType>
        static constexpr auto of(PropertyType (ParentClassType::* get)(void)&, void (ParentClassType::* set)(PropertyType)&)
        {
            return function_data(get, set);
        }

        template <typename ParentClassType>
        static constexpr auto of(PropertyType (ParentClassType::* get)(void)&, typename type_identity<PropertyType (ParentClassType::*)(void)&>::type)
        {
            return function_data(get, get);
        }
    };

    template <typename DirtyPropertyType>
    struct property<DirtyPropertyType>
    {
        using PropertyType = typename ::xxrew_alias<DirtyPropertyType>::R;

        template <typename ParentClassType>
        static constexpr auto of(PropertyType ParentClassType::* get, PropertyType ParentClassType::* set)
        {
            return property_data(get, set);
        }

        static constexpr auto of(PropertyType* get, PropertyType* set) { return std::make_pair(get, set); }
    };

    template <typename...>
    struct function
    {
        template <typename ReturnType, typename... ArgumentTypes>
        static constexpr auto of(ReturnType (ClassType::* function)(ArgumentTypes...) const) { return function; }
        template <class ParentClassType, typename ReturnType, typename... ArgumentTypes>
        static constexpr auto of(ReturnType (ParentClassType::* function)(ArgumentTypes...) const) { return function_data(function); }

        template <typename ReturnType, typename... ArgumentTypes>
        static constexpr auto of(ReturnType (ClassType::* function)(ArgumentTypes...) const&) { return function; }
        template <class ParentClassType, typename ReturnType, typename... ArgumentTypes>
        static constexpr auto of(ReturnType (ParentClassType::* function)(ArgumentTypes...) const&) { return function_data(function); }

        template <typename ReturnType, typename... ArgumentTypes>
        static constexpr auto of(ReturnType (ClassType::* function)(ArgumentTypes...)) { return function; }
        template <class ParentClassType, typename ReturnType, typename... ArgumentTypes>
        static constexpr auto of(ReturnType (ParentClassType::* function)(ArgumentTypes...)) { return function_data(function); }

        template <typename ReturnType, typename... ArgumentTypes>
        static constexpr auto of(ReturnType (ClassType::* function)(ArgumentTypes...)&) { return function; }
        template <class ParentClassType, typename ReturnType, typename... ArgumentTypes>
        static constexpr auto of(ReturnType (ParentClassType::* function)(ArgumentTypes...)&) { return function_data(function); }

        template <typename ReturnType, typename... ArgumentTypes>
        static constexpr auto of(ReturnType (*function)(ArgumentTypes...)) { return function; }
    };

    template <typename DirtyReturnType, typename... DirtyArgumentTypes>
    struct function<DirtyReturnType(DirtyArgumentTypes...) const>
    {
        static constexpr auto of(typename ::xxrew_alias<DirtyReturnType>::R (ClassType::* function)(typename ::xxrew_alias<DirtyArgumentTypes>::R...) const) { return function; }

        template <class ParentClassType>
        static constexpr auto of(typename ::xxrew_alias<DirtyReturnType>::R (ParentClassType::* function)(typename ::xxrew_alias<DirtyArgumentTypes>::R...) const)
        {
            return function_data(function);
        }
    };

    template <typename DirtyReturnType, typename... DirtyArgumentTypes>
    struct function<DirtyReturnType(DirtyArgumentTypes...) const&>
    {
        static constexpr auto of(typename ::xxrew_alias<DirtyReturnType>::R (ClassType::* function)(typename ::xxrew_alias<DirtyArgumentTypes>::R...) const&) { return function; }

        template <class ParentClassType>
        static constexpr auto of(typename ::xxrew_alias<DirtyReturnType>::R (ParentClassType::* function)(typename ::xxrew_alias<DirtyArgumentTypes>::R...) const&)
        {
            return function_data(function);
        }
    };

    template <typename DirtyReturnType, typename... DirtyArgumentTypes>
    struct function<DirtyReturnType(DirtyArgumentTypes...)>
    {
        static constexpr auto of(typename ::xxrew_alias<DirtyReturnType>::R (ClassType::* function)(typename ::xxrew_alias<DirtyArgumentTypes>::R...)) { return function; }

        template <class ParentClassType>
        static constexpr auto of(typename ::xxrew_alias<DirtyReturnType>::R (ParentClassType::* function)(typename ::xxrew_alias<DirtyArgumentTypes>::R...))
        {
            return function_data(function);
        }

        static constexpr auto of(typename ::xxrew_alias<DirtyReturnType>::R (*function)(typename ::xxrew_alias<DirtyArgumentTypes>::R...)) { return function; }
    };

    template <typename DirtyReturnType, typename... DirtyArgumentTypes>
    struct function<DirtyReturnType(DirtyArgumentTypes...)&>
    {
        static constexpr auto of(typename ::xxrew_alias<DirtyReturnType>::R (ClassType::* function)(typename ::xxrew_alias<DirtyArgumentTypes>::R...)&) { return function; }

        template <class ParentClassType>
        static constexpr auto of(typename ::xxrew_alias<DirtyReturnType>::R (ParentClassType::* function)(typename ::xxrew_alias<DirtyArgumentTypes>::R...)&)
        {
            return function_data(function);
        }
    };
};

} // namespace meta

} // namespace rew

#endif // REW_DETAIL_META_HPP
