#include <iostream>

#include <Zanix/Core/ZPlugin.hpp>
#include <Zanix/Core/ZString.hpp>
#include <Zanix/Core/ZException.hpp>
#include <Zanix/Renderer/ZVulkan.hpp>

using namespace Zx;

int main(void) 
{
	//--------Zanix Engine--------	
	
	ZVulkan::Initialize("Zanix");

	system("PAUSE");
  
  return 0;
}