#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "flights.h"
#define MAX_NODES 20


edge* createEdge(int dest, int w) {
    edge* temp = (edge*)malloc(sizeof(edge));
    temp->dest = dest;
    temp->w = w;
    temp->next = NULL;
    return temp;
}

graph* createGraph(int numberOfNodes) {
    graph* g = (graph*)malloc(sizeof(graph));
    g->numberOfNodes = numberOfNodes;
    for (int i = 0; i < numberOfNodes; i++) {
        g->nodes[i] = (node*)malloc(sizeof(node));
        g->nodes[i]->id = i;
        g->nodes[i]->edges = NULL;
    }
    return g;
}

void addEdge(graph* g, int src, int dest, int w) {
    edge* temp = createEdge(dest, w);
    temp->next = g->nodes[src]->edges;
    g->nodes[src]->edges = temp;
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

int main() {
    graph* g = createGraph(6);
    addEdge(g, 0, 1, 4);
    addEdge(g, 0, 2, 4);
    addEdge(g, 1, 2, 2);
    addEdge(g, 2, 3, 3);
    addEdge(g, 2, 4, 1);
    addEdge(g, 3, 5, 2);
    addEdge(g, 2, 5, 6);
    addEdge(g, 4, 5, 3);

    floydWarshall(g);

    return 0;
}
