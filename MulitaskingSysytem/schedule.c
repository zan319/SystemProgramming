#include <stdio.h>
#include "type.h"
#include "queue.c"
#include "schedule.h"

PROC proc[NPROC];
PROC *freeList;
PROC *readyQueue;
PROC *sleepList;
PROC *running;
PROC *INIT;

// int scheduler();

int kcreate() {
    int i;
    PROC *p = dequeue(&freeList);
    if(!p){
        printf("no more proc !\n");
        return -1;
    }
    p->status = READY;
    p->priority = 1;
    p->ppid = running->ppid;
    p->child = NULL;
    p->sibling = NULL;
    p->parent = running->parent;
    p->event = 0;
    p->exitCode = 0;

    //set sibling of the running thread
    struct proc *bro = running->sibling;
    if(!bro) {
        running->sibling = p;
    }else{
        while(bro->sibling) {
            bro = bro->sibling;
        }
        bro->sibling = p;
    }


    for(i = 1; i <= NPROC; i++) {
        p->kstack[SSIZE - i] = 0;
    }
    p->kstack[SSIZE - 1] = (int)body;
    p->ksp = &p->kstack[SSIZE - 9];
    enqueue(&readyQueue, p);
    return p->pid;
}

int do_create() {
    int sibling = kcreate();
    if(sibling < 0) {
        printf("kfork failed\n");
    }else{
        printf("create a sibling thread = %d\n",sibling);
        printList("readyQueue",readyQueue);
    }
}

int kfork() {
    int i;
    PROC *p = dequeue(&freeList);
    if(!p){
        printf("no more proc !\n");
        return -1;
    }
    p->status = READY;
    p->priority = 1;
    p->ppid = running->pid;
    p->parent = running;
    p->child = NULL;
    p->sibling = NULL;
    p->event = 0;
    p->exitCode = 0;

    // set child of the running thread 
    // set sibling of the new thread 
    struct proc *temp = running->child;
    if(!temp){
        running->child = p;
    }else{
        p->sibling = temp;
        running->child = p;
    }
    /************************************************************
    kstack content: |retPC|eax|ebx|ecx|edx|ebp|esi|edi|eflag|
                       -1  -2  -3  -4  -5  -6  -7  -8  -9
    ************************************************************/   
    for(i =1; i <= NPROC; i++) {
        p->kstack[SSIZE - i] = 0;
    }
    p->kstack[SSIZE - 1] = (int)body;
    p->ksp = &p->kstack[SSIZE - 9];
    enqueue(&readyQueue, p);
    return p->pid;
}

int do_fork(){
    int child = kfork();
    if(child < 0){
        printf("kfork failed\n");
    }else{
        printf("proc %d forked a child = %d\n",running->pid,child);
        printList("readyQueue", readyQueue);
    }
    return child;
}

int do_switch(){
    tswitch();
}

int kexit(int exitCode){
    running->status = ZOMBIE;
    running->priority = 0;
    // exit code = 1 , it means no error happens
    running->exitCode = exitCode;
    // childen's parent are dead, so INIT becomes their new parent
    struct proc* parent = INIT;
    struct proc* child = running->child;
    if(running->child){

        //move child to the end of INIT
        struct proc* temp = parent->child;
        if(!temp){
            parent->child = child;
        }else{
            while(temp->sibling){
                temp = temp->sibling;
            }
            temp->sibling = child;
        }
        //change pid ppid of the child
        while(child){
            child->ppid = INIT->pid;
            child->parent = INIT;
            child = child->sibling;
        }
        running->child = NULL;
    }
    //enqueue(&freeList, running);
    //printList("freelist", freeList);
    printList("readyQueue", readyQueue);
    tswitch();
}

int do_exit(int exitCode) {
    kexit(exitCode);
}

int kwait(int *status) {
    if(!running->child){
        return -1;
    }
    PROC *p = running->child;
    PROC *q = running;
    while(p){
        if(p->status == ZOMBIE){
            if(p == running->child){
                running->child = p->sibling;
            }else{
                q->sibling = p->sibling;
            }
            *status = p->exitCode;
            delete(&readyQueue,p->pid);
            enqueue(&freeList, p);
            return p->pid;
        }
        q = p;
        p = p->sibling;
    }
    ksleep(running);

}

int do_wait() {
    int status = 0;
    int pid = kwait(&status);
    printList("readyQueue",readyQueue);
    printf("running : %d wait: %d status: %d\n",running->pid, pid, status);
}

int ksleep(int event){
    running->event = event;
    running->status = SLEEP;
    enqueue(&sleepList, running);
    tswitch();

}

int kwakeup(int event){
    PROC *p = sleepList;
    PROC *temp;
    while(p) {
        if(p->event == event) {
            temp = delete(&sleepList, p->pid);
            temp->status = READY;
            enqueue(&readyQueue, temp);
        }
    }
}

int init() {
    int i;
    PROC *p;
    for(i = 0; i < NPROC; i++) {
        p = &proc[i];
        p->priority = 0;
        p->status = FREE;
        p->pid = i;
        p->next = p + 1;
    }
    p->next = NULL;
    freeList = &proc[0];
    readyQueue = NULL;

    p = running = dequeue(&freeList);
    p->status = READY;
    p->ppid = 0;
    p->parent = p;
    p->child = NULL;
    p->sibling = NULL;
    p->event = 0;
    p->exitCode = 0;
    
    printList("freeList",freeList);
    printf("init completel: P0 running\n");

}

void printLine(PROC *node, int depth){
    if(node == NULL) {
        return;
    }
    if(node->status == READY || node->status == ZOMBIE){
        for(int i = 0; i < depth - 1; i++){
            printf("|    ");
        }
        printf("|----|-%d\n",node->pid);
    }
    printLine(node->child, depth+1);
    printLine(node->sibling,depth);
}

void pstree() {
    PROC *head = findProc(readyQueue,0);
    printf("|-%d\n",head->pid);
    printLine(head->child,1);
}

void do_pstree(){
    pstree();
}


int body() {
    char c;
    printf("proc %d started from body()\n",running->pid);
    while (1) {
        printf("*********************************\n");
        printf("proc %d is running: parent = %d\n",running->pid,running->ppid);
        printf("enter a key (f|c|s|q|p|w) : ");
        c = getchar(); getchar();
        switch (c) {
            case 'f': do_fork();            break;
            case 's': do_switch();          break;
            case 'q': do_exit(1);  break;
            case 'c': do_create();          break;
            case 'p': do_pstree();          break;
            case 'w': do_wait();            break;
        }
    }
}

int main() {

    printf("Welcome to the Multitasking System\n");
    init();
    kfork();
    INIT = readyQueue;
    while (1) {
        printf("P0 : switch process\n");
        if(readyQueue){
            tswitch();
        }
    }

    return 0;
}

int scheduler(){
    printf("proc %d is in scheduler()\n",running->pid);
    if(running->status == READY || running->status == ZOMBIE){
        enqueue(&readyQueue, running);
    }
    printList("readyQueue",readyQueue);
    running = dequeue(&readyQueue);
    printf("next running = %d\n",running->pid);
}