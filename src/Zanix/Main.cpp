#include <iostream>
#include <vector>
#include <map>

#include <vulkan/vulkan.h>

#include <Zanix/Core/ZPlugin.hpp>
#include <Zanix/Core/ZString.hpp>
#include <Zanix/Core/ZException.hpp>
#include <Zanix/Renderer/ZDevice.hpp>
#include <Zanix/Renderer/ZSwapChain.hpp>
#include <Zanix/Renderer/ZRenderer.hpp>
#include <Zanix/Renderer/ZWindow.hpp>
#include <Zanix/Renderer/ZCommandBuffers.hpp>

using namespace Zx;

int main(void) 
{
	//--------Zanix Engine--------	
	
	ZWindow::CreateZWindow(500, 500, "toto");
	
	ZRenderer::Initialize();

	ZCommandBuffers::CreateCommandBuffers();

	system("PAUSE");
  
	return 0;
}