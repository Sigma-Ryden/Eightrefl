#ifndef REW_CONTEXT_HPP
#define REW_CONTEXT_HPP

#include <any> // any
#include <memory> // shared_ptr, weak_ptr

#include <utility>// addressof

namespace rew
{

namespace meta
{

template <typename ReflectableType, typename enable = void>
struct context_traits_t
{
    static std::any context(std::any& object)
    {
        return std::addressof(std::any_cast<ReflectableType&>(object));
    }

    static ReflectableType* access(std::any& context)
    {
        return std::any_cast<ReflectableType*>(context);
    }

    template <typename OtherReflectableType>
    static OtherReflectableType* cast(std::any& context)
    {
        return static_cast<OtherReflectableType*>(access(context));
    }
};

template <typename ReflectableType>
struct context_traits_t<ReflectableType*>
{
    static std::any context(std::any& object)
    {
        return std::addressof(std::any_cast<ReflectableType&>(object));
    }

    static ReflectableType* access(std::any& context)
    {
        return std::any_cast<ReflectableType*>(context);
    }

    template <typename OtherReflectableType>
    static OtherReflectableType* cast(std::any& context)
    {
        return static_cast<OtherReflectableType*>(access(context));
    }
};

template <typename ReflectableType>
struct context_traits_t<std::shared_ptr<ReflectableType>>
{
    static std::any context(std::any& object)
    {
        return object;
    }

    static std::shared_ptr<ReflectableType> access(std::any& context)
    {
        return std::any_cast<std::shared_ptr<ReflectableType>&>(context);
    }

    template <typename OtherReflectableType>
    static std::shared_ptr<OtherReflectableType> cast(std::any& context)
    {
        return std::static_pointer_cast<OtherReflectableType>(access(context));
    }
};

} // namespace meta

} // namespace rew

#endif // REW_CONTEXT_HPP
