#include <iostream>
#include <vector>
#include <map>

#include <vulkan/vulkan.h>
#include <thread>

#include <Zanix/Core/Plugin.hpp>
#include <Zanix/Core/String.hpp>
#include <Zanix/Core/File.hpp>
#include <Zanix/Core/Exception.hpp>
#include <Zanix/Renderer/Device.hpp>
#include <Zanix/Renderer/SwapChain.hpp>
#include <Zanix/Renderer/Renderer.hpp>
#include <Zanix/Renderer/Pipeline.hpp>
#include <Zanix/Renderer/RenderPass.hpp>
#include <Zanix/Renderer/Window.hpp>
#include <Zanix/Renderer/ShaderModule.hpp>
#include <Zanix/Renderer/CommandBuffers.hpp>

using namespace Zx;


void ChildClear(Device& device, CommandBuffers& cmd, Pipeline& pip, RenderPass& pass) {
	if (device.GetDevice()->logicalDevice != VK_NULL_HANDLE) {
		vkDeviceWaitIdle(device.GetDevice()->logicalDevice);

		if ((cmd.GetCommandBuffers().size() > 0) && (cmd.GetCommandBuffers(0) != VK_NULL_HANDLE)) {
			cmd.FreeCommandBuffers();
		}

		if (cmd.GetCommandPool() != VK_NULL_HANDLE) {
			cmd.DestroyCommandPool();
		}

		if (pip.GetPipeline() != VK_NULL_HANDLE) {
			pip.DestroyPipeline();
		}

		if (pass.GetRenderPass()->renderPass != VK_NULL_HANDLE) {
			pass.DestroyRenderPass();
		}

		pass.DestroyFrameBuffer();

		pass.GetRenderPass()->framebuffer.clear();
	}
}

bool ChildOnWindowSizeChanged(RenderPass& renderPass, Pipeline& pip, CommandBuffers& cmd, Device& device, SwapChain& swap)
{
	if (!renderPass.CreateRenderPass())
		return false;

	if (!renderPass.CreateFramebuffer())
		return false;
	
	if (!pip.CreatePipeline())
		return false;

	CommandBuffers buff(device, swap, pip, renderPass);
	cmd = buff;
	
	if (!cmd.CreateCommandBuffers())
		return false;
	
	if (!cmd.RecordCommandBuffers())
		return false;

	return true;
}

bool OnWindowSizeChanged(Device& device, SwapChain& swap, RenderPass& renderPass, Pipeline& pip, CommandBuffers& cmd) {
	if (device.GetDevice()->logicalDevice != VK_NULL_HANDLE) {
		vkDeviceWaitIdle(device.GetDevice()->logicalDevice);
	}

	ChildClear(device, cmd, pip, renderPass);

	if (swap.CreateSwapChain()) {
		if (swap.IsRenderAvailable()) {
			return ChildOnWindowSizeChanged(renderPass, pip, cmd, device, swap);
		}
		return true;
	}

	return false;
}

bool Draw(SwapChain& swapChain, Device& device, CommandBuffers& cmd, RenderPass& renderPass, Pipeline& pip) {
	VkSwapchainKHR swap_chain = swapChain.GetSwapChain()->swapChain;
	uint32_t image_index = 0;

	VkResult result = vkAcquireNextImageKHR(device.GetDevice()->logicalDevice, swap_chain, UINT64_MAX, device.GetDevice()->imageAvailableSemaphore, VK_NULL_HANDLE, &image_index);
	
	switch (result) {
	case VK_SUCCESS:
	case VK_SUBOPTIMAL_KHR:
		break;
	case VK_ERROR_OUT_OF_DATE_KHR:
		return OnWindowSizeChanged(device, swapChain, renderPass, pip, cmd);
	default:
		std::cout << "Problem occurred during swap chain image acquisition!" << std::endl;
		return false;
	}

	VkPipelineStageFlags wait_dst_stage_mask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	VkSubmitInfo submit_info = {
		VK_STRUCTURE_TYPE_SUBMIT_INFO,          
		nullptr,    
		1,                                       
		&device.GetDevice()->imageAvailableSemaphore,     
		&wait_dst_stage_mask,                     
		1,                                          
		&cmd.GetCommandBuffers()[image_index], 
		1,                                          
		&device.GetDevice()->renderingFinishedSemaphore 
	};

	if (vkQueueSubmit(device.GetDevice()->graphicsQueue, 1, &submit_info, VK_NULL_HANDLE) != VK_SUCCESS) 
	{
		return false;
	}

	VkPresentInfoKHR present_info = 
	{
		VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
		nullptr,                                    
		1,                                           
		&device.GetDevice()->renderingFinishedSemaphore,
		1,                                        
		&swap_chain,                                 
		&image_index,                                
		nullptr                                       
	};
	result = vkQueuePresentKHR(device.GetDevice()->presentQueue, &present_info);

	switch (result) {
	case VK_SUCCESS:
		break;
	case VK_ERROR_OUT_OF_DATE_KHR:
	case VK_SUBOPTIMAL_KHR:
		return OnWindowSizeChanged(device, swapChain, renderPass, pip, cmd);
	default:
		std::cout << "Problem occurred during image presentation!" << std::endl;
		return false;
	}

	return true;
}

bool RenderingLoop(Window& window, SwapChain& swap, Device& device, CommandBuffers& cmd, RenderPass& renderPass, Pipeline& pip) {
	// Display window
	ShowWindow(window.GetHandle(), SW_SHOWNORMAL);
	UpdateWindow(window.GetHandle());

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
				if (!OnWindowSizeChanged(device, swap, renderPass, pip, cmd))
				{
					result = false;
					break;
				}
			}
			// Draw
			if (swap.IsRenderAvailable())
			{
				if (!Draw(swap, device, cmd, renderPass, pip))
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

int main(void) 
{
	//--------Zanix Engine--------
	Window window;
	Renderer renderer;
	Device device;
	
	SwapChain swap;

	window.CreateZWindow(500, 500, "toto");

	if (!renderer.Initialize(window, device, swap))
		std::cout << "Pas initialiser" << std::endl;

	RenderPass renderPass(device, swap);

	if(!renderPass.CreateRenderPass())
		std::cout << "Render pass pas créé" << std::endl;
	if(!renderPass.CreateFramebuffer())
		std::cout << "Framebuffer pas créé" << std::endl;

	Pipeline pipeline(device, renderPass, swap);

	if(!pipeline.CreatePipeline())
		std::cout << "Pipeline pas créé" << std::endl;
	if(!device.CreateSemaphores())
		std::cout << "Semaphores pas créé" << std::endl;
	
	CommandBuffers commandBuffers(device, swap, pipeline, renderPass);

	commandBuffers.CreateCommandBuffers();
	commandBuffers.RecordCommandBuffers();

	RenderingLoop(window, swap, device, commandBuffers, renderPass, pipeline);

	system("PAUSE");
  
	return 0;
}