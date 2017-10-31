#include "sem.h"  
static int semid=-1;  
void sem_init()  
{  
        semid=semget((key_t)1234,1,IPC_CREAT | IPC_EXCL |0600);//创建信号量  
        if(semid==-1)//失败的话就进行获取保证两个进程都能获得同一个信号量  
        {  
                semid=semget((key_t)1234,1,0600);//获取信号量  
                if(semid==-1)  
                {  
                        perror("erro\n");  
                }  
  
        }  
        else  
        {  
                union semun a;  
                a.val=1;  
                if(semctl(semid,0,SETVAL,a)==-1)//添加控制信号量的信息  
                {  
                        perror("semctl erro");  
                }  
        }  
}  
void sem_p()  
{  
        struct sembuf a; //用当前的结构对自己定义的信号进行操作  
        a.sem_num=0;  
        a.sem_op=-1;  
        a.sem_flg=SEM_UNDO;  
        if(semop(semid,&a,1)==-1)  
        {  
                perror("erro\n");  
        }  
}  
void sem_v()  
{  
        struct sembuf a;  
        a.sem_num=0;  
        a.sem_op=1;  
        a.sem_flg=SEM_UNDO;  
        if(semop(semid,&a,1)==-1)  
        {  
                perror("error\n");  
        }  
  
}  
void del_sem()  
{  
        //union semun sem_union;  
        if(semctl(semid,0,IPC_RMID)==-1)  
        {  
                perror("erro\n");  
        }  
}  
