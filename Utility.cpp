typedef char s8;
typedef unsigned char u8;
typedef short u16;
typedef unsigned short s18;
typedef int s32;
typedef unsigned int u32;
typedef long long s64;
typedef unsigned long long u64;

#define global_variable static
#define internal static

inline  int clamp(int min, int num, int max) {
	if (num < min) {
		return min;
	}
	if (num > max) {
		return max;
	}
	return num;
}