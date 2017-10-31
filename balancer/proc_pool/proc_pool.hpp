#ifndef _PROC_POOL_
#define _PROC_POOL_
#include "sem.h"  
#include <sys/shm.h>  
#include <unistd.h>  
#include <assert.h>  
#include <stdio.h> 
#include<sys/wait.h>
#include <stdlib.h>  
#include <signal.h>  
int shmid;  
typedef void (*process)(void *arg);  
typedef struct work1  
{  
    process dealfun;  
    void* arg;  
    struct work1 *next;  
}CProc_work;  
typedef struct  
{  
        int max_work;  
        int num_work;  
        int deal_work;  
        int deal_max_work;  
        int exit;  
        int rdc_work;  
        CProc_work work[20];  
  
}CProc_pool;  
void f(void *arg)  
{  
        long p=(long)arg;  
        printf("%d\n",p);  
}  
void fun(int i)  
{  
        pid_t pid;  
        int stat;  
        while((pid==waitpid(-1,&stat,WNOHANG))>0)  
        {  
                printf("child_proc exit\n",pid);  
        }  
}  
void init(CProc_pool *pool)  
{  
    pool->max_work=10;  
    pool->num_work=3;  
    pool->deal_work=0;  
    pool->exit=0;  
    pool->rdc_work=0;  
    pool->deal_max_work=20;  
    int i=0;  
    int pid;  
    for(;i<pool->num_work;i++)  
    {  
            pid=fork();  
            if(pid==0)  
            {  
                sem_init();  
                CProc_pool *pool=(CProc_pool*)shmat(shmid,NULL,0);  
            //  printf("%d\n",pool->deal_work);  
                CProc_work work;  
                while(1)  
                {  
                       while(pool->deal_work==0&&pool->exit==0);
						//需要解决方案 
						if(pool->exit==1)
						{
							exit(0);
						}
                        sem_p();  
                        if(pool->deal_work!=0)  
                        {  
                            printf("获取当前的任务 pid=%d\n",getpid());  
                            pool->deal_work--;  
                            work=pool->work[pool->deal_work];  
                            //获取当前的任务；  
                        }  
                        else  
                        {  
                                sem_v();  
                                continue;  
                        }  
                        printf("runing \n");  
                        work.dealfun(work.arg);  
                        sem_v();  
                        int n=rand()%3;  
                        sleep(n);  
                }  
                  
                  
            }  
    }  
}  
void add(CProc_pool *pool,process p,void *arg)  
{  
        CProc_work tmp;  
        tmp.dealfun=p;  
        tmp.arg=arg;  
        sem_p();  
        pool->work[pool->deal_work]=tmp;  
        pool->deal_work++;  
        sem_v();  
} 
#endif
