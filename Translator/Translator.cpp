///////////////////////////////////////////////////////////////////////////
// Translator.cpp  : C++\CLI3 translater to connect the Client GUI       //
//                   to the Code Publisher                               //
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
*  ITranslator.h IPublisher.h 
*
*  Maintainence History:
* =======================
*  ver 1.0 - 1 April 2019
*  - first release
*/

#include "ITranslator.h"
#include "../Publisher/IPublisher.h"

using namespace System;
using namespace System::Text;

public ref class Translator : ITranslator
{
public: 
  Translator()
  {
    publisher_ = createNativeCpp();
  }
  ~Translator()
  {
    delete publisher_;
    publisher_ = nullptr;
  }
  void show_html(String^ str) override     // -----< using interface to startup chrome and show web page  >------------------------------------
  {
	  publisher_->show_html(mToN(str));
  }
  List<String^>^ accept_cmd(array<System::String ^> ^args) override;
  void convert_files() override         // -----< using interface to convert target files  >------------------------------------
  {
	  publisher_->dependencyadd();
  }
private:
  std::string mToN(String^ str);
  String^ nToM(const std::string& str);
  List<String^>^ cToS(const std::vector<std::string> &files);
  IPublisher* publisher_ = nullptr;
};

ITranslator^ TranslatorFactory::createTranslator()
{
  return gcnew Translator();
}
// -----< transfer String^ to std::string >------------------------------------
std::string Translator::mToN(String^ str)
{
  std::string temp;
  for (int i = 0; i < str->Length; ++i)
    temp += char(str[i]);
  return temp;
}
// -----< transfer std::string to String^ >------------------------------------
String^ Translator::nToM(const std::string& str)
{
  StringBuilder^ sb = gcnew StringBuilder();
  for (size_t i = 0; i < str.size(); ++i)
    sb->Append((wchar_t)str[i]);
  return sb->ToString();
}

// -----< transfer std::vector<std::string> to List<String^>^ >------------------------------------
List<String^>^ Translator::cToS(const std::vector<std::string>& files)
{
	List<String^>^ temp = gcnew List<String^>();
	for (size_t i = 0; i < files.size(); ++i)
	{
		StringBuilder^ sb = gcnew StringBuilder();
		for (size_t j = 0; j < files[i].size(); ++j)
			sb->Append((wchar_t)files[i][j]);
		temp->Add(sb->ToString());
	}
	return temp;
}
// -----< using interface to load files with default regex  >------------------------------------
List<String^>^ Translator::accept_cmd(array<System::String ^> ^args)
{
	std::vector<std::string> temp;
	for (int i = 0; i < args->Length; i++)
	{
		temp.push_back(mToN(args[i]));
	}
	std::vector<std::string> files_;
	files_ = publisher_->accept_cmd(args->Length, temp);
	return cToS(files_);
}
