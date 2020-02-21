///////////////////////////////////////////////////////////////////////////
// Display.h     : defines webpage display using browser functions       //
//                 wrapping  parts of the web page in <div> blocks       //
// ver 1.0                                                               //
//                                                                       // 
// Application   : OOD-S19 Instructor Solution                           //
// Platform      : Visual Studio Community 2017, Windows 10              //
///////////////////////////////////////////////////////////////////////////
// Author        : Yue Liu , Syracuse University                         //
//                 315-278-7363, yliu241@syr.edu                         //                
///////////////////////////////////////////////////////////////////////////
/*
*  Package Operations:
* =======================
*  This package defines Display class which accepts a list of files as a
*  vector<string> and uses the default internet browser to display them
*  one by one. Please note that the functionality has limiations:
*   1) Opera/MS Edge: will pop-up all tabs instantly.
*   2) Chrome/Firefox: will pop-up windows separately only if no
*      already existing Chrome/Firefox window is opened (all must be
*      closed before running this).
*
*
*   The Display package support wrapping selected parts of the web page in <div> 
*   blocks that can be displayed or hidden6. The parts to be selected are:
*   -Comment blocks at the begining of each source code file, if they exist
*   -Function bodies, including the opening and closing braces
*   -Class bodies, including the opening and closing braces
*
*   NOTE: there are TWO modes for compiling this file:
*   1) Preprocessor TEST_DISPLAY : runs in full demo mode for this package.
*
*  Required Files:
* =======================
*  Display.h Display.cpp Process.h Process.cpp
*  Logger.h Logger.cpp FileSystem.h FileSystem.cpp
*
*  Maintainence History:
* =======================
*  ver 1.0 - 5 March 2019
*  - first release
*/

#include "Display.h"
#include "../Process/Process.h"
#include "../Logger/Logger.h"
#include "../FileSystem/FileSystem.h"
#include <string>

using namespace Logging;

// -----< default ctor >--------------------------------------------------
Display::Display() { }

// -----< display and hide comment, add blocks to HTML files >--------------
std::vector<std::string> Display::comment(std::vector<std::string> content)
{
	for (auto it1 = content.begin(); it1 != content.end(); ++it1)
	{
		std::size_t pos1 = (*it1).find("//");
		std::size_t pos2 = (*it1).find("/*");
		std::size_t pos3 = (*it1).find("*/");
		std::string mark1 = "<div class=\"toggleCommentsDisplay\">";
		std::string mark2 = "</div>";
		if (pos1 != (*it1).npos)
		{
			(*it1) = (*it1).insert(pos1, mark1);
			if(pos1 == 0)
				find_next(++it1);
			(*it1) = (*it1) + mark2;
		}
		else if (pos2 != (*it1).npos)
		{
			(*it1) = (*it1).insert(pos2, mark1);
			if (pos3 != (*it1).npos)
				(*it1) = (*it1) + mark2;
		}
		else if (pos3 != (*it1).npos)
			(*it1) = (*it1) + mark2;
	}
	return content;
}

// -----< display and hide class and function, add blocks to HTML files >-----------
std::vector<std::string> Display::classandfunct(std::vector<std::string> line)
{
	std::string mark1 = "<div class=\"toggleCommentsDisplay\">";
	std::string mark2 = "<div class=\"toggleMethodDisplay\">";
	std::string mark3 = "<div class=\"toggleClassDisplay\">";
	std::string markDemo = "</div>";
	for (auto i : classbegin_) 
	{
		size_t pos = line[i-1].find_first_of("{");
		if (pos != line[i - 1].npos)
			line[i - 1] = line[i - 1].insert(pos, mark3);
	}
	for (auto i : classend_)
	{
		if (i <= 1) continue;
		size_t pos = line[i - 1].find_last_of("}");
		if (pos != line[i - 1].npos)
			line[i - 1] = line[i - 1] + markDemo;
		else
			line[i - 2] = line[i - 2] + markDemo;
	}
	for (auto i : functbegin_)
	{
		size_t pos = line[i - 1].find_first_of("{");
		if (pos != line[i - 1].npos)
			line[i - 1] = line[i - 1].insert(pos, mark2);
	}
	for (auto i : functend_)
	{
		if (i <= 1) continue;
		size_t pos = line[i - 1].find_last_of("}");
		if (pos != line[i - 1].npos)
			line[i - 1] = line[i - 1] + markDemo;
		else
			line[i - 2] = line[i - 2] + markDemo;
	}
	return line;
}

// -----< set begin and end lines of the file, receive information from AbsTree >----------------
void Display::set_line(std::vector<int> classbegin, std::vector<int> classend, std::vector<int> functbegin, std::vector<int> functend)
{
	classbegin_ = classbegin;
	classend_ = classend;
	functbegin_ = functbegin;
	functend_ = functend;
}

// -----< display single file >-------------------------------------------
void Display::display(const std::string & file)
{
  LoggerDebug::write("\n  Displaying file: " + FileSystem::Path::getName(file) + " in browser");
  LoggerDemo::write("\n  Displaying file: " + FileSystem::Path::getName(file) + " in browser");

  CBP callback = []() {
    LoggerDebug::write("\n  --- child browser exited with this message ---");
    LoggerDemo::write("\n  --- child browser exited with this message ---");
  };

  Process p;
  p.application("C:\\Windows\\System32\\cmd.exe");
  std::string cmd = "/C " + FileSystem::Path::getFullFileSpec(file);

  p.commandLine(cmd);
  p.create();
  p.setCallBackProcessing(callback);
  p.registerCallback();
  WaitForSingleObject(p.getProcessHandle(), INFINITE);
}

// -----< display multiple files  >---------------------------------------
void Display::display(const std::vector<std::string>& files)
{
  for (auto file : files) 
    display(file);
}

void Display::find_next(std::vector<std::string>::iterator &it)
{
	std::size_t pos1 = (*it).find("//");
	if (pos1 != (*it).npos)
	{
		find_next(++it);
	}
	else
		--it;
}

//#define TEST_DISPLAY
#ifdef TEST_DISPLAY

#include <iostream>

int main() {
  LoggerDebug::attach(&std::cout);
  LoggerDebug::start();

  std::vector<std::string> files;
  files.push_back("..\\ConvertedWebpages\\Converter.h.html");
  files.push_back("..\\ConvertedWebpages\\Converter.cpp.html");

  Display d;
  d.display(files);

  LoggerDebug::stop();
}

#endif