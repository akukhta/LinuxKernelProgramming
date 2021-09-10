#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
<<<<<<< HEAD
#include "SharedBuffer.h"
#include <iostream>

int main(int argc, char **argv)
{
	if (argc < 2)
=======

int main(int argc, char **argv)
{
	if (argc < 3 || argc > 3)
>>>>>>> 6f4aedec76e91e8885b1848e182a73c22a1d0547
	{
		printf("Wrong arguments!\n");
		return 1;
	}
	
<<<<<<< HEAD
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
	
=======
	char *bufferIndex = argv[1];
	char const *name = "/dev/SharedBuffer\0";
	char *fileName = (char*) malloc(strlen(bufferIndex) + strlen(name)); 
	char *msg = argv[2];
	
	sprintf(fileName, "%s%s", name, bufferIndex);
	printf("Filename: %s\n", fileName);
	
	int f = open("/dev/SharedBuffer2", O_WRONLY);

	if (f == NULL)
	{
		printf("Can't open the file\n");
		return 1;
	}

	write(f,  msg, strlen(msg));
>>>>>>> 6f4aedec76e91e8885b1848e182a73c22a1d0547
	return 0;
}
