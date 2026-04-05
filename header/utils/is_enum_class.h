#pragma once
#include <type_traits>
namespace vkm::utils {
	template<class T>
	constexpr bool is_enum_class_v = std::is_enum_v<T> && !std::is_convertible_v<T, int>;
}
