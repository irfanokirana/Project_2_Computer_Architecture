//
// This file defines the function signatures necessary for creating the three
// prefetchers and defines the prefetcher struct.
//

#ifndef PREFETCHERS_H
#define PREFETCHERS_H

#include <stdint.h>
#include <stdlib.h>
#include <time.h>

struct cache_system;
#include "memory_system.h"

// This struct describes the functionality of a prefetcher. The function
// pointers describe the two functions that every prefetch strategy must
// implement. Arbitrary data can be stored in the data pointer and can be used
// to store the state of the prefetcher between calls to handle_mem_access.
//
// See the documentation in replacement_policies.h for details on function
// pointers.
struct prefetcher {
    // This function allows the prefetcher to prefetch any lines it deems
    // necessary for the given memory access.
    //
    // This function should call the cache_system_mem_access function to
    // prefetch lines. It is important to pass `true` to the `is_prefetch`
    // parameter so you don't end up in an infinite-prefetch loop.
    //
    // Arguments:
    //  * prefetcher: the instance of the prefetcher
    //  * cache_system: a pointer to the cache system (useful for passing to
    //    cache_system_mem_access)
    //  * address: the memory address being accessed
    //  * is_miss: whether the access was a miss
    // Returns: how many lines were prefetched (this requires).
    uint32_t (*handle_mem_access)(struct prefetcher *prefetcher, struct cache_system *cache_system,
                                  uint32_t address, bool is_miss);

    // This function is called right before the prefetcher is deallocated. You
    // should perform any necessary cleanup operations here. (This is where you
    // should free the prefetcher->data, for example.)
    //
    // Arguments:
    //  * prefetcher: the instance of prefetcher to clean up
    void (*cleanup)(struct prefetcher *prefetcher);

    // Use this pointer to store any data that the prefetcher needs.
    void *data;
};

// Constructors for each of the replacement policies.
struct prefetcher *null_prefetcher_new();
struct prefetcher *adjacent_prefetcher_new();
struct prefetcher *sequential_prefetcher_new(uint32_t prefetch_amount);
struct prefetcher *custom_prefetcher_new();

#endif
