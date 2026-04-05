#pragma once
#include <functional>
namespace vkm::utils {
	template<typename T, typename... Rest>
		requires requires{std::hash<T>{};(std::hash<Rest>{}, ...);}
	constexpr size_t hashCombine(const T& first, const Rest&... rest) {
		size_t seed = std::hash<T>{}(first);
		((seed ^= (std::hash<Rest>{}(rest)+size_t(0x9e3779b97f4a7c15ull) + (seed << 6) + (seed >> 2))), ...);
		return seed;
	}
}