#pragma once
#include<gtest/gtest.h>
#include "../header/static_assertions.h"

#include "./utils/bit_flags.h"

int main(int argc, char** argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}