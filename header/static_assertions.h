#pragma once
static_assert(__cplusplus >= 202002L);
#include <type_traits>
#include <limits>
#include <climits>
#include <bit>
#include <cstdint>
#include <cfloat>
static_assert(std::numeric_limits<float>::is_iec559);
static_assert(std::numeric_limits<double>::is_iec559);
static_assert(FLT_HAS_SUBNORM == 1);
static_assert(DBL_HAS_SUBNORM == 1);
static_assert(CHAR_BIT == 8);
static_assert(sizeof(bool) == 1);
static_assert(sizeof(char) == 1);
static_assert(sizeof(short) == 2);
static_assert(sizeof(int) == 4);
static_assert(sizeof(long long) == 8);
static_assert(sizeof(float) == 4);
static_assert(sizeof(double) == 8);
static_assert(std::endian::native == std::endian::little);
static_assert(sizeof(char8_t) == 1);
static_assert(sizeof(char16_t) == 2);
static_assert(sizeof(char32_t) == 4);
static_assert(sizeof(size_t) == sizeof(void*));
#ifndef INT8_MAX
static_assert(false);
#endif
#ifndef INT16_MAX
static_assert(false);
#endif
#ifndef INT32_MAX
static_assert(false);
#endif
#ifndef INT64_MAX
static_assert(false);
#endif
#ifndef UINT8_MAX
static_assert(false);
#endif
#ifndef UINT16_MAX
static_assert(false);
#endif
#ifndef UINT32_MAX
static_assert(false);
#endif
#ifndef UINT64_MAX
static_assert(false);
#endif
static_assert(std::is_same_v<int8_t, signed char>);
static_assert(std::is_same_v<int16_t, short>);
static_assert(std::is_same_v<int32_t, int>);
static_assert(std::is_same_v<uint8_t, unsigned char>);
static_assert(std::is_same_v<uint16_t, unsigned short>);
static_assert(std::is_same_v<uint32_t, unsigned int>);
static_assert((std::is_same_v<int64_t, long long>&& std::is_same_v<uint64_t, unsigned long long>) ||
	(sizeof(long) == 8 && std::is_same_v<int64_t, long> && std::is_same_v<uint64_t, unsigned long>));