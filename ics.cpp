#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include "platform.h"
#include "ics.h"
#include <curl/curl.h>

struct ics_event_list {
    ics_event ev;
    ics_event_list* next;
};

struct __ics {
    ics_event_list* head;
    ics_event_list* cur;
};

struct curl_callback_info {
    CURL* curl;
    ics_event_list* head;
    ics_event_list* last;
    ics_event_list* cur;
    char rxbuf[1024];
    int rxbuf_ptr = 0;
};

static void split_kv(curl_callback_info* inf, char** key, char** value) {
    char* buf = inf->rxbuf;
    char* end = inf->rxbuf + inf->rxbuf_ptr;
    *key = buf;
    while (*buf != ' ' && *buf != ':') {
        buf++;
    }
    *buf++ = 0;
    while ((*buf == ' ' || *buf == ':') && buf != end) {
        buf++;
    }
    *value = buf;
    while (*buf != '\n' && buf != end) {
        buf++;
    }
    *buf = 0;
}

static bool parse_icstime(const char* icstime, u64* out) {
    bool ret = false;
    struct tm tp = {0};
    char* fret;

    if(icstime && out) {
        // timezone is hardcoded here but Neptun
        // never sends any TZ other than Zulu
        fret = strptime(icstime, "%Y%m%dT%H%M%SZ", &tp); 
        if(fret && *fret == 0) {
            *out = (u64)timegm(&tp);
            ret = true;
        } else {
            fprintf(stderr, "strptime failed\n");
        }
    } else {
    }

    return ret;
}

static void process_kv(curl_callback_info* inf, const char* key, const char* value) {
    if(strcmp(key, "BEGIN") == 0 && strcmp(value, "VEVENT") == 0) {
        assert(inf->cur == NULL);
        if(inf->last) {
            assert(inf->last->next == NULL);
            ics_event_list* node = new ics_event_list;
            inf->last->next = node;
            inf->last = node;
            inf->cur = node;
            node->next = NULL;

            node->ev.name = NULL;
            node->ev.location = NULL;
            node->ev.date_start = node->ev.date_end = 0;
        } else {
            inf->head = inf->cur = inf->last = new ics_event_list;
            inf->head->next = NULL;
        }
    } else if(strcmp(key, "END") == 0 && strcmp(value, "VEVENT") == 0) {
        assert(inf->cur != NULL);
        inf->cur = NULL;
    } else if(strcmp(key, "SUMMARY") == 0) {
        assert(inf->cur != NULL);
        int len = strlen(value);
        inf->cur->ev.name = new char[len + 1];
        memcpy(inf->cur->ev.name , value, len + 1);
    } else if(strcmp(key, "LOCATION") == 0) {
        assert(inf->cur != NULL);
        int len = strlen(value);
        inf->cur->ev.location = new char[len + 1];
        memcpy(inf->cur->ev.location, value, len + 1);
    } else if(strcmp(key, "DTSTART") == 0) {
        assert(inf->cur != NULL);
        parse_icstime(value, &inf->cur->ev.date_start);
    } else if(strcmp(key, "DTEND") == 0) {
        assert(inf->cur != NULL);
        parse_icstime(value, &inf->cur->ev.date_end);
    }
}


static size_t curl_callback(char* buffer, size_t size, size_t nmemb, curl_callback_info* inf) {
    size_t ret = nmemb;
    size_t i = 0;
    char c;
    char *key, *value;

    while(i < nmemb) {
        c = buffer[i++];
        if(c == '\r') {
            // rxbuf now contains a line
            split_kv(inf, &key, &value);
            process_kv(inf, key, value);

            inf->rxbuf_ptr = 0; // reset rx ptr
            i++; // skip linefeed
        } else if(c == '\n') {
            i++; // skip linefeed
        } else {
            inf->rxbuf[inf->rxbuf_ptr++] = c;
            assert(inf->rxbuf_ptr != 1024);
        }
    }

    return ret;
}

ics open_ics(const char* URL) {
    __ics* ret = NULL;
    CURL* curl;

    curl = curl_easy_init();
    assert(curl);
    if(curl) {
        curl_callback_info cbinf;
        cbinf.curl = curl;
        cbinf.head = cbinf.last = cbinf.cur = NULL;
        curl_easy_setopt(curl, CURLOPT_URL, URL);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &cbinf);
        curl_easy_setopt(curl, CURLOPT_BUFFERSIZE, CURL_MAX_READ_SIZE);
        curl_easy_perform(curl);
        curl_easy_cleanup(curl);

        ret = new __ics;
        assert(ret);
        ret->head = ret->cur = cbinf.head;
    } else {
        fprintf(stderr, "Failed to load ICS file from URL '%s'\n", URL);
    }

    return ret;
}

void close_ics(ics instance) {
    if(instance) {
        ics_event_list *cur, *next;
        cur = instance->head;
        while(cur) {
            delete[] cur->ev.name;
            delete[] cur->ev.location;
            next = cur->next;
            delete cur;
            cur = next;
        }
        delete instance;
    }
}

bool next_ics_event(ics inst, ics_event* ev) {
    bool ret = false;
    assert(inst);
    assert(ev);
    if(inst && ev) {
        if(inst->cur) {
            *ev = inst->cur->ev;
            inst->cur = inst->cur->next;
            ret = true;
        } else {
            // reset current ptr if we've reached end of list
            inst->cur = inst->head;
        }
    }
    return ret;
}
