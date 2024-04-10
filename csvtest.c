#include<stdio.h>
#include<string.h>
#include<stdbool.h>
#include"hash.c"
#include "flights.h"
#include<limits.h>
#define MAXCHAR 1000
typedef struct info{
    char src[MAXCHAR];
    char dest[MAXCHAR];
    char weight[MAXCHAR];
}info;
edge* createEdge(int dest, int w){
    edge* temp = (edge*) malloc(sizeof(edge));
    temp->dest = dest;
    temp->w = w;
    temp->next = NULL;
    return temp;
}

graph* createGraph(int numberOfNodes){
    graph* g = (graph*) malloc(sizeof(graph));
    g->numberOfNodes = numberOfNodes;
    for(int i=0; i<numberOfNodes; i++){
        g->nodes[i] = (node*)malloc(sizeof(node));
        g->nodes[i]->id = i;
        g->nodes[i]->edges = NULL;
    }
    return g;
}
void addEdge(graph* g, int src, int dest, int w){
    edge* temp = createEdge(dest, w);

    // while(g->nodes[i]->id != src){
    //     i++;
    // }
    temp->next = g->nodes[src]->edges;
    g->nodes[src]->edges = temp;
    // edge* ptr = g->nodes[i]->edges;
    // if(!ptr) g->nodes[i]->edges = temp;
    // else{
    //     while(ptr->next){
    //         ptr = ptr->next;
    //     }
    // }
    // ptr->next = temp;
}
int minDistance(int dist[], bool set[], int V){
    int min = INT_MAX, min_index;
    for(int v=0; v<V; v++){
        if(set[v] == false && dist[v] <= min){
            min = dist[v], min_index = v;
        }
    }
    return min_index;
}
void dijkstra(graph* g, int src){
    int V = g->numberOfNodes;
    int dist[MAX_NODES];
    bool set[MAX_NODES];
    for(int i=0; i<V; i++){
        dist[i] = INT_MAX; set[i] = false;
    }
    dist[src] = 0;
    for(int i=0; i<V-1; i++){
        int u = minDistance(dist, set, V);
        set[u] = true;
        for(edge* e = g->nodes[u]->edges; e != NULL; e = e->next){
            if(!set[e->dest] && dist[u]!=INT_MAX && dist[u] + e->w < dist[e->dest]){
                dist[e->dest] = dist[u] + e->w;
            }
        }
    }
    for(int i=0; i<V; i++){
        printf("%d => %d \n", i, dist[i]);
    }
}

void floydWarshall(graph* g) {
    int V = g->numberOfNodes;
    int dist[MAX_NODES][MAX_NODES];

    // Initialize distance matrix
    for (int i = 0; i < V; i++) {
        for (int j = 0; j < V; j++) {
            dist[i][j] = (i == j) ? 0 : INT_MAX;
        }
    }

    // Update distance matrix
    for (int u = 0; u < V; u++) {
        for (edge* e = g->nodes[u]->edges; e != NULL; e = e->next) {
            int v = e->dest;
            int w = e->w;
            dist[u][v] = w;
        }
    }

    // Floyd-Warshall algorithm
    for (int k = 0; k < V; k++) {
        for (int i = 0; i < V; i++) {
            for (int j = 0; j < V; j++) {
                if (dist[i][k] != INT_MAX && dist[k][j] != INT_MAX &&
                    dist[i][k] + dist[k][j] < dist[i][j]) {
                    dist[i][j] = dist[i][k] + dist[k][j];
                }
            }
        }
    }

    // Print the shortest distances
    for (int i = 0; i < V; i++) {
        for (int j = 0; j < V; j++) {
            printf("%d ", (dist[i][j] == INT_MAX) ? -1 : dist[i][j]);
        }
        printf("\n");
    }
}
int main(){

    FILE *fp;
    char row[MAXCHAR];
    char *token;
    char table[MAXCHAR][MAXCHAR];
    info arr[50];
    fp = fopen("myflights-test.csv","r");
    if(!fp){
        printf("file is wrong");
        return 0;
    }
    int count = 0;
    fgets(row, MAXCHAR, fp);
    while (feof(fp) != true)
    {   
        fgets(row, MAXCHAR, fp);
        token = strtok(row, ",");
        int i = 0;
        while(token != NULL)
        {              
            if(i == 1){
                //printf("rtoken: %s\n", token);
                strcpy(table[count], token);
                strcpy(arr[count].src, token);
            }
            else if(i == 3){
                strcpy(arr[count].dest, token);
            }
            else if(i == 5){
                strcpy(arr[count].weight, token);
            }
            token = strtok(NULL, ",");
            i++;
        }
        count++;
    }
    hashmap* m = hashmap_create();
    uintptr_t res, res2;
    int val = 0;
    for(int i =0; i<count; i++){
        if(hashmap_get(m, table[i], strlen(table[i]), &res)){
            continue;
        }
        hashmap_set(m, table[i], strlen(table[i]), val);
        val++;
    }
    graph* g = createGraph(10);
    // for(int i=0; i<m->cap; i++){
    //     struct bucket* entry= &m->buckets[i];
    //     if(!entry->key){
    //         printf("Not found\n");
    //     }
    //     else{
    //         printf("Key: %s, val: %lu\n", (char*)entry->key, entry->value);
    //     }
    // }
    //printf("\n");
    for(int j = 0; j<count-1; j++){
       // printf("%s %s %s\n", arr[j].src, arr[j].dest, arr[j].weight);
        hashmap_get(m, arr[j].src, strlen(arr[j].src), &res2);
        int sr = (int)res2;
        hashmap_get(m, arr[j].dest, strlen(arr[j].dest), &res2);
        int des = (int)res2;
        int w = atoi(arr[j].weight);
        addEdge(g, sr, des, w);
        printf("%d %d %d \n", sr, des, w);
    }
    char input[500];
    printf("Enter your choice: ");
    scanf("%s", input);
    hashmap_get(m, input, strlen(input), &res);
    int start = (int)res;
    //dijkstra(g, start);
    floydWarshall(g);
    return 0;
}
