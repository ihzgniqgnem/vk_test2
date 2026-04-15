#pragma once
#include <functional>
namespace vkm::utils {
	template<typename T, typename... Rest>
		requires requires{std::hash<T>{};(std::hash<Rest>{}, ...);}
	constexpr size_t hashCombine(const T& first, const Rest&... rest) {
		size_t seed = std::hash<T>{}(first);
		if constexpr (sizeof(size_t) == 8){
			((seed ^= (std::hash<Rest>{}(rest)+0x9e3779b97f4a7c15ull + (seed << 6) + (seed >> 2))), ...);
		} else {
			((seed ^= (std::hash<Rest>{}(rest)+0x9e3779b9u + (seed << 6) + (seed >> 2))), ...);
		}
		return seed;
	}
}