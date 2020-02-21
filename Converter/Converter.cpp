///////////////////////////////////////////////////////////////////////////
// Converter.cpp : implements and provides test stub for Converter.h     //
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
#include <regex>
#include "Converter.h"
#include "../Logger/Logger.h"
#include "../FileSystem/FileSystem.h"

using namespace FileSystem;
using namespace Logging;

// -----< default ctor >--------------------------------------------------
CodeConverter::CodeConverter()
{
}

// -----< ctor to set dep table >-----------------------------------------
CodeConverter::CodeConverter(const DependencyTable & dt) : dt_(dt)
{
  LoggerDebug::write("\n  CodeConverter initialized with DependencyTable\n");
}

// -----< set dependency table function >---------------------------------
void CodeConverter::setDepTable(const DependencyTable & dt)
{
  dt_ = dt;
}

// -----< set Display function >---------------------------------
void CodeConverter::setDisplay(const Display & dd)
{
	dd_ = dd;
}

// -----< show converted files >---------------------------------
void CodeConverter::showconverted()
{
	LoggerDemo::write("\n\n  Converting files to webpages");
	LoggerDebug::write("\n\n  Converting files to webpages");
	LoggerDebug::write("\n  Output directory: " + Path::getFullFileSpec(outputDir_));
	LoggerDemo::write("\n  Output directory: " + Path::getFullFileSpec(outputDir_));
	LoggerDemo::write("\n  -- Converted: \n");
    LoggerDebug::write("\n  -- Converted: \n");
	for (auto filename : convertedFiles_)
	{
	    filename = Path::getName(filename);
		LoggerDebug::write("  ->" + filename);
        LoggerDemo::write("  ->" + filename);
		LoggerDebug::write("\n");
		LoggerDemo::write("\n");
	}
}

// -----< convert pre-set dependency table >------------------------------
std::vector<std::string> CodeConverter::convert()
{
  if (!createOutpurDir()) {
    LoggerDebug::write("\n\n  Failed to start conversion due to inability to create output directory");
    LoggerDebug::write("\n  -- Output Directory: " + Path::getFullFileSpec(outputDir_));
    return std::vector<std::string>();
  }

  for (auto entry : dt_) {
    if (convertFile(entry.first)) {
    }
    else {
      LoggerDemo::write("\n  -- Failed:    ");
      LoggerDebug::write("\n  -- Failed:    ");
    }
    std::string filename = Path::getName(entry.first);
  }
  return convertedFiles_;
}

// -----< convert single file given path >----------------------------------
std::string CodeConverter::convert(const std::string & filepath)
{
  if (!createOutpurDir()) {
    LoggerDebug::write("\n\n  Failed to start conversion due to inability to create output directory");
    LoggerDebug::write("\n  -- Output Directory: " + Path::getFullFileSpec(outputDir_));
    return "";
  }

  if (convertFile(filepath)) {
  }
  else {
    LoggerDemo::write("\n  -- Failed:    ");
    LoggerDebug::write("\n  -- Failed:    ");
  }

  std::string filename = Path::getName(filepath);
  return filename + ".html";
}

// -----< convert file given path >----------------------------------
std::vector<std::string> CodeConverter::convert(const std::vector<std::string>& files)
{
  clear();
  dt_ = DependencyTable(files);
  return convert();
}

// -----< get set output directory >----------------------------------------
const std::string & CodeConverter::outputDir() const
{
  return outputDir_;
}

// -----< set output directory >--------------------------------------------
void CodeConverter::outputDir(const std::string & dir)
{
  outputDir_ = dir;
}

// -----< create output directory >-----------------------------------------
/* returns success of creation. it could fail if dir is sys protected */
bool CodeConverter::createOutpurDir()
{
  if (!Directory::exists(outputDir_)) {
    LoggerDebug::write("\n  Creating output directory. Path: " + Path::getFullFileSpec(outputDir_));
    return Directory::create(outputDir_);
  }
  LoggerDebug::write("\n  Output directory already exists. Proceeding...");
  return true;
}

// -----< get list of converted files >-------------------------------------
const std::vector<std::string> CodeConverter::convertedFiles() const
{
	std::vector<std::string> filteredFiles;
	for (auto file : convertedFiles_) {
		if (std::regex_search(file, std::regex("A")) == 1 || std::regex_search(file, std::regex("D")) == 1)
			filteredFiles.push_back(file);
	}
	return filteredFiles;
}

// -----< clear dependency table and convertedFiles_ >--------------------
void CodeConverter::clear()
{
  convertedFiles_.clear();
  dt_.clear();
}

// -----< private - read file and create webpage >--------------------------
bool CodeConverter::convertFile(std::string file)
{
	int nR = std::count(converted.begin(), converted.end(), file);
	if (nR == 0)
	{
		std::ifstream in(file);
		if (!in.is_open() || !in.good()) {
			LoggerDebug::write("Error -- unable to read file, path may be invalid.");
			in.close();
			return false;
		}
		std::string filename = Path::getName(file);
		std::string outputPath = outputDir_ + filename + ".html";
		out_.open(outputPath, std::ofstream::out);
		if (!out_.is_open() || !out_.good()) {
			LoggerDebug::write("Error -- unable to open output file for writing.");
			in.close();
			return false;
		}
		addPreCodeHTML(filename);		// write everything to file 
		addDependencyLinks(file);
		addButton();
		addPreTag();
		std::vector<std::string> content;
		while (in.good()) {
			std::string line;
			while (std::getline(in, line)) {
				skipSpecialChars(line);
				content.push_back(line);
			}
			content = dd_.comment(content);
			content = dd_.classandfunct(content);
			for (auto i : content)
			{
				out_ << i << std::endl;
			}
		}
		addClosingTags();
		out_.close();
		convertedFiles_.push_back(outputPath);
		converted.push_back(file);
		return true;
	}
	else
		return true;
}

// -----< private - add generic HTML preliminary markup >-------------------
void CodeConverter::addPreCodeHTML(const std::string& title)
{
  out_ << "<DOCTYPE !HTML>" << std::endl;
  out_ << "<html>" << std::endl;
  out_ << "  <head>" << std::endl;
  out_ << "    <Title>" << title << "</Title>" << std::endl;
  out_ << "    <style>" << std::endl;
  out_ << "      body {" << std::endl;
  out_ << "        padding:15px 40px;" << std::endl;
  out_ << "        font-family: Consolas;" << std::endl;
  out_ << "        font-size: 1.25em;" << std::endl;
  out_ << "        font-weight: normal;" << std::endl;
  out_ << "      }" << std::endl;
  out_ << "      .toggleClassDisplay {}" << std::endl;
  out_ << "      .toggleMethodDisplay {}" << std::endl;
  out_ << "      .toggleCommentsDisplay{}" << std::endl;
  out_ << "      </style>" << std::endl;
  out_ << "<script>" << std::endl;
  out_ << "function showHideClass(className) {" << std::endl;
  out_ << "        let elems = document.getElementsByClassName(className);" << std::endl;
  out_ << "        if (elems === null) {" << std::endl;
  out_ << "          return;" << std::endl;
  out_ << "        }" << std::endl;
  out_ << "        for (let i = 0; i < elems.length; ++i) {" << std::endl;
  out_ << "          let displayProp = elems[i].style.display;" << std::endl;
  out_ << "          if (displayProp === 'none')" << std::endl;
  out_ << "            elems[i].style['display'] = \"inline\";" << std::endl;
  out_ << "          else" << std::endl;
  out_ << "            elems[i].style['display'] = \"none\";" << std::endl;
  out_ << "        }" << std::endl;
  out_ << "      }" << std::endl;
  out_ << "</script>" << std::endl << std::endl;

  out_ << "  </head>" << std::endl << std::endl;
  out_ << "  <body>" << std::endl;
}

// -----< private - add button to HTML files >-------------------
void CodeConverter::addButton()
{
	out_ << "<div>" << std::endl;
	out_ << "<label><input type=\"checkbox\" id =\"hideF\" value=\"hidefuncs\" checked=\"true\" onclick=\"showHideClass('toggleMethodDisplay')\">Functions</label><br>" << std::endl;
	out_ << "<label><input type=\"checkbox\" id =\"hideC\" value=\"hideclass\" checked=\"true\" onclick=\"showHideClass('toggleClassDisplay')\">Classes</label><br>" << std::endl;
	out_ << "<label><input type=\"checkbox\" id =\"hidec\" value=\"hidecomms\" checked=\"true\" onclick=\"showHideClass('toggleCommentsDisplay')\">Comments</label><br>" << std::endl;
	out_ << "</div>" << std::endl;
}

// -----< private - add pre tag >------------------------------------------
/* seperated into seperate function to allow for dependencies addition
*  before the actual code of the file */
void CodeConverter::addPreTag()
{
  out_ << "    <pre>" << std::endl;
}

// -----< private - add depedency links markup code >----------------------
void CodeConverter::addDependencyLinks(std::string file)
{
  std::string filename = Path::getName(file);
  if (!dt_.has(file)) { // in case of single file conversion
    LoggerDebug::write("\n    No entry found in DependencyTable for [" + filename + "]. Skipping dependency links..");
    return;
  }

  if (dt_[file].size() == 0) { // in case the file has no dependencies
    LoggerDebug::write("\n    No dependencies found for [" + filename + "]. Skipping dependency links..");
    return;
  }

  out_ << "    <h3>Dependencies: " << std::endl;
  for (auto dep : dt_[file]) {
    out_ << "      <a href=\"" << Path::getName(dep) << ".html\">" << Path::getName(dep) << "</a>" << std::endl;
  }
  out_ << "    </h3>";
}

// -----< private - add generic HTML markup closing tags >-----------------
void CodeConverter::addClosingTags()
{
  out_ << "    </pre>" << std::endl;
  out_ << "  </body>" << std::endl;
  out_ << "</html>" << std::endl;
}

// -----< private - replace HTML special chars >---------------------------
/* note: the function uses while loop to make sure ALL special characters
*  are replaced instead of just the first encounter. */
void CodeConverter::skipSpecialChars(std::string & line)
{
  size_t pos = line.npos;
  while((pos = line.find('<')) != line.npos) 
    line.replace(pos, 1, "&lt;");

  while ((pos = line.find('>')) != line.npos)
    line.replace(pos, 1, "&gt;");
}

//#define TEST_CONVERTER
#ifdef TEST_CONVERTER

// -----< test stub for converter class >-----------------------------------
int main() {
  LoggerDebug::attach(&std::cout);
  LoggerDebug::start();

  LoggerDebug::title("Testing CodeConverter functions");

  LoggerDebug::stop(); // do not log DependencyTable logs
  
  DependencyTable dt;
  dt.addDependency("Converter.h", "Converter.cpp");
  dt.addDependency("Converter.cpp", "Converter.h");
  dt.addDependency("A.h", "A.h");
  
  LoggerDebug::start(); // log from now on

  CodeConverter cc(dt);
  dt.display();
  LoggerDebug::write("\n");
  //cc.outputDir("C:\\windows\\system32\\"); // will trigger errors which are handled
  cc.convert();
  

  LoggerDebug::stop();
  return 0;
}

#endif