#include<stdlib.h>
#include<stdbool.h>
#define MAX_NODES 20
#define SIZE 10
typedef struct edge{
    int dest;
    int w;
    struct edge* next;
}edge;
typedef struct node{
    int id;
    edge* edges;
}node; 
typedef struct graph{
    int numberOfNodes;
    node* nodes[MAX_NODES];
}graph;

edge* createEdge(int dest, int w);
graph* createGraph(int numberofNodes);
void addEdge(graph* g, int src, int dest, int w);
int minDistance(int dist[], bool sptSet[], int V);
void dijkstra(graph* g, int src);