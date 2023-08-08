#ifndef SCHEDULE_H
#define SCHEDULE_H

#include "type.h"

int kfork();
int kexit(int exitCode);
int kwait(int *status);
int kcreate();
int ksleep(int event);
int kwakeup(int event);
void printLine(PROC *node, int depth);
int do_create();
int do_kfork();
int do_kexit(int exitCode);
int do_switch();
void do_pstree();
int do_wait();
int body();
int init();

#endif