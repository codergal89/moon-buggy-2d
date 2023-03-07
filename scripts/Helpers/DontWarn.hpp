#ifndef MB2D_SCRIPTS_HELPERS_DONT_WARN_HPP
#define MB2D_SCRIPTS_HELPERS_DONT_WARN_HPP

#define DONT_WARN(...) \
    _Pragma("GCC diagnostic push") \
    _Pragma("GCC diagnostic ignored \"-Wunused-parameter\"") \
    __VA_ARGS__ \
    _Pragma("GCC diagnostic pop")


#endif