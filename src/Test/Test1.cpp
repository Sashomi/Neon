#include <thread>

#include <Neon/Utils.hpp>
#include <Neon/Core/Plugin.hpp>
#include <Neon/Core/String.hpp>
#include <Neon/Core/File.hpp>
#include <Neon/Core/Exception.hpp>
#include <Neon/Renderer/Device.hpp>
#include <Neon/Renderer/SwapChain.hpp>
#include <Neon/Renderer/Renderer.hpp>
#include <Neon/Renderer/Pipeline.hpp>
#include <Neon/Renderer/RenderPass.hpp>
#include <Neon/Renderer/Window.hpp>
#include <Neon/Renderer/VertexBuffer.hpp>
#include <Neon/Renderer/ShaderModule.hpp>
#include <Neon/Renderer/Sync.hpp>
#include <Neon/Renderer/CommandBuffers.hpp>
#include <Test/Test1.hpp>

namespace Zx
{
	Test1::Test1(const RenderPass& renderPass, const SwapChain& swapChain, const Pipeline& pipeline, const VertexBuffer& vertexBuffer, const Device& device, const Window& window,
		const CommandBuffers& commandBuffers, const std::vector<RenderingResourcesData>& renderingResources)
	{
		m_renderPass = std::make_shared<RenderPass>(renderPass);
		m_swapChain = std::make_shared<SwapChain>(swapChain);
		m_pipeline = std::make_shared<Pipeline>(pipeline);
		m_vertexBuffer = std::make_shared<VertexBuffer>(vertexBuffer);
		m_device = std::make_shared<Device>(device);
		m_window = std::make_shared<Window>(window);
		m_commandBuffers = std::make_shared<CommandBuffers>(commandBuffers);
		m_renderingResources = std::make_shared<std::vector<RenderingResourcesData>>(renderingResources);
	}

	bool Test1::PrepareFrame(VkCommandBuffer commandBuffer, const VkImageView& view, VkFramebuffer& framebuffer)
	{
		if (!m_renderPass->CreateFramebuffer(framebuffer, view))
			return false;

		VkCommandBufferBeginInfo commandBuffersBeginInfo =
		{
			VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
			nullptr,
			VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
			nullptr
		};

		vkBeginCommandBuffer(commandBuffer, &commandBuffersBeginInfo);

		VkClearValue clearValue =
		{
			{ 1.0f, 0.8f, 0.4f, 0.0f },
		};

		VkRenderPassBeginInfo renderPassBeginInfo =
		{
			VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
			nullptr,
			m_renderPass->GetRenderPass(),
			framebuffer,
		{
			{
				0,
				0
			},
			m_swapChain->GetSwapChain()->extent,
		},
			1,
			&clearValue
		};

		vkCmdBeginRenderPass(commandBuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline->GetPipeline());

		VkViewport viewPort =
		{
			0,
			0,
			static_cast<float>(m_swapChain->GetSwapChain()->extent.width),
			static_cast<float>(m_swapChain->GetSwapChain()->extent.height),
			0,
			1,
		};

		VkRect2D scissor =
		{
			{
				0,
				0
			},
			m_swapChain->GetSwapChain()->extent
		};

		vkCmdSetViewport(commandBuffer, 0, 1, &viewPort);
		vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

		VkDeviceSize offset = 0;
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, &m_vertexBuffer->GetVertexBuffer(), &offset);

		vkCmdDraw(commandBuffer, 4, 1, 0, 0);

		vkCmdEndRenderPass(commandBuffer);

		if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
		{
			std::cout << "Failed to end command buffers" << std::endl;
			return false;
		}

		return true;
	}

	void Test1::ChildClear() {
		//Inutilisť pour le moment
	}

	bool Test1::ChildOnWindowSizeChanged()
	{
		return true;
	}

	bool Test1::OnWindowSizeChanged()
	{
		return true;
	}

	bool Test1::Draw() 
	{
		static std::size_t resourcesIndex = 0;

		RenderingResourcesData &currentRenderingResources = (*m_renderingResources)[resourcesIndex];

		VkSwapchainKHR swap_chain = m_swapChain->GetSwapChain()->swapChain;
		uint32_t imageIndex = 0;

		resourcesIndex = (resourcesIndex + 1) % 3;

		if (vkWaitForFences(m_device->GetDevice()->logicalDevice, 1, &currentRenderingResources.fence, VK_FALSE, 1000000000) != VK_SUCCESS)
		{
			std::cout << "The waiting time for fence is exceeded" << std::endl;
			return false;
		}

		vkResetFences(m_device->GetDevice()->logicalDevice, 1, &currentRenderingResources.fence);

		VkResult result = vkAcquireNextImageKHR(m_device->GetDevice()->logicalDevice, swap_chain, UINT64_MAX, currentRenderingResources.imageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex);

		switch (result) {
		case VK_SUCCESS:
		case VK_SUBOPTIMAL_KHR:
			break;
		case VK_ERROR_OUT_OF_DATE_KHR:
			return OnWindowSizeChanged();
		default:
			std::cout << "Problem occurred during swap chain image acquisition!" << std::endl;
			return false;
		}

		if (!PrepareFrame(currentRenderingResources.commandBuffer, m_swapChain->GetSwapChain()->imageView[imageIndex], currentRenderingResources.framebuffer))
		{
			std::cout << "Failed to prepare frame" << std::endl;
			return false;
		}

		VkPipelineStageFlags wait_dst_stage_mask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		VkSubmitInfo submit_info = {
			VK_STRUCTURE_TYPE_SUBMIT_INFO,
			nullptr,
			1,
			&currentRenderingResources.imageAvailableSemaphore,
			&wait_dst_stage_mask,
			1,
			&(*m_renderingResources)[imageIndex].commandBuffer,
			1,
			&currentRenderingResources.finishedRenderingSemaphore
		};

		if (vkQueueSubmit(m_device->GetDevice()->graphicsQueue, 1, &submit_info, currentRenderingResources.fence) != VK_SUCCESS)
		{
			return false;
		}

		VkPresentInfoKHR present_info =
		{
			VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
			nullptr,
			1,
			&currentRenderingResources.finishedRenderingSemaphore,
			1,
			&swap_chain,
			&imageIndex,
			nullptr
		};
		result = vkQueuePresentKHR(m_device->GetDevice()->presentQueue, &present_info);

		switch (result) {
		case VK_SUCCESS:
			break;
		case VK_ERROR_OUT_OF_DATE_KHR:
		case VK_SUBOPTIMAL_KHR:
			return OnWindowSizeChanged();
		default:
			std::cout << "Problem occurred during image presentation!" << std::endl;
			return false;
		}

		return true;
	}

#if defined(NEON_WINDOWS)
	bool Test1::RenderingLoop()
	{
		// Display window
		ShowWindow(m_window->GetHandle(), SW_SHOWNORMAL);
		UpdateWindow(m_window->GetHandle());

		// Main message loop
		MSG message;
		bool loop = true;
		bool resize = false;
		bool result = true;

		while (loop) {
			if (PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
			{
				// Process events
				switch (message.message) {
					// Resize
				case WM_USER + 1:
					resize = true;
					break;
					// Close
				case WM_USER + 2:
					loop = false;
					break;
				}
				TranslateMessage(&message);
				DispatchMessage(&message);
			}
			else {
				// Resize
				if (resize) {
					resize = false;
					if (!OnWindowSizeChanged())
					{
						result = false;
						break;
					}
				}
				// Draw
				if (m_swapChain->IsRenderAvailable())
				{
					if (!Draw())
					{
						result = false;
						break;
					}
				}
				else
				{
					std::this_thread::sleep_for(std::chrono::milliseconds(100));
				}
			}
		}

		return result;
	}

	#elif defined(NEON_POSIX)

	bool Test1::RenderingLoop()
	{
		xcb_intern_atom_cookie_t  protocols_cookie = xcb_intern_atom(m_window->GetInstance(), 1, 12, "WM_PROTOCOLS");
		xcb_intern_atom_reply_t  *protocols_reply = xcb_intern_atom_reply(m_window->GetInstance(), protocols_cookie, 0);
		xcb_intern_atom_cookie_t  delete_cookie = xcb_intern_atom(m_window->GetInstance(), 0, 16, "WM_DELETE_WINDOW");
		xcb_intern_atom_reply_t  *delete_reply = xcb_intern_atom_reply(m_window->GetInstance(), delete_cookie, 0);
		xcb_change_property(m_window->GetInstance(), XCB_PROP_MODE_REPLACE, m_window->GetHandle(), (*protocols_reply).atom, 4, 32, 1, &(*delete_reply).atom);
		free(protocols_reply);

		xcb_map_window(m_window->GetInstance(), m_window->GetHandle());
		xcb_flush(m_window->GetInstance());

		xcb_generic_event_t *event;
		bool loop = true;
		bool resize = false;
		bool result = true;

		while (loop) 
		{
			event = xcb_poll_for_event(m_window->GetInstance());

			if (event) 
			{
				switch (event->response_type & 0x7f) 
				{
				case XCB_CONFIGURE_NOTIFY: {
					xcb_configure_notify_event_t *configure_event = (xcb_configure_notify_event_t*)event;
					static uint16_t width = configure_event->width;
					static uint16_t height = configure_event->height;

					if (((configure_event->width > 0) && (width != configure_event->width)) ||
						((configure_event->height > 0) && (height != configure_event->height))) 
					{
						resize = true;
						width = configure_event->width;
						height = configure_event->height;
					}
				}
				break;
				
				case XCB_CLIENT_MESSAGE:
					if ((*(xcb_client_message_event_t*)event).data.data32[0] == (*delete_reply).atom) 
					{
						loop = false;
						free(delete_reply);
					}
					break;
				case XCB_KEY_PRESS:
					loop = false;
					break;
				}
				free(event);
			}
			else 
			{
				if (resize) 
				{
					resize = false;
					if (!OnWindowSizeChanged()) 
					{
						result = false;
						break;
					}
				}
				if (m_swapChain->IsRenderAvailable()) 
				{
					if (!Draw()) {
						result = false;
						break;
					}
				}
				else 
				{
					std::this_thread::sleep_for(std::chrono::milliseconds(100));
				}
			}
		}

		return result;

	}

	#endif
}



