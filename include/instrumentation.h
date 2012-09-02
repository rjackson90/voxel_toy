#ifndef INSTRUMENTATION_H
#define INSTRUMENTATION_H

#include <pthread.h>
#include <time.h>
#include <list>
#include <iostream>
#include <fstream>

#define EVENT_FUNC_ENTER    0x01
#define EVENT_FUNC_EXIT     0x02

typedef struct
{
    unsigned char event_code;
    intptr_t func_addr;
    intptr_t caller_addr;
    pthread_t thread_id;
    timespec ts;
}Event;

typedef struct
{
    unsigned char sizeof_size_t;
    unsigned char sizeof_event;
    unsigned char sizeof_intptr;
    unsigned char sizeof_pthread;
    unsigned char sizeof_time;
    unsigned char sizeof_long;
    long record_count;
}Header;

using namespace std;

static list<Event> *events = new list<Event>;

#define ALLOC_SIZE      4096
#define ALLOC_THRESHOLD 128

void instrumentation_status(bool);

void setup(void) __attribute__ ((constructor));
void teardown(void) __attribute__ ((destructor));

extern "C"
{
    void __cyg_profile_func_enter(void*, void*);
    void __cyg_profile_func_exit(void*, void*);
}

#endif

