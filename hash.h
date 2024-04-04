#include<stdint.h>
#include<stddef.h>
#include<stdbool.h>

#define HASHMAP_DEFAULT_CAPACITY 20
#define HASHMAP_MAX_LOAD 0.75f
#define HASHMAP_RESIZE_FACTOR 2
typedef struct hashmap hashmap;
typedef void (*hashmap_callback)(void* key, size_t ksize, uintptr_t value, void* usr);

hashmap* hashmap_create(void);
void hashmap_set(hashmap* map, const void* key, size_t ksize, uintptr_t value);
bool hashmap_get_set(hashmap* map, const void* key, size_t ksize, uintptr_t* out_in);
bool hashmap_get(hashmap* map, const void* key, size_t ksize, uintptr_t* out_val);
int hashmap_size(hashmap* map);