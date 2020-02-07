#ifndef HASHMAP_ALGOS_H
#define HASHMAP_ALGOS_H
#include <stdint.h>
uint32_t Hashmap_fnv1a_hash(void* data);
uint32_t Hashmap_adler32_hash(void* data);
uint32_t Hashmap_djb_hash(void *data);
uint32_t default_hash(void* data);
uint32_t my_hash(void *data);
#endif