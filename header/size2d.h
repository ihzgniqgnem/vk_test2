#pragma once
#include <cstdint>
#include <functional>

#include "./utils/hash_combine.h"
namespace vkm {
	class Size2D {
	public:
		uint32_t x, y;
		constexpr Size2D() : x(0), y(0) {}
		constexpr Size2D(uint32_t x, uint32_t y) : x(x), y(y) {}
		constexpr Size2D(const Size2D& other) noexcept = default;
		constexpr Size2D(Size2D&& other) noexcept = default;
		constexpr Size2D& operator=(const Size2D& other) noexcept = default;
		constexpr Size2D& operator=(Size2D&& other) noexcept = default;
		constexpr bool operator==(const Size2D& other) const = default;
	};
}
namespace std {
	template<>
	struct hash<vkm::Size2D> {
		constexpr size_t operator()(const vkm::Size2D& k) const noexcept {
			return vkm::utils::hashCombine(k.x, k.y);
		}
	};
}