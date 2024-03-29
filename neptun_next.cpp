#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include "platform.h"
#include "ics.h"
#include "ini_config.h"
#include "neptun_next.h"

static ini_config cfg = NULL;

//
// Function: load_cfg
// Load the configuration file
//
void load_cfg() {
    char* path = NULL;
    int path_len = 0;
    const char* homedir = getenv("HOME");
    path_len = snprintf(NULL, 0, "%s/.config/neptun_next.cfg", homedir);
    path = (char*)malloc(path_len + 1);
    path_len = snprintf(path, path_len + 1, "%s/.config/neptun_next.cfg", homedir);

    cfg = open_ini_config(path);

    free(path);
}

//
// Function: unload_cfg
// Unload the configuration file
//
void unload_cfg() {
    if(cfg) {
        close_ini_config(cfg);
    }
}

//
// Function: open_ics_cfg_url
// Read the ICS URL from the config file
// and open it.
//
// Returns: the ICS handle
[[nodiscard]]
static ics open_ics_cfg_url() {
    ics ret = NULL;
    const char* url = NULL;

    if(cfg) {
        if(get_ini_config_str(cfg, "ics_url", &url)) {
            ret = open_ics(url);
        }
    }

    return ret;
}

//
// Function: is_same_day
// Determines whether two `struct tm`s are describing
// a point in time on the same day.
//
// lhs: a struct tm
// rhs: a struct tm
//
// Returns: the result
[[nodiscard]]
static bool is_same_day(const struct tm* lhs, const struct tm* rhs) {
    bool ret = true;

    if(lhs->tm_year != rhs->tm_year) {
        ret = false;
    }
    if(lhs->tm_mon != rhs->tm_mon) {
        ret = false;
    }
    if(lhs->tm_mday != rhs->tm_mday) {
        ret = false;
    }

    return ret;
}

//
// Function: print_event_short
// Print the ICS event in the short format
// Example: "⏩09:30AM Analízis I. (ABCDEF...) @ A.105"
//
// ev: pointer to the ICS event
// ongoing: is the event an ongoing event
static void print_event_short(const ics_event* ev, bool ongoing) {
    char timebuf[64];
    char truncbuf[32];
    struct tm tp;
    const char* prefix = NULL;
    time_t t ;

    if(ongoing) {
        t = (time_t)ev->date_end;
        prefix = "⏩";
    } else {
        t = (time_t)ev->date_start;
        prefix = "⏳";
    }
    localtime_r(&t, &tp);

    strncpy(truncbuf, ev->name, 32);
    truncbuf[31] = 0; truncbuf[30] = '.'; truncbuf[29] = '.'; truncbuf[28] = '.';
    strftime(timebuf, 64, "%I:%M%p", &tp);
    fprintf(stdout, "%s%s %s @ %s\n", prefix, timebuf, truncbuf, ev->location);
}

//
// Function: print_event_verbose
// Print an event in the long format
// Example:
// ```
// NAME
// Location: LOCATION
// Starts at: DATE TIME // not present if ongoing
// Ends at: [DATE] TIME // DATE not present if start and end time is on the same day
// ```
//
// ev: pointer to the event
// ongoing: is the event an ongoing event
static void print_event_verbose(const ics_event* ev, bool ongoing) {
    char buf[64];
    struct tm tmstart, tmend;
    time_t tstart, tend;

    tstart = (time_t)ev->date_start;
    tend = (time_t)ev->date_end;
    localtime_r(&tstart, &tmstart);
    localtime_r(&tend, &tmend);

    fprintf(stdout, "%s\n", ev->name);
    fprintf(stdout, "Location: %s\n", ev->location);
    if(!ongoing) {
        strftime(buf, 64, "%F %I:%M%p", &tmstart);
        fprintf(stdout, "Starts at: %s\n", buf);
    }
    if(is_same_day(&tmstart, &tmend)) {
        strftime(buf, 64, "%I:%M%p", &tmend);
        fprintf(stdout, "Ends at: %s\n", buf);
    } else {
        strftime(buf, 64, "%F %I:%M%p", &tmend);
        fprintf(stdout, "Ends at: %s\n", buf);
    }
}

// 
// Function: get_next_event
// Retrieves the next future event in an ICS file
// 
// ev_buf: where the details of the event will be written (REQUIRED)
// ongoing: whether the event is ongoing or not (REQUIRED)
// ics_inst: handle to ICS file (REQUIRED)
//
// Returns: value of ev_buf if such an event was found or NULL
[[nodiscard]]
ics_event* get_next_event(ics_event* ev_buf, bool* ongoing, ics ics_inst, u64 curtime) {
    ics_event* ret = NULL;
    ics_event ev, ev_next;
    u64 time_next = (u64)-1;
    bool found_event = false;

    assert(ics_inst);
    assert(ongoing);

    // Iterate over events in the ICS file. Iteration stops when we reached the
    // end of the calendar or if we found out that one of the events is
    // currently ongoing.
    while(next_ics_event(ics_inst, &ev) && !(*ongoing)) {
        // Interval: [date_start; date_end[
        // Case 1: event is in the future
        if(ev.date_start > curtime && ev.date_end > curtime) {
            if(ev.date_start < time_next) {
                time_next = ev.date_start;
                ev_next = ev;
                found_event = true;
            }
        }
        // Case 2: event is ongoing
        else if(ev.date_start <= curtime && ev.date_end > curtime) {
            ev_next = ev;
            time_next = ev.date_start;
            curtime = ev.date_start;
            *ongoing = true;
            found_event = true;
        }
        // Case 3: event is in the past
        else if(ev.date_start <= curtime && ev.date_end <= curtime) {
            // Ignore
        }
    }
    if(found_event) {
        *ev_buf = ev_next;
        ret = ev_buf;
    }
    return ret;
}

void status_bar(const cmdargs_t* args) {
    ics ics_inst;
    ics_event ev = {0};
    bool ongoing = false;
    u64 curtime = time(NULL);

    ics_inst = open_ics_cfg_url();
    if(ics_inst) {
        if(get_next_event(&ev, &ongoing, ics_inst, curtime)) {
            print_event_short(&ev, ongoing);
        }
        close_ics(ics_inst);
    }
}

void verbose_mode(const cmdargs_t* args) {
    ics ics_inst;
    ics_event ev = {0};
    bool ongoing = false;
    u64 curtime = time(NULL);

    ics_inst = open_ics_cfg_url();
    if(ics_inst) {
        if(get_next_event(&ev, &ongoing, ics_inst, curtime)) {
            print_event_verbose(&ev, ongoing);
        }
        close_ics(ics_inst);
    }
}
