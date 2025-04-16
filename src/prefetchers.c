//
// This file defines the function signatures necessary for creating the three
// cache systems and defines the prefetcher struct.
//

#include "prefetchers.h"

// Null Prefetcher
// ============================================================================
uint32_t null_handle_mem_access(struct prefetcher *prefetcher, struct cache_system *cache_system,
                                uint32_t address, bool is_miss)
{
    return 0; // No lines prefetched
}

void null_cleanup(struct prefetcher *prefetcher) {}

struct prefetcher *null_prefetcher_new()
{
    struct prefetcher *null_prefetcher = calloc(1, sizeof(struct prefetcher));
    null_prefetcher->handle_mem_access = &null_handle_mem_access;
    null_prefetcher->cleanup = &null_cleanup;
    return null_prefetcher;
}

// Sequential Prefetcher
// ============================================================================
// TODO feel free to create additional structs/enums as necessary

typedef struct {
    uint32_t prefetch_amount;
} sequential_data;

uint32_t sequential_handle_mem_access(struct prefetcher *prefetcher,
                                      struct cache_system *cache_system, uint32_t address,
                                      bool is_miss)
{
    sequential_data *data = (sequential_data *)prefetcher->data;
    uint32_t prefetched = 0;

    for (uint32_t i = 1; i<= data->prefetch_amount; i++) {
        uint32_t prefetch_address = address + i * cache_system->line_size;
        cache_system_mem_access(cache_system, prefetch_address, 'r', true);
        prefetched++;
    }
    return prefetched;
}

void sequential_cleanup(struct prefetcher *prefetcher)
{
    free(prefetcher->data);
}

struct prefetcher *sequential_prefetcher_new(uint32_t prefetch_amount)
{
    struct prefetcher *sequential_prefetcher = calloc(1, sizeof(struct prefetcher));
    sequential_prefetcher->handle_mem_access = &sequential_handle_mem_access;
    sequential_prefetcher->cleanup = &sequential_cleanup;

    sequential_data * data = malloc(sizeof(sequential_data));
    data->prefetch_amount = prefetch_amount;

    sequential_prefetcher->data = data;

    return sequential_prefetcher;
}

// Adjacent Prefetcher
// ============================================================================
uint32_t adjacent_handle_mem_access(struct prefetcher *prefetcher,
                                    struct cache_system *cache_system, uint32_t address,
                                    bool is_miss)
{
    uint32_t prefetch_address = address + cache_system->line_size;
    cache_system_mem_access(cache_system, prefetch_address, 'r', true);
    return 1;
}

void adjacent_cleanup(struct prefetcher *prefetcher)
{
    free(prefetcher->data);
}

struct prefetcher *adjacent_prefetcher_new()
{
    struct prefetcher *adjacent_prefetcher = calloc(1, sizeof(struct prefetcher));
    adjacent_prefetcher->handle_mem_access = &adjacent_handle_mem_access;
    adjacent_prefetcher->cleanup = &adjacent_cleanup;

    return adjacent_prefetcher;
}

// Custom Prefetcher
// ============================================================================

#define RPT_SIZE 256

// defining prediction table attributes
typedef struct {
    uint32_t last_address;
    int32_t stride;
    bool valid;
} prediction_table_entry;

typedef struct {
    prediction_table_entry table[RPT_SIZE];
} custom_data;

// hash function
uint32_t prediction_table_index(uint32_t address, uint32_t offset_bits) {
    // shift by 5 to ignore offset bits
    return (address >> offset_bits) % RPT_SIZE;
}

uint32_t custom_handle_mem_access(struct prefetcher *prefetcher, struct cache_system *cache_system,
                                  uint32_t address, bool is_miss)
{
    uint32_t prefetched = 0;
    custom_data *data = (custom_data *)prefetcher->data;
    // hash the current memory address to get an index into the prediction table
    uint32_t index = prediction_table_index(address, cache_system->offset_bits);
    // get the specific entry in the prediction table that corresponds to this access
    // entry tracks the last addess, stride, and if the slot has been used yet
    prediction_table_entry *entry = &data->table[index];


    // check if the table entry has been initialized already
    // if so that means there has already been an access in this slot before and we have history
    if (entry->valid) {
        // get previously recorded stride between accesses
        int32_t stride = entry->stride;
        // if the stride is not zero we have a useful prediction for the next access pattern, else dont prefetch bc we might be initializing
        if (stride != 0) {
            // get next likely memory address based of stride history and perform prefetch
            uint32_t prefetch_address = address + stride * cache_system->line_size;
            cache_system_mem_access(cache_system, prefetch_address, 'r', true);
            prefetched++;
        } 
        // update stride using the new access and the previous one. 
        // divide by line size to convert from bytes to line stride. 
        entry->stride = (address - entry->last_address) / cache_system->line_size;
        
    }
    else {
        // initialize slot and stride
        entry->valid = true;
        entry->stride = 0;
    }
    

    // update the last address to the current address to set up stride calculation 
    entry->last_address = address;

    return prefetched;
}

void custom_cleanup(struct prefetcher *prefetcher)
{
    free(prefetcher->data); 
}

struct prefetcher *custom_prefetcher_new()
{
    struct prefetcher *custom_prefetcher = calloc(1, sizeof(struct prefetcher));
    custom_prefetcher->handle_mem_access = &custom_handle_mem_access;
    custom_prefetcher->cleanup = &custom_cleanup;

    custom_data * data = calloc(1, sizeof(custom_data));
    custom_prefetcher->data = data;

    return custom_prefetcher;
}
