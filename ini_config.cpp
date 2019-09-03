#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include "platform.h"
#include "hash.h"
#include "ini_config.h"

// Implementation of a section-less INI config file parser

struct __ini_config {
    u64 H;
    ini_config next;
    const char* value;
};

static int read_line(FILE* f, char* buf, int bufsiz) {
    int i = 0;
    do {
        fread(buf + i, 1, 1, f);
    } while (!feof(f) && buf[i++] != '\n' && i < bufsiz - 1);
    assert(i != bufsiz - 1);
    buf[i] = 0;
    return i;
}

static void split_line(char* buf, int bufsiz, char** key, char** value) {
    *key = buf;
    while (*buf != ' ' && *buf != '=') {
        buf++;
    }
    *buf++ = 0;
    while ((*buf == ' ' || *buf == '=') && *buf != 0) {
        buf++;
    }
    *value = buf;
    while (*buf != '\n' && *buf != 0) {
        buf++;
    }
    *buf = 0;
}

ini_config open_ini_config(const char* path) {
    ini_config ret = NULL;
    ini_config cur = NULL;
    FILE* f;
    char buf_line[512];
    
    if (path) {
        f = fopen(path, "r");
        if (f) {
            while (!feof(f)) {
                char *key, *value;
                size_t vlen;
                if(read_line(f, buf_line, 512)) {
                    split_line(buf_line, 512, &key, &value);
                    vlen = strlen(value);
                    if (ret) {
                        cur = (cur->next = new __ini_config);
                    } else {
                        cur = ret = new __ini_config;
                    }
                    cur->H = hash(key, strlen(key));
                    cur->next = NULL;
                    cur->value = new char[vlen + 1];
                    memcpy((char*)cur->value, value, vlen + 1);
                }
            }
            fclose(f);
        }
    }
    
    return ret;
}

void close_ini_config(ini_config cfg) {
    ini_config cur = cfg;
    ini_config next;
    while (cur) {
        next = cur->next;
        delete[] cur->value;
        delete cur;
        cur = next;
    }
}

bool get_ini_config_str(ini_config cfg, const char* key, const char** value) {
    bool ret = false;
    u64 H = hash(key, strlen(key));
    ini_config cur = cfg;
    
    while(cur && !ret) {
        if(cur->H == H) {
            ret = true;
            if(value) {
                *value = cur->value;
            }
        }
        cur = cur->next;
    }
    
    return ret;
}

bool get_ini_config_switch(ini_config cfg, const char* key, bool* value) {
    bool ret = false;
    const char* s;
    
    if(get_ini_config_str(cfg, key, &s)) {
        ret = true;
        if (value) {
            *value = (strcmp(s, "on") == 0);
        }
    }
    
    return ret;
}

bool get_ini_config_int(ini_config cfg, const char* key, int* value) {
    bool ret = false;
    const char* s;
    int v;
    
    if(get_ini_config_str(cfg, key, &s)) {
        v = strtoll(s, NULL, 10);
        if(errno != ERANGE) {
            ret = true;
            if(value) {
                *value = v;
            }
        }
    }
    
    return ret;
}
