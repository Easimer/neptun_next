#include "hash.h"
#include "meow_hash/meow_hash_x64_aesni.h"

uint64_t hash(const void* p, uint64_t len) {
	auto H = MeowHash(MeowDefaultSeed, len, (void*)p);
	return MeowU64From(H, 0);
}

void hash_begin(void* state) {
    MeowBegin((meow_state*)state, MeowDefaultSeed);
}

void hash_feed(void* state, const void* p, uint64_t len) {
    MeowAbsorb((meow_state*)state, len, (void*)p); // NOTE(easimer): MeowAbsorb doesn't
    // modify the data at `p`
}

uint64_t hash_finish(void* state) {
    auto H = MeowEnd((meow_state*)state, NULL);
    return MeowU64From(H, 0);
}
