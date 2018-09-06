#include <iostream>
#include <vector>
#include <map>
#include <memory>

#include <Zanix/Renderer/Device.hpp>
#include <Zanix/Renderer/SwapChain.hpp>
#include <Zanix/Renderer/Renderer.hpp>
#include <Zanix/Renderer/Pipeline.hpp>
#include <Zanix/Renderer/RenderPass.hpp>
#include <Zanix/Renderer/Window.hpp>
#include <Zanix/Renderer/VertexBuffer.hpp>
#include <Zanix/Renderer/ShaderModule.hpp>
#include <Zanix/Renderer/Sync.hpp>
#include <Zanix/Renderer/CommandBuffers.hpp>
#include <Test/Test1.hpp>

using namespace Zx;

int main(void) 
{
	//--------Zanix Engine--------
	Window window;
	Device device;
	SwapChain swap;

	window.CreateZWindow(500, 500, "toto");

	Renderer renderer(device, window, swap);

	RenderPass renderPass(device, swap);

	Pipeline pipeline(device, renderPass, swap);

	VertexBuffer vertexBuffer(device);

	CommandBuffers commandBuffers(device, swap, pipeline, renderPass);

	Sync sync(device, commandBuffers.GetRenderingResources());

	Test1 test1(renderPass, swap, pipeline, vertexBuffer, device, window, commandBuffers, commandBuffers.GetRenderingResources());
	
	test1.RenderingLoop();

	system("PAUSE");
  
	return 0;
}