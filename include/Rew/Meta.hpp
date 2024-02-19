#ifndef REW_META_HPP
#define REW_META_HPP

#define RAW_META(meta_name_str, ...)                                                                    \
    {                                                                                                   \
        auto __meta = ::rew::find_or_add_meta(__reflection, meta_name_str, __VA_ARGS__);                \
        injection.template meta<R, decltype(__VA_ARGS__)>(meta_name_str, *__meta);                      \
    }

#define META(name, ...) RAW_META(name, __VA_ARGS__)

#endif // REW_META_HPP
