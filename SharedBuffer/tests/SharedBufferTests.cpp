#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Main

#include <boost/test/unit_test.hpp>
#include <vector>
#include "SharedBuffer.h"


std::vector<unsigned char> generate()
{
	size_t countOfElements = rand() % 100;
	std::vector<unsigned char> result(countOfElements);
	
	for (size_t i = 0; i < countOfElements; i++)
	{
		result.push_back(rand() % CHAR_MAX);
	}
	
	return result;
}

bool writingTest()
{
	SharedBuffer buf(1, Mode::READWRITE);
	
	auto before = generate();
	
	buf.Write(before);
	
	auto after = buf.Read();
	
	return before == after;
}

bool countOfElementsTest()
{
	SharedBuffer buf(1, Mode::READWRITE);
	
	size_t countOfElements = rand() % 50;
	
	for (size_t i = 0; i < countOfElements; i++)
	{
		auto randomBuffer = generate();
		buf.Write(randomBuffer);
	}
	
	size_t *countOfElementsFromKLM = new size_t;
	
	if (ioctl(buf, SHARED_BUFFER_GET_COUNT_OF_ELEMENTS, reinterpret_cast<unsigned long>(countOfElementsFromKLM)) < 0)
		throw std::runtime_error("Error");
	
	for (size_t i = 0; i < countOfElements; i++)
	{
		buf.Read();
	}
	
	return *countOfElementsFromKLM == countOfElements;
}

bool currentHeadSize()
{
	auto head = generate();
	
	size_t *headSize = new size_t;
	
	if (ioctl(buf, SHARED_BUFFER_GET_SIZE_OF_CURR_HEAD, reinterpret_cast<unsigned long>(headSize)) < 0)
		throw std::runtime_error("Error");
		
	buf.Read();
	
	return *headSize == head.size();
}


BOOST_AUTO_TEST_SUITE(SharedBufferWrittingTests)

BOOST_AUTO_TEST_CASE(Writing1)
{
	BOOST_REQUIRE_EQUAL(writingTest(), true);	
}

BOOST_AUTO_TEST_CASE(Writing2)
{
	BOOST_REQUIRE_EQUAL(writingTest(), true);	
}

BOOST_AUTO_TEST_CASE(Writing3)
{
	BOOST_REQUIRE_EQUAL(writingTest(), true);	
}

BOOST_AUTO_TEST_CASE(Writing4)
{
	BOOST_REQUIRE_EQUAL(writingTest(), true);	
}

BOOST_AUTO_TEST_CASE(Writing5)
{
	BOOST_REQUIRE_EQUAL(writingTest(), true);	
}

BOOST_AUTO_TEST_SUITE_END()



BOOST_AUTO_TEST_SUITE(IOCTLCountOfElementsTests)

BOOST_AUTO_TEST_CASE(Count1)
{
	BOOST_REQUIRE_EQUAL(countOfElementsTest(), true);	
}

BOOST_AUTO_TEST_CASE(Count2)
{
	BOOST_REQUIRE_EQUAL(countOfElementsTest(), true);	
}

BOOST_AUTO_TEST_CASE(Count3)
{
	BOOST_REQUIRE_EQUAL(countOfElementsTest(), true);	
}

BOOST_AUTO_TEST_CASE(Count4)
{
	BOOST_REQUIRE_EQUAL(countOfElementsTest(), true);	
}

BOOST_AUTO_TEST_CASE(Count5)
{
	BOOST_REQUIRE_EQUAL(countOfElementsTest(), true);	
}
BOOST_AUTO_TEST_SUITE_END()



BOOST_AUTO_TEST_SUITE(CurrHeadSize)

BOOST_AUTO_TEST_CASE(HeadSize1)
{
	BOOST_REQUIRE_EQUAL(currentHeadSize(), true);
}

BOOST_AUTO_TEST_CASE(HeadSize2)
{
	BOOST_REQUIRE_EQUAL(currentHeadSize(), true);
}

BOOST_AUTO_TEST_CASE(HeadSize3)
{
	BOOST_REQUIRE_EQUAL(currentHeadSize(), true);
}

BOOST_AUTO_TEST_CASE(HeadSize4)
{
	BOOST_REQUIRE_EQUAL(currentHeadSize(), true);
}

BOOST_AUTO_TEST_CASE(HeadSize5)
{
	BOOST_REQUIRE_EQUAL(currentHeadSize(), true);
}

BOOST_AUTO_TEST_SUOTE_END()


