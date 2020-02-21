#pragma once
///////////////////////////////////////////////////////////////////////////
// Converter.h   : defines source code conversion to webpage functions   //
// ver 1.0                                                               //
//                                                                       // 
// Application   : OOD-S19 Instructor Solution                           //
// Platform      : Visual Studio Community 2017, Windows 10 Pro x64      //
// Author        : Ammar Salman, Syracuse University                     //
//                 313/788-4694, assalman@syr.edu                        //
///////////////////////////////////////////////////////////////////////////
// Modified by   : Yue Liu , Syracuse University                         //
//                 315-278-7363, yliu241@syr.edu                         //                
///////////////////////////////////////////////////////////////////////////
/*
*  Package Operations:
* =======================
*  This package defines CodeConverter class which accepts DependencyTable
*  and uses its relationships to create linked webpages that point to each
*  other based on dependencies. It can also convert single files if needed.
*  The conversion process filters HTML special characters before printing
*  them into output files. The resulting output of this converter is a list
*  of files (vector<string>) of the created webpages.
*
*  NOTE: there are TWO modes for compiling this file:
*   1) Preprocessor TEST_CONVERTER : runs in full demo mode for this package.
*
*  Required Files:
* =======================
*  Converter.h Logger.h FileSystem.h DependencyTable.h Display.h
*
*  Maintainence History:
* =======================
*  ver 1.0 - 5 March 2019
*  - first release
*/
#include "../DependencyTable/DependencyTable.h"
#include "../Display/Display.h"
#include <vector>
#include <string>
#include <fstream>

class CodeConverter
{
public:
  CodeConverter();
  CodeConverter(const DependencyTable& dt);

  const std::string& outputDir() const;
  void outputDir(const std::string& dir);
  bool createOutpurDir();

  void setDepTable(const DependencyTable& dt);
  void setDisplay(const Display& dd);
  void showconverted();
  std::vector<std::string> convert();
  std::string convert(const std::string& filepath);
  std::vector<std::string> convert(const std::vector<std::string>& files);
  
  const std::vector<std::string> convertedFiles() const;

  void clear();

private:
  bool convertFile(std::string file);
  void addPreCodeHTML(const std::string& title);
  void addButton();
  void addPreTag();
  void addDependencyLinks(std::string file);
  void addClosingTags();
  void skipSpecialChars(std::string& line);

private:
  Display dd_;
  DependencyTable dt_;
  std::string outputDir_ = "..\\RemoteConvertedWebpages\\";
  std::vector<std::string> convertedFiles_;
  std::vector<std::string> converted;
  std::ofstream out_;
};

