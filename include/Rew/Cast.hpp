#ifndef REW_CAST_HPP
#define REW_CAST_HPP

#include <any> // any

#include <memory> // unique_otr, weak_ptr, shared_ptr
#include <utility> // addressof

namespace rew
{

template <typename Type>
struct cast_type_traits;

template <typename Type>
struct cast_type_traits
{
    void* operator()(std::any& object) const
    {
        return std::addressof(std::any_cast<Type&>(object));
    }
};

template <typename Type>
struct cast_type_traits<std::unique_ptr<Type>>
{
    void* operator()(std::any& object) const
    {
        return std::any_cast<std::unique_ptr<Type>&>(object).get();
    }
};

template <typename Type>
struct cast_type_traits<std::weak_ptr<Type>>
{
    void* operator()(std::any& object) const
    {
        return std::any_cast<std::weak_ptr<Type>&>(object).lock().get();
    }
};

template <typename Type>
struct cast_type_traits<std::shared_ptr<Type>>
{
    void* operator()(std::any& object) const
    {
        return std::any_cast<std::shared_ptr<Type>&>(object).get();
    }
};

} // namespace rew

#endif // REW_CAST_HPP
