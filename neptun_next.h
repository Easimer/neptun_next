#pragma once

#include "platform.h"
#include "ics.h"

struct cmdargs_t {
    bool status_bar = false; // -s
};

void status_bar(const cmdargs_t* args);
void verbose_mode(const cmdargs_t* args);

void load_cfg();
void unload_cfg();

[[nodiscard]]
ics_event* get_next_event(ics_event* ev_buf, bool* ongoing, ics ics_inst, u64 curtime);
