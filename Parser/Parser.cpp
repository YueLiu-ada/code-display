/////////////////////////////////////////////////////////////////////
//  Parser.cpp - Analyzes C++ and C# language constructs           //
//  ver 1.0                                                        //
//  Language:      Visual C++, Visual Studio 2015                  //
//  Platform:      Dell XPS 8920, Windows 10                       //
//  Application:   Prototype for CSE687 - OOD Projects             //
//  Author:        Jim Fawcett, CST 4-187, Syracuse University     //
//                 (315) 443-3948, jfawcett@twcny.rr.com           //
/////////////////////////////////////////////////////////////////////
/*
  Module Operations:
  ==================
  This module defines a Parser class.  Its instances collect
  semi-expressions from one or more files for analysis.
  Analysis consists of applying a set of rules to the semi-expression,
  and for each rule that matches, invoking a set of one or more actions.

  NOTE: there are TWO modes for compiling this file:
  1) Preprocessor TEST_PARSER : runs in full demo mode for this package.

  Build Process:
  ==============
  Required files
	- Parser.h, Parser.cpp,
	- ITokenCollection.h,
	- Semi.h, Semi.cpp,
	- toker.h, toker.cpp,
	- ActionsAndRules.h, ActionsAndRules.cpp,
	- GrammarHelpers.h, GrammarHelpers.cpp,
	- AbstrSynTree.h, AbstrSynTree.cpp,
	- ScopeStack.h, ScopeStack.cpp
	- ConfigureParser.h, ConfigureParser.cpp,
	- FileSystem.h, FileSystem.cpp,
	- Logger.h, Logger.cpp,
	- Utilities.h, Utilities.cpp

  Maintenance History:
  ===================
  ver 1.0 : 5 March 2019
  - first release
  - This is a modification of the parser used in CodeAnalyzer.
  - The most important change is replacing its tokenizer with a
	State Pattern based design
*/

#include <iostream>
#include <vector>
#include <string>
#include <regex>
#include "../Utilities/Utilities.h"
#include "../Tokenizer/Toker.h"
#include "../SemiExpression/Semi.h"
#include "Parser.h"
#include "ActionsAndRules.h"
#include "ConfigureParser.h"

//using namespace CodeAnalysis;
using namespace Lexer;
using namespace Utilities_;
using Demo = Logging::StaticLogger<1>;

namespace CodeAnalysis {

  //----< register parsing rule >--------------------------------

  void Parser::addRule(IRule* pRule)
  {
    rules.push_back(pRule);
  }

  void Parser::depanalysis(Lexer::ITokenCollection* pT)
  {
	  size_t pos1 = 0;
	  size_t pos2 = 0;
	  if (pT->find("include",pos1,pos2))
	  {
		  auto it1 = pT->begin();
		  while (it1 != pT->end())
		  {
			  if (regex_search((*it1), std::regex("\\.cpp")) || regex_search((*it1), std::regex("\\.h")))
			  {
				  int t = (*it1).length() - 2;
				  *it1 = (*it1).substr(1, t);
				  dependency_files.push_back(*it1);
			  }
			  it1++;
		  }
	  }
  }
  void Parser::show_dependency()
  {
	  for (std::string file : dependency_files)
	  {
		  std::cout << file << std::endl;
	  }
  }
  //----< get next ITokenCollection >------------------------------

  bool Parser::next()
  {
    bool succeeded = pTokColl->get();
    if (!succeeded)
    {
      return false;
    }

    pTokColl->trim();

    return true;
  }

  void Parser::tree(Parser *p, ASTNode *pItem)
  {
	  if (pItem->type_ == "class")
	  {
		  p->classbegin.push_back(pItem->startLineCount_);
		  p->classend.push_back(pItem->endLineCount_);
	  }
	  else if (pItem->type_ == "function")
	  {
		  p->functbegin.push_back(pItem->startLineCount_);
		  p->functend.push_back(pItem->endLineCount_);
	  }
	  static std::string path;
	  auto iter = pItem->children_.begin();
	  while (iter != pItem->children_.end())
	  {
		  tree(p,*iter);
		  ++iter;
	  }
  }

  Parser * Parser::clone(Parser * p, ASTNode *pGlobalScope_)
  {
	  Parser *temp = new Parser(nullptr);
	  for (auto file : p->dependency_files)
	  {
		  temp->dependency_files.push_back(file);
	  }
	  tree(temp, pGlobalScope_);
	  return temp;
  }

  Parser* Parser::handledep(const std::string & file)
  {
	  std::string fileSpec_;
	  std::vector<std::string> temp;
	  fileSpec_ = FileSystem::Path::getFullFileSpec(file);
	  std::string msg = "Processing file" + fileSpec_;
	  ConfigParseForCodeAnal configure_;
	  Parser* pParser_ = configure_.Build();
	  std::string name_;
	  try
	  {
		  if (pParser_)
		  {
			  name_ = FileSystem::Path::getName(file);
			  if (!configure_.Attach(fileSpec_))
			  {
				  std::cout << "\n  could not open file " << name_ << std::endl;
				  return pParser_;
			  }
		  }
		  else
		  {
			  std::cout << "\n\n  Parser not built\n\n";
			  return pParser_;
		  }
		  Repository* pRepo_ = Repository::getInstance();
		  pRepo_->package() = name_;
		  while (pParser_->next())
		  {
			  pParser_->parse();
		  }
		  ASTNode* pGlobalScope_ = pRepo_->getGlobalScope();
		  for (size_t i = 0; i < pGlobalScope_->statements_.size(); ++i)
		  {
			  ITokenCollection* ad;
			  ad = pGlobalScope_->statements_[i];
			  pParser_->depanalysis(ad);
		  }
		  complexityEval(pGlobalScope_);
		  temp = pParser_->get_dependency();
		  return clone(pParser_, pGlobalScope_);
	  }
	  catch (std::exception& ex)
	  {
		  std::cout << "\n\n    " << ex.what() << "\n\n";
		  std::cout << "\n  exception caught at line " << __LINE__ << " ";
		  std::cout << "\n  in package \"" << name_ << "\"";
		  return nullptr;
	  }
  }

  std::vector<std::string> Parser::get_dependency()
  {
	  std::vector<std::string> temp = dependency_files;
	  return temp;
  }

  std::vector<int> Parser::cbl()
  {
	  return classbegin;
  }

  std::vector<int> Parser::cel()
  {
	  return classend;
  }

  std::vector<int> Parser::fbl()
  {
	  return functbegin;
  }

  std::vector<int> Parser::fel()
  {
	  return functend;
  }


  //----< parse the Semi by applying all rules to it >--------

  bool Parser::parse()
  {
    for (size_t i = 0; i < rules.size(); ++i)
    {
      bool doWhat = rules[i]->doTest(pTokColl);
      if (doWhat == IRule::Stop)
        break;
    }
    return true;
  }
  //----< register action with a rule >--------------------------

  void IRule::addAction(IAction *pAction)
  {
    actions.push_back(pAction);
  }
  //----< invoke all actions associated with a rule >------------

  void IRule::doActions(const Lexer::ITokenCollection* pTokColl)
  {
    if (actions.size() > 0)
      for (size_t i = 0; i < actions.size(); ++i)
        actions[i]->doAction(pTokColl);
  }


  //----< test stub >--------------------------------------------
}
//#define TEST_PARSER
#ifdef TEST_PARSER

#include "../FileSystem/FileSystem.h"
#include <queue>
#include <string>
#define Util StringHelper

using namespace CodeAnalysis;

int main(int argc, char* argv[])
{
  Util::Title("Testing Parser Class");
  putline();
  // Analyzing files, named on the command line
  if(argc < 2)
  {
    std::cout 
      << "\n  please enter name of file to process on command line\n\n";
    return 1;
  }
  std::string fileSpec;
  for(int i=1; i<argc; ++i)  // iterate over files
  {
    fileSpec = FileSystem::Path::getFullFileSpec(argv[i]);
    std::string msg = "Processing file" + fileSpec;
    Util::title(msg);
    ConfigParseForCodeAnal configure;
    Parser* pParser = configure.Build();
    std::string name;
    try
    {
      if(pParser)
      {
        name = FileSystem::Path::getName(argv[i]);
        if(!configure.Attach(fileSpec))
        {
          std::cout << "\n  could not open file " << name << std::endl;
          continue;
        }
      }
      else
      {
        std::cout << "\n\n  Parser not built\n\n";
        return 1;
      }
      // save current package name
      Repository* pRepo = Repository::getInstance();
      pRepo->package() = name;
      // parse the package
      while (pParser->next())
      {
        pParser->parse();
      }
      std::cout << "\n";
      // final AST operations
      ASTNode* pGlobalScope = pRepo->getGlobalScope();
      // walk AST, computing complexity for each node
      // and record in AST node's element
      complexityEval(pGlobalScope);     
      // Walk AST, displaying each element, indented
      // by generation
      TreeWalk(pGlobalScope);
    }
    catch(std::exception& ex)
    {
      std::cout << "\n\n    " << ex.what() << "\n\n";
      std::cout << "\n  exception caught at line " << __LINE__ << " ";
      std::cout << "\n  in package \"" << name << "\"";
    }
    std::cout << "\n";
  }  
  std::cout << "\n";
}

#endif
