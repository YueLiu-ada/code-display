///////////////////////////////////////////////////////////////////////
// StringUtilities.cpp - small, generally useful, helper classes     //
// ver 1.0                                                           //
// Language:    C++, Visual Studio 2017                              //
// Application: Most Projects, CSE687 - Object Oriented Design       //
// Author:      Jim Fawcett, Syracuse University, CST 4-187          //
//              jfawcett@twcny.rr.com                                //
///////////////////////////////////////////////////////////////////////

#include <cctype>
#include <iostream>
#include "StringUtilities.h"
#include "../CodeUtilities/CodeUtilities.h"


using namespace Utilities;

//----< write major title to console >-------------------------------

void StringHelper::title(const std::string& src)
{
	std::cout << "\n  " << src;
	std::cout << "\n " << std::string(src.size() + 2, '-');
}
//----< write minor title to console >-------------------------------

void StringHelper::Title(const std::string& src, char underline)
{
	std::cout << "\n  " << src;
	std::cout << "\n " << std::string(src.size() + 2, underline);
}
//----< convert comma separated list into vector<std::string> >------
/*
*  - also works for newline separated list
*/
std::vector<std::string> StringHelper::split(const std::string& src)
{
	std::vector<std::string> accum;
	std::string temp;
	size_t index = 0;
	do
	{
		while ((src[index] == ',' || src[index] == '\n') && index < src.length())
		{
			++index;
			if (temp.size() > 0)
			{
				accum.push_back(temp);
				temp.clear();
			}
		}
		if (src[index] != '\0')
			temp += src[index];
	} while (index++ < src.size());
	if (temp.size() > 0)
		accum.push_back(temp);
	return accum;
}
//----< remove leading and trailing whitespace >---------------------

std::string StringHelper::trim(const std::string& src)
{
	std::locale loc;
	std::string trimmed = src;
	size_t first = 0;
	while (true)
	{
		if (std::isspace(trimmed[first], loc))
			++first;
		else
			break;
	}
	size_t last = trimmed.size() - 1;
	while (true)
	{
		if (std::isspace(trimmed[last], loc) && last > 0)
			--last;
		else
			break;

	}
	return trimmed.substr(first, last - first + 1);
}
//----< wrap string in lines >---------------------------------------

std::string StringHelper::addHeaderAndFooterLines(const std::string& src)
{
	std::string line = "------------------------------";
	return line + "\n" + src + "\n" + line + "\n";
}

#ifdef TEST_STRINGUTILITIES
int main()
{
  Title("Testing Utilities Package");
  putline();

  title("test split(std::string, ',')");

  std::string test = "a, \n, bc, de, efg, i, j k lm nopq rst";
  std::cout << "\n  test string = " << test;

  std::vector<std::string> result = split(test);
  showSplits(result);

  title("test split(std::string, ' ')");

  std::cout << "\n  test string = " << test;

  result = split(test, ' ');
  showSplits(result);

  putline(2);
  return 0;
}
#endif
