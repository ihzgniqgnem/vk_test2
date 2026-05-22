#pragma once
#include <gtest/gtest.h>
#include "../../header/utils/func_table.h"

namespace vkm_test::utils::FuncTable {
	// 测试用的枚举类
	enum class TestFuncEnum {
		Func0,
		Func1,
		Func2,
		FuncMax = 3
	};

	// 测试用的函数类型
	using TestFuncType = vkm::utils::FuncTableType<
		void(*)(),
		int(*)(int),
		const char* (*)(const char*)
	>;

	// 模拟的测试函数
	void testFunc0() {}
	int testFunc1(int x) { return x + 1; }
	const char* testFunc2(const char* s) { return s; }

	TEST(FuncTable, small_table)
	{
		enum class E {
			F0, F1, F2,
			FuncMax
		};
		using FT = vkm::utils::FuncTableType<void(*)()>;
		vkm::utils::FuncTable<E, FT> x;
		EXPECT_EQ(sizeof(x), 3 * sizeof(void*));
	}

	TEST(FuncTable, big_table)
	{
		enum class E {
			FuncMax = 1000
		};
		using FT = vkm::utils::FuncTableType<void(*)()>;
		vkm::utils::FuncTable<E, FT> x;
		EXPECT_EQ(sizeof(x), 1000 * sizeof(void*));
	}

	TEST(FuncTable, init)
	{
		enum class E {
			F0, F1, F2, F3, F4,
			FuncMax
		};
		using FT = vkm::utils::FuncTableType<void(*)(), int(*)(), void(*)(), int(*)(), void(*)()>;
		alignas(vkm::utils::FuncTable<E, FT>) unsigned char buffer[sizeof(vkm::utils::FuncTable<E, FT>)];
		std::memset(buffer, 0xff, sizeof(buffer));
		auto* y = new(buffer) vkm::utils::FuncTable<E, FT>;
		for (size_t i = 0; i < static_cast<size_t>(E::FuncMax); ++i) {
			EXPECT_EQ((*y)[static_cast<E>(i)], static_cast<void*>(nullptr));
		}
		y->~FuncTable();
	}

	TEST(FuncTable, basic_attr)
	{
		enum class E {
			F0, F1, F2,
			FuncMax
		};
		using FuncTableType2 = vkm::utils::FuncTable<E, TestFuncType>;

		EXPECT_TRUE(std::is_default_constructible_v<FuncTableType2>);
		EXPECT_TRUE(std::is_copy_constructible_v<FuncTableType2>);
		EXPECT_TRUE(std::is_copy_assignable_v<FuncTableType2>);
		EXPECT_TRUE(std::is_move_constructible_v<FuncTableType2>);
		EXPECT_TRUE(std::is_move_assignable_v<FuncTableType2>);
		EXPECT_TRUE(std::is_destructible_v<FuncTableType2>);

		EXPECT_TRUE(std::is_trivially_copyable_v<FuncTableType2>);

		EXPECT_TRUE(std::is_nothrow_default_constructible_v<FuncTableType2>);
		EXPECT_TRUE(std::is_nothrow_copy_constructible_v<FuncTableType2>);
		EXPECT_TRUE(std::is_nothrow_copy_assignable_v<FuncTableType2>);
		EXPECT_TRUE(std::is_nothrow_move_constructible_v<FuncTableType2>);
		EXPECT_TRUE(std::is_nothrow_move_assignable_v<FuncTableType2>);
		EXPECT_TRUE(std::is_nothrow_destructible_v<FuncTableType2>);

		EXPECT_TRUE(std::is_standard_layout_v<FuncTableType2>);
	}

	TEST(FuncTable, DefaultConstruction) {
		vkm::utils::FuncTable<TestFuncEnum, TestFuncType> table;
		EXPECT_EQ(table.size(), size_t(3));

		// 默认构造的表应该都是nullptr
		for (size_t i = 0; i < table.size(); ++i) {
			EXPECT_EQ(table[static_cast<TestFuncEnum>(i)], static_cast<void*>(nullptr));
		}
	}

	TEST(FuncTable, AssignmentAndAccess) {
		vkm::utils::FuncTable<TestFuncEnum, TestFuncType> table;

		// 使用Proxy赋值
		table[TestFuncEnum::Func0] = testFunc0;
		table[TestFuncEnum::Func1] = testFunc1;
		table[TestFuncEnum::Func2] = testFunc2;

		// 验证赋值结果
		EXPECT_EQ(table[TestFuncEnum::Func0], reinterpret_cast<void*>(testFunc0));
		EXPECT_EQ(table[TestFuncEnum::Func1], reinterpret_cast<void*>(testFunc1));
		EXPECT_EQ(table[TestFuncEnum::Func2], reinterpret_cast<void*>(testFunc2));

		// 使用get方法访问
		auto& func0 = table.template get<TestFuncEnum::Func0>();
		auto& func1 = table.template get<TestFuncEnum::Func1>();
		auto& func2 = table.template get<TestFuncEnum::Func2>();

		EXPECT_EQ(func0, testFunc0);
		EXPECT_EQ(func1, testFunc1);
		EXPECT_EQ(func2, testFunc2);

		// 测试函数调用
		func0();
		EXPECT_EQ(func1(5), 6);
		EXPECT_STREQ(func2("hello"), "hello");
	}

	TEST(FuncTable, const_access) {
		vkm::utils::FuncTable<TestFuncEnum, TestFuncType> table;
		table[TestFuncEnum::Func0] = testFunc0;
		table[TestFuncEnum::Func1] = testFunc1;
		table[TestFuncEnum::Func2] = testFunc2;

		const auto& ctable = table;

		// const operator[] returns const void* const&
		EXPECT_EQ(ctable[TestFuncEnum::Func0], reinterpret_cast<void*>(testFunc0));
		EXPECT_EQ(ctable[TestFuncEnum::Func1], reinterpret_cast<void*>(testFunc1));
		EXPECT_EQ(ctable[TestFuncEnum::Func2], reinterpret_cast<void*>(testFunc2));
	}

	TEST(FuncTable, CopyAndMoveSemantics) {
		vkm::utils::FuncTable<TestFuncEnum, TestFuncType> original;
		original[TestFuncEnum::Func0] = testFunc0;
		original[TestFuncEnum::Func1] = testFunc1;

		// 测试拷贝构造
		vkm::utils::FuncTable<TestFuncEnum, TestFuncType> copy(original);
		EXPECT_EQ(copy[TestFuncEnum::Func0], original[TestFuncEnum::Func0]);
		EXPECT_EQ(copy[TestFuncEnum::Func1], original[TestFuncEnum::Func1]);

		// 测试拷贝赋值
		vkm::utils::FuncTable<TestFuncEnum, TestFuncType> copy_assign;
		copy_assign = original;
		EXPECT_EQ(copy_assign[TestFuncEnum::Func0], original[TestFuncEnum::Func0]);
		EXPECT_EQ(copy_assign[TestFuncEnum::Func1], original[TestFuncEnum::Func1]);

		// 测试移动构造
		vkm::utils::FuncTable<TestFuncEnum, TestFuncType> move_constructed(std::move(original));
		EXPECT_EQ(move_constructed[TestFuncEnum::Func0], reinterpret_cast<void*>(testFunc0));
		EXPECT_EQ(move_constructed[TestFuncEnum::Func1], reinterpret_cast<void*>(testFunc1));

		// 原对象应该仍然有效（因为是默认移动语义，plain array逐元素移动=拷贝）
		EXPECT_EQ(original[TestFuncEnum::Func0], reinterpret_cast<void*>(testFunc0));
	}

	TEST(FuncTable, move_assignment) {
		vkm::utils::FuncTable<TestFuncEnum, TestFuncType> src;
		src[TestFuncEnum::Func0] = testFunc0;
		src[TestFuncEnum::Func2] = testFunc2;

		vkm::utils::FuncTable<TestFuncEnum, TestFuncType> dst;
		dst[TestFuncEnum::Func1] = testFunc1;

		dst = std::move(src);
		EXPECT_EQ(dst[TestFuncEnum::Func0], reinterpret_cast<void*>(testFunc0));
		EXPECT_EQ(dst[TestFuncEnum::Func1], static_cast<void*>(nullptr));   // 被覆盖
		EXPECT_EQ(dst[TestFuncEnum::Func2], reinterpret_cast<void*>(testFunc2));

		}

	TEST(FuncTable, EqualityComparison) {
		vkm::utils::FuncTable<TestFuncEnum, TestFuncType> table1;
		vkm::utils::FuncTable<TestFuncEnum, TestFuncType> table2;

		// 两个默认构造的表应该相等
		EXPECT_TRUE(table1 == table2);

		// 给table1赋值
		table1[TestFuncEnum::Func0] = testFunc0;
		table1[TestFuncEnum::Func1] = testFunc1;

		// 现在应该不相等
		EXPECT_FALSE(table1 == table2);

		// 给table2赋相同的值
		table2[TestFuncEnum::Func0] = testFunc0;
		table2[TestFuncEnum::Func1] = testFunc1;

		// 现在应该相等
		EXPECT_TRUE(table1 == table2);
	}

	TEST(FuncTable, operator_ne)
	{
		vkm::utils::FuncTable<TestFuncEnum, TestFuncType> x, y;

		// 默认构造相等 → != 为 false
		EXPECT_FALSE(x != y);

		x[TestFuncEnum::Func0] = testFunc0;
		EXPECT_TRUE(x != y);

		y[TestFuncEnum::Func0] = testFunc0;
		EXPECT_FALSE(x != y);
	}

	TEST(FuncTable, IteratorInterface) {
		vkm::utils::FuncTable<TestFuncEnum, TestFuncType> table;
		table[TestFuncEnum::Func0] = testFunc0;
		table[TestFuncEnum::Func1] = testFunc1;
		table[TestFuncEnum::Func2] = testFunc2;

		// 测试begin/end迭代器
		size_t count = 0;
		for (auto it = table.begin(); it != table.end(); ++it) {
			EXPECT_NE(*it, static_cast<void*>(nullptr));
			count++;
		}
		EXPECT_EQ(count, size_t(3));

		// 测试const迭代器
		const auto& const_table = table;
		count = 0;
		for (auto it = const_table.begin(); it != const_table.end(); ++it) {
			EXPECT_NE(*it, static_cast<void*>(nullptr));
			count++;
		}
		EXPECT_EQ(count, size_t(3));

		// range-based for
		count = 0;
		for (auto ptr : table) {
			EXPECT_NE(ptr, static_cast<void*>(nullptr));
			count++;
		}
		EXPECT_EQ(count, size_t(3));

		// const range-based for
		const auto& ctable = table;
		count = 0;
		for (auto ptr : ctable) {
			EXPECT_NE(ptr, static_cast<void*>(nullptr));
			count++;
		}
		EXPECT_EQ(count, size_t(3));
	}

	TEST(FuncTable, HashFunction) {
		vkm::utils::FuncTable<TestFuncEnum, TestFuncType> table1;
		vkm::utils::FuncTable<TestFuncEnum, TestFuncType> table2;

		// 相同内容应该有相同哈希值
		std::hash<vkm::utils::FuncTable<TestFuncEnum, TestFuncType>> hasher;
		EXPECT_EQ(hasher(table1), hasher(table2));

		// 不同内容应该有不同的哈希值（大概率）
		table1[TestFuncEnum::Func0] = testFunc0;
		EXPECT_NE(hasher(table1), hasher(table2));

		// 哈希一致性：同一个对象多次求哈希应该相同
		EXPECT_EQ(hasher(table1), hasher(table1));
	}

	TEST(FuncTable, TupleInterface) {
		vkm::utils::FuncTable<TestFuncEnum, TestFuncType> table;
		table[TestFuncEnum::Func0] = testFunc0;
		table[TestFuncEnum::Func1] = testFunc1;

		// 测试tuple_size
		EXPECT_EQ(std::tuple_size_v<decltype(table)>, size_t(3));

		// 测试tuple_element
		using ElementType0 = std::tuple_element_t<0, decltype(table)>;
		using ElementType1 = std::tuple_element_t<1, decltype(table)>;
		using ElementType2 = std::tuple_element_t<2, decltype(table)>;

		EXPECT_TRUE((std::is_same_v<ElementType0, void(*)()>));
		EXPECT_TRUE((std::is_same_v<ElementType1, int(*)(int)>));
		EXPECT_TRUE((std::is_same_v<ElementType2, const char* (*)(const char*)>));
	}

	// ==================== Proxy 边缘情况 ====================

	TEST(FuncTable, proxy_nullptr_assign)
	{
		enum class E {
			F0, F1,
			FuncMax
		};
		using FT = vkm::utils::FuncTableType<void(*)(), int(*)(int)>;
		vkm::utils::FuncTable<E, FT> table;
		table[E::F0] = testFunc0;
		table[E::F1] = testFunc1;

		// 通过Proxy赋值nullptr
		table[E::F0] = nullptr;
		EXPECT_EQ(table[E::F0], static_cast<void*>(nullptr));
		EXPECT_NE(table[E::F1], static_cast<void*>(nullptr));

		// 通过void*赋值nullptr
		table[E::F0] = static_cast<void*>(nullptr);
		EXPECT_EQ(table[E::F0], static_cast<void*>(nullptr));
	}

	TEST(FuncTable, proxy_comparison)
	{
		enum class E {
			F0, F1,
			FuncMax
		};
		using FT = vkm::utils::FuncTableType<void(*)(), void(*)()>;
		vkm::utils::FuncTable<E, FT> table;

		// Proxy == Proxy
		EXPECT_TRUE(table[E::F0] == table[E::F1]);  // both nullptr

		table[E::F0] = testFunc0;
		EXPECT_FALSE(table[E::F0] == table[E::F1]);

		table[E::F1] = testFunc0;
		EXPECT_TRUE(table[E::F0] == table[E::F1]);

		// Proxy == void*
		EXPECT_TRUE(table[E::F0] == reinterpret_cast<void*>(testFunc0));
		EXPECT_TRUE(table[E::F1] == reinterpret_cast<void*>(testFunc0));
		EXPECT_FALSE(table[E::F0] == nullptr);

		// void* conversion
		void* p = table[E::F0];
		EXPECT_EQ(p, reinterpret_cast<void*>(testFunc0));
		const void* cp = table[E::F0];
		EXPECT_EQ(cp, reinterpret_cast<void*>(testFunc0));
	}

	TEST(FuncTable, proxy_copy_and_move)
	{
		enum class E {
			F0, F1,
			FuncMax
		};
		using FT = vkm::utils::FuncTableType<void(*)(), int(*)(int)>;
		vkm::utils::FuncTable<E, FT> table;
		table[E::F0] = testFunc0;

		auto proxy0 = table[E::F0];   // Proxy copy
		auto proxy1 = table[E::F1];   // Proxy copy
		EXPECT_EQ(proxy0, reinterpret_cast<void*>(testFunc0));
		EXPECT_EQ(proxy1, static_cast<void*>(nullptr));

		// 通过拷贝的proxy修改原表
		proxy1 = testFunc1;
		EXPECT_EQ(table[E::F1], reinterpret_cast<void*>(testFunc1));

		// Proxy move
		auto proxy2 = table[E::F0];
		EXPECT_EQ(proxy2, reinterpret_cast<void*>(testFunc0));
	}

	TEST(FuncTable, proxy_self_assign)
	{
		enum class E {
			F0,
			FuncMax
		};
		using FT = vkm::utils::FuncTableType<void(*)()>;
		vkm::utils::FuncTable<E, FT> table;
		table[E::F0] = testFunc0;

		// self-assign via proxy (operator[] returns by value; copy + assign is safe)
		auto proxy = table[E::F0];
		proxy = proxy;
		EXPECT_EQ(proxy, reinterpret_cast<void*>(testFunc0));
	}

	// ==================== FuncTableType 直接测试 ====================

	TEST(FuncTable, func_table_type)
	{
		using FT = vkm::utils::FuncTableType<void(*)(), int(*)(int, float), const char* (*)()>;
		EXPECT_EQ(FT::size, size_t(3));
		EXPECT_TRUE((std::is_same_v<FT::P<0>, void(*)()>));
		EXPECT_TRUE((std::is_same_v<FT::P<1>, int(*)(int, float)>));
		EXPECT_TRUE((std::is_same_v<FT::P<2>, const char* (*)()>));
	}

	// ==================== SFINAE 约束验证 ====================

	template<typename>
	constexpr bool t = false;
	template<typename T>
		requires requires { std::declval<vkm::utils::FuncTable<T, TestFuncType>>(); }
	constexpr bool t<T> = true;

	TEST(FuncTable, compiling_wrong_parameter)
	{
		// 非enum类
		class NotEnum {};
		EXPECT_FALSE(t<NotEnum>);

		// enum但没有FuncMax
		enum class NoFuncMax { A, B };
		EXPECT_FALSE(t<NoFuncMax>);

		// enum有FuncMax=0 (requires中 (size_t)T::FuncMax > 0 为false)
		enum class ZeroFuncMax { FuncMax };
		EXPECT_FALSE(t<ZeroFuncMax>);

		// 有效enum + FuncMax > 0
		enum class ValidEnum { A, B, C, FuncMax };
		EXPECT_TRUE(t<ValidEnum>);
	}

	// ==================== 大枚举的完整功能测试 ====================

	enum class BigEnum {
		F0, F1, F2, F3, F4, F5, F6, F7, F8, F9,
		FuncMax
	};

	using BigFuncType = vkm::utils::FuncTableType<
		void(*)(), int(*)(), void(*)(), int(*)(), void(*)(),
		int(*)(), void(*)(), int(*)(), void(*)(), int(*)()
	>;

	int g_counter = 0;
	void counterFunc() { g_counter++; }
	static int identity(int x) { return x; }

	TEST(FuncTable, large_enum_exhaustive)
	{
		vkm::utils::FuncTable<BigEnum, BigFuncType> table;

		// 全部赋值
		for (size_t i = 0; i < table.size(); ++i) {
			if (i % 2 == 0)
				table[static_cast<BigEnum>(i)] = counterFunc;
			else
				table[static_cast<BigEnum>(i)] = identity;
		}

		// 遍历验证
		size_t count = 0;
		for (auto ptr : table) {
			EXPECT_NE(ptr, static_cast<void*>(nullptr));
			count++;
		}
		EXPECT_EQ(count, size_t(10));

		// 相等性：构造副本
		auto copy = table;
		EXPECT_TRUE(table == copy);

		// 修改一个元素后不相等
		table[BigEnum::F5] = nullptr;
		EXPECT_FALSE(table == copy);

		// hash
		std::hash<decltype(table)> hasher;
		EXPECT_NE(hasher(table), hasher(copy));
	}
}