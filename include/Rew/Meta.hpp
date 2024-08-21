#ifndef REW_META_HPP
#define REW_META_HPP

// .meta(name, expr)
#define META(name_str, ...)                                                                             \
    {                                                                                                   \
        auto xxlocalmeta = xxmeta->find(name_str);                                                      \
        if (xxlocalmeta == nullptr) xxlocalmeta = &xxmeta->add(name_str, __VA_ARGS__);                  \
        injection.template meta<CleanR, decltype(__VA_ARGS__)>(name_str, *xxlocalmeta);                 \
    }

#endif // REW_META_HPP
