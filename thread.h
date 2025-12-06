#ifndef THREAD_H
#define THREAD_H

typedef struct {
    int id;
    int burst;
    int priority; // priority scheduler için opsiyonel
} Thread;

#endif
