#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include "SharedBuffer.h"
#include <iostream>

int main(int argc, char **argv)
{
	if (argc < 2)
	{
		printf("Wrong arguments!\n");
		return 1;
	}
	
	size_t number = std::stoi(argv[1]);
	try
	{
		SharedBuffer buf(number, Mode::WRITE);
		std::string test = argc == 3 ? argv[2] : "Hello World, Yepta!";
		std::vector<unsigned char> b(test.begin(), test.end());
		buf.Write(b);
	}
	catch (std::runtime_error const &err)
	{
		std::cout << err.what() << std::endl;
		return 1;
	}
	
	return 0;
}
