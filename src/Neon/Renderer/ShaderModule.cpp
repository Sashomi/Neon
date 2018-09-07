#include <Neon/Core/File.hpp>
#include <Neon/Core/SmartDeleter.hpp>
#include <Neon/Renderer/Device.hpp>
#include <Neon/Renderer/ShaderModule.hpp>

namespace Zx
{
	/*
	@brief : Creates shader module with a spv file
	@param : The spv file
	@return : Returns a VkShaderModule corresponding to the file
	*/
	SmartDeleter<VkShaderModule, PFN_vkDestroyShaderModule> CreateShaderModule(const String& filename, const Device& device)
	{
		File file(filename);
		const std::vector<char> code = file.GetBinaryFileContent();
		
		if (code.size() == 0)
			return SmartDeleter<VkShaderModule, PFN_vkDestroyShaderModule>();

		VkShaderModuleCreateInfo shaderModuleCreateInfo =
		{
			VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
			nullptr,
			0,
			code.size(),
			reinterpret_cast<const uint32_t*>(code.data())
		};

		VkShaderModule shaderModule = VK_NULL_HANDLE;
		if (vkCreateShaderModule(device.GetDevice()->logicalDevice, &shaderModuleCreateInfo, nullptr, &shaderModule) != VK_SUCCESS)
		{
			std::cout << "Could not create shader module from '" << filename << " ' file" << std::endl;
			return SmartDeleter<VkShaderModule, PFN_vkDestroyShaderModule>();
		}

		return SmartDeleter<VkShaderModule, PFN_vkDestroyShaderModule>(shaderModule, vkDestroyShaderModule, device.GetDevice()->logicalDevice);
	}
}