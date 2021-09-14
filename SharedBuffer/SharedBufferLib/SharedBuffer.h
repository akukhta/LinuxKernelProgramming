#include <vector>
#include <string>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <memory>
#include <iostream>
#include "bufioctl.h"

enum class Mode
{
	READ,
	WRITE,
	READWRITE
};

class SharedBuffer
{
public:
	
	explicit SharedBuffer(size_t number, Mode m);	
	SharedBuffer() = delete;		
	~SharedBuffer();
	std::vector<unsigned char> Read();
	size_t Write(std::vector<unsigned char> const & buf);
	operator int() { return fd; };

private:
	int fd;
	static std::string const fileName;
};
