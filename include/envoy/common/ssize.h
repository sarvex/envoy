
#if defined(WIN32)
#if defined(_M_IX86)
typedef int32_t ssize_t;
#elif defined(_M_AMD64)
typedef int64_t ssize_t;
#else
#error add typedef for platform
#endif
#endif
