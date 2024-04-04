#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include"hash.h"
#define HASHMAP_HASH_INIT 2166136261u
struct bucket{
    struct bucket* next;
    const void* key;
    size_t ksize;
    uint32_t hash;
    uintptr_t value;
};
struct hashmap{
    struct bucket* buckets;
    int cap;
    int count;
    struct bucket* first;
    struct bucket* last;
};
hashmap* hashmap_create(void){
    hashmap* m = malloc(sizeof(hashmap));
    if(!m){
        return NULL;
    }
    m->cap = HASHMAP_DEFAULT_CAPACITY;
    m->count = 0;
    m->buckets = calloc(HASHMAP_DEFAULT_CAPACITY, sizeof(struct bucket));
    m->first = NULL;
    m->last = (struct bucket*)&m->first;
}
static inline uint32_t hash_data(const unsigned char* data, size_t size){
    uint64_t hash = HASHMAP_HASH_INIT;
    uint64_t last = size & 0xff;
    switch(size % 8){
        case 7:
            last |= (uint64_t)data[6] << 56;
        case 6:
            last |= (uint64_t)data[5] << 48;
        case 5:
            last |= (uint64_t)data[4] << 40;
        case 4:
            last |= (uint64_t)data[3] << 32;
        case 3:
            last |= (uint64_t)data[2] << 24;
        case 2:
            last |= (uint64_t)data[1] << 16;
        case 1:
            last |= (uint64_t)data[0] << 8;
            hash ^= last;
            hash *= 0xd6e8feb86659fd93;
    }
    return (uint64_t)(hash ^ hash >> 32);
}
static struct bucket* find_entry(hashmap* m, const void* key, size_t ksize, uint32_t hash){
    uint32_t index = hash % m->cap;
    for(;;){
        struct bucket* entry = &m->buckets[index];
        if(entry->key == NULL || (entry->ksize == ksize && entry->hash == hash && memcmp(entry->key, key, ksize)==0)){
            return entry;
        } 
        printf("Collision!\n");
        index = (index + 1) % m->cap;
    } 
}
void hashmap_set(hashmap* m, const void* key, size_t ksize, uintptr_t val){
    uint32_t hash = hash_data(key, ksize);
    struct bucket* entry = find_entry(m, key, ksize, hash);
    if(!entry->key){
        m->last->next = entry;
        m->last = entry;
        entry->next = NULL;
        ++m->count;
        entry->key = key;
        entry->ksize = ksize;
        entry->hash = hash;
    }
    entry->value = val;
}
bool hashmap_get_set(hashmap* m, const void* key, size_t ksize, uintptr_t* out_in){
    uint32_t hash = hash_data(key, ksize);
    struct bucket* entry = find_entry(m, key, ksize, hash);
    if(!entry->key){
        m->last->next = entry;
        m->last = entry;
        ++m->count;
        entry->next = NULL;
        entry->value = *out_in;
        entry->key = key;
        entry->ksize = ksize;
        entry->hash = hash;
        return false;
    }
    *out_in = entry->value;
    return true;
}
bool hashmap_get(hashmap* m, const void* key, size_t ksize, uintptr_t* out_val){
    uint32_t hash = hash_data(key, ksize);
    struct bucket* entry = find_entry(m, key, ksize, hash);
    *out_val = entry->value;
    return entry->key != NULL;
}
int hashmap_size(hashmap* m){
    return m->count;
}

int main(){
    hashmap* m = hashmap_create();
    uintptr_t res;
    FILE *file = fopen("sample.txt", "r");
    if(file == NULL){
        perror("Not found file");
        return 1;
    }
    char ids[20][6];
    int count = 0;
    char line[100];
    while(fgets(line, sizeof(line), file)){
        char source[6], destination[6];
        sscanf(line, "%5s, %5s", source, destination);
        int found = 0;
        for(int i=0; i<count; i++){
            if(strcmp(ids[i], source) == 0){
                found = 1;
                break;
            }
        }
        if(!found){
            strcpy(ids[count], source);
            count++;
        }
    }
    for(int i=0; i<15; i++){
        hashmap_set(m, ids[i], strlen(ids[i]), i);
    }

    for(int i=0; i<m->cap; i++){
        struct bucket* entry= &m->buckets[i];
        if(!entry->key){
            printf("Not found\n");
        }
        else{
            printf("Key: %s, val: %lu\n", (char*)entry->key, entry->value);
        }
    }
    printf("Lets check something ...\n");
    hashmap_get(m, "FVITQ", strlen("FVITQ"), &res);
    printf("value is : %d", res);
    return 0;
}