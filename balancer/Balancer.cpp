
//负载均衡器的实现
#include "Balancer.h"
#include "../json/MyJson.hpp"
#include <stdlib.h>
#include <string>
#include <string.h>
int Balancer::count=0;
Balancer::Balancer(const char * ip,unsigned short port)
{
	mListenfd=socket(AF_INET,SOCK_STREAM,0);
	log=Logger::GetLog(string("Balancer"));
	if(mListenfd==-1)
	{
		log->LogError(string("Create mListenfd is fail"));
		exit(0);
	}
	sockaddr_in saddr,caddr;
	memset(&saddr,0,sizeof(saddr));
	saddr.sin_family=AF_INET;
	saddr.sin_port=htons(port);
	saddr.sin_addr.s_addr=inet_addr(ip);
	int res=bind(mListenfd,(struct sockaddr*)&saddr,sizeof(saddr));
	if(res==-1)
	{
		log->LogError(string("Bind mListen is fail"));
		exit(0);
	}
	listen(mListenfd,5);
	log->LogNotice("Server Listen Start is Ok ");
	if(LoadEtcSer("etc.txt")!=true&&mSerMap.size()==0)
	{
		exit(0);
	}
	InitServers();
	if(mCOnser.size()<1)
	{
		exit(0);
	}
}
int Balancer::Accept()
{
	sockaddr caddr;
	socklen_t len=sizeof(caddr);
	return accept(mListenfd,(sockaddr*)&caddr,&len);
}
void  Balancer::RecvCli(int c)
{
	MyJson::Value root("root");
	root["cfd"]=c;
	char buff[128]="";
	recv(c,buff,128,0);
	root["data"]=buff;
	string str=root.toStyedString();
	send(vec[count++%vec.size()],str.c_str(),strlen(str.c_str()),0);//客户端以来就将他转发；防止惊群现象采用锁
}
bool Balancer::LoadEtcSer(const char *etcpath)
{
	int fd=open(etcpath,O_RDONLY);
	if(fd==-1)
	{
		log->LogError(string("Open etcfile is fail"));
		return false;
	}
	log->LogNotice("Open Etcfile is Ok");
	MyJson::Reader reader;
	char etc[128]="";
	read(fd,etc,128);
	MyJson::Value root("root1");
	reader.parse(etc,root);
	int sers=root["num"].asInt();
	char serinfo[40]="";
	char serinfo2[40]="";
	char *p=NULL;
	char *q=NULL;
	char ip[16]="";
	char sport[20]="";
	unsigned port;
	for(int i=1;i<sers+1;i++)
	{
		p=q=NULL;
		sprintf(serinfo,"ser%d",i);
		string ssr=root[serinfo].asString();
		strcpy(serinfo2,ssr.c_str());
		p=strchr(serinfo2,'=');
		q=strchr(serinfo2,',');
		strncpy(ip,p+1,q-p-1);
		p=q+1;
		p=strchr(p,'=');
		q=strchr(p,';');
		strncpy(sport,p+1,q-p-1);
	    port=atoi(sport);
		mSerMap.insert(make_pair(ip,port));
		memset(serinfo,0,40);
		memset(serinfo2,0,40);
	}
	return true;
}
void Balancer::InitServers()
{
	int Confd;
	map<string,unsigned short>::iterator it=mSerMap.begin();
	char buff[128]="";
	for(;it!=mSerMap.end();it++)
	{
		Confd=socket(AF_INET,SOCK_STREAM,0);
		sockaddr_in caddr;
		memset(&caddr,0,sizeof(caddr));
		caddr.sin_family=AF_INET;
		caddr.sin_port=htons(it->second);
		caddr.sin_addr.s_addr=inet_addr((it->first).c_str());
		int res=connect(Confd,(sockaddr*)&caddr,sizeof(caddr));
		strcpy(buff,(it->first).c_str());
		sprintf(buff+strlen(buff)," port=%d",it->second);
		if(res==-1)
		{
			strcat(buff,":Server COnnect fail");
			log->LogError(buff);
		}
		else
		{
			strcat(buff,":Server Connect success");
			mCOnser.insert(make_pair(it->first,Confd));
			vec.push_back(Confd);
			log->LogNotice(buff);
		}
		memset(buff,0,128);
	}
}
void Balancer::RecvSer()
{
	MyJson::Reader reader;
	char buff[128];
	MyJson::Value root("root");
	recv(vec[count%vec.size()],buff,128,0);
	reader.parse(buff,root);
	int x=root["cfd"].asInt();
	string str1=root["data"].asString();
	send(x,str1.c_str(),strlen(str1.c_str()),0);
}




