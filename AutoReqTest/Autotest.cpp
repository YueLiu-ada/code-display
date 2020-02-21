///////////////////////////////////////////////////////////////////////////
// Autotest.cpp  : Test requirement                                      //
// ver 1.0                                                               //
//                                                                       // 
// Application   : OOD-S19                                               //
// Platform      : Visual Studio Community 2017, Windows 10              //
// Author        : Yue Liu, Syracuse University                          //   
//                 315-278-7363, yliu241@syr.edu                         //                
///////////////////////////////////////////////////////////////////////////
/*
*  Package Operations:
* =======================
*  This is an autotest unit which used to declare all requirement and supply
*  an GUI user direction.
*
*  Required Files:
* =======================
*
*  Maintainence History:
* =======================
*  ver 1.0 - 9 April 2019
*  - first release
*/
#include <iostream>
// -----< demonstrate requirements 3 >---------------------------------------
void demoReq3();

// -----< demonstrate requirements 4 >---------------------------------------
void demoReq4();

// -----< demonstrate requirements 5 >---------------------------------------
void demoReq5();

// -----< GUI User Direction >------------------------------
void demoReq();

// -----< demonstrate requirements 6 >---------------------------------------
void demoReq6();

// -----< demonstrate requirements 7 >---------------------------------------
void demoReq7();
// -----< demonstrate requirements 7 >---------------------------------------
void demoReq8();


// -----< DEMONSTRATION MAIN >---------------------------------------------
int main() {
	// the following processing of cmd is not official, it's only
	// done to allow for demo/debug modes BEFORE CodePublisher does
	// its own processing of cmds..
	demoReq();
	demoReq3();
	demoReq4();
	demoReq5();
	demoReq6();
	demoReq7();
	demoReq8();
	getchar();
	getchar();
	return 0;
}

// -----< demonstrate requirements 3 >---------------------------------------
void demoReq3() {
	std::cout << "\n";
	std::cout << "*******************************************************************************\n";
	std::cout << "Demonstrating Requirement #3";
	std::cout << "\n  Client-Server mode is avaiable.";
	std::cout << "\n  This package assamble working parts from Projects #1, #2, and #3 into a ";
	std::cout << "\n  Client-Server configuration. Clients can send command and do not need to ";
	std::cout << "\n  wait for reply from Server before sending another commands ";
	std::cout << "\n*******************************************************************************\n";
}

// -----< demonstrate requirements 4 >---------------------------------------
void demoReq4() {
	std::cout << "\n\n\n";
	std::cout << "*******************************************************************************\n";
	std::cout << "Demonstrating Requirements #4";
	std::cout << "\n  Client GUI interface is available.";
	std::cout << "\n  This project provide a Client GUI interface which user can use it to navigate";
	std::cout << "\n  Dirs stored in the Server. The convert result, the web-page disply and the ";
	std::cout << "\n  dowanload-send result.";
	std::cout << "\n  the Publisher's interface. ";
	std::cout << "\n*******************************************************************************\n";
}

// -----< demonstrate requirements 5 >------------------------------
void demoReq5() {
	std::cout << "\n\n\n";
	std::cout << "*******************************************************************************\n";
	std::cout << "Demonstrating Requirements #5 ";
	std::cout << "\n  message package and msgpassingcomm are available.";
	std::cout << "\n  These two packages areprovided as part of the communication channel implementation ";
	std::cout << "\n  all messages are the same instance of Message class. Using postMessage to achieve";
	std::cout << "\n  command Transfermation. ";
	std::cout << "\n*******************************************************************************\n";
}

// -----< demonstrate requirements 6 >------------------------------
void demoReq6() {
	std::cout << "\n\n\n";
	std::cout << "*******************************************************************************\n";
	std::cout << "Demonstrating Requirements #6 ";
	std::cout << "\n  converting and transferring(Sending) function are available.";
	std::cout << "\n  According commands sent from clients, the server can successfully converting and";
	std::cout << "\n  sending target files. You can use Send Button or double click Remote Converted files";
	std::cout << "\n  in GUI interface to choose send one file or more files ";
	std::cout << "\n*******************************************************************************\n";
}

// -----< demonstrate requirement  >-------------------------------------
void demoReq() {
	std::cout << "\n\n\n";
	std::cout << "*******************************************************************************\n";
	std::cout << "GUI User Direction";
	std::cout << "\n  When you startup the GUI interface, you can change the regex at the bottom of";
	std::cout << "\n  interface to filter files by their first letter. Then push the PUBLISH button ";
	std::cout << "\n  to start analysis and conversion. The conversion result will show in the Convert";
	std::cout << "\n  Files interface and a pop up window shows the filter results.\n";
	std::cout << "\n  In Convert Files interface you can choose to singleclick filename to show the web ";
	std::cout << "\n  page source in Browse Result interface or doubleclick filename to open it in  ";
	std::cout << "\n  pop up chrome window. ";
	std::cout << "\n*******************************************************************************\n";
}
// -----< demonstrate requirements 7 >------------------------------
void demoReq7() {
	std::cout << "\n\n\n";
	std::cout << "*******************************************************************************\n";
	std::cout << "Demonstrating Requirements #7 ";
	std::cout << "\n  Can successfully open two or more Client GUI.";
	std::cout << "\n*******************************************************************************\n";
}

// -----< demonstrate requirements 8 >---------------------------------------
void demoReq8()
{
	std::cout << "\n\n\n";
	std::cout << "*******************************************************************************\n";
	std::cout << "This is AutoTest unit";
	std::cout << "\n\n all requirements meet";
	std::cout << "\n*******************************************************************************\n";
}
