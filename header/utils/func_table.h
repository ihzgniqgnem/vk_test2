#pragma once
#include <type_traits>
#include <cstring>
#include <string_view>
#include "is_enum_class.h"
#include "hash_combine.h"

namespace vkm_impl::utils::FuncTable {
	using namespace vkm::utils;
	template<typename>
	constexpr bool is_func_ptr=false;
	template<typename T,typename... Args>
	constexpr bool is_func_ptr<T(*)(Args...)> =true;
}
namespace vkm::utils {
	template<typename... T>
		requires (vkm_impl::utils::FuncTable::is_func_ptr<T> && ...)
	class FuncTableType;
}
namespace vkm_impl::utils::FuncTable {
	template<typename T>
	constexpr bool is_func_table_type=false;
	template<typename... T>
	constexpr bool is_func_table_type<vkm::utils::FuncTableType<T...>> = true;
}
namespace vkm::utils{
	template<typename T,typename U>
		requires is_enum_class_v<T> && (requires{T::FuncMax;} && ((size_t)T::FuncMax > 0)) && vkm_impl::utils::FuncTable::is_func_table_type<U>
	class FuncTable;
}
namespace vkm_impl::utils::FuncTable {
	template<typename>
	constexpr bool is_func=false;
	template<typename T,typename... Args>
	constexpr bool is_func<T(*)(Args...)> =true;
	template<typename T,typename... Args>
	constexpr bool is_func<T(Args...)> =true;
	class Proxy{
		void** raw_data;
		template<typename T,typename U>
			requires is_enum_class_v<T> && (requires{T::FuncMax;} && ((size_t)T::FuncMax > 0)) && vkm_impl::utils::FuncTable::is_func_table_type<U>
		friend class vkm::utils::FuncTable;
		constexpr Proxy(void*& x):raw_data(&x){}
	public:
		constexpr Proxy() noexcept = default;
		constexpr Proxy(const Proxy&) noexcept = default;
		constexpr Proxy(Proxy&&) noexcept = default;
		constexpr Proxy& operator=(const Proxy& other) noexcept = default;
		constexpr Proxy& operator=(Proxy&& other) noexcept = default;
		template<typename T>
			requires is_func<T>
		constexpr Proxy& operator=(const T x) noexcept {
			*raw_data=reinterpret_cast<void*>(x);
			return *this;
		}
		constexpr Proxy& operator=(void* x) noexcept {
			*raw_data=reinterpret_cast<void*>(x);
			return *this;
		}
		constexpr operator void*() noexcept {
			return *raw_data;
		}
		constexpr operator const void*() const noexcept {
			return *raw_data;
		}
		constexpr bool operator==(const Proxy& other) const noexcept {
			return *raw_data==*other.raw_data;
		}
		constexpr bool operator==(void * const & other) const noexcept {
			return *raw_data==other;
		}
	};
}
namespace vkm::utils {
	template<typename... T>
		requires (vkm_impl::utils::FuncTable::is_func_ptr<T> && ...)
	class FuncTableType {
		using U = std::tuple<T...>;
	public:
		template<size_t index>
		using P=std::tuple_element_t<index,U>;
		static constexpr size_t size=sizeof...(T);
	};
	template<typename T,typename U>
		requires is_enum_class_v<T> && (requires{T::FuncMax;} && ((size_t)T::FuncMax > 0)) && vkm_impl::utils::FuncTable::is_func_table_type<U>
	class FuncTable {
		using P = std::underlying_type_t<T>;
		static constexpr size_t data_length = (size_t)T::FuncMax;
		void* data[data_length] = {};
		template<typename>
		friend struct std::hash;
		template<typename>
		friend struct std::tuple_size;
	public:
		constexpr FuncTable() noexcept = default;
		constexpr FuncTable(const FuncTable<T, U>&) = default;
		constexpr FuncTable(FuncTable<T, U>&&) = default;
		constexpr FuncTable<T, U>& operator=(const FuncTable<T, U>&) = default;
		constexpr FuncTable<T, U>& operator=(FuncTable<T, U>&&) = default;
		template <T index>
		constexpr typename U::P<static_cast<P>(index)>& get() noexcept { 
			return reinterpret_cast<typename U::P<static_cast<P>(index)>&>(data[static_cast<P>(index)]);
		} 
		template <T index>
		constexpr const typename U::P<static_cast<P>(index)>& get() const noexcept { 
			return reinterpret_cast<const (typename U::P<static_cast<P>(index)>)&>(data[static_cast<P>(index)]);
		} 
		constexpr vkm_impl::utils::FuncTable::Proxy operator[](const T enum_index) noexcept {
			return data[static_cast<P>(enum_index)];
		}
		constexpr const void* const & operator[](const T enum_index) const noexcept {
			return data[static_cast<P>(enum_index)];
		}
		constexpr bool operator==(const FuncTable<T,U>& other) const noexcept {
			for (size_t i = 0;i < data_length;i++) {
				if (data[i] != other.data[i])return false;
			}
			return true;
		}
		constexpr size_t size() const noexcept {
			return data_length;
		}
		constexpr void** begin() noexcept {
			return data;
		}
		constexpr void** end() noexcept {
			return data + data_length;
		}
		constexpr void* const* begin() const noexcept {
			return data;
		}
		constexpr void* const* end() const noexcept {
			return data + data_length;
		}
	};
}
namespace std {
	template <typename T,typename U>
	struct tuple_size<vkm::utils::FuncTable<T,U>> : public integral_constant<size_t,vkm::utils::FuncTable<T,U>::data_length> {};
	
	template <size_t index,typename T,typename U>
	struct tuple_element<index,vkm::utils::FuncTable<T,U>> {
		using type = typename U::P<index>;
	};

	template <typename T,typename U>
	struct hash<vkm::utils::FuncTable<T,U>> {
	private:
		template<size_t... Is>
		static constexpr auto helper(const vkm::utils::FuncTable<T, U>& array,std::index_sequence<Is...>) noexcept {
			return vkm::utils::hashCombine(array[static_cast<T>(Is)]...);
		}
	public:
		size_t operator()(const vkm::utils::FuncTable<T,U>& array) const noexcept {
			return helper(array, make_index_sequence<vkm::utils::FuncTable<T,U>::data_length>{});
		}
	};
}