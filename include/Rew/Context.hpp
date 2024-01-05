#ifndef REW_CONTEXT_HPP
#define REW_CONTEXT_HPP

#include <any> // any

#include <memory> // unique_otr, weak_ptr, shared_ptr
#include <utility> // addressof

namespace rew
{

namespace meta
{

template <typename Type>
struct context_traits_t;

template <typename Type>
struct context_traits_t
{
    void* operator()(std::any& object) const
    {
        return std::addressof(std::any_cast<Type&>(object));
    }
};

//template <typename Type>
//struct context_traits_t<std::unique_ptr<Type>>
//{
//    void* operator()(std::any& object) const
//    {
//        return std::any_cast<std::unique_ptr<Type>&>(object).get();
//    }
//};

//template <typename Type>
//struct context_traits_t<std::weak_ptr<Type>>
//{
//    void* operator()(std::any& object) const
//    {
//        return std::any_cast<std::weak_ptr<Type>&>(object).lock().get();
//    }
//};

//template <typename Type>
//struct context_traits_t<std::shared_ptr<Type>>
//{
//    void* operator()(std::any& object) const
//    {
//        return std::any_cast<std::shared_ptr<Type>&>(object).get();
//    }
//};

} // namespace meta

} // namespace rew

#endif // REW_CONTEXT_HPP
