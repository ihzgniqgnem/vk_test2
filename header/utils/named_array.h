#pragma once
#include <type_traits>
#include <cstring>
#include <string_view>
#include "is_enum_class.h"
#include "min_type.h"
#include "hash_combine.h"

namespace vkm::utils {
	template<typename T, typename U>
		requires is_enum_class_v<T>&& requires{T::ArrayMax;} && ((size_t)T::ArrayMax > 0)
	class NamedArray;
}

namespace vkm_impl::utils::NamedArray {
	using namespace vkm::utils;
	template<size_t l,typename U>
	class ProxyArray {
		U* data[l];
		template<typename T, typename U>
			requires is_enum_class_v<T>&& requires{T::ArrayMax;} && ((size_t)T::ArrayMax > 0)
		friend class NamedArray;
	public:
		template<size_t index>
		constexpr U& get() noexcept {
			return *data[index];
		}
		constexpr U& operator[](size_t index) noexcept {
			return *data[index];
		}
		template<size_t index>
		constexpr const U& get() const noexcept {
			return *data[index];
		}
		constexpr const U& operator[](size_t index) const noexcept {
			return *data[index];
		}
		constexpr size_t size() const noexcept {
			return l;
		}
		constexpr auto& operator=(const U(&args)[l]) noexcept(std::is_nothrow_copy_constructible_v<U>) {
			for (size_t i = 0;i < l;i++) {
				*data[i] = args[i];
			}
			return *this;
		}
		constexpr auto& operator=(const ProxyArray& other) noexcept(std::is_nothrow_copy_constructible_v<U>) {
			std::array<U,l> temp_other;
			for (size_t i = 0;i < l;i++) {
				temp_other[i]=*other.data[i];
			}
			for (size_t i = 0;i < l;i++) {
				*data[i] = temp_other[i];
			}
			return *this;
		}
		constexpr bool operator==(const ProxyArray& other) const noexcept(std::declval<const U&>() != std::declval<const U&>()) {
			for (size_t i = 0;i < l;i++) {
				if (*data[i] != *other.data[i])return false;
			}
			return true;
		}
	};
}
namespace vkm::utils {
	template<typename T,typename U>
		requires is_enum_class_v<T>&& requires{T::ArrayMax;} && ((size_t)T::ArrayMax > 0)
	class NamedArray {
		using P = std::underlying_type_t<T>;
		static constexpr size_t data_length = (size_t)T::ArrayMax;
		U data[data_length];
		friend struct std::hash<NamedArray<T,U>>;
	public:
		constexpr NamedArray<T,U>() noexcept = default;
		constexpr NamedArray<T, U>(const NamedArray<T, U>&) = default;
		constexpr NamedArray<T, U>(NamedArray<T, U>&&) = default;
		constexpr NamedArray<T, U>& operator=(const NamedArray<T, U>&) = default;
		constexpr NamedArray<T, U>& operator=(NamedArray<T, U>&&) = default;
		constexpr U operator[](const T enum_index) noexcept {
			return data[static_cast<P>(enum_index)];
		}
		constexpr const U operator[](const T enum_index) const noexcept {
			return data[static_cast<P>(enum_index)];
		}
		template<size_t l>
		constexpr auto operator[](const T(&enum_indexes)[l]) noexcept {
			vkm_impl::utils::NamedArray::ProxyArray<l,U> res;
			P index;
			for (size_t i = 0;i < l;i++) {
				index = static_cast<P>(enum_indexes[i]);
				res.data[i] = &data[index];
			}
			return res;
		}
		template<size_t l>
		constexpr const auto operator[](const T(&enum_indexes)[l]) const noexcept {
			vkm_impl::utils::NamedArray::ProxyArray<l, U> res;
			P index;
			for (size_t i = 0;i < l;i++) {
				index = static_cast<P>(enum_indexes[i]);
				res.data[i] = &data[index];
			}
			return res;
		}
		constexpr bool operator==(const NamedArray& other) const noexcept(std::declval<const U&>() != std::declval<const U&>()) {
			for (size_t i = 0;i < data_length;i++) {
				if (data[i] != other.data[i])return false;
			}
			return true;
		}
		constexpr size_t size() const noexcept {
			return data_length;
		}
		constexpr U* begin() noexcept {
			return data;
		}
		constexpr U* end() noexcept {
			return data + data_length;
		}
		constexpr const U* begin() const noexcept {
			return data;
		}
		constexpr const U* end() const noexcept {
			return data + data_length;
		}
	};
}
namespace std {
	template<typename T,typename U>
	struct hash<vkm::utils::NamedArray<T,U>> {
	private:
		static constexpr auto U_hash = std::hash<std::string_view>{};
		template<size_t... Is>
		static constexpr auto helper(const vkm::utils::NamedArray<T, U>& array,std::index_sequence<Is...>) noexcept {
			return vkm::utils::hashCombine(array[Is]...);
		}
	public:
		size_t operator()(const vkm::utils::NamedArray<T,U>& array) const noexcept {
			return helper(array, make_index_sequence<array::data_length>{});
		}
	};
}