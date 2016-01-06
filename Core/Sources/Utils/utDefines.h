#define COUNT_OF_C_ARRAY(ary) (sizeof(ary)/sizeof(ary[0]))
#define BYTES_OF_STD_VECTOR(vec) (static_cast<uint32_t>(sizeof(decltype(vec.data())) * vec.size()))
#define CHECK(expr) checkSucceeded(expr);
#define PRINT(fmt, ...) print(fmt, ##__VA_ARGS__)

#define WIN_WIDTH 1280
#define WIN_HEIGHT 720