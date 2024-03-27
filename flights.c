#include<stdio.h>
#include "flights.h"
#include<limits.h>

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
    int i = 0;
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
int main(){
    graph* g = createGraph(5);
    addEdge(g, 0, 1, 4);
    addEdge(g, 0, 2, 2);
    addEdge(g, 1, 2, 5);
    addEdge(g, 1, 3, 10);
    addEdge(g, 2, 3, 3);
    addEdge(g, 2, 4, 6);
    addEdge(g, 3, 4, 7);
    dijkstra(g, 0);
    return 0;
}