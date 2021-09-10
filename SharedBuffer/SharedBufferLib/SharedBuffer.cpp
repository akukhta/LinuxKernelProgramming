#include "SharedBuffer.h"

std::string const SharedBuffer::fileName = "/dev/SharedBuffer";

SharedBuffer::SharedBuffer(size_t number, Mode m)
{
	int flag;
	
	switch(m)
	{
		case Mode::READ:
		{
			flag = O_RDONLY;
			break;
		}
		
		case Mode::WRITE:
		{
			flag = O_WRONLY;
			break;
		}
		
		case Mode::READWRITE:
		{
			flag = O_RDWR;
			break;
		}
	}
	
	std::string name = fileName;
	name += std::to_string(number);
	
	fd = open(name.data(), flag);
	
	std::cout << "Here " << fd << std::endl; 
	
	if (fd < 0)
	{
		throw std::runtime_error("Can`t open the Shared Buffer Driver!");
	}
}

size_t SharedBuffer::Write(std::vector<unsigned char>  const & buf)
{
	return write(fd, buf.data(), buf.size());
}

std::vector<unsigned char> SharedBuffer::Read()
{
	size_t *sizeOfBuf = new size_t;
	if (ioctl(fd, get_size_cmd, 
		reinterpret_cast<unsigned long>(sizeOfBuf)) < 0)
	{
		throw std::runtime_error("No data");
	}

	std::vector<unsigned char> buffer(*sizeOfBuf);
	size_t bytesReaded = read(fd, buffer.data(), buffer.size());
	
	return buffer;
}
	
SharedBuffer::~SharedBuffer()
{
	close(fd);
}
