#define USE_ASSERTS 1

#if USE_ASSERTS == 1

#define ASSERT2(condition, message, ...) if (!(condition)) { \
    APP_LOG(APP_LOG_LEVEL_ERROR, "Assertion failure at %s:%d : " message, __FILE__, __LINE__, ##__VA_ARGS__); \
    int* crash = 0; \
    *crash = 1; \
}

#else

static inline _nop() {}
#define ASSERT2(condition, message, ...) _nop()

#endif

#define ASSERT(condition) ASSERT2(condition, #condition)
