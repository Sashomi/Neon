#include <iostream>

#include <Zanix/Graphics/ZWindow.hpp>

int main(void) {

	Zx::ZWindow a;

	a.CreateWindow(1600, 820, "Zanix Engine");

	while (a.IsOpen())
	{
		
	}

	a.CloseWindow();

	system("pause");
  
  return 0;
}