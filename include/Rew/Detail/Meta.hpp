// TODO: changr function name building and other possible to struct traits
#ifndef REW_DETAIL_META_HPP
#define REW_DETAIL_META_HPP

#include <type_traits> // conjunction, disjunction, false_type, true_type, void_t

#include <utility> // pair

template <typename ReflectableType, typename enable = void>
struct rew_reflection_registry_t;

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
struct property_traits<PropertyType (ReflectableType::*)(void) &>
    : property_traits<PropertyType> {};

template <typename ReflectableType, typename PropertyType>
struct property_traits<void (ReflectableType::*)(PropertyType)>
    : property_traits<PropertyType> {};

template <typename ReflectableType, typename PropertyType>
struct property_traits<void (ReflectableType::*)(PropertyType) &>
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
struct function_traits<ReturnType(ArgumentTypes...)>
{
    using dirty_type = ReturnType(ArgumentTypes...);
    using dirty_pointer = ReturnType(*)(ArgumentTypes...);

    using type = clean<ReturnType>(clean<ArgumentTypes>...);
    using pointer = clean<ReturnType>(*)(clean<ArgumentTypes>...);
};

template <typename ReturnType, typename... ArgumentTypes>
struct function_traits<ReturnType(ArgumentTypes...) const>
{
    using dirty_type = ReturnType(ArgumentTypes...) const;
    using dirty_pointer = ReturnType(*)(ArgumentTypes...);

    using type = clean<ReturnType>(clean<ArgumentTypes>...) const;
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
struct function_traits<ReturnType (ClassType::*)(ArgumentTypes...) &>
    : function_traits<ReturnType(ArgumentTypes...) &> {};

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
constexpr auto function_ptr_impl(ReturnType (ParentReflectableType::* function)(ArgumentTypes...) &)
{
    struct __inner : protected ReflectableType
    {
        static constexpr auto get(ReturnType (ParentReflectableType::* function)(ArgumentTypes...) &)
        {
            return static_cast<ReturnType (ReflectableType::*)(ArgumentTypes...) &>(function);
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
    struct property<PropertyType>
    {
        static constexpr auto of(clean<PropertyType> (*get)(void), void (*set)(clean<PropertyType>))
        {
            return std::make_pair(get, set);
        }

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
    template <typename...>
    struct property
    {
        template <typename ParentClassType, typename PropertyType>
        static constexpr auto of(PropertyType ParentClassType::* get,
                                 PropertyType ParentClassType::* set)
        {
            return std::make_pair(detail::property_ptr_impl<ClassType>(get), detail::property_ptr_impl<ClassType>(set));
        }

        template <typename ParentClassType, typename PropertyType>
        static constexpr auto of(PropertyType (ParentClassType::* get)(void) const,
                                 void (ParentClassType::* set)(PropertyType))
        {
            return std::make_pair(detail::function_ptr_impl<ClassType>(get), detail::function_ptr_impl<ClassType>(set));
        }

        template <typename ParentClassType, typename PropertyType>
        static constexpr auto of(PropertyType (ParentClassType::* get)(void) const,
                                 void (ParentClassType::* set)(PropertyType) &)
        {
            return std::make_pair(detail::function_ptr_impl<ClassType>(get), detail::function_ptr_impl<ClassType>(set));
        }

        template <typename ParentClassType, typename PropertyType>
        static constexpr auto of(PropertyType (ParentClassType::* get)(void) const&,
                                 void (ParentClassType::* set)(PropertyType))
        {
            return std::make_pair(detail::function_ptr_impl<ClassType>(get), detail::function_ptr_impl<ClassType>(set));
        }

        template <typename ParentClassType, typename PropertyType>
        static constexpr auto of(PropertyType (ParentClassType::* get)(void) const&,
                                 void (ParentClassType::* set)(PropertyType) &)
        {
            return std::make_pair(detail::function_ptr_impl<ClassType>(get), detail::function_ptr_impl<ClassType>(set));
        }

        template <typename ParentClassType, typename PropertyType>
        static constexpr auto of(PropertyType (ParentClassType::* get)(void),
                                 void (ParentClassType::* set)(PropertyType))
        {
            return std::make_pair(detail::function_ptr_impl<ClassType>(get), detail::function_ptr_impl<ClassType>(set));
        }

        template <typename ParentClassType, typename PropertyType>
        static constexpr auto of(PropertyType (ParentClassType::* get)(void),
                                 void (ParentClassType::* set)(PropertyType) &)
        {
            return std::make_pair(detail::function_ptr_impl<ClassType>(get), detail::function_ptr_impl<ClassType>(set));
        }

        template <typename ParentClassType, typename PropertyType>
        static constexpr auto of(PropertyType (ParentClassType::* get)(void) &,
                                 void (ParentClassType::* set)(PropertyType))
        {
            return std::make_pair(detail::function_ptr_impl<ClassType>(get), detail::function_ptr_impl<ClassType>(set));
        }

        template <typename ParentClassType, typename PropertyType>
        static constexpr auto of(PropertyType (ParentClassType::* get)(void) &,
                                 void (ParentClassType::* set)(PropertyType) &)
        {
            return std::make_pair(detail::function_ptr_impl<ClassType>(get), detail::function_ptr_impl<ClassType>(set));
        }

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

    template <typename DirtyPropertyType>
    struct property<DirtyPropertyType>
    {
        using PropertyType = clean<DirtyPropertyType>;

        template <typename ParentClassType>
        static constexpr auto of(PropertyType ParentClassType::* get,
                                 PropertyType ParentClassType::* set)
        {
            return std::make_pair(detail::property_ptr_impl<ClassType>(get), detail::property_ptr_impl<ClassType>(set));
        }

        template <typename ParentClassType>
        static constexpr auto of(PropertyType (ParentClassType::* get)(void) const,
                                 void (ParentClassType::* set)(PropertyType))
        {
            return std::make_pair(detail::function_ptr_impl<ClassType>(get), detail::function_ptr_impl<ClassType>(set));
        }

        template <typename ParentClassType>
        static constexpr auto of(PropertyType (ParentClassType::* get)(void) const,
                                 void (ParentClassType::* set)(PropertyType) &)
        {
            return std::make_pair(detail::function_ptr_impl<ClassType>(get), detail::function_ptr_impl<ClassType>(set));
        }

        template <typename ParentClassType>
        static constexpr auto of(PropertyType (ParentClassType::* get)(void) const&,
                                 void (ParentClassType::* set)(PropertyType))
        {
            return std::make_pair(detail::function_ptr_impl<ClassType>(get), detail::function_ptr_impl<ClassType>(set));
        }

        template <typename ParentClassType>
        static constexpr auto of(PropertyType (ParentClassType::* get)(void) const&,
                                 void (ParentClassType::* set)(PropertyType) &)
        {
            return std::make_pair(detail::function_ptr_impl<ClassType>(get), detail::function_ptr_impl<ClassType>(set));
        }

        template <typename ParentClassType>
        static constexpr auto of(PropertyType (ParentClassType::* get)(void),
                                 void (ParentClassType::* set)(PropertyType))
        {
            return std::make_pair(detail::function_ptr_impl<ClassType>(get), detail::function_ptr_impl<ClassType>(set));
        }

        template <typename ParentClassType>
        static constexpr auto of(PropertyType (ParentClassType::* get)(void),
                                 void (ParentClassType::* set)(PropertyType) &)
        {
            return std::make_pair(detail::function_ptr_impl<ClassType>(get), detail::function_ptr_impl<ClassType>(set));
        }

        template <typename ParentClassType>
        static constexpr auto of(PropertyType (ParentClassType::* get)(void) &,
                                 void (ParentClassType::* set)(PropertyType))
        {
            return std::make_pair(detail::function_ptr_impl<ClassType>(get), detail::function_ptr_impl<ClassType>(set));
        }

        template <typename ParentClassType>
        static constexpr auto of(PropertyType (ParentClassType::* get)(void) &,
                                 void (ParentClassType::* set)(PropertyType) &)
        {
            return std::make_pair(detail::function_ptr_impl<ClassType>(get), detail::function_ptr_impl<ClassType>(set));
        }

        static constexpr auto of(PropertyType (*get)(void), void (*set)(PropertyType))
        {
            return std::make_pair(get, set);
        }

        static constexpr auto of(PropertyType* get, PropertyType* set)
        {
            return std::make_pair(get, set);
        }
    };

    template <typename DirtyPropertyType>
    struct property<DirtyPropertyType()>
    {
        template <typename ParentClassType>
        static constexpr auto of(clean<DirtyPropertyType> (ParentClassType::* get)(void),
                                 void (ParentClassType::* set)(clean<DirtyPropertyType>))
        {
            return std::make_pair(detail::function_ptr_impl<ClassType>(get), detail::function_ptr_impl<ClassType>(set));
        }

        template <typename ParentClassType>
        static constexpr auto of(clean<DirtyPropertyType> (ParentClassType::* get)(void),
                                 void (ParentClassType::* set)(clean<DirtyPropertyType>) &)
        {
            return std::make_pair(detail::function_ptr_impl<ClassType>(get), detail::function_ptr_impl<ClassType>(set));
        }

        template <typename ParentClassType>
        static constexpr auto of(clean<DirtyPropertyType> (ParentClassType::* get)(void) &,
                                 void (ParentClassType::* set)(clean<DirtyPropertyType>))
        {
            return std::make_pair(detail::function_ptr_impl<ClassType>(get), detail::function_ptr_impl<ClassType>(set));
        }

        template <typename ParentClassType>
        static constexpr auto of(clean<DirtyPropertyType> (ParentClassType::* get)(void) &,
                                 void (ParentClassType::* set)(clean<DirtyPropertyType>) &)
        {
            return std::make_pair(detail::function_ptr_impl<ClassType>(get), detail::function_ptr_impl<ClassType>(set));
        }

        static constexpr auto of(clean<DirtyPropertyType> (*get)(void),
                                 void (*set)(clean<DirtyPropertyType>))
        {
            return std::make_pair(get, set);
        }
    };

    template <typename DirtyPropertyType>
    struct property<DirtyPropertyType() const>
    {
        template <typename ParentClassType>
        static constexpr auto of(clean<DirtyPropertyType> (ParentClassType::* get)(void) const,
                                 void (ParentClassType::* set)(clean<DirtyPropertyType>))
        {
            return std::make_pair(detail::function_ptr_impl<ClassType>(get), detail::function_ptr_impl<ClassType>(set));
        }

        template <typename ParentClassType>
        static constexpr auto of(clean<DirtyPropertyType> (ParentClassType::* get)(void) const,
                                 void (ParentClassType::* set)(clean<DirtyPropertyType>) &)
        {
            return std::make_pair(detail::function_ptr_impl<ClassType>(get), detail::function_ptr_impl<ClassType>(set));
        }

        template <typename ParentClassType>
        static constexpr auto of(clean<DirtyPropertyType> (ParentClassType::* get)(void) const&,
                                 void (ParentClassType::* set)(clean<DirtyPropertyType>))
        {
            return std::make_pair(detail::function_ptr_impl<ClassType>(get), detail::function_ptr_impl<ClassType>(set));
        }

        template <typename ParentClassType>
        static constexpr auto of(clean<DirtyPropertyType> (ParentClassType::* get)(void) const&,
                                 void (ParentClassType::* set)(clean<DirtyPropertyType>) &)
        {
            return std::make_pair(detail::function_ptr_impl<ClassType>(get), detail::function_ptr_impl<ClassType>(set));
        }

        static constexpr auto of(clean<DirtyPropertyType> (*get)(void),
                                 void (*set)(clean<DirtyPropertyType>))
        {
            return std::make_pair(get, set);
        }
    };

    template <typename...>
    struct function
    {
        template <typename ReturnType, typename... ArgumentTypes>
        static constexpr auto of(ReturnType (ClassType::* function)(ArgumentTypes...) const) { return function; }
        template <typename ReturnType, typename... ArgumentTypes>
        static constexpr auto of(ReturnType (ClassType::* function)(ArgumentTypes...) const&) { return function; }

        template <typename ReturnType, typename... ArgumentTypes>
        static constexpr auto of(ReturnType (ClassType::* function)(ArgumentTypes...)) { return function; }
        template <typename ReturnType, typename... ArgumentTypes>
        static constexpr auto of(ReturnType (ClassType::* function)(ArgumentTypes...) &) { return function; }

        template <class ParentClassType, typename ReturnType, typename... ArgumentTypes>
        static constexpr auto of(ReturnType (ParentClassType::* function)(ArgumentTypes...) const)
        {
            return detail::function_ptr_impl<ClassType>(function);
        }

        template <class ParentClassType, typename ReturnType, typename... ArgumentTypes>
        static constexpr auto of(ReturnType (ParentClassType::* function)(ArgumentTypes...) const&)
        {
            return detail::function_ptr_impl<ClassType>(function);
        }

        template <class ParentClassType, typename ReturnType, typename... ArgumentTypes>
        static constexpr auto of(ReturnType (ParentClassType::* function)(ArgumentTypes...))
        {
            return detail::function_ptr_impl<ClassType>(function);
        }

        template <class ParentClassType, typename ReturnType, typename... ArgumentTypes>
        static constexpr auto of(ReturnType (ParentClassType::* function)(ArgumentTypes...) &)
        {
            return detail::function_ptr_impl<ClassType>(function);
        }

        template <typename ReturnType, typename... ArgumentTypes>
        static constexpr auto of(ReturnType (*function)(ArgumentTypes...)) { return function; }
    };

    template <typename DirtyReturnType, typename... DirtyArgumentTypes>
    struct function<DirtyReturnType(DirtyArgumentTypes...) const>
    {
        static constexpr auto of(clean<DirtyReturnType> (ClassType::* function)(clean<DirtyArgumentTypes>...) const) { return function; }
        static constexpr auto of(clean<DirtyReturnType> (ClassType::* function)(clean<DirtyArgumentTypes>...) const&) { return function; }

        template <class ParentClassType>
        static constexpr auto of(clean<DirtyReturnType> (ParentClassType::* function)(clean<DirtyArgumentTypes>...) const)
        {
            return detail::function_ptr_impl<ClassType>(function);
        }

        template <class ParentClassType>
        static constexpr auto of(clean<DirtyReturnType> (ParentClassType::* function)(clean<DirtyArgumentTypes>...) const&)
        {
            return detail::function_ptr_impl<ClassType>(function);
        }

        static constexpr auto of(clean<DirtyReturnType> (*function)(clean<DirtyArgumentTypes>...)) { return function; }
    };

    template <typename DirtyReturnType, typename... DirtyArgumentTypes>
    struct function<DirtyReturnType(DirtyArgumentTypes...)>
    {
        static constexpr auto of(clean<DirtyReturnType> (ClassType::* function)(clean<DirtyArgumentTypes>...)) { return function; }
        static constexpr auto of(clean<DirtyReturnType> (ClassType::* function)(clean<DirtyArgumentTypes>...) &) { return function; }

        template <class ParentClassType>
        static constexpr auto of(clean<DirtyReturnType> (ParentClassType::* function)(clean<DirtyArgumentTypes>...))
        {
            return detail::function_ptr_impl<ClassType>(function);
        }

        template <class ParentClassType>
        static constexpr auto of(clean<DirtyReturnType> (ParentClassType::* function)(clean<DirtyArgumentTypes>...) &)
        {
            return detail::function_ptr_impl<ClassType>(function);
        }

        static constexpr auto of(clean<DirtyReturnType> (*function)(clean<DirtyArgumentTypes>...)) { return function; }
    };
};

} // namespace meta

} // namespace rew

#endif // REW_DETAIL_META_HPP
