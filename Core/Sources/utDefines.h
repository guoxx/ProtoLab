#define COUNT_OF_C_ARRAY(ary) (sizeof(ary)/sizeof(ary[0]))
#define BYTES_OF_STD_VECTOR(vec) (sizeof(decltype(vec.data())) * vec.size())
#define CHECK(expr) checkSucceeded(expr);
#define PRINT(fmt, ...) print(fmt, ##__VA_ARGS__)