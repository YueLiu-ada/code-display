#pragma once
///////////////////////////////////////////////////////////////////////////
// Translator.h  : C++\CLI3 translater to connect the Client GUI         //
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
*  
*
*  Maintainence History:
* =======================
*  ver 1.0 - 1 April 2019
*  - first release
*/
using namespace System;
using namespace System::Collections::Generic;

public ref class ITranslator abstract
{
public:
  virtual void show_html(String^ str) = 0;     // -----< using interface to startup chrome and show web page  >------------------------------------
  virtual List<String^>^ accept_cmd(array<System::String ^> ^args) = 0;      // -----< using interface to load files with default regex  >------------------------------------
  virtual void convert_files() = 0;           // -----< using interface to convert target files  >------------------------------------
};

public ref class TranslatorFactory
{
public:
  static ITranslator^ createTranslator();
};

