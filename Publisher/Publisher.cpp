///////////////////////////////////////////////////////////////////////////
// Publisher.cpp  : This step replaces the Executive package, from       //
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
*  IPublisher.h Display.h CodeUtilities.h Loader.h
*  DependencyTable.h Converter.h Parser.h
*
*  Maintainence History:
* =======================
*  ver 1.0 - 1 April 2019
*  - first release
*/

#include "IPublisher.h"
#include "../Display/Display.h"
#include "../Utilities/CodeUtilities/CodeUtilities.h"
#include "../Loader/Loader.h"
#include "../DependencyTable/DependencyTable.h"
#include "../Converter/Converter.h"
#include "../Parser/Parser.h"
#include <iostream>

class Publisher : public IPublisher
{
public:
	virtual void show_html(const std::string& str) override    // -----< using interface to startup chrome and show web page  >------------------------------------
	{
		display.display(str);
	}
	virtual std::vector<std::string> accept_cmd(int argc, std::vector<std::string> argv) override;
	virtual bool processCommandLineArgs(int argc, char ** argv) override;
	virtual bool extractFiles() override;
	virtual void dependencyadd() override;
private:
	Utilities::ProcessCmdLine *pcl_;
	Display display;
	std::string str_;
	std::string dirIn_;
	std::vector<std::string> files_;
};

// -----< process cmd to get info >------------------------------------
bool Publisher::processCommandLineArgs(int argc, char ** argv)
{
	pcl_ = new Utilities::ProcessCmdLine(argc, argv);

	if (pcl_->parseError())
	{
		pcl_->usage();
		std::cout << "\n\n";
		return false;
	}
	dirIn_ = pcl_->path();
	return true;
}

// -----< extract files - after processing cmd >-------------------------
bool Publisher::extractFiles()
{
	FileSystem::DirExplorerN de(pcl_->path());
	for (auto patt : pcl_->patterns())
	{
		de.addPattern(patt);
	}
	if (pcl_->hasOption("s"))
	{
		de.recurse();
	}
	bool res = de.search() && de.match_regexes(pcl_->regexes());
	files_ = de.filesList();
	return res;
}

// -----< using cmd to load files >-------------------------
std::vector<std::string> Publisher::accept_cmd(int argc, std::vector<std::string> argv)
{
	int size = argv.size();
	char** arg = new char*[size];
	for (int i = 0; i < size; i++)
	{
		arg[i] = (char*)argv[i].c_str();
	}

	if (!processCommandLineArgs(argc, arg)) int b = 0;
	if (!extractFiles()) int a = 0;
	return files_;
}

// -----< dependency analysis, convert files >------------------
void Publisher::dependencyadd()
{
	DependencyTable dt;
	CodeConverter cc;
	Display dd;
	for (auto file : files_)
	{
		CodeAnalysis::Parser *pParser_(nullptr);
		pParser_ = pParser_->handledep(file);
		std::vector<int> classbegin = pParser_->cbl();
		std::vector<int> classend = pParser_->cel();
		std::vector<int> functbegin = pParser_->fbl();
		std::vector<int> functend = pParser_->fel();
		dd.set_line(classbegin, classend, functbegin, functend);
		cc.setDisplay(dd);
		std::vector<std::string> temp = pParser_->get_dependency();
		if (temp.size() == 0)
		{
			cc.convert(file);
		}
		for (std::string dep_file : temp)
		{
			dep_file = FileSystem::Path::getFullFileSpec(dep_file);
			dt.addDependency(file, dep_file);
		}
		cc.setDepTable(dt);
		cc.convert();
	}
}

// -----< create a class pointer >------------------
IPublisher* createNativeCpp()
{
	return new Publisher();
}