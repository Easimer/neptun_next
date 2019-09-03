#pragma once

typedef struct __ics *ics;

struct ics_event {
    char* name;
    char* location;
    u64 date_start;
    u64 date_end;
};

ics open_ics(const char* URL);
void close_ics(ics instance);
bool next_ics_event(ics inst, ics_event* ev);
