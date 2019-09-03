#pragma once
#include <stdint.h>
#include <stddef.h>

uint64_t hash(const void* p, uint64_t len);

const size_t hash_stream_state_size = 432;
void hash_begin(void* state);
void hash_feed(void* state, const void* p, uint64_t len);
uint64_t hash_finish(void* state);
