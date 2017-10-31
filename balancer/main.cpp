#include "Balancer.h"

int main()
{
	Balancer ba("127.0.0.1",10000);
	int i=ba.Accept();
	while(1)
	{
		if(i!=-1)
		{
			ba.RecvCli(i);	
		}
		ba.RecvSer();
	}
	return 0;
}
