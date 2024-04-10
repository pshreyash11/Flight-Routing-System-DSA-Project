#include<stdio.h>
#include<string.h>
#include<stdbool.h>
#include"hash.c"
#define MAXCHAR 1000

int main(){

    FILE *fp;
    char row[MAXCHAR];
    char *token;
    char table[MAXCHAR][MAXCHAR];
    fp = fopen("myflights-test.csv","r");
    if(!fp){
        printf("file is wrong");
        return 0;
    }
    int count = 0;
    fgets(row, MAXCHAR, fp);
    while ( count< 14)
    {   
        fgets(row, MAXCHAR, fp);
        token = strtok(row, ",");
        int i = 0;
        while(token != NULL)
        {   
            if(i == 1){
                printf("rtoken: %s\n", token);
                strcpy(table[count], token);
            }
            token = strtok(NULL, ",");
            i++;
        }
        count++;
    }
    hashmap* m = hashmap_create();
    uintptr_t res;
    for(int i =0; i<count; i++){
        if(hashmap_get(m, table[i], strlen(table[i]), &res)){
            continue;
        }
        hashmap_set(m, table[i], strlen(table[i]), i);
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

    return 0;
}
