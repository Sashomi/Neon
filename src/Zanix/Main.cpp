#include <iostream>

#include <vector>

#include <Zanix/Core/ZPlugin.hpp>
#include <Zanix/Core/ZString.hpp>
#include <Zanix/Core/ZException.hpp>
#include <Zanix/Graphics/ZWindow.hpp>
#include <Zanix/Renderer/ZVulkan.hpp>
#include <Zanix/Component/ZDevice.hpp>

using namespace Zx;

int main(void) 
{
	//--------Zanix Engine--------	
	
	ZWindow w(600, 200, "Zanix");
	ZVulkan::Initialize(w);

	ZVulkan::UnInitialize();
	w.CloseWindow();

	system("PAUSE");
  
  return 0;
}