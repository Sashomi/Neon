#ifndef SHADERMODULE_HPP
#define SHADERMODULE_HPP

#include <memory>
#include <vulkan/vulkan.h>

#include <Zanix/Core/String.hpp>

namespace Zx
{
	template <class O, class F>
	class SmartDeleter;
	class Device;

	class ShaderModule
	{
	public:
		SmartDeleter<VkShaderModule, PFN_vkDestroyShaderModule> CreateShaderModule(const String& filename, const Device& device);
	};
}

#endif //SHADERMODULE_HPP