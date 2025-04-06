// //
// // This file contains all of the implementations of the replacement_policy
// // constructors from the replacement_policies.h file.
// //
// // It also contains stubs of all of the functions that are added to each
// // replacement_policy struct at construction time.
// //
// // ============================================================================
// // NOTE: It is recommended that you read the comments in the
// // replacement_policies.h file for further context on what each function is
// // for.
// // ============================================================================
// //

// #include "replacement_policies.h"

// // LRU Replacement Policy
// // ============================================================================
// // TODO feel free to create additional structs/enums as necessary

// void lru_cache_access(struct replacement_policy *replacement_policy,
//                       struct cache_system *cache_system, uint32_t set_idx, uint32_t tag)
// {
//     // TODO update the LRU replacement policy state given a new memory access
// }

// uint32_t lru_eviction_index(struct replacement_policy *replacement_policy,
//                             struct cache_system *cache_system, uint32_t set_idx)
// {
//     // TODO return the index within the set that should be evicted.

//     return 0;
// }

// void lru_replacement_policy_cleanup(struct replacement_policy *replacement_policy)
// {
//     // TODO cleanup any additional memory that you allocated in the
//     // lru_replacement_policy_new function.
// }

// struct replacement_policy *lru_replacement_policy_new(uint32_t sets, uint32_t associativity)
// {
//     struct replacement_policy *lru_rp = calloc(1, sizeof(struct replacement_policy));
//     lru_rp->cache_access = &lru_cache_access;
//     lru_rp->eviction_index = &lru_eviction_index;
//     lru_rp->cleanup = &lru_replacement_policy_cleanup;

//     // TODO allocate any additional memory to store metadata here and assign to
//     // lru_rp->data.

//     return lru_rp;
// }

// // RAND Replacement Policy
// // ============================================================================
// void rand_cache_access(struct replacement_policy *replacement_policy,
//                        struct cache_system *cache_system, uint32_t set_idx, uint32_t tag)
// {
//     // TODO update the RAND replacement policy state given a new memory access
// }

// uint32_t rand_eviction_index(struct replacement_policy *replacement_policy,
//                              struct cache_system *cache_system, uint32_t set_idx)
// {
//     // TODO return the index within the set that should be evicted.

//     return 0;
// }

// void rand_replacement_policy_cleanup(struct replacement_policy *replacement_policy)
// {
//     // TODO cleanup any additional memory that you allocated in the
//     // rand_replacement_policy_new function.
// }

// struct replacement_policy *rand_replacement_policy_new(uint32_t sets, uint32_t associativity)
// {
//     // Seed randomness
//     srand(time(NULL));

//     struct replacement_policy *rand_rp = malloc(sizeof(struct replacement_policy));
//     rand_rp->cache_access = &rand_cache_access;
//     rand_rp->eviction_index = &rand_eviction_index;
//     rand_rp->cleanup = &rand_replacement_policy_cleanup;

//     // TODO allocate any additional memory to store metadata here and assign to
//     // rand_rp->data.

//     return rand_rp;
// }

// // LRU_PREFER_CLEAN Replacement Policy
// // ============================================================================
// void lru_prefer_clean_cache_access(struct replacement_policy *replacement_policy,
//                                    struct cache_system *cache_system, uint32_t set_idx,
//                                    uint32_t tag)
// {
//     // TODO update the LRU_PREFER_CLEAN replacement policy state given a new
//     // memory access
//     // NOTE: you may be able to share code with the LRU policy
// }

// uint32_t lru_prefer_clean_eviction_index(struct replacement_policy *replacement_policy,
//                                          struct cache_system *cache_system, uint32_t set_idx)
// {
//     // TODO return the index within the set that should be evicted.

//     return 0;
// }

// void lru_prefer_clean_replacement_policy_cleanup(struct replacement_policy *replacement_policy)
// {
//     // TODO cleanup any additional memory that you allocated in the
//     // lru_prefer_clean_replacement_policy_new function.
// }

// struct replacement_policy *lru_prefer_clean_replacement_policy_new(uint32_t sets,
//                                                                    uint32_t associativity)
// {
//     struct replacement_policy *lru_prefer_clean_rp = malloc(sizeof(struct replacement_policy));
//     lru_prefer_clean_rp->cache_access = &lru_prefer_clean_cache_access;
//     lru_prefer_clean_rp->eviction_index = &lru_prefer_clean_eviction_index;
//     lru_prefer_clean_rp->cleanup = &lru_prefer_clean_replacement_policy_cleanup;

//     // TODO allocate any additional memory to store metadata here and assign to
//     // lru_prefer_clean_rp->data.

//     return lru_prefer_clean_rp;
// }




//
// This file contains all of the implementations of the replacement_policy
// constructors from the replacement_policies.h file.
//
// It also contains stubs of all of the functions that are added to each
// replacement_policy struct at construction time.
//
// ============================================================================
// NOTE: It is recommended that you read the comments in the
// replacement_policies.h file for further context on what each function is
// for.
// ============================================================================
//

#include "replacement_policies.h"

// LRU Replacement Policy
// ============================================================================
// TODO feel free to create additional structs/enums as necessary

struct lru_metadata {
    uint32_t *order;
};

void lru_cache_access(struct replacement_policy *replacement_policy,
                      struct cache_system *cache_system, uint32_t set_idx, uint32_t tag)
{
    // TODO update the LRU replacement policy state given a new memory access

    struct lru_metadata *lru = (struct lru_metadata *)replacement_policy->data;
    int set_start_idx = set_idx * cache_system->associativity;

    for (int i = set_start_idx; i < set_start_idx + cache_system->associativity; i++) {
        struct cache_line *cl = &cache_system->cache_lines[i];
        if (cl->tag == tag) {
            lru->order[i] = cache_system->stats.accesses;
        }
    }

}

uint32_t lru_eviction_index(struct replacement_policy *replacement_policy,
                            struct cache_system *cache_system, uint32_t set_idx)
{
    // TODO return the index within the set that should be evicted.

    struct lru_metadata *lru = (struct lru_metadata *)replacement_policy->data;
    int set_start_idx = set_idx * cache_system->associativity;

    int min_idx = set_start_idx;
    uint32_t min_val = lru->order[set_start_idx];


    for (int i = set_start_idx; i < set_start_idx + cache_system->associativity; i++) {
        if (lru->order[i] < min_val) {
            min_val = lru->order[i];
            min_idx = i;
        }
    }

    return min_idx % cache_system->associativity;
}

void lru_replacement_policy_cleanup(struct replacement_policy *replacement_policy)
{
    // TODO cleanup any additional memory that you allocated in the
    // lru_replacement_policy_new function.
    struct lru_metadata *lru = (struct lru_metadata *)replacement_policy->data;
    if(lru) {
        free(lru->order);
    }
    free(lru);

}

struct replacement_policy *lru_replacement_policy_new(uint32_t sets, uint32_t associativity)
{
    struct replacement_policy *lru_rp = calloc(1, sizeof(struct replacement_policy));
    lru_rp->cache_access = &lru_cache_access;
    lru_rp->eviction_index = &lru_eviction_index;
    lru_rp->cleanup = &lru_replacement_policy_cleanup;

    // TODO allocate any additional memory to store metadata here and assign to
    // lru_rp->data.
   
    struct lru_metadata *lru = malloc(sizeof(struct lru_metadata));
    lru->order = calloc(sets * associativity, sizeof(uint32_t));

    lru_rp->data = lru;

    return lru_rp;
}

// RAND Replacement Policy
// ============================================================================
void rand_cache_access(struct replacement_policy *replacement_policy,
                       struct cache_system *cache_system, uint32_t set_idx, uint32_t tag)
{
    // TODO update the RAND replacement policy state given a new memory access
    // DONE
}

uint32_t rand_eviction_index(struct replacement_policy *replacement_policy,
                             struct cache_system *cache_system, uint32_t set_idx)
{
    // TODO return the index within the set that should be evicted.
    // DONE

    int evicted_index = rand() % cache_system->associativity;

    // return eviction index
    return evicted_index;
}

void rand_replacement_policy_cleanup(struct replacement_policy *replacement_policy)
{
    // TODO cleanup any additional memory that you allocated in the
    // rand_replacement_policy_new function.
    // DONE
}

struct replacement_policy *rand_replacement_policy_new(uint32_t sets, uint32_t associativity)
{
    // Seed randomness
    srand(time(NULL));

    struct replacement_policy *rand_rp = malloc(sizeof(struct replacement_policy));
    rand_rp->cache_access = &rand_cache_access;
    rand_rp->eviction_index = &rand_eviction_index;
    rand_rp->cleanup = &rand_replacement_policy_cleanup;

    // TODO allocate any additional memory to store metadata here and assign to
    // rand_rp->data.
    // DONE

    rand_rp->data = rand_rp;
    return rand_rp;
}

// LRU_PREFER_CLEAN Replacement Policy
// ============================================================================
void lru_prefer_clean_cache_access(struct replacement_policy *replacement_policy,
                                   struct cache_system *cache_system, uint32_t set_idx,
                                   uint32_t tag)
{
    // TODO update the LRU_PREFER_CLEAN replacement policy state given a new
    // memory access
    // NOTE: you may be able to share code with the LRU policy

    // should be the same as the LRU replacement policy
    lru_cache_access(replacement_policy, cache_system, set_idx, tag);
}

uint32_t lru_prefer_clean_eviction_index(struct replacement_policy *replacement_policy,
                                         struct cache_system *cache_system, uint32_t set_idx)
{
    // TODO return the index within the set that should be evicted.
    struct lru_metadata *lru = (struct lru_metadata *)replacement_policy->data;
    int set_start_idx = set_idx * cache_system->associativity;

    int min_clean_idx = set_start_idx;
    uint32_t min_clean_val = lru->order[set_start_idx];

    for (int i = set_start_idx; i < set_start_idx + cache_system->associativity; i++) {
        struct cache_line *cl = &cache_system->cache_lines[i];
        if (cl->status == EXCLUSIVE) {
            min_clean_idx = i;
            min_clean_val = lru->order[i];
            break;
        }
    }

    int min_dirty_idx = set_start_idx;
    uint32_t min_dirty_val = lru->order[set_start_idx];

    for (int i = set_start_idx; i < set_start_idx + cache_system->associativity; i++) {
        struct cache_line *cl = &cache_system->cache_lines[i];
        if (cl->status == MODIFIED) {
            min_dirty_idx = i;
            min_dirty_val = lru->order[i];
            break;
        }
    }

    bool isClean = false;

    for (int i = set_start_idx; i < set_start_idx + cache_system->associativity; i++) {
        struct cache_line *cl = &cache_system->cache_lines[i];

        if (cl->status == EXCLUSIVE) {
            isClean = true;
            if (lru->order[i] < min_clean_val) {
                min_clean_val = lru->order[i];
                min_clean_idx = i;
            }
        }
        else if (cl->status == MODIFIED) {
            if (lru->order[i] < min_dirty_val) {
                min_dirty_val = lru->order[i];
                min_dirty_idx = i;
            }
        }
    }

    if (isClean) {
        printf("[DEBUG] clean");
        return min_clean_idx % cache_system->associativity;
    }
    else {
        printf("[DEBUG] dirty");
        return min_dirty_idx % cache_system->associativity;
    }
}

void lru_prefer_clean_replacement_policy_cleanup(struct replacement_policy *replacement_policy)
{
    // TODO cleanup any additional memory that you allocated in the
    // lru_prefer_clean_replacement_policy_new function.

    lru_replacement_policy_cleanup(replacement_policy);
}

struct replacement_policy *lru_prefer_clean_replacement_policy_new(uint32_t sets,
                                                                   uint32_t associativity)
{
    struct replacement_policy *lru_prefer_clean_rp = malloc(sizeof(struct replacement_policy));
    lru_prefer_clean_rp->cache_access = &lru_prefer_clean_cache_access;
    lru_prefer_clean_rp->eviction_index = &lru_prefer_clean_eviction_index;
    lru_prefer_clean_rp->cleanup = &lru_prefer_clean_replacement_policy_cleanup;

    // TODO allocate any additional memory to store metadata here and assign to
    // lru_prefer_clean_rp->data.

    lru_prefer_clean_rp->data = lru_replacement_policy_new(sets, associativity)->data;

    return lru_prefer_clean_rp;
}

