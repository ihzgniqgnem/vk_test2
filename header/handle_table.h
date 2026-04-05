#pragma once
#include <unordered_map>
#include <vulkan/vulkan.h>
namespace vkm_impl {
	inline std::unordered_map<void*, void*> handle_table;
}