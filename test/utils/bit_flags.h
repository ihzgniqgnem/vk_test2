#pragma once
#include <type_traits>

#include <gtest/gtest.h>
#include "../../header/utils/bit_flags.h"
namespace vkm_test::utils {
	TEST(BitFlags, small_flag)
	{
		enum class FlagIndex {
			Flag1, Flag2,
			FlagMax
		};
		vkm::utils::BitFlags<FlagIndex> x;
		EXPECT_EQ(sizeof(x), 1);
	}
	TEST(BitFlags, big_flag)
	{
		enum class FlagIndex {
			FlagMax = 10000
		};
		vkm::utils::BitFlags<FlagIndex> x;
		EXPECT_EQ(sizeof(x), 1250);
	}
	TEST(BitFlags, init) {
		enum class FlagIndex {
			Flag1, Flag2, Flag3, Flag4, Flag5, Flag6, Flag7, Flag8, Flag9, Flag10,
			FlagMax
		};
		unsigned char x[2];
		x[0] = x[1] = 0xff;
		auto* y = new(x) vkm::utils::BitFlags<FlagIndex>;
		EXPECT_EQ(x[0], 0);
		EXPECT_EQ(x[1], 0);
		y->~BitFlags();
	}
	TEST(BitFlags, basic_attr) {
		enum class FlagIndex {
			Flag1, Flag2, Flag3, Flag4, Flag5, Flag6, Flag7, Flag8, Flag9, Flag10,
			FlagMax
		};
		using BitFlagsType = vkm::utils::BitFlags<FlagIndex>;

		EXPECT_TRUE(std::is_default_constructible_v<BitFlagsType>);
		EXPECT_TRUE(std::is_copy_constructible_v<BitFlagsType>);
		EXPECT_TRUE(std::is_copy_assignable_v<BitFlagsType>);
		EXPECT_TRUE(std::is_move_constructible_v<BitFlagsType>);
		EXPECT_TRUE(std::is_move_assignable_v<BitFlagsType>);
		EXPECT_TRUE(std::is_destructible_v<BitFlagsType>);

		EXPECT_TRUE(std::is_trivially_copyable_v<BitFlagsType>);

		EXPECT_TRUE(std::is_nothrow_default_constructible_v<BitFlagsType>);
		EXPECT_TRUE(std::is_nothrow_copy_constructible_v<BitFlagsType>);
		EXPECT_TRUE(std::is_nothrow_copy_assignable_v<BitFlagsType>);
		EXPECT_TRUE(std::is_nothrow_move_constructible_v<BitFlagsType>);
		EXPECT_TRUE(std::is_nothrow_move_assignable_v<BitFlagsType>);
		EXPECT_TRUE(std::is_nothrow_destructible_v<BitFlagsType>);

		EXPECT_TRUE(std::is_standard_layout_v<BitFlagsType>);
	}
	TEST(BitFlags, op_eq)
	{
		enum class FlagIndex {
			Flag1, Flag2, Flag3, Flag4, Flag5, Flag6, Flag7, Flag8, Flag9, Flag10,
			FlagMax
		};
		vkm::utils::BitFlags<FlagIndex> x, y;

		EXPECT_TRUE(x == y);
		x[FlagIndex::Flag1] = true;
		EXPECT_TRUE(x != y);
		y[FlagIndex::Flag1] = true;
		EXPECT_TRUE(x == y);
	}
	TEST(BitFlags, write)
	{
		enum class FlagIndex {
			Flag1, Flag2, Flag3, Flag4, Flag5, Flag6, Flag7, Flag8, Flag9, Flag10,
			FlagMax
		};
		vkm::utils::BitFlags<FlagIndex> x;
		x[FlagIndex::Flag1] = true;
		EXPECT_TRUE(x[FlagIndex::Flag1]);
		x[FlagIndex::Flag2] = true;
		EXPECT_TRUE(x[FlagIndex::Flag2]);
		x[FlagIndex::Flag8] = true;
		EXPECT_TRUE(x[FlagIndex::Flag8]);
		x[FlagIndex::Flag9] = true;
		EXPECT_TRUE(x[FlagIndex::Flag9]);
		x[FlagIndex::Flag10] = true;
		EXPECT_TRUE(x[FlagIndex::Flag1]);
		EXPECT_TRUE(x[FlagIndex::Flag2]);
		EXPECT_TRUE(x[FlagIndex::Flag8]);
		EXPECT_TRUE(x[FlagIndex::Flag9]);
		EXPECT_TRUE(x[FlagIndex::Flag10]);
		auto [a, b] = x[{FlagIndex::Flag1, FlagIndex::Flag10 }];
		a = b = false;
		EXPECT_FALSE(x[FlagIndex::Flag1]);
		EXPECT_TRUE(x[FlagIndex::Flag2]);
		EXPECT_TRUE(x[FlagIndex::Flag8]);
		EXPECT_TRUE(x[FlagIndex::Flag9]);
		EXPECT_FALSE(x[FlagIndex::Flag10]);
	}
	TEST(BitFlags, batch_write)
	{
		enum class FlagIndex {
			Flag1, Flag2, Flag3, Flag4, Flag5, Flag6, Flag7, Flag8, Flag9, Flag10,
			FlagMax
		};
		vkm::utils::BitFlags<FlagIndex> x;
		x[{FlagIndex::Flag1, FlagIndex::Flag2, FlagIndex::Flag8, FlagIndex::Flag9, FlagIndex::Flag10}] = { true, true, true, true, true };
		EXPECT_TRUE(x[FlagIndex::Flag1]);
		EXPECT_TRUE(x[FlagIndex::Flag2]);
		EXPECT_TRUE(x[FlagIndex::Flag8]);
		EXPECT_TRUE(x[FlagIndex::Flag9]);
		EXPECT_TRUE(x[FlagIndex::Flag10]);
		x[{FlagIndex::Flag1, FlagIndex::Flag10}] = { false,false };
		EXPECT_FALSE(x[FlagIndex::Flag1]);
		EXPECT_TRUE(x[FlagIndex::Flag2]);
		EXPECT_TRUE(x[FlagIndex::Flag8]);
		EXPECT_TRUE(x[FlagIndex::Flag9]);
		EXPECT_FALSE(x[FlagIndex::Flag10]);
	}
	TEST(BitFlags, read) {
		enum class FlagIndex {
			Flag1, Flag2, Flag3, Flag4, Flag5, Flag6, Flag7, Flag8, Flag9, Flag10,
			FlagMax
		};
		vkm::utils::BitFlags<FlagIndex> x;
		x[{FlagIndex::Flag1, FlagIndex::Flag2, FlagIndex::Flag8, FlagIndex::Flag9, FlagIndex::Flag10}] = { true, true, true, true, true };
		const vkm::utils::BitFlags<FlagIndex>& y = const_cast<const vkm::utils::BitFlags<FlagIndex>&>(x);
		auto [a, b] = y[{FlagIndex::Flag1, FlagIndex::Flag3}];
		EXPECT_TRUE(a);
		EXPECT_FALSE(b);
		EXPECT_TRUE((std::is_same_v<std::remove_cvref_t<decltype(a)>,bool>));
		auto [c, d] = x[{FlagIndex::Flag1, FlagIndex::Flag3}];
		EXPECT_FALSE((std::is_same_v<std::remove_cvref_t<decltype(c)>, bool>));
	}
	template<typename>
	constexpr bool t = false;
	template<typename T>
		requires requires{ std::declval<vkm::utils::BitFlags<T>>();}
	constexpr bool t < T > = true;
	TEST(BitFlags, compiling_wrong_parameter)
	{
		class FlagIndex1 {};
		EXPECT_FALSE(t<FlagIndex1>);
		enum class FlagIndex2 {
			Flag1
		};
		EXPECT_FALSE(t<FlagIndex2>);
		enum class FlagIndex3 {
			FlagMax
		};
		EXPECT_FALSE(t<FlagIndex3>);
		enum class FlagIndex4 {
			Flag1, Flag2, Flag3, Flag4, Flag5, Flag6, Flag7, Flag8, Flag9, Flag10,
			FlagMax
		};
		EXPECT_TRUE(t<FlagIndex4>);
	}
	TEST(BitFlags, hash) {
		enum class FlagIndex {
			Flag1, Flag2, Flag3, Flag4, Flag5, Flag6, Flag7, Flag8, Flag9, Flag10,
			FlagMax
		};
		vkm::utils::BitFlags<FlagIndex> x;
		auto a = std::hash<decltype(x)>{}(x);
		x[FlagIndex::Flag1] = true;
		auto b = std::hash<decltype(x)>{}(x);
		x[FlagIndex::Flag1] = false;
		x[FlagIndex::Flag9] = true;
		auto c = std::hash<decltype(x)>{}(x);
		EXPECT_NE(a, b);
		EXPECT_NE(b, c);
		auto y = x;
		EXPECT_EQ(std::hash<decltype(x)>{}(x), std::hash<decltype(y)>{}(y));
	}
	TEST(BitFlags, proxy_array_assign) {
		enum class FlagIndex {
			Flag1, Flag2, Flag3, Flag4, Flag5, Flag6, Flag7, Flag8, Flag9, Flag10,
			FlagMax
		};
		vkm::utils::BitFlags<FlagIndex> x;
		x[FlagIndex::Flag1] = true;
		x[{FlagIndex::Flag1, FlagIndex::Flag9}] = x[{FlagIndex::Flag9, FlagIndex::Flag1}];
		EXPECT_FALSE(x[FlagIndex::Flag1]);
		EXPECT_TRUE(x[FlagIndex::Flag9]);
	}
}