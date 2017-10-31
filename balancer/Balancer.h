#ifndef _BALANCER_
#define _BALANCER_
#include  <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <map>
#include <string.h>
#include <vector>
#include "../log/Log.h"
using namespace std;
class Balancer
{
	public:
	Balancer(const char *ip,unsigned short port);
	bool LoadEtcSer(const char *etcpath);
	void InitServers();
	int Accept();
	void RecvCli(int fd);
	void RecvSer();
	private:
	char* mIp;
	unsigned short mport;
	vector<int> vec;
	map<string,int> mCOnser;
	multimap<string,unsigned short> mSerMap;
	int mListenfd;
	static int count;
	Logger *log;
};
#endif
