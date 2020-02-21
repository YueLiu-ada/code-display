#pragma once
///////////////////////////////////////////////////////////////////////////
// Publisher.h    : This package replaces the Executive package, from    //
//                  Project #2, with an interface and a class that       //
//                  implements the interface to use CodePublisher's      //
//                  facilities.                                          //
// ver 1.0                                                               //
//                                                                       // 
// Application   : OOD-S19 Instructor Solution                           //
// Platform      : Visual Studio Community 2017, Windows 10              //
// Author        : Jim Fawcett, Syracuse University                      //   
///////////////////////////////////////////////////////////////////////////
// Modified by   : Yue Liu , Syracuse University                         //
//                 315-278-7363, yliu241@syr.edu                         //                
///////////////////////////////////////////////////////////////////////////
/*
*  Package Operations:
* =======================
*  This package built a C++\CLI translater to connect the Client GUI to the
*  Code Publisher, built as a dynamic link library. The translater simply
*  delegates function invocations in the GUI to calls into methods in the
*  Code Publisher, using the Publisher's interface.
*
*   NOTE: there are TWO modes for compiling this file:
*   1) Preprocessor TEST_TRAN : runs in full demo mode for this package.
*
*  Required Files:
* =======================
*
*  Maintainence History:
* =======================
*  ver 1.0 - 1 April 2019
*  - first release
*/

#include <string>
#include <vector>

struct IPublisher
{
  virtual ~IPublisher() {}
  virtual void show_html(const std::string& str) = 0;    // -----< using interface to startup chrome and show web page  >------------------------------------
  virtual std::vector<std::string> accept_cmd(int argc, std::vector<std::string> argv) = 0;     // -----< using cmd to load files >-------------------------
  virtual bool processCommandLineArgs(int argc, char ** argv) = 0;    // -----< process cmd to get info >------------------------------------
  virtual bool extractFiles() = 0;      // -----< extract files - after processing cmd >-------------------------
  virtual void dependencyadd() = 0;     // -----< dependency analysis, convert files >------------------
};

IPublisher* createNativeCpp();
