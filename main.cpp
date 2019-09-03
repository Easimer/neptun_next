#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <time.h>
#include "platform.h"
#include "ics.h"
#include "ini_config.h"

static ini_config cfg = NULL;

static void load_cfg() {
    char* path = NULL;
    int path_len = 0;
    const char* homedir = getenv("HOME");
    path_len = snprintf(NULL, 0, "%s/.config/neptun_next.cfg", homedir);
    path = (char*)malloc(path_len + 1);
    path_len = snprintf(path, path_len + 1, "%s/.config/neptun_next.cfg", homedir);

    cfg = open_ini_config(path);

    free(path);
}

static void unload_cfg() {
    if(cfg) {
        close_ini_config(cfg);
    }
}

static void print_event(const ics_event* ev, bool ongoing) {
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

int main(int argc, char** argv) {
    ics ics_inst;
    const char* ics_url;
    ics_event ev, ev_next;
    load_cfg();
    u64 curtime = time(NULL);
    u64 time_next = (u64)-1;
    bool ongoing = false;
    bool found_event = false;

    ev_next.name = ev_next.location = NULL;

    if(cfg) {
        if(get_ini_config_str(cfg, "ics_url", &ics_url)) {
            ics_inst = open_ics(ics_url);
            if(ics_inst) {
                while(next_ics_event(ics_inst, &ev)) {
                    if(ev.date_end > curtime) {
                        if(ev.date_start <= curtime) {
                            // event is ongoing
                            ev_next = ev;
                            curtime = ev.date_start;
                            ongoing = true;
                            found_event = true;
                        } else {
                            // event is coming up
                            if(ev.date_start < time_next) {
                                time_next = ev.date_start;
                                ev_next = ev;
                                found_event = true;
                            }
                        }
                    }
                    if(ev.date_end >= curtime && ev.date_start < ev_next.date_start) {
                        ev_next = ev;
                        found_event = true;
                    }
                }
                if(found_event) {
                    print_event(&ev_next, ongoing);
                }
                close_ics(ics_inst);
            } else {
                fprintf(stderr, "Failed to open ICS file\n");
            }
        } else {
            fprintf(stderr, "No ics_url present in config file! (~/.config/neptun_next.cfg)\n");
        }
        unload_cfg();
    } else {
        fprintf(stderr, "Failed to open config file\n");
    }

    return 0;
}
