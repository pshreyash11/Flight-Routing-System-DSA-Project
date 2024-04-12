#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "hash.c"
#include "flights.h"
#include <limits.h>

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
void printPath(int path[], int len, int cost){
    for(int i=0; i<len; i++){
        printf("%d ", path[i]);
        if(i != len - 1) printf("-> ");
    }
    printf(" ====> Cost = %d", cost);
    printf("\n");
}
void findPathsHelper(graph* g, int src, int dest, bool visited[], int path[], int index, int cost, bool* found){
    visited[src] = true;
    path[index] = src;
    index++;
    if(src == dest) {
        printPath(path, index, cost);
        *found = true;
    } else{
        edge* cur = g->nodes[src]->edges;
        while(cur){
            int c = cost;
            if(!visited[cur->dest]){
                c += cur->w;
                findPathsHelper(g, cur->dest, dest, visited, path, index, c, found);
            }
            cur = cur->next;
        }
    }
    visited[src] = false;
}
void findAllPaths(graph* g, int src, int dest){
    bool visited[MAX_NODES] = {false};
    int path[MAX_NODES];
    int index = 0;
    int cost = 0;
    bool found = false;
    findPathsHelper(g, src, dest, visited, path, index, cost, &found);
    if(!found){
        printf("No paths available\n");
    }
}

int main(){
    graph* g = createGraph(5);
    addEdge(g, 0, 1, 4);
    addEdge(g, 0, 2, 2);
    addEdge(g, 1, 2, 5);
    addEdge(g, 1, 3, 10);
    addEdge(g, 2, 3, 3);
    addEdge(g, 2, 4, 6);
    addEdge(g, 3, 4, 7);
    findAllPaths(g, 0, 4);
    return 0;
}