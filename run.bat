cd .\Debug
start AutoReqTest.exe
start Server.exe
cd ../
cd .\ClientGui\bin\Debug\
start ClientGui.exe .. /s /demo *.h *.cpp [A-B](.*)
