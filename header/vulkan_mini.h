#pragma once
#include "./static_assertions.h"

#include <vulkan/vulkan.h>
#include <glfw/glfw3.h>

namespace vkm_impl{
	class InitHelper {
	public:
		InitHelper() {
			glfwInit();
		}
		~InitHelper() {
			glfwTerminate();
		}
	};
	inline InitHelper init_helper;
}

#include "./window.h"
#include "./size2d.h"
#include "./utils/bit_flags.h"
#include "./utils/hash_combine.h"