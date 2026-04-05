#pragma once
#include <cstdint>
#include <cassert>
#include <array>
#include <vulkan/vulkan.h>
#include <glfw/glfw3.h>

#include "./size2d.h"
#include "./utils/bit_flags.h"

namespace vkm {
	class Window {
	private:
		Size2D window_size;
		Size2D window_pos;
		Size2D framebuffer_size;
		enum class FlagIndex {
			Fullscreen,
			Exclusive,
			FlagMax
		};
		vkm::utils::BitFlags<FlagIndex> flags;
		uint32_t monitor_index = 0;
		std::string title="";
		GLFWwindow* handle;
		Window() noexcept = default;
		friend class Instance;
	public:

		void set_fullscreen() {
			if (flags[{FlagIndex::Fullscreen, FlagIndex::Exclusive}].eq({ true,false })) {
				return;
			}
			flags[{FlagIndex::Fullscreen, FlagIndex::Exclusive}] = { true,false };
			GLFWmonitor* monitor = glfwGetPrimaryMonitor();
			const GLFWvidmode& mode = *glfwGetVideoMode(monitor);

			glfwSetWindowAttrib(handle, GLFW_DECORATED, GLFW_FALSE);
			glfwSetWindowMonitor(handle, nullptr, 0, 0, mode.width, mode.height, 0);
		}

		void set_fullscreen_exclusive(uint32_t monitor_index) {
			if (flags[{FlagIndex::Fullscreen, FlagIndex::Exclusive}].eq({ true,true }) &&
				this->monitor_index == monitor_index) {
				return;
			}
			flags[{FlagIndex::Fullscreen, FlagIndex::Exclusive}] = { true,true };
			this->monitor_index = monitor_index;

			uint32_t monitor_count;
			GLFWmonitor** monitors = glfwGetMonitors((int*)&monitor_count);
			assert(monitor_index < monitor_count);
			GLFWmonitor* monitor = monitors[monitor_index];
			const GLFWvidmode& mode = *glfwGetVideoMode(monitor);

			glfwSetWindowMonitor(handle, monitor, 0, 0,
				mode.width, mode.height, mode.refreshRate);
		}

		void set_window_mode(const Size2D size, const Size2D pos) {
			if (!flags[FlagIndex::Fullscreen] && window_size == size && window_pos == pos) {
				return;
			}
			flags[FlagIndex::Fullscreen] = false;
			flags[FlagIndex::Exclusive] = false;

			window_size = size;
			window_pos = pos;

			glfwSetWindowAttrib(handle, GLFW_DECORATED, GLFW_TRUE);
			glfwSetWindowMonitor(handle, nullptr, pos.x, pos.y, size.x, size.y, 0);
		}
		[[nodiscard]] bool is_fullscreen() const noexcept {
			return flags[FlagIndex::Fullscreen];
		}

		[[nodiscard]] bool is_exclusive() const noexcept {
			return flags[FlagIndex::Exclusive];
		}

		[[nodiscard]] Size2D get_framebuffer_size() const noexcept {
			return framebuffer_size;
		}
		[[nodiscard]] Size2D get_window_size() const noexcept {
			return window_size;
		}
		[[nodiscard]] bool is_valid() const noexcept {
			return handle;
		}
		void destroy() noexcept {
			if (handle) {
				glfwDestroyWindow(handle);
				handle = nullptr;
			}
		}
		static void update() {
			glfwPollEvents();
		}

		Window(const Window&) = delete;
		Window& operator=(const Window&) = delete;
		[[nodiscard]] Window(Window&& other) noexcept : window_size(other.window_size),framebuffer_size(other.framebuffer_size),
		flags(other.flags), handle(other.handle),callbacks(other.callbacks) {
			other.handle = nullptr;
			if (handle) {
				glfwSetWindowUserPointer(handle, this);
			}
		}
		Window& operator=(Window&& other) noexcept {
			if (this != &other) {
				if (handle) glfwDestroyWindow(handle);
				window_size = other.window_size;
				framebuffer_size = other.framebuffer_size;
				flags = other.flags;
				callbacks = other.callbacks;
				handle = other.handle;
				other.handle = nullptr;
				if (handle) {
					glfwSetWindowUserPointer(handle, this);
				}
			}
			return *this;
		}
		~Window() noexcept {
			destroy();
		}
	public:
		enum class CallbackType:uint32_t {
			FramebufferSize,
			WindowSize,
			Close,
			CallbackMax
		};
	private:
		std::array<void*, (uint32_t)CallbackType::CallbackMax> callbacks;
	public:
		constexpr void set_callback(CallbackType type, void* callback) noexcept {
			callbacks[(uint32_t)type] = callback;
		}
	public:
		static void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
			Window& self = *(Window*)glfwGetWindowUserPointer(window);
			self.framebuffer_size = { (uint32_t)width,(uint32_t)height };
			auto userFramebufferSizeCallback = static_cast<void(*)(Window&, Size2D)>(self.callbacks[(uint32_t)CallbackType::FramebufferSize]);
			if (userFramebufferSizeCallback) {
				userFramebufferSizeCallback(self, self.framebuffer_size);
			}
		}
		static void windowSizeCallback(GLFWwindow* window, int width, int height) {
			Window& self = *(Window*)glfwGetWindowUserPointer(window);
			self.window_size = { (uint32_t)width,(uint32_t)height };
			auto userWindowSizeCallback = static_cast<void(*)(Window&, Size2D)>(self.callbacks[(uint32_t)CallbackType::WindowSize]);
			if (userWindowSizeCallback) {
				userWindowSizeCallback(self, self.window_size);
			}
		}
		static void windowCloseCallback(GLFWwindow* window) {
			Window& self = *(Window*)glfwGetWindowUserPointer(window);
			auto userCloseCallback = static_cast<bool(*)(Window&)>(self.callbacks[(uint32_t)CallbackType::Close]);
			if (userCloseCallback && !userCloseCallback(self)) {
				glfwSetWindowShouldClose(window, false);
			}else {
				self.~Window();
			}
		}
	};
}