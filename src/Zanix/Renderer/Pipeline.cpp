#include <vector>
#include <iostream>

#include <Zanix/Core/SmartDeleter.hpp>
#include <Zanix/Renderer/ShaderModule.hpp>
#include <Zanix/Renderer/Device.hpp>
#include <Zanix/Renderer/SwapChain.hpp>
#include <Zanix/Renderer/RenderPass.hpp>
#include <Zanix/Renderer/Pipeline.hpp>

namespace Zx
{
	/*
	@brief : Constructor with the needed informations
	@param : The device of the application
	@param : The renderPass of the application
	@param : The swapChain of the application
	*/
	Pipeline::Pipeline(const Device& device, const RenderPass& renderPass, const SwapChain& swapChain)
	{
		m_device = std::make_shared<Device>(device);
		m_renderPass = std::make_shared<RenderPass>(renderPass);
		m_swapChain = std::make_shared<SwapChain>(swapChain);
		m_pipeline = VK_NULL_HANDLE;
	}

	/*
	@brief : Copy constructor
	@param : A constant reference to Pipeline to copy
	*/
	Pipeline::Pipeline(const Pipeline& pipeline) : m_pipeline(pipeline.m_pipeline), m_device(pipeline.m_device), m_renderPass(pipeline.m_renderPass)
		, m_swapChain(pipeline.m_swapChain)
	{}

	/*
	@brief : Creates a graphics pipeline
	@return : Returns true if the creation of the graphics pipeline is a success, false otherwise
	*/
	bool Pipeline::CreatePipeline()
	{
		ShaderModule v, f;
		//Chemin absolu pour RenderDoc
		SmartDeleter<VkShaderModule, PFN_vkDestroyShaderModule> vertex = v.CreateShaderModule("C:/Users/Lucas/Documents/ZanixEngine/shaders/vert.spv", *m_device);
		SmartDeleter<VkShaderModule, PFN_vkDestroyShaderModule> frag = f.CreateShaderModule("C:/Users/Lucas/Documents/ZanixEngine/shaders/frag.spv", *m_device);

		if (!vertex || !frag)
			return false;

		std::vector<VkPipelineShaderStageCreateInfo> shaderStageInfo = 
		{
			{
				VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
				nullptr,
				0,
				VK_SHADER_STAGE_VERTEX_BIT,
				vertex.GetObj(),
				"main",
				nullptr
			},
			{
				VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
				nullptr,
				0,
				VK_SHADER_STAGE_FRAGMENT_BIT,
				frag.GetObj(),
				"main",
				nullptr
			}
		};

		VkPipelineVertexInputStateCreateInfo pipelineVertexInfo =
		{
			VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
			nullptr,
			0,
			0,
			nullptr,
			0,
			nullptr
		};

		VkPipelineInputAssemblyStateCreateInfo pipelineInputAssembly =
		{
			VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
			nullptr,
			0,
			VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
			VK_FALSE
		};

		VkViewport viewport =
		{
			0.0f,
			0.0f,
			300.0f,
			300.0f,
			0.0f,
			1.0f
		};

		VkRect2D scissor =
		{
			{
				0,
				0
			},
			{
				300,
				300
			}
		};

		VkPipelineViewportStateCreateInfo pipelineViewportInfo =
		{
			VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
			nullptr,
			0,
			1,
			&viewport,
			1,
			&scissor
		};

		VkPipelineRasterizationStateCreateInfo pipelineRasterizationInfo =
		{
			VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
			nullptr,
			0,
			VK_FALSE,
			VK_FALSE,
			VK_POLYGON_MODE_FILL,
			VK_CULL_MODE_BACK_BIT,
			VK_FRONT_FACE_COUNTER_CLOCKWISE,
			VK_FALSE,
			0.0f,
			0.0f,
			0.0f,
			1.0f
		};

		VkPipelineMultisampleStateCreateInfo pipelineMultisampleInfo =
		{
			VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
			nullptr,
			0,
			VK_SAMPLE_COUNT_1_BIT,
			VK_FALSE,
			1.0f,
			nullptr,
			VK_FALSE,
			VK_FALSE
		};

		VkPipelineColorBlendAttachmentState pipelineColorBendAttachmentState =
		{
			VK_FALSE,
			VK_BLEND_FACTOR_ONE,
			VK_BLEND_FACTOR_ZERO,
			VK_BLEND_OP_ADD,
			VK_BLEND_FACTOR_ONE,
			VK_BLEND_FACTOR_ZERO,
			VK_BLEND_OP_ADD,
			VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT
		};

		VkPipelineColorBlendStateCreateInfo pipelineColorBlendStateCreateInfo =
		{
			VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
			nullptr,
			0,
			VK_FALSE,
			VK_LOGIC_OP_COPY,
			1,
			&pipelineColorBendAttachmentState,
			{0.0f, 0.0f, 0.0f, 0.0f }
		};

		VkPipelineLayout pipelineLayout = CreatePipelineLayout();
		
		if (!pipelineLayout)
			return false;

		VkGraphicsPipelineCreateInfo graphicsPipelineInfo =
		{
			VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
			nullptr,
			0,
			static_cast<uint32_t>(shaderStageInfo.size()),
			shaderStageInfo.data(),
			&pipelineVertexInfo,
			&pipelineInputAssembly,
			nullptr,
			&pipelineViewportInfo,
			&pipelineRasterizationInfo,
			&pipelineMultisampleInfo,
			nullptr,
			&pipelineColorBlendStateCreateInfo,
			nullptr,
			pipelineLayout,
			m_renderPass->GetRenderPass()->renderPass,
			0,
			VK_NULL_HANDLE,
			-1
		};

		if (vkCreateGraphicsPipelines(m_device->GetDevice()->logicalDevice, VK_NULL_HANDLE, 1, &graphicsPipelineInfo, nullptr, &m_pipeline) != VK_SUCCESS)
		{
			std::cout << "Failed to create graphics pipeline" << std::endl;
			return false;
		}

		return true;
	}

	/*
	@brief : Destroys a graphics pipeline
	*/
	void Pipeline::DestroyPipeline()
	{
		vkDestroyPipeline(m_device->GetDevice()->logicalDevice, m_pipeline, nullptr);
		m_pipeline = VK_NULL_HANDLE;
	}

	//-------------------------Private method-------------------------

	VkPipelineLayout Pipeline::CreatePipelineLayout()
	{
		// Pour l'instant on créé un pipeline "vide"

		VkPipelineLayoutCreateInfo pipelineLayoutInfo = 
		{
			VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
			nullptr,
			0,
			0,
			nullptr,
			0,
			nullptr
		};

		VkPipelineLayout pipelineLayout = VK_NULL_HANDLE;
		if (vkCreatePipelineLayout(m_device->GetDevice()->logicalDevice, &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
		{
			std::cout << "Could not create pipeline layout" << std::endl;
			return VK_NULL_HANDLE;
		}

		return pipelineLayout;
	}
}