/////////////////////////////////////////////////////////////////////////
// ServerPrototype.cpp - Console App that processes incoming messages  //
// ver 1.0                                                             //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2018           //
/////////////////////////////////////////////////////////////////////////
// Modified by   : Yue Liu , Syracuse University                       //
//                 315-278-7363, yliu241@syr.edu                       //                
/////////////////////////////////////////////////////////////////////////
/*
*  Package Operations:
* ---------------------
*  Package contains one class, Server, that contains a Message-Passing Communication
*  facility. It processes each message by invoking an installed callable object
*  defined by the message's command key.
*
*  Message handling runs on a child thread, so the Server main thread is free to do
*  any necessary background processing (none, so far).
*
*  Required Files:
* -----------------
*  Server.h, Server.cpp
*  Comm.h, Comm.cpp, IComm.h
*  Message.h, Message.cpp
*  FileSystem.h, FileSystem.cpp
*  Utilities.h IPublisher.h
*
*  Maintenance History:
* ----------------------
*  ver 1.1 : 4/30/2019
   - add some new delegate function, such as convertfiles sendfiles
*  ver 1.0 : 3/27/2018
*  - first release
*/

#include "Server.h"
#include "../FileSystem/FileSystem.h"
#include "../Utilities/CodeUtilities/CodeUtilities.h"
#include "../Publisher/IPublisher.h"

#include <windows.h>
#include <chrono>

namespace MsgPassComm = MsgPassingCommunication;

using namespace Repository;
using namespace FileSystem;
using Msg = MsgPassingCommunication::Message;

//----< get files from target Dir  >---------------------------
Files Server::getFiles(const std::string &patt, const Repository::SearchPath& path)
{
	return Directory::getFiles(path,patt);
}
//----< get Dirs from target Dir  >---------------------------
Dirs Server::getDirs(const Repository::SearchPath& path)
{
	return Directory::getDirectories(path);
}


template<typename T>
void show(const T& t, const std::string& msg)
{
	std::cout << "\n  " << msg.c_str();
	for (auto item : t)
	{
		std::cout << "\n    " << item.c_str();
	}
}
//----< Server self test unit  >---------------------------
std::function<Msg(Msg)> echo = [](Msg msg) {
	Msg reply = msg;
	reply.to(msg.from());
	reply.from(msg.to());
	return reply;
};

//----< Server that getFiles command message  >---------------------------
std::function<Msg(Msg)> getFiles = [](Msg msg) {
	Msg reply;
	reply.to(msg.from());
	reply.from(msg.to());
	reply.command("getFiles");
	std::string path = msg.value("path");
	std::string suffix1 = msg.value("suffix1");
	std::string suffix2 = msg.value("suffix2");
	if (path != "")
	{
		std::string searchPath = storageRoot;
		if (path != ".")
			searchPath = searchPath + "\\" + path;
		Files files;
		Files files1 = Server::getFiles(suffix1,searchPath);
		Files files2 = Server::getFiles(suffix2,searchPath);
		for (auto item : files1)
		{
			files.push_back(item);
		}
		for (auto item : files2)
		{
			files.push_back(item);
		}
		size_t count = 0;
		for (auto item : files)
		{
			std::string countStr = Utilities::Converter<size_t>::toString(++count);
			reply.attribute("file" + countStr, item);
		}
	}
	else
	{
		std::cout << "\n  getFiles message did not define a path attribute";
	}
	return reply;
};
//----< Server that getDirs command message  >---------------------------
std::function<Msg(Msg)> getDirs = [](Msg msg) {
	Msg reply;
	reply.to(msg.from());
	reply.from(msg.to());
	reply.command("getDirs");
	std::string path = msg.value("path");
	if (path != "")
	{
		std::string searchPath = storageRoot;
		if (path != ".")
			searchPath = searchPath + "\\" + path;
		Files dirs = Server::getDirs(searchPath);
		size_t count = 0;
		for (auto item : dirs)
		{
			if (item != ".." && item != ".")
			{
				std::string countStr = Utilities::Converter<size_t>::toString(++count);
				reply.attribute("dir" + countStr, item);
			}
		}
		std::string filespath = Path::getFullFileSpec(searchPath);
		reply.attribute("path", filespath);
	}
	else
	{
		std::cout << "\n  getDirs message did not define a path attribute";
	}
	return reply;
};
//----< Server that convertFiles command message  >---------------------------
std::function<Msg(Msg)> convertFiles = [](Msg msg) {
	Msg reply;
	reply.to(msg.from());
	reply.from(msg.to());
	reply.command("convertFiles");
	std::string cmd = msg.value("cmd");
	std::stringstream cmdTran(cmd);
	std::string temp;
	std::vector<std::string> cmdset;
	std::vector<std::string> files_;
	while (cmdTran >> temp)
	{
		cmdset.push_back(temp);
	}
	int argc = cmdset.size();
	IPublisher * Publisher_ = nullptr;
	Publisher_ = createNativeCpp();
	files_ = Publisher_->accept_cmd(argc, cmdset);
	Publisher_->dependencyadd();
	size_t count = 0;
	for (auto item : files_)
	{
		std::string countStr = Utilities::Converter<size_t>::toString(++count);
		item = Path::getName(item);
		reply.attribute("ConvertedFiles" + countStr, item);
	}
	return reply;
};
//----< Server that sendfiles command message  >---------------------------
std::function<Msg(Msg)> sendfiles = [](Msg msg) {
	Msg reply;
	reply.to(msg.from());
	reply.from(msg.to());
	reply.command("sendfiles");
	std::string path = msg.value("path");
	std::string filename = msg.value("filename");
	if (path != "")
	{
		std::string searchPath = storageRoot;
		if (path != ".")
			searchPath = searchPath + "\\" + path;
		Files files = Server::getFiles("*.*",searchPath);
		for (auto tempfile : files)
		{
			if(tempfile.find(filename) != tempfile.npos && filename != "")
				reply.file(filename);
		}
	}
	return reply;
};

int main()
{
	std::cout << "\n  Testing Server Prototype";
	std::cout << "\n ==========================";
	std::cout << "\n";
	//StaticLogger<1>::attach(&std::cout);
	//StaticLogger<1>::start();
	Server server(serverEndPoint, "ServerPrototype");
	server.start();
	std::string suffix = "*.cpp";
	std::cout << "\n  testing getFiles and getDirs methods";
	std::cout << "\n --------------------------------------";
	Files files = server.getFiles(suffix);
	show(files, "Files:");
	Dirs dirs = server.getDirs();
	show(dirs, "Dirs:");
	std::cout << "\n";
	std::cout << "\n  testing message processing";
	std::cout << "\n ----------------------------";
	server.addMsgProc("echo", echo);
	server.addMsgProc("getFiles", getFiles);
	server.addMsgProc("getDirs", getDirs);
	server.addMsgProc("convertFiles", convertFiles);
	server.addMsgProc("sendfiles", sendfiles);
	server.addMsgProc("serverQuit", echo);
	server.processMessages();
	Msg msg(serverEndPoint, serverEndPoint);  // send to self
	msg.name("msgToSelf");
	msg.command("echo");
	msg.attribute("verbose", "show me");
	server.postMessage(msg);
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	msg.command("getFiles");
	msg.remove("verbose");
	msg.attributes()["path"] = storageRoot;
	server.postMessage(msg);
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	msg.command("getDirs");
	msg.attributes()["path"] = storageRoot;
	server.postMessage(msg);
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	std::cout << "\n  press enter to exit";
	std::cin.get();
	std::cout << "\n";
	msg.command("serverQuit");
	server.postMessage(msg);
	server.stop();
	return 0;
}

