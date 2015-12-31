
#ifdef _MSC_VER
#define cbuffer struct __declspec(align(16))
#else
#define cbuffer alignas(16)
#endif
