#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

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
}
