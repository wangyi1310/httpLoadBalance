#ifndef _SEM_H_
#define _SEM_H_
#include <stdio.h>  
#include <signal.h>  
#include <assert.h>  
#include <sys/sem.h>  
union semun  
{  
        int val;  
  
};  
void sem_init();  
void sem_p();  
void sem_v();  
void del_sem();
#endif
