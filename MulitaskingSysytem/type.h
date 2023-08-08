#ifndef TYPE_H
#define TYPE_H

#define NPROC 12
#define SSIZE 1024

#define FREE 0
#define READY 1
#define SLEEP 2
#define ZOMBIE 3


typedef struct proc{
    struct proc *next;
    int *ksp;
    int pid;
    int ppid;
    int status;
    int priority;
    int kstack[SSIZE];
    int event;
    int exitCode;
    struct proc *child;
    struct proc *sibling;
    struct proc *parent;
}PROC;
#endif