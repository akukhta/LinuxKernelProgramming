#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char **argv)
{
	if (argc < 3 || argc > 3)
	{
		printf("Wrong arguments!\n");
		return 1;
	}
	
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
	return 0;
}
