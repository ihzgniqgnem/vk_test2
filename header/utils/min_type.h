#pragma once
#include <cstdint>
#include <limits>
namespace vkm_impl::utils::UnsignedMinType {
	template<uint64_t x>
	constexpr auto min_type() {
		if constexpr (x <= std::numeric_limits<uint8_t>::max()) {
			return uint8_t{};
		}
		else if constexpr (x <= std::numeric_limits<uint16_t>::max()) {
			return uint16_t{};
		}
		else if constexpr (x <= std::numeric_limits<uint32_t>::max()) {
			return uint32_t{};
		}
		else {
			return uint64_t{};
		}
	}
}
namespace vkm::utils {
	template<uint64_t x>
	using UnsignedMinType = decltype(vkm_impl::utils::UnsignedMinType::min_type<x>());
}