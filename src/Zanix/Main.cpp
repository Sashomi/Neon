#include <iostream>
#include <vector>
#include <map>

#include <Zanix/Core/ZPlugin.hpp>
#include <Zanix/Core/ZString.hpp>
#include <Zanix/Core/ZException.hpp>
#include <Zanix/Graphics/ZWindow.hpp>
#include <Zanix/Vulkan/ZVulkan.hpp>
#include <Zanix/Vulkan/ZDevice.hpp>

using namespace Zx;

int main(void) 
{
	//--------Zanix Engine--------	

	ZWindow w(1200, 600, "Zanix");
	ZVulkan::Initialize(w);

	ZVulkan::UnInitialize();
	w.CloseWindow();

	system("PAUSE");
  
  return 0;
}