#pragma once
/////////////////////////////////////////////////////////////////////
// Loader.h - Naive directory explorer                             //
// ver 1.5                                                         //
// Author  : Ammar Salman, assalman@syr.edu                        //
// Source  : Jim Fawcett, CSE687 - OOD, Fall 2018                  //
/////////////////////////////////////////////////////////////////////
// Modified by   : Yue Liu , Syracuse University                   //
//                 315-278-7363, yliu241@syr.edu                   //                
/////////////////////////////////////////////////////////////////////
/*
 * Package Operations:
 * -------------------
 * Loader provides a class, of the same name, that executes a
 * depth first search of a directory tree rooted at a specified path.
 * Each time it enters a directory, it invokes its member function
 * doDir, and for every file in that directory, it invokes doFile.
 *
 *  NOTE: there are TWO modes for compiling this file:
 *   1) Preprocessor TEST_DIREXPLORERN : runs in full demo mode for this package.
 *
 * Required Files:
 * ---------------
 * Loader.h, Loader.cpp
 * FileSystem.h, FileSystem.cpp      // Directory and Path classes
 * Logger.h
 * StringUtilities.h                 // Title function
 * CodeUtilities.h                   // ProcessCmdLine class
 *
 * Maintenance History:
 * --------------------
 * ver 1.5 : 5 March 2019
 * - release
 * ver 1.4 : 11 Feb 2019
 * - Added file containers
 *   Added regex matching
 * ver 1.3 : 19 Aug 2018
 * - Removed some options to make this version simple.  Those are
 *   implemented in the more advanced navigators, presented here.
 * ver 1.2 : 17 Aug 2018
 * - Moved method definitions to inlines, below the class declaration.
 * ver 1.1 : 16 Aug 2018
 * - Made no recursion default, added option /s for recursion.
 * ver 1.0 : 11 Aug 2018
 * - first release
 *
*/

#include <vector>
#include <iostream>
#include "../FileSystem/FileSystem.h"

namespace FileSystem
{
  class DirExplorerN
  {
  public:
    using patterns = std::vector<std::string>;
    using files = std::vector<std::string>;

    static std::string version() { return "ver 1.4"; }

    DirExplorerN(const std::string& path);

    // set options for analysis

    void addPattern(const std::string& patt);
    void recurse(bool doRecurse = true);

    // conduct depth first search

    bool search();
    void find(const std::string& path);
    bool match_regexes(const std::vector<std::string>& regexes);

    // define what happens when a file or dir is encountered

    void doFile(const std::string& dir, const std::string& filename);
    void doDir(const std::string& dirname);

    // extract traversal statistics 
    const files& filesList() const; 
    size_t fileCount();
    size_t dirCount();
    void showStats();

  private:
    std::string path_;
    patterns patterns_;
    files files_;
    size_t dirCount_ = 0;
    size_t fileCount_ = 0;
    bool recurse_ = false;
  };

  
}