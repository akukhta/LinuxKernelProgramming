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

BOOST_AUTO_TEST_SUITE(SharedBufferWrittingTests)

BOOST_AUTO_TEST_CASE(Writing1)
{
	SharedBuffer buf(1, Mode::READWRITE);
	
	auto before = generate();
	
	buf.Write(before);
	
	auto after = buf.Read();
	
	BOOST_REQUIRE_EQUAL(before == after, true);
	
}

BOOST_AUTO_TEST_CASE(Writing2)
{
	SharedBuffer buf(1, Mode::READWRITE);
	
	auto before = generate();
	
	buf.Write(before);
	
	auto after = buf.Read();
	
	BOOST_REQUIRE_EQUAL(before == after, true);
	
}

BOOST_AUTO_TEST_CASE(Writing3)
{
	SharedBuffer buf(1, Mode::READWRITE);
	
	auto before = generate();
	
	buf.Write(before);
	
	auto after = buf.Read();
	
	BOOST_REQUIRE_EQUAL(before == after, true);
	
}

BOOST_AUTO_TEST_CASE(Writing4)
{
	SharedBuffer buf(1, Mode::READWRITE);
	
	auto before = generate();
	
	buf.Write(before);
	
	auto after = buf.Read();
	
	BOOST_REQUIRE_EQUAL(before == after, true);
	
}

BOOST_AUTO_TEST_CASE(Writing5)
{
	SharedBuffer buf(1, Mode::READWRITE);
	
	auto before = generate();
	
	buf.Write(before);
	
	auto after = buf.Read();
	
	BOOST_REQUIRE_EQUAL(before == after, true);
	
}

BOOST_AUTO_TEST_SUITE_END()

