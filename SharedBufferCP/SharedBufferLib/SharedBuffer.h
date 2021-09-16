#include <vector>
#include <string>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <memory>
#include <iostream>

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

private:
	int fd;
	static std::string const fileName;
	unsigned long const get_size_cmd = 0x8f0f0f00;
};
