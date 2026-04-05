#pragma once
#include <type_traits>
#include <cstring>
#include <string_view>
#include "is_enum_class.h"

namespace vkm::utils {
	template<typename T>
		requires is_enum_class_v<T>&& requires{T::FlagMax;} && ((size_t)T::FlagMax > 0)
	class BitFlags;
}
namespace vkm_impl::utils::BitFlags {
	using namespace vkm::utils;
	class Proxy {
		uint8_t* raw_data;
		uint8_t index;
		template<typename T>
			requires is_enum_class_v<T>&& requires{T::FlagMax;} && ((size_t)T::FlagMax > 0)
		friend class BitFlags;
		template<size_t>
		friend class ProxyArray;
		constexpr Proxy(uint8_t& raw_data, uint8_t index) :raw_data(&raw_data), index(index){}
	public:
		constexpr Proxy() noexcept = default;
		constexpr Proxy(const Proxy&) noexcept = default;
		constexpr Proxy(Proxy&&) noexcept = default;
		constexpr Proxy& operator=(const Proxy& other) noexcept = default;
		constexpr Proxy& operator=(Proxy&& other) noexcept = default;
		constexpr operator bool() const noexcept {
			return (*raw_data >> index) & 1u;
		}
		constexpr bool operator=(bool other) noexcept {
			*raw_data = *raw_data & ~(1u << index) | (other << index);
			return other;
		}
	};
	template<size_t l>
	class BoolArray {
		bool data[l];
		template<typename T>
			requires is_enum_class_v<T>&& requires{T::FlagMax;} && ((size_t)T::FlagMax > 0)
		friend class BitFlags;
		template<size_t>
		friend class ProxyArray;
		constexpr BoolArray() noexcept = default;
	public:
		template<typename... Args>
			requires (std::is_same_v<bool, Args> && ...) && (sizeof...(Args) == l)
		constexpr BoolArray(Args... args) noexcept {
			size_t i = 0;
			((data[i++] = args), ...);
		}
		template<size_t index>
		constexpr bool get() const noexcept {
			return data[index];
		}
		constexpr bool operator[](size_t index) const noexcept {
			return data[index];
		}
		constexpr size_t size() const noexcept {
			return l;
		}
	};
	template<size_t l>
	class ProxyArray {
		Proxy data[l];
		template<typename T>
			requires is_enum_class_v<T>&& requires{T::FlagMax;} && ((size_t)T::FlagMax > 0)
		friend class BitFlags;
	public:
		template<size_t index>
		constexpr Proxy get() noexcept {
			return data[index];
		}
		constexpr Proxy operator[](size_t index) noexcept {
			return data[index];
		}
		template<size_t index>
		constexpr bool get() const noexcept {
			return data[index];
		}
		constexpr bool operator[](size_t index) const noexcept {
			return data[index];
		}
		constexpr size_t size() const noexcept {
			return l;
		}
		constexpr auto& operator=(const bool(&args)[l]) noexcept {
			for (size_t i = 0;i < l;i++) {
				data[i] = args[i];
			}
			return *this;
		}
		constexpr auto& operator=(const BoolArray<l>& other) noexcept {
			for (size_t i = 0;i < l;i++) {
				data[i] = other[i];
			}
			return *this;
		}
		constexpr auto& operator=(const ProxyArray<l>& other) noexcept {
			auto temp_other = (BoolArray<l>)other;
			for (size_t i = 0;i < l;i++) {
				data[i] = temp_other[i];
			}
			return *this;
		}
		constexpr operator BoolArray<l>() const noexcept {
			BoolArray<l> res{};
			for (size_t i = 0;i < l;i++) {
				res.data[i] = bool(data[i]);
			}
			return res;
		}
		constexpr bool eq(const BoolArray<l>& other) const noexcept {
			for (size_t i = 0;i < l;i++) {
				if (data[i] != other.data[i])return false;
			}
			return true;
		}
	};
}
namespace vkm::utils {
	template<typename T>
		requires is_enum_class_v<T>&& requires{T::FlagMax;} && ((size_t)T::FlagMax > 0)
	class BitFlags {
		static constexpr size_t data_length = (size_t)T::FlagMax;
		static constexpr size_t item_num = (data_length + 7) >> 3;
		using U = std::underlying_type_t<T>;
		uint8_t data[item_num] = {};
		template<typename>
		friend struct std::hash;
	public:
		constexpr BitFlags<T>() noexcept = default;
		constexpr BitFlags<T>(const BitFlags<T>&) = default;
		constexpr BitFlags<T>(BitFlags<T>&&) = default;
		constexpr BitFlags& operator=(const BitFlags<T>&) = default;
		constexpr BitFlags& operator=(BitFlags<T>&&) = default;
		constexpr bool operator[](const T enum_index) const noexcept {
			const U index = static_cast<U>(enum_index);
			return (data[index >> 3] >> (index & 7)) & 1u;
		}
		constexpr vkm_impl::utils::BitFlags::Proxy operator[](T enum_index) noexcept {
			const U index = static_cast<U>(enum_index);
			return vkm_impl::utils::BitFlags::Proxy(data[index >> 3], index & 7);
		}
		template<size_t l>
		constexpr auto operator[](const T(&enum_indexes)[l]) noexcept {
			vkm_impl::utils::BitFlags::ProxyArray<l> res;
			U index;
			for (size_t i = 0;i < l;i++) {
				index = static_cast<U>(enum_indexes[i]);
				res.data[i] = vkm_impl::utils::BitFlags::Proxy(data[index >> 3], index & 7);
			}
			return res;
		}
		template<size_t l>
		constexpr auto operator[](const T (&enum_indexes)[l]) const noexcept {
			vkm_impl::utils::BitFlags::BoolArray<l> res;
			U index;
			for (size_t i = 0;i < l;i++) {
				index = static_cast<U>(enum_indexes[i]);
				res.data[i] = (data[index >> 3] >> (index & 7)) & 1u;
			}
			return res;
		}
		constexpr bool operator==(BitFlags<T> other) const noexcept {
			return !std::memcmp(data,other.data,item_num);
		}
	};
}
namespace std {
	template<size_t l>
	struct tuple_size<vkm_impl::utils::BitFlags::ProxyArray<l>>: public integral_constant<size_t, l> {};
	template<size_t index, size_t l>
	struct tuple_element<index, vkm_impl::utils::BitFlags::ProxyArray<l>> {
		using type = vkm_impl::utils::BitFlags::Proxy;
	};
	template<size_t index, size_t l>
	struct tuple_element<index, const vkm_impl::utils::BitFlags::ProxyArray<l>> {
		using type = bool;
	};

	template<size_t l>
	struct tuple_size<vkm_impl::utils::BitFlags::BoolArray<l>> : public integral_constant<size_t, l> {};
	template<size_t index, size_t l>
	struct tuple_element<index, vkm_impl::utils::BitFlags::BoolArray<l>> {
		using type = bool;
	};
	template<typename T>
	struct hash<vkm::utils::BitFlags<T>> {
	private:
		static constexpr auto string_view_hash = std::hash<std::string_view>{};
	public:
		size_t operator()(const vkm::utils::BitFlags<T>& flags) const noexcept {
			std::string_view view{ reinterpret_cast<const char*>(flags.data), vkm::utils::BitFlags<T>::item_num };
			return string_view_hash(view);
		}
	};
}