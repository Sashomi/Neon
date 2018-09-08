#include <iostream>
#include <vector>
#include <map>
#include <memory>

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

using namespace Zx;

int main(void) 
{
	//--------Neon Engine--------
	Window window;
	Device device;
	SwapChain swap;

	window.CreateZWindow(500, 500, "toto");

	Renderer renderer(device, window, swap);

	RenderPass renderPass(device, swap);

	Pipeline pipeline(device, renderPass, swap);

	VertexBuffer vertexBuffer(device);

	CommandBuffers commandBuffers(device, swap, pipeline, renderPass);

	std::shared_ptr<std::vector<RenderingResourcesData>> renderingRessources = std::make_shared<std::vector<RenderingResourcesData>>(commandBuffers.GetRenderingResources());

	Sync sync(device, *renderingRessources);

	Test1 test1(renderPass, swap, pipeline, vertexBuffer, device, window, commandBuffers, *renderingRessources);
	
	test1.RenderingLoop();

	system("PAUSE");
  
	return 0;
}