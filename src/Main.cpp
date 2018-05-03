#include <iostream>
#include <memory>

#include <Zanix/Core/ZFile.hpp>
#include <Zanix/Core/ZException.hpp>

#include <boost/filesystem.hpp>

using namespace boost::filesystem;
using namespace Zx;

int main(void) {

	Zx::ZFile f("toto.txt");

	std::cout << f.GetFileName() << std::endl;

	system("pause");
  
  return 0;
}	
