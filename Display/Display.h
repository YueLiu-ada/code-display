#pragma once
///////////////////////////////////////////////////////////////////////////
// Display.h     : defines webpage display using browser functions       //
//                 wrapping  parts of the web page in <div> blocks       //
// ver 1.0                                                               //
//                                                                       // 
// Application   : OOD-S19 Instructor Solution                           //
// Platform      : Visual Studio Community 2017, Windows 10              //
///////////////////////////////////////////////////////////////////////////
// Author.       : Yue Liu , Syracuse University                         //
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


#include <vector>
#include <string>
#include <unordered_map>

class Display
{
public:
  Display();
  std::vector<std::string> comment(std::vector<std::string> line);
  std::vector<std::string> classandfunct(std::vector<std::string> line);
  void set_line(std::vector<int> classbegin, std::vector<int> classend, std::vector<int> functbegin, std::vector<int> functend);
  void display(const std::string& file);
  void display(const std::vector<std::string>& files);
  void find_next(std::vector<std::string>::iterator &it);
private:
	std::vector<int> classbegin_;
	std::vector<int> classend_;
	std::vector<int> functbegin_;
	std::vector<int> functend_;
};


