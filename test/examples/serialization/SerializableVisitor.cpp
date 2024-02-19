#include "SerializableVisitor.hpp"

void reflectable_saveload(sf::core::IOArchive& archive, void* context, std::type_index typeindex, const char* mode)
{
    using saveload_function_t = serializable_visitor_t::saveload_function_t;

    auto type = rew::global.find(typeindex);

    for (auto& [name, parent] : type->reflection->parent.all)
    {
        auto& saveload = std::any_cast<saveload_function_t&>(*parent.meta.find(mode));
        saveload(archive, context);
    }

    for (auto& [name, property] : type->reflection->property.all)
    {
        auto& saveload = std::any_cast<saveload_function_t&>(*property.meta.find(mode));
        saveload(archive, property.ptr(context));
    }
}
