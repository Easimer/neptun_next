#pragma once

typedef struct __ini_config *ini_config;

ini_config open_ini_config(const char* path);
void close_ini_config(ini_config cfg);

bool get_ini_config_str(ini_config cfg, const char* key, const char** value);
bool get_ini_config_switch(ini_config cfg, const char* key, bool* value);
bool get_ini_config_int(ini_config cfg, const char* key, int* value);
