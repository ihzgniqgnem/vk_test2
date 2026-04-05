#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <string>

namespace vkm_impl::Instance {
	inline uint32_t get_vulkan_max_api_version() noexcept {
		if (!vkGetInstanceProcAddr(nullptr, "vkEnumerateInstanceVersion")) {
			return VK_API_VERSION_1_0;
		}
		else {
			uint32_t api_version;
			vkEnumerateInstanceVersion(&api_version);
			return api_version;
		}
	}
	inline const uint32_t max_api_version = get_vulkan_max_api_version();
}

namespace vkm {
	class Window;
	class Instance {
	private:
		VkInstance m_instance;
		VkPhysicalDevice m_phys_dev;
		VkDevice m_device;
		std::vector<std::string> layer;
		std::vector<std::string> extension;
	public:
		Instance() {
			VkApplicationInfo app_info;
			app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
			app_info.pNext = nullptr;
			app_info.pApplicationName = nullptr;
			app_info.pEngineName = nullptr;
			app_info.applicationVersion = 0;
			app_info.engineVersion = 0;
			app_info.apiVersion = vkm_impl::Instance::max_api_version;
			uint32_t extensionCount = 0;
			vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
			VkInstanceCreateInfo instance_create_info;
			instance_create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
			instance_create_info.pNext = nullptr;
			instance_create_info.flags = 0;
			instance_create_info.enabledLayerCount = layer.size();
			instance_create_info.ppEnabledLayerNames = layer.data();
		}
	};
}