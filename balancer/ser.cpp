//服务器代码
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "../json/MyJson.hpp"
#include <sys/socket.h>
int main()
{
	int sockfd=socket(AF_INET,SOCK_STREAM,0);
	assert(sockfd!=-1);
	struct sockaddr_in saddr,caddr;
	memset(&saddr,0,sizeof(saddr));
	saddr.sin_family=AF_INET;
	saddr.sin_port=htons(6000);
	saddr.sin_addr.s_addr=inet_addr("127.0.0.1");
	int res=bind(sockfd,(struct sockaddr *)&saddr,sizeof(saddr));
	assert(res!=1);
	listen(sockfd,5);
	pthread_t id;
	int i=0;
	socklen_t len=sizeof(caddr);
	char buff[1024]="";
	int c=accept(sockfd,(struct sockaddr*)&caddr,&len);
	if(c<0)
	{
		exit(0);	
	}
	MyJson::Value root("root");
	MyJson::Reader reader;
	while(1)
	{
		recv(c,buff,1024,0);
		printf("%s\n",buff);
		reader.parse(buff,root);
		int x;
		x=root["cfd"].asInt();
		MyJson::Value root1("root1");
		root1["cfd"]=x;
		root1["data"]="hellowld";
		string str=root1.toStyedString();
		send(c,str.c_str(),strlen(str.c_str()),0);
	}
}
