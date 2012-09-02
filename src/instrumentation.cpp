#include "instrumentation.h"

static bool enabled = false;
static ofstream fout("profile.dat", ios::binary);

void instrumentation_status(bool state)
{
    enabled = state;
}

void startup(void)
{
    enabled = false;
}

void teardown(void)
{
    cout << "Writing profile data to file" << endl;
    ofstream fout("profile.dat", ios::binary);

    Header h;

    h.sizeof_size_t     = (unsigned char) sizeof(size_t);
    h.sizeof_event      = (unsigned char) sizeof(Event);
    h.sizeof_intptr     = (unsigned char) sizeof(intptr_t);
    h.sizeof_pthread    = (unsigned char) sizeof(pthread_t);
    h.sizeof_time       = (unsigned char) sizeof(time_t);
    h.sizeof_long       = (unsigned char) sizeof(long);
    h.record_count      = (long) events->size();
    
    unsigned char size_h = (unsigned char)sizeof(h);
    fout.write((char *)&size_h, sizeof(size_h));
    fout.write((char *)&h, sizeof(h));

    cout << "Writing " << h.record_count << " Event logs to file" << endl;
    while(!events->empty())
    {
        Event ev = events->front();
        fout.write((char *)&ev, sizeof(ev));
        events->pop_front();
    }
    delete events;

    fout << flush;
    fout.close();
}

extern "C" void __cyg_profile_func_enter(void *func, void *caller)
{
    if(!enabled)
    {
        return;
    }

    Event ev;

    ev.event_code   = EVENT_FUNC_ENTER;
    ev.func_addr    = (intptr_t) func;
    ev.caller_addr  = (intptr_t) caller;
    ev.thread_id    = pthread_self();
    clock_gettime(CLOCK_REALTIME, &ev.ts);

    events->push_back(ev);
    //fout.write((char *)&ev, sizeof(ev));
}

extern "C" void __cyg_profile_func_exit(void *func, void *caller)
{
    if(!enabled)
    {
        return;
    }

    Event ev;

    ev.event_code   = EVENT_FUNC_EXIT;
    ev.func_addr    = (intptr_t) func;
    ev.caller_addr  = (intptr_t) caller;
    ev.thread_id    = pthread_self();
    clock_gettime(CLOCK_REALTIME, &ev.ts);

    events->push_back(ev);
    // fout.write((char *) &ev, sizeof(ev));

}



