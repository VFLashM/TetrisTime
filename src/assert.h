#define USE_ASSERTS 1

#if USE_ASSERTS == 1

static void _crash(const char* file, int line, const char* message) {
    APP_LOG(APP_LOG_LEVEL_ERROR, "Assertion failure at %s:%d%s", file, line, message);
    int* crash = 0;
    *crash = 1;
}
#define _ASSERT(condition, message) if (!(condition)) _crash(__FILE__, __LINE__, message)

#else

static inline _nop() {}
#define _ASSERT(condition, message) _nop()

#endif

#define ASSERT2(condition, message) _ASSERT(condition, " : " message)
#define ASSERT(condition) _ASSERT(condition, "")
