#pragma once
#include <type_traits>
#include <cstring>
#include <string_view>
#include <array>
#include <stdexcept>
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
	
	// Proxy class for multi-element access with reference semantics
	// Note: data member is public to allow NamedArray to initialize it directly
	template<size_t l, typename U>
	class ProxyArray {
	public:
		U* data[l];
		
		template<typename T2, typename U2>
			requires is_enum_class_v<T2>&& requires{T2::ArrayMax;} && ((size_t)T2::ArrayMax > 0)
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
		
		// Optimized assignment operators - direct element-wise copy
		constexpr auto& operator=(const U(&args)[l]) noexcept(std::is_nothrow_copy_constructible_v<U>) {
			for (size_t i = 0; i < l; i++) {
				*data[i] = args[i];
			}
			return *this;
		}
		
		constexpr auto& operator=(const ProxyArray& other) noexcept(std::is_nothrow_copy_constructible_v<U>) {
			for (size_t i = 0; i < l; i++) {
				*data[i] = *other.data[i];
			}
			return *this;
		}
		
		constexpr auto& operator=(std::initializer_list<U> init) noexcept(std::is_nothrow_copy_constructible_v<U>) {
			size_t i = 0;
			for (const auto& val : init) {
				if (i < l) {
					*data[i++] = val;
				}
			}
			return *this;
		}
		
		constexpr bool operator==(const ProxyArray& other) const noexcept {
			for (size_t i = 0; i < l; i++) {
				if (*data[i] != *other.data[i]) return false;
			}
			return true;
		}
		
		// Fill all elements with a value
		constexpr void fill(const U& value) noexcept(std::is_nothrow_copy_constructible_v<U>) {
			for (size_t i = 0; i < l; i++) {
				*data[i] = value;
			}
		}
	};
}
namespace vkm::utils {
	template<typename T, typename U>
		requires is_enum_class_v<T>&& requires{T::ArrayMax;} && ((size_t)T::ArrayMax > 0)
	class NamedArray {
		using P = std::underlying_type_t<T>;
		static constexpr size_t data_length = (size_t)T::ArrayMax;
		U data[data_length];
		friend struct std::hash<NamedArray<T,U>>;
		template<size_t l, typename U2>
		friend class vkm_impl::utils::NamedArray::ProxyArray;
	public:
		// Constructors
		constexpr NamedArray() noexcept(std::is_nothrow_default_constructible_v<U>) {
			for (size_t i = 0; i < data_length; i++) {
				data[i] = U{};
			}
		}
		constexpr NamedArray(const NamedArray&) = default;
		constexpr NamedArray(NamedArray&&) = default;
		constexpr NamedArray& operator=(const NamedArray&) = default;
		constexpr NamedArray& operator=(NamedArray&&) = default;
		
		// Initialize from initializer list
		constexpr NamedArray(std::initializer_list<U> init) noexcept(std::is_nothrow_copy_constructible_v<U>) {
			size_t i = 0;
			for (const auto& val : init) {
				if (i < data_length) {
					data[i++] = val;
				}
			}
			// Zero-initialize remaining elements
			for (; i < data_length; i++) {
				data[i] = U{};
			}
		}
		
		// Single element access with bounds checking (debug builds)
		constexpr U& at(T enum_index) {
			#ifdef NDEBUG
			return data[static_cast<P>(enum_index)];
			#else
			auto idx = static_cast<P>(enum_index);
			if (idx >= data_length) {
				throw std::out_of_range("NamedArray index out of range");
			}
			return data[idx];
			#endif
		}
		constexpr const U& at(T enum_index) const {
			#ifdef NDEBUG
			return data[static_cast<P>(enum_index)];
			#else
			auto idx = static_cast<P>(enum_index);
			if (idx >= data_length) {
				throw std::out_of_range("NamedArray index out of range");
			}
			return data[idx];
			#endif
		}
		
		// Single element access (no bounds checking)
		constexpr U& operator[](T enum_index) noexcept {
			return data[static_cast<P>(enum_index)];
		}
		constexpr const U& operator[](T enum_index) const noexcept {
			return data[static_cast<P>(enum_index)];
		}
		
		// Compile-time single element access
		template<P index>
			requires(index < data_length)
		constexpr U& get() noexcept {
			return data[index];
		}
		template<P index>
			requires(index < data_length)
		constexpr const U& get() const noexcept {
			return data[index];
		}
		
		// Multi-element access via enum array
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
		
		// Comparison operators
		constexpr bool operator==(const NamedArray& other) const noexcept {
			for (size_t i = 0; i < data_length; i++) {
				if (data[i] != other.data[i]) return false;
			}
			return true;
		}
		constexpr bool operator!=(const NamedArray& other) const noexcept {
			return !(*this == other);
		}
		
		// Size and iteration
		constexpr size_t size() const noexcept {
			return data_length;
		}
		constexpr bool empty() const noexcept {
			return data_length == 0;
		}
		
		// Iterators
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
		constexpr const U* cbegin() const noexcept {
			return data;
		}
		constexpr const U* cend() const noexcept {
			return data + data_length;
		}
		
		// Fill all elements with a value
		constexpr void fill(const U& value) noexcept(std::is_nothrow_copy_constructible_v<U>) {
			for (size_t i = 0; i < data_length; i++) {
				data[i] = value;
			}
		}
		
		// Get raw pointer (for interop)
		constexpr U* data_ptr() noexcept {
			return data;
		}
		constexpr const U* data_ptr() const noexcept {
			return data;
		}
	};
}
namespace std {
	template<typename T, typename U>
	struct hash<vkm::utils::NamedArray<T,U>> {
	private:
		template<size_t... Is>
		static constexpr size_t helper(const vkm::utils::NamedArray<T, U>& array, std::index_sequence<Is...>) noexcept {
			return vkm::utils::hashCombine(array.data[Is]...);
		}
	public:
		constexpr size_t operator()(const vkm::utils::NamedArray<T,U>& array) const noexcept {
			constexpr auto data_length = (size_t)T::ArrayMax;
			return helper(array, std::make_index_sequence<data_length>{});
		}
	};
}