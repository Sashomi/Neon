#include <iostream>
#include <vector>
#include <map>

#include <vulkan/vulkan.h>

#include <Zanix/Core/Plugin.hpp>
#include <Zanix/Core/String.hpp>
#include <Zanix/Core/Exception.hpp>
#include <Zanix/Renderer/Device.hpp>
#include <Zanix/Renderer/SwapChain.hpp>
#include <Zanix/Renderer/Renderer.hpp>
#include <Zanix/Renderer/Window.hpp>
#include <Zanix/Renderer/CommandBuffers.hpp>

using namespace Zx;

int main(void) 
{
	//--------Zanix Engine--------	
	
	Window::CreateZWindow(500, 500, "toto");
	
	Renderer::Initialize();

	CommandBuffers::CreateCommandBuffers();

	system("PAUSE");
  
	return 0;
}