#include <iostream>
#include <vector>
#include <map>

#include <vulkan/vulkan.h>

#include <Zanix/Core/ZPlugin.hpp>
#include <Zanix/Core/ZString.hpp>
#include <Zanix/Core/ZException.hpp>
#include <Zanix/Graphics/ZWindow.hpp>
#include <Zanix/Renderer/ZDevice.hpp>
#include <Zanix/Renderer/ZRenderer.hpp>

using namespace Zx;

int main(void) 
{
	//--------Zanix Engine--------	

	ZWindow w(1200, 600, "Zanix");

	w.CloseWindow();

	system("PAUSE");
  
	return 0;
}