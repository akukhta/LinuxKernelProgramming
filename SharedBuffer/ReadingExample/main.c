#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include "SharedBuffer.h"

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		std::cout << "Wrong parameters list!" << std::endl;
		return 1;	
	}
	
	size_t number = std::stoi(argv[1]);
	
	try
	{
		SharedBuffer buf(number, Mode::READ);
		auto result = buf.Read();
		std::string str(result.begin(), result.end());
		std::cout << str << std::endl;
		return 0; 
	} 
	catch (std::runtime_error const &err)
	{
		std::cout << err.what() << std::endl;
		return 2;
	}
}
