#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
<<<<<<< HEAD
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
=======

int main(int argc, char **argv)
{
	int f = open("/dev/SharedBuffer2", O_RDONLY);

	if (f == NULL)
	{
		printf("Can't open the file\n");
		return 1;
	}

	int x;
	read(f, &x, sizeof(x));
	printf("Value from shared buffer = %i\n", x);
	return 0;
>>>>>>> 6f4aedec76e91e8885b1848e182a73c22a1d0547
}
