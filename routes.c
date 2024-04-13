#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "hash.c"
#include "flights.h"
#include <limits.h>
#include <time.h>
#include <math.h>
#define MAXCHAR 1000
#define R 6371
#define MAX_PQ_SIZE 100
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
char hashtable[MAX_NODES][MAXCHAR];
typedef struct info
{
    char src[MAXCHAR];
    char dest[MAXCHAR];
    char weight[MAXCHAR];
} info;
typedef struct pqNode
{
    int vertex;
    double fScore;
} pqNode;
pqNode priorityQueue[MAX_PQ_SIZE];
int pqSize = 0;
typedef struct city
{
    double latitude;
    double longitude;
} city;
city cities[MAX_NODES] = {
    {28.7041, 77.1025},    // Delhi
    {19.0760, 72.8777},    // Mumbai
    {12.9716, 77.5946},    // Bangalore
    {13.0827, 80.2707},    // Chennai
    {22.5726, 88.3639},    // Kolkata
    {17.3850, 78.4867},    // Hyderabad
    {23.0225, 72.5714},    // Ahmedabad
    {18.5204, 73.8567},    // Pune
    {26.9124, 75.7873},    // Jaipur
    {26.8467, 80.9462},    // Lucknow
};
// city cities[MAX_NODES] = {
//     {2.2, 7.2},    // Delhi
//     {2.2, 7.2},
//     {2.2, 7.2},
//     {2.2, 7.2},
//     {2.2, 7.2},
//     {2.2, 7.2},
//     {2.2, 7.2},
//     {2.2, 7.2},
//     {2.2, 7.2},
//     {2.2, 7.2},    
// };
bool isEmpty()
{
    return pqSize == 0;
}
void swap(pqNode *a, pqNode *b)
{
    pqNode temp = *a;
    *a = *b;
    *b = temp;
}
void heapify(int i)
{
    int smallest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < pqSize && priorityQueue[left].fScore < priorityQueue[smallest].fScore)
    {
        smallest = left;
    }

    if (right < pqSize && priorityQueue[right].fScore < priorityQueue[smallest].fScore)
    {
        smallest = right;
    }

    if (smallest != i)
    {
        swap(&priorityQueue[smallest], &priorityQueue[i]);
        heapify(smallest);
    }
}
pqNode extractMin()
{
    if (isEmpty())
    {
        printf("Error: Priority queue is empty.\n");
        exit(1);
    }
    pqNode min = priorityQueue[0];
    pqSize--;
    priorityQueue[0] = priorityQueue[pqSize];
    heapify(0);
    return min;
}
int parent(int i)
{
    return (i - 1) / 2;
}
void insert(int vertex, double fScore)
{
    if (pqSize == MAX_PQ_SIZE)
    {
        printf("Error: Priority queue overflow.\n");
        exit(1);
    }
    pqSize++;
    int i = pqSize - 1;
    priorityQueue[i].vertex = vertex;
    priorityQueue[i].fScore = fScore;
    while (i > 0 && priorityQueue[parent(i)].fScore > priorityQueue[i].fScore)
    {
        swap(&priorityQueue[i], &priorityQueue[parent(i)]);
        i = parent(i);
    }
}
double heuristic(city a, city b)
{
    double dLat = (a.latitude - b.latitude) * M_PI / 180.0;
    double dLon = (a.longitude - b.longitude) * M_PI / 180.0;
    double a1 = sin(dLat / 2) * sin(dLat / 2);
    double a2 = cos(M_PI * a.latitude / 180.0) * cos(M_PI * b.latitude / 180.0) * sin(dLon / 2) * sin(dLon / 2);
    double c = 2 * atan2(sqrt(a1 + a2), sqrt(1.0 - (a1 + a2)));
    return R * c;
}
void aStarSearch(graph *g, int src, int dest)
{
    int V = g->numberOfNodes;
    int parent[MAX_NODES];
    bool closedSet[MAX_NODES];
    double fScore[MAX_NODES];
    pqSize = 0;
    for (int i = 0; i < V; i++)
    {
        parent[i] = -1;
        closedSet[i] = false;
        fScore[i] = INT_MAX;
    }
    fScore[src] = heuristic(cities[src], cities[dest]);
    insert(src, fScore[src]);
    while (!isEmpty())
    {
        int u = extractMin().vertex;
        if (u == dest)
        {
            int path[MAX_NODES];
            int pathLength = 0;
            int current = u;
            while (parent[current] != -1)
            {
                path[pathLength++] = current;
                current = parent[current];
            }
            path[pathLength++] = src;
            printf("A* Search Path: ");
            for (int i = pathLength - 1; i >= 0; i--)
            {   
                if(i)printf("%s -> ", hashtable[path[i]]);
                else printf("%s ", hashtable[path[i]]);
            }
            printf("\n");
            //printf("\nTotal Cost (distance + heuristic): %lf\n", fScore[u]);

            break;
        }
        closedSet[u] = true;
        for (edge *e = g->nodes[u]->edges; e != NULL; e = e->next)
        {
            int v = e->dest;
            double tentativeGScore = fScore[u] + e->w;
            if (!closedSet[v] && tentativeGScore < fScore[v])
            {
                parent[v] = u;
                fScore[v] = tentativeGScore + heuristic(cities[v], cities[dest]);
                insert(v, fScore[v]);
            }
        }
    }
    // if (isEmpty())
    // {
    //     printf("No path found from %s to %s using A* search.\n", hashtable[src], hashtable[dest]);
    // }
}

edge *createEdge(int dest, int w)
{
    edge *temp = (edge *)malloc(sizeof(edge));
    temp->dest = dest;
    temp->w = w;
    temp->next = NULL;
    return temp;
}

graph *createGraph(int numberOfNodes)
{
    graph *g = (graph *)malloc(sizeof(graph));
    g->numberOfNodes = numberOfNodes;
    for (int i = 0; i < numberOfNodes; i++)
    {
        g->nodes[i] = (node *)malloc(sizeof(node));
        g->nodes[i]->id = i;
        g->nodes[i]->edges = NULL;
    }
    return g;
}
void addEdge(graph *g, int src, int dest, int w)
{
    edge *temp = createEdge(dest, w);

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
int minDistance(int dist[], bool set[], int V)
{
    int min = INT_MAX, min_index;
    for (int v = 0; v < V; v++)
    {
        if (set[v] == false && dist[v] <= min)
        {
            min = dist[v], min_index = v;
        }
    }
    return min_index;
}
// void dijkstra(graph *g, int src)
// {
//     int V = g->numberOfNodes;
//     int dist[MAX_NODES];
//     int parent[MAX_NODES]; // Array to store the parent nodes
//     bool set[MAX_NODES];

//     // Initialize distances, parent nodes, and set array
//     for (int i = 0; i < V; i++)
//     {
//         dist[i] = INT_MAX;
//         parent[i] = -1;
//         set[i] = false;
//     }

//     // Distance from source to itself is 0
//     dist[src] = 0;

//     // Dijkstra's algorithm
//     for (int i = 0; i < V - 1; i++)
//     {
//         int u = minDistance(dist, set, V);
//         set[u] = true;

//         // Update distances of adjacent vertices
//         for (edge *e = g->nodes[u]->edges; e != NULL; e = e->next)
//         {
//             int v = e->dest;
//             int weight = e->w;
//             if (!set[v] && dist[u] != INT_MAX && dist[u] + weight < dist[v])
//             {
//                 dist[v] = dist[u] + weight;
//                 parent[v] = u; // Update parent node
//             }
//         }
//     }

//     // Print shortest path and distance from source to each vertex
//     // Print shortest path and distance from source to each vertex
//     printf("Vertex   Distance from Source   Shortest Path\n");
//     for (int i = 0; i < V; i++)
//     {
//         printf("%d \t\t %d \t\t ", i, dist[i]);
//         if (dist[i] == INT_MAX)
//         {
//             printf("No path");
//         }
//         else
//         {
//             printf("%d ", src);
//             int current = i;
//             int path[MAX_NODES], pathLength = 0;

//             // Backtrack from destination to source to get the correct order of vertices
//             while (parent[current] != -1)
//             {
//                 path[pathLength++] = current;
//                 current = parent[current];
//             }

//             // Print the reversed path
//             for (int j = pathLength - 1; j >= 0; j--)
//             {
//                 printf(" -> %d", path[j]);
//             }
//         }
//         printf("\n");
//     }
// }

void dijkstra(graph *g, int src, int dest)
{
    int V = g->numberOfNodes;
    int dist[MAX_NODES];
    int parent[MAX_NODES]; // Array to store the parent nodes
    bool set[MAX_NODES];

    // Initialize distances, parent nodes, and set array
    for (int i = 0; i < V; i++)
    {
        dist[i] = INT_MAX;
        parent[i] = -1;
        set[i] = false;
    }

    // Distance from source to itself is 0
    dist[src] = 0;

    // Dijkstra's algorithm
    for (int i = 0; i < V - 1; i++)
    {
        int u = minDistance(dist, set, V);
        set[u] = true;

        // Update distances of adjacent vertices
        for (edge *e = g->nodes[u]->edges; e != NULL; e = e->next)
        {
            int v = e->dest;
            int weight = e->w;
            if (!set[v] && dist[u] != INT_MAX && dist[u] + weight < dist[v])
            {
                dist[v] = dist[u] + weight;
                parent[v] = u; // Update parent node
            }
        }

        // Exit the loop if destination vertex is reached
        if (u == dest)
            break;
    }

    // Print shortest path from source to destination
    if (dist[dest] == INT_MAX)
    {
        printf("No path from %d to %d\n", src, dest);
        return;
    }

    printf("Shortest path from %s to %s: ", hashtable[src], hashtable[dest]);
    int current = dest;
    int path[MAX_NODES], pathLength = 0;
    // Backtrack from destination to source to get the correct order of vertices
    while (parent[current] != -1)
    {
        path[pathLength++] = current;
        current = parent[current];
    }

    // Print the reversed path
    printf("%s", hashtable[src]);
    for (int j = pathLength - 1; j >= 0; j--)
    {
        printf(" -> %s", hashtable[path[j]]);
    }
    printf("\nCost of Shortest Path: %d\n", dist[dest]);
    printf("\n");
}

// void floydWarshall(graph *g)
// {
//     int V = g->numberOfNodes;
//     int dist[MAX_NODES][MAX_NODES];

//     // Initialize distance matrix
//     for (int i = 0; i < V; i++)
//     {
//         for (int j = 0; j < V; j++)
//         {
//             dist[i][j] = (i == j) ? 0 : INT_MAX;
//         }
//     }

//     // Update distance matrix
//     for (int u = 0; u < V; u++)
//     {
//         for (edge *e = g->nodes[u]->edges; e != NULL; e = e->next)
//         {
//             int v = e->dest;
//             int w = e->w;
//             dist[u][v] = w;
//         }
//     }

//     // Floyd-Warshall algorithm
//     for (int k = 0; k < V; k++)
//     {
//         for (int i = 0; i < V; i++)
//         {
//             for (int j = 0; j < V; j++)
//             {
//                 if (dist[i][k] != INT_MAX && dist[k][j] != INT_MAX &&
//                     dist[i][k] + dist[k][j] < dist[i][j])
//                 {
//                     dist[i][j] = dist[i][k] + dist[k][j];
//                 }
//             }
//         }
//     }

//     // Print the shortest distances
//     for (int i = 0; i < V; i++)
//     {
//         for (int j = 0; j < V; j++)
//         {
//             printf("%d ", (dist[i][j] == INT_MAX) ? -1 : dist[i][j]);
//         }
//         printf("\n");
//     }
// }
int floydWarshall(graph *g, int start, int end)
{
    int V = g->numberOfNodes;
    int dist[MAX_NODES][MAX_NODES];

    // Initialize distance matrix
    for (int i = 0; i < V; i++)
    {
        for (int j = 0; j < V; j++)
        {
            dist[i][j] = (i == j) ? 0 : INT_MAX;
        }
    }

    // Update distance matrix with direct edges
    for (int u = 0; u < V; u++)
    {
        for (edge *e = g->nodes[u]->edges; e != NULL; e = e->next)
        {
            int v = e->dest;
            int w = e->w;
            dist[u][v] = w;
        }
    }

    // Floyd-Warshall algorithm
    for (int k = 0; k < V; k++)
    {
        for (int i = 0; i < V; i++)
        {
            for (int j = 0; j < V; j++)
            {
                if (dist[i][k] != INT_MAX && dist[k][j] != INT_MAX &&
                    dist[i][k] + dist[k][j] < dist[i][j])
                {
                    dist[i][j] = dist[i][k] + dist[k][j];
                }
            }
        }
    }

    // Return the distance from start to end
    return dist[start][end];
}
void printPath(int path[], int len, int cost)
{
    for (int i = 0; i < len; i++)
    {
        printf("%s ", hashtable[path[i]]);
        if (i != len - 1)
            printf("-> ");
    }
    printf(" ====> Cost = %d", cost);
    printf("\n");
}
void findPathsHelper(graph *g, int src, int dest, bool visited[], int path[], int index, int cost, bool *found)
{
    visited[src] = true;
    path[index] = src;
    index++;
    if (src == dest)
    {
        printPath(path, index, cost);
        *found = true;
    }
    else
    {
        edge *cur = g->nodes[src]->edges;
        while (cur)
        {
            int c = cost;
            if (!visited[cur->dest])
            {
                c += cur->w;
                findPathsHelper(g, cur->dest, dest, visited, path, index, c, found);
            }
            cur = cur->next;
        }
    }
    visited[src] = false;
}
void findAllPaths(graph *g, int src, int dest)
{
    bool visited[MAX_NODES] = {false};
    int path[MAX_NODES];
    int index = 0;
    int cost = 0;
    bool found = false;
    findPathsHelper(g, src, dest, visited, path, index, cost, &found);
    if (!found)
    {
        printf("No paths available\n");
    }
}

double measureTime(void (*algo)(graph *, int, int), graph *g, int start, int end)
{
    clock_t startTime = clock();
    algo(g, start, end);
    clock_t endTime = clock();
    double timeTaken = ((double)(endTime - startTime)) / CLOCKS_PER_SEC;
    return timeTaken;
}

void floydWarshallWrapper(graph *g, int start, int end)
{
    int x = floydWarshall(g, start, end);
    return;
}