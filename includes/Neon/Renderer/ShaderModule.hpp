#ifndef SHADERMODULE_HPP
#define SHADERMODULE_HPP

#include <memory>
#include <vulkan/vulkan.h>

#include <Neon/Core/String.hpp>

namespace Zx
{
	template <class O, class F>
	class SmartDeleter;
	class Device;

	SmartDeleter<VkShaderModule, PFN_vkDestroyShaderModule> CreateShaderModule(const String& filename, const Device& device);
}

#endif //SHADERMODULE_HPP