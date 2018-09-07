#include <vector>
#include <iostream>

#include <Neon/Core/SmartDeleter.hpp>
#include <Neon/Renderer/ShaderModule.hpp>
#include <Neon/Renderer/Device.hpp>
#include <Neon/Renderer/SwapChain.hpp>
#include <Neon/Renderer/RenderPass.hpp>
#include <Neon/Renderer/VertexBuffer.hpp>
#include <Neon/Renderer/Pipeline.hpp>

namespace Zx
{
	/*
	@brief : Creates a pipeline
	@param : The device of the application
	@param : The renderPass of the application
	@param : The swapChain of the application
	*/
	Pipeline::Pipeline(Device& device, RenderPass& renderPass, SwapChain& swapChain)
	{
		m_device = std::make_shared<Device>(device);
		m_renderPass = std::make_shared<RenderPass>(renderPass);
		m_swapChain = std::make_shared<SwapChain>(swapChain);
		m_pipeline = VK_NULL_HANDLE;

		if (!CreatePipeline())
			std::cout << "Failed to create pipeline" << std::endl;

		device = std::move(*m_device);
		renderPass = std::move(*m_renderPass);
		swapChain = std::move(*m_swapChain);
	}

	/*
	@brief : Copy constructor
	@param : A constant reference to Pipeline to copy
	*/
	Pipeline::Pipeline(const Pipeline& pipeline) : m_pipeline(pipeline.m_pipeline), m_device(pipeline.m_device), m_renderPass(pipeline.m_renderPass)
		, m_swapChain(pipeline.m_swapChain)
	{}

	/*
	@brief : Destroys a pipeline
	*/
	Pipeline::~Pipeline()
	{
		vkDestroyPipeline(m_device->GetDevice()->logicalDevice, m_pipeline, nullptr);
		m_pipeline = VK_NULL_HANDLE;
	}

	/*
	@brief : Assigns the pipeline by move semantic
	@param : The pipeline to move
	@return : A reference to this
	*/
	Pipeline& Pipeline::operator=(Pipeline&& pipeline) noexcept
	{
		std::swap(m_device, pipeline.m_device);
		std::swap(m_pipeline, pipeline.m_pipeline);
		std::swap(m_renderPass, pipeline.m_renderPass);
		std::swap(m_swapChain, pipeline.m_swapChain);

		return (*this);
	}

	//-------------------------Private method-------------------------
	bool Pipeline::CreatePipeline()
	{
		//Chemin absolu pour RenderDoc
		SmartDeleter<VkShaderModule, PFN_vkDestroyShaderModule> vertex = CreateShaderModule("C:/Users/Lucas/Documents/Neon/shaders/vert.spv", *m_device);
		SmartDeleter<VkShaderModule, PFN_vkDestroyShaderModule> frag = CreateShaderModule("C:/Users/Lucas/Documents/Neon/shaders/frag.spv", *m_device);

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

		std::vector<VkVertexInputBindingDescription> inputBindingDescription =
		{
			{
				0,
				sizeof(VertexData),
				VK_VERTEX_INPUT_RATE_VERTEX
			}
		};

		std::vector<VkVertexInputAttributeDescription> inputAttributeDescription =
		{
			{
				0,
				inputBindingDescription.data()->binding,
				VK_FORMAT_R32G32B32A32_SFLOAT,
				offsetof(struct VertexData, x)
			},
			{
				1,
				inputBindingDescription.data()->binding,
				VK_FORMAT_R32G32B32A32_SFLOAT,
				offsetof(struct VertexData, r)
			}
		};

		VkPipelineVertexInputStateCreateInfo pipelineVertexInfo =
		{
			VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
			nullptr,
			0,
			static_cast<uint32_t>(inputBindingDescription.size()),
			inputBindingDescription.data(),
			static_cast<uint32_t>(inputAttributeDescription.size()),
			inputAttributeDescription.data()
		};

		VkPipelineInputAssemblyStateCreateInfo pipelineInputAssembly =
		{
			VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
			nullptr,
			0,
			VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP,
			VK_FALSE
		};

		VkPipelineViewportStateCreateInfo pipelineViewportInfo =
		{
			VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
			nullptr,
			0,
			1,
			nullptr,
			1,
			nullptr
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
			{ 0.0f, 0.0f, 0.0f, 0.0f }
		};

		std::vector<VkDynamicState> dynamicState =
		{
			VK_DYNAMIC_STATE_VIEWPORT,
			VK_DYNAMIC_STATE_SCISSOR
		};

		VkPipelineDynamicStateCreateInfo dynamicStateCreateInfo =
		{
			VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
			nullptr,
			0,
			static_cast<uint32_t>(dynamicState.size()),
			dynamicState.data()
		};

		SmartDeleter<VkPipelineLayout, PFN_vkDestroyPipelineLayout> pipelineLayout = CreatePipelineLayout();

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
			&dynamicStateCreateInfo,
			pipelineLayout.GetObj(),
			m_renderPass->GetRenderPass(),
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

	//----------------------------------------------------------------

	SmartDeleter<VkPipelineLayout, PFN_vkDestroyPipelineLayout> Pipeline::CreatePipelineLayout()
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
			return SmartDeleter<VkPipelineLayout, PFN_vkDestroyPipelineLayout>();
		}

		return SmartDeleter<VkPipelineLayout, PFN_vkDestroyPipelineLayout>(pipelineLayout, vkDestroyPipelineLayout, m_device->GetDevice()->logicalDevice);
	}
}