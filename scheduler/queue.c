#include <stdio.h>
#include "type.h"

int enqueue(PROC **queue, PROC *p) {
    PROC *q = *queue;
    if(q == NULL || p->priority > q->priority){
        *queue = p;
        p->next = q;
    }else{
        while(q->next && p->priority <= q->next->priority){
            q = q->next;
        }
        p->next = q->next;
        q->next = p;
    }
    return 1;
}

PROC *dequeue(PROC **queue) {
    PROC *p = *queue;
    if(p){
        *queue = (*queue)->next;
    }
    return p;
}

PROC *findProc(PROC *queue, int pid){
    PROC *p = queue;
    while(p) {
        if(p->pid == pid){
            return p;
        }
        p = p->next;
    }
    return NULL;
}

int printList(char *name, PROC *p) {
    printf("%s = ", name);
    char string[40];
    while(p){
        if(p->status == 0){
            sprintf(string,"[%d %s %d]->", p->pid, "FREE", p->ppid);
        }else if(p->status == 1){
            sprintf(string,"[%d %s %d]->", p->pid, "READY", p->ppid);
        }else if(p->status == 2){
            sprintf(string,"[%d %s %d]->", p->pid, "SLEEP", p->ppid);
        }else if(p->status == 3){
            sprintf(string,"[%d %s %d]->", p->pid, "ZOMBIE", p->ppid);
        }
        printf("%s",string);
        p = p->next;
    }
    printf("NULL\n");
    return 1;
}

PROC *delete(PROC **queue, int pid) {
    PROC *p,*q;
    if(*queue == NULL){
        return NULL;
    }
    p = *queue;
    if(p->pid == pid){
        *queue = p->next;
        return p;
    }
    q = p->next;
    while(q){
        if (q->pid == pid){
            p->next = q->next;
            return q;
        }
        p = q;
        q = q->next;
    }
    return NULL;
}
