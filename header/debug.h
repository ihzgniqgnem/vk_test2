#pragma once
#include <vulkan/vulkan.h>
#include <cassert>
#include <utility>
#include "./handle_table.h"
namespace vkm {
	class Debug {
	public:
		enum class Level {
			VERBOSE,
			INFO,
			WARNING,
			ERROR,
			EnumMax
		};
		static constexpr Level convert(VkDebugUtilsMessageSeverityFlagBitsEXT input) {
			switch (input) {
			case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT: return Level::VERBOSE;
			case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT: return Level::INFO;
			case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT: return Level::WARNING;
			case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT: return Level::ERROR;
			default: return Level::EnumMax;
			}
		}
		enum class Reason {
			GENERAL,
			VALIDATION,
			PERFORMANCE,
			DEVICE_ADDRESS_BINDING,
			EnumMax
		};
		static constexpr Reason convert(VkDebugUtilsMessageTypeFlagsEXT input) {
			switch (input) {
			case VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT: return Reason::GENERAL;
			case VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT: return Reason::VALIDATION;
			case VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT: return Reason::PERFORMANCE;
			case VK_DEBUG_UTILS_MESSAGE_TYPE_DEVICE_ADDRESS_BINDING_BIT_EXT: return Reason::DEVICE_ADDRESS_BINDING;
			default: return Reason::EnumMax;
			}
		}
		enum class ObjectType {
			UNKNOWN,
			INSTANCE,
			PHYSICAL_DEVICE,
			DEVICE,
			QUEUE,
			SEMAPHORE,
			COMMAND_BUFFER,
			FENCE,
			DEVICE_MEMORY,
			BUFFER,
			IMAGE,
			EVENT,
			QUERY_POOL,
			BUFFER_VIEW,
			IMAGE_VIEW,
			SHADER_MODULE,
			PIPELINE_CACHE,
			PIPELINE_LAYOUT,
			RENDER_PASS,
			PIPELINE,
			DESCRIPTOR_SET_LAYOUT,
			SAMPLER,
			DESCRIPTOR_POOL,
			DESCRIPTOR_SET,
			FRAMEBUFFER,
			COMMAND_POOL,
			SAMPLER_YCBCR_CONVERSION,
			DESCRIPTOR_UPDATE_TEMPLATE,
			PRIVATE_DATA_SLOT,
			SURFACE,
			SWAPCHAIN,
			DISPLAY,
			DISPLAY_MODE,
			DEBUG_REPORT_CALLBACK,
			VIDEO_SESSION,
			VIDEO_SESSION_PARAMETERS,
			DEBUG_UTILS_MESSENGER,
			ACCELERATION_STRUCTURE,
			VALIDATION_CACHE,
			DEFERRED_OPERATION,
			MICROMAP,
			SHADER,
			PIPELINE_BINARY,
			INDIRECT_COMMANDS_LAYOUT,
			INDIRECT_EXECUTION_SET,
			EnumMax
		};
		static constexpr ObjectType convert(VkObjectType input) {
			switch (input) {
			case VK_OBJECT_TYPE_UNKNOWN: return ObjectType::UNKNOWN;
			case VK_OBJECT_TYPE_INSTANCE: return ObjectType::INSTANCE;
			case VK_OBJECT_TYPE_PHYSICAL_DEVICE: return ObjectType::PHYSICAL_DEVICE;
			case VK_OBJECT_TYPE_DEVICE: return ObjectType::DEVICE;
			case VK_OBJECT_TYPE_QUEUE: return ObjectType::QUEUE;
			case VK_OBJECT_TYPE_SEMAPHORE: return ObjectType::SEMAPHORE;
			case VK_OBJECT_TYPE_COMMAND_BUFFER: return ObjectType::COMMAND_BUFFER;
			case VK_OBJECT_TYPE_FENCE: return ObjectType::FENCE;
			case VK_OBJECT_TYPE_DEVICE_MEMORY: return ObjectType::DEVICE_MEMORY;
			case VK_OBJECT_TYPE_BUFFER: return ObjectType::BUFFER;
			case VK_OBJECT_TYPE_IMAGE: return ObjectType::IMAGE;
			case VK_OBJECT_TYPE_EVENT: return ObjectType::EVENT;
			case VK_OBJECT_TYPE_QUERY_POOL: return ObjectType::QUERY_POOL;
			case VK_OBJECT_TYPE_BUFFER_VIEW: return ObjectType::BUFFER_VIEW;
			case VK_OBJECT_TYPE_IMAGE_VIEW: return ObjectType::IMAGE_VIEW;
			case VK_OBJECT_TYPE_SHADER_MODULE: return ObjectType::SHADER_MODULE;
			case VK_OBJECT_TYPE_PIPELINE_CACHE: return ObjectType::PIPELINE_CACHE;
			case VK_OBJECT_TYPE_PIPELINE_LAYOUT: return ObjectType::PIPELINE_LAYOUT;
			case VK_OBJECT_TYPE_RENDER_PASS: return ObjectType::RENDER_PASS;
			case VK_OBJECT_TYPE_PIPELINE: return ObjectType::PIPELINE;
			case VK_OBJECT_TYPE_DESCRIPTOR_SET_LAYOUT: return ObjectType::DESCRIPTOR_SET_LAYOUT;
			case VK_OBJECT_TYPE_SAMPLER: return ObjectType::SAMPLER;
			case VK_OBJECT_TYPE_DESCRIPTOR_POOL: return ObjectType::DESCRIPTOR_POOL;
			case VK_OBJECT_TYPE_DESCRIPTOR_SET: return ObjectType::DESCRIPTOR_SET;
			case VK_OBJECT_TYPE_FRAMEBUFFER: return ObjectType::FRAMEBUFFER;
			case VK_OBJECT_TYPE_COMMAND_POOL: return ObjectType::COMMAND_POOL;
			case VK_OBJECT_TYPE_SAMPLER_YCBCR_CONVERSION: return ObjectType::SAMPLER_YCBCR_CONVERSION;
			case VK_OBJECT_TYPE_DESCRIPTOR_UPDATE_TEMPLATE: return ObjectType::DESCRIPTOR_UPDATE_TEMPLATE;
			case VK_OBJECT_TYPE_PRIVATE_DATA_SLOT: return ObjectType::PRIVATE_DATA_SLOT;
			case VK_OBJECT_TYPE_SURFACE_KHR: return ObjectType::SURFACE;
			case VK_OBJECT_TYPE_SWAPCHAIN_KHR: return ObjectType::SWAPCHAIN;
			case VK_OBJECT_TYPE_DISPLAY_KHR: return ObjectType::DISPLAY;
			case VK_OBJECT_TYPE_DISPLAY_MODE_KHR: return ObjectType::DISPLAY_MODE;
			case VK_OBJECT_TYPE_DEBUG_REPORT_CALLBACK_EXT: return ObjectType::DEBUG_REPORT_CALLBACK;
			case VK_OBJECT_TYPE_VIDEO_SESSION_KHR: return ObjectType::VIDEO_SESSION;
			case VK_OBJECT_TYPE_VIDEO_SESSION_PARAMETERS_KHR: return ObjectType::VIDEO_SESSION_PARAMETERS;
			case VK_OBJECT_TYPE_DEBUG_UTILS_MESSENGER_EXT: return ObjectType::DEBUG_UTILS_MESSENGER;
			case VK_OBJECT_TYPE_ACCELERATION_STRUCTURE_KHR: return ObjectType::ACCELERATION_STRUCTURE;
			case VK_OBJECT_TYPE_VALIDATION_CACHE_EXT: return ObjectType::VALIDATION_CACHE;
			case VK_OBJECT_TYPE_DEFERRED_OPERATION_KHR: return ObjectType::DEFERRED_OPERATION;
			case VK_OBJECT_TYPE_MICROMAP_EXT: return ObjectType::MICROMAP;
			case VK_OBJECT_TYPE_SHADER_EXT: return ObjectType::SHADER;
			case VK_OBJECT_TYPE_PIPELINE_BINARY_KHR: return ObjectType::PIPELINE_BINARY;
			case VK_OBJECT_TYPE_INDIRECT_COMMANDS_LAYOUT_EXT: return ObjectType::INDIRECT_COMMANDS_LAYOUT;
			case VK_OBJECT_TYPE_INDIRECT_EXECUTION_SET_EXT: return ObjectType::INDIRECT_EXECUTION_SET;
			default: return ObjectType::EnumMax;
			}
		}
		std::pair<void (*)(Level,Reason,const char*,uint32_t,const char*,const char**,const char**, std::pair<ObjectType, void*>*,void*),void*> callback[(uint32_t)Level::EnumMax][(uint32_t)Reason::EnumMax];
		static VkBool32 vkDebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
				VkDebugUtilsMessageTypeFlagsEXT messageTypes,
				const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData ) {
			auto& self = *(Debug*)pUserData;
			Level level = convert(messageSeverity);
			Reason reason = convert(messageTypes);
			if (self.callback[(uint32_t)level][(uint32_t)reason].first) {
				std::vector<const char*> queue_lable{ pCallbackData->queueLabelCount };
				for (int i = 0; i < pCallbackData->queueLabelCount; i++) {
					queue_lable[i] = pCallbackData->pQueueLabels[i].pLabelName;
				}
				std::vector<const char*> cmd_buf_label{ pCallbackData->cmdBufLabelCount };
				for (int i = 0; i < pCallbackData->cmdBufLabelCount; i++) {
					cmd_buf_label[i] = pCallbackData->pCmdBufLabels[i].pLabelName;
				}
				std::vector<std::pair<ObjectType, void*>> objects{ pCallbackData->objectCount };
				for (int i = 0; i < pCallbackData->objectCount; i++) {
					auto it = vkm_impl::handle_table.find((void*)pCallbackData->pObjects[i].objectHandle);
					if (it != vkm_impl::handle_table.end()) {
						objects[i].first = self.convert(pCallbackData->pObjects[i].objectType);
						objects[i].second = it->second;
					}
					else {
						objects[i].first = ObjectType::UNKNOWN;
						objects[i].second = (void*)pCallbackData->pObjects[i].objectHandle;
					}
				}
				auto& target=self.callback[(uint32_t)level][(uint32_t)reason];
				target.first(level,reason,pCallbackData->pMessageIdName,pCallbackData->messageIdNumber,pCallbackData->pMessage, queue_lable.data(), cmd_buf_label.data(), objects.data(), target.second);
			}
			return VK_FALSE;
		}
	};
}