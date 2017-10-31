#include"proc_pool.hpp"
#include <stdio.h>
int main()
{
  	signal(SIGCHLD,fun);  
    shmid=shmget((key_t)6666,sizeof(CProc_pool)+1,IPC_CREAT |0600);  
    assert(shmid!=-1);  
    sem_init();  
    CProc_pool *pool=(CProc_pool*)shmat(shmid,NULL,0);  
    assert(pool!=(CProc_pool*)-1);  
    init(pool);  
    CProc_work work;  
    int i=0;  
    for(;i<10;i++)  
    {  
        while(pool->deal_work==20);  
        add(pool,f,(void*)i);  
        sleep(3);  
    } 
	pool->exit=1;
    shmdt(pool);
} 
