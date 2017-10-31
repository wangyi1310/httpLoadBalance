#include <iostream>
#include "log/Log.h"
#include "json/MyJson.hpp"
using namespace std;
int main()
{
	Logger *log=Logger::GetLog(string("log.txt"));
	MyJson::Value root("root");
	root["num"]=2;
	root["ser1"]="ip=127.0.0.1,port=6000";
	root["ser2"]="ip=127.0.0.1,port=5000";
	int fd=open("etc.txt",O_WRONLY);
    if(fd==-1)
	{
		 log->LogError("etc error");
	}
	string str=root.toStyedString();
	write(fd,str.c_str(),strlen(str.c_str()));
	cout<<str.c_str();
	close(fd);
	log->LogError(string("cmd fail"));
	MyJson::Reader reader;
	MyJson::Value root2("root2");
	char buff[128]="";
	strcpy(buff,str.c_str());
	reader.parse(buff,root2);
	cout<<root2["ser1"].asString();
}
