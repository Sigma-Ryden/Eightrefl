#ifndef REW_CAST_HPP
#define REW_CAST_HPP

#include <string> // string
#include <any> // any

#include <memory> // unique_otr, weak_ptr, shared_ptr
#include <functional> // function

#include <Rew/Attribute.hpp>
#include <Rew/Meta.hpp>

namespace rew
{

struct cast_meta_t
{
    const std::function<void*(std::any&)> call = nullptr;
};

template <typename Type>
struct cast_type_traits;

template <typename Type>
auto cast_call_handler()
{
    return [](std::any& object) -> void*
    {
        return cast_type_traits<Type>{}(object);
    };
}

template <typename Type>
struct cast_type_traits
{
    void* operator()(std::any& object)
    {
        return std::addressof(std::any_cast<Type&>(object));
    }
};

template <typename Type>
struct cast_type_traits<std::unique_ptr<Type>>
{
    void* operator()(std::any& object)
    {
        return std::any_cast<std::unique_ptr<Type>&>(object).get();
    }
};

template <typename Type>
struct cast_type_traits<std::weak_ptr<Type>>
{
    void* operator()(std::any& object)
    {
        return std::any_cast<std::weak_ptr<Type>&>(object).lock().get();
    }
};

template <typename Type>
struct cast_type_traits<std::shared_ptr<Type>>
{
    void* operator()(std::any& object)
    {
        return std::any_cast<std::shared_ptr<Type>&>(object).get();
    }
};

} // namespace rew

#endif // REW_CAST_HPP
