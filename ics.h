#pragma once

typedef struct __ics *ics;

struct ics_event {
    char* name;
    char* location;
    u64 date_start;
    u64 date_end;
};

// 
// Function: open_ics
// Load an ICS file from the location pointed at by 'URL' which is a
// standard URL that may use any protocol provided that cURL
// supports it.
// A return value of NULL signals that either the retrieval or
// the parsing of the file has failed.
//
// URL: the location of the ICS file
//
// Returns: the handle to the ICS file or NULL
ics open_ics(const char* URL);

//
// Function: close_ics
// Close the ICS handle and free any memory it may use.
//
// ics: the handle
//
void close_ics(ics instance);

//
// Function: next_ics_event
// Fetch the details of the next VEVENT in the ICS file and write
// them into the ics_event structure pointed to by 'ev'.
// This structure contains pointers to other data structures
// allocated by this API. The caller code must not free them
// and may not overwrite their contents.
// These pointers are valid and will point to valid data
// until close_ics is called.
//
// ics: the handle
// ev: pointer to an ics_event structure where the output will be written
//
// Returns: - false if the end of the event list has been reached
//          - true otherwise
bool next_ics_event(ics inst, ics_event* ev);
