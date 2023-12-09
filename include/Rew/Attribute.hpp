#ifndef REW_ATTRIBUTE_HPP
#define REW_ATTRIBUTE_HPP

#include <string> // string

namespace rew
{

template <class ClassType>
struct attribute_t
{
    auto find(const std::string& name)
    {
        auto it = self()->all.find(name);
        return it != self()->all.end() ? &it->second : nullptr;
    }

    template <class MetaType>
    bool add(const std::string& name, const MetaType& meta)
    {
        return self()->all.emplace(name, meta).second;
    }

    bool remove(const std::string& name)
    {
        return self()->all.erase(name)>0;
    }

private:
    ClassType* self()
    {
        return static_cast<ClassType*>(this);
    }
};

} // namespace rew

#endif // REW_ATTRIBUTE_HPP
