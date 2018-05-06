#include <iostream>

#include <Zanix/Maths/ZVector3.hpp>

int main(void) 
{
	//--------Zanix Engine--------

	Zx::ZVector3<int> v(10, 10, 10);
	Zx::ZVector3<int> v2;

	std::cout << v + v2 << std::endl;

	system("PAUSE");
  
  return 0;
}