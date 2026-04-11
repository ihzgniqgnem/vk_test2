#ifndef ENABLE_TEST
#define ENABLE_TEST 1
#endif
#if ENABLE_TEST
#undef ENABLE_TEST
#include "test/test.h"
#else
#undef ENABLE_TEST
#include "temp_entry.h"
#endif
