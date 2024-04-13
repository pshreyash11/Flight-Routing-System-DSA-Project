#include <stdio.h>
#include <string.h>
#include "routes.c"

// Function prototypes
flightinfo airinfo[MAX_NODES];
void printMenu();
void printAirInfo(char *src, char *dest);

int main()
{
    FILE *fp;
    char row[MAXCHAR];
    char *token;
    char table[MAXCHAR][MAXCHAR];
    info arr[50];
    fp = fopen("myflights-test.csv", "r");
    if (!fp)
    {
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
        while (token != NULL)
        {
            if (i == 1)
            {
                // printf("rtoken: %s\n", token);
                strcpy(table[count], token);
                strcpy(arr[count].src, token);
                strcpy(airinfo[count].src, token);
            }
            else if (i == 3)
            {
                strcpy(arr[count].dest, token);
                strcpy(airinfo[count].dest, token);
            }
            else if (i == 4)
            {
                strcpy(airinfo[count].distance, token);
            }
            else if (i == 5)
            {
                strcpy(arr[count].weight, token);
                strcpy(airinfo[count].cost, token);
            }
            else if (i == 6)
            {
                strcpy(airinfo[count].airline, token);
            }
            else if (i == 7)
            {
                strcpy(airinfo[count].food, token);
            }
            token = strtok(NULL, ",");
            i++;
        }
        count++;
    }
    hashmap *m = hashmap_create();
    uintptr_t res, res2;
    int val = 0;
    for (int i = 0; i < count; i++)
    {
        if (hashmap_get(m, table[i], strlen(table[i]), &res))
        {
            continue;
        }
        strcpy(hashtable[val], table[i]);
        hashmap_set(m, table[i], strlen(table[i]), val);
        val++;
    }
    graph *g = createGraph(10);
    // for(int i=0; i<m->cap; i++){
    //     struct bucket* entry= &m->buckets[i];
    //     if(!entry->key){
    //         printf("Not found\n");
    //     }
    //     else{
    //         printf("Key: %s, val: %lu\n", (char*)entry->key, entry->value);
    //     }
    // }
    // printf("\n");
    for (int j = 0; j < count - 1; j++)
    {
        // printf("%s %s %s\n", arr[j].src, arr[j].dest, arr[j].weight);
        hashmap_get(m, arr[j].src, strlen(arr[j].src), &res2);
        int sr = (int)res2;
        hashmap_get(m, arr[j].dest, strlen(arr[j].dest), &res2);
        int des = (int)res2;
        int w = atoi(arr[j].weight);
        addEdge(g, sr, des, w);
        // printf("%d %d %d \n", sr, des, w);
    }

    char input[500];
    char output[500];
    int choice;

    // ASCII art welcome banner with color and border
    printf("\033[1;33m********************************************\033[0m\n");
    printf("\033[1;33m*\033[0m                                          \033[1;33m*\033[0m\n");
    printf("\033[1;33m*                \033[1;34mWelcome to the\033[1;33m            *\033[0m\n");
    printf("\033[1;33m*      \033[1;34mOptimal Flight Routing System\033[1;33m       *\033[0m\n");
    printf("\033[1;33m*\033[0m                                          \033[1;33m*\033[0m\n");
    printf("\033[1;33m********************************************\033[0m\n");

    printf("\n=> Enter your source city: ");
    scanf("%s", input);

    printf("=> Enter your destination city: ");
    scanf("%s", output);

    hashmap_get(m, input, strlen(input), &res);
    int start = (int)res;
    hashmap_get(m, output, strlen(output), &res);
    int end = (int)res;

    do
    {
        printMenu();
        printf("=> Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            findAllPaths(g, start, end);
            break;
        case 2:
            dijkstra(g, start, end);
            printf("Minimum money to travel is : %d\n", floydWarshall(g, start, end));
            printf("Testing A* algorithm: \n");
            aStarSearch(g, start, end);
            printf("\n");
            break;
        case 3:
            printAirInfo(hashtable[start], hashtable[end]);
            break;
        case 4:
            double dijkstraTime = measureTime(dijkstra, g, start, end);
            double aStarTime = measureTime(aStarSearch, g, start, end);
            double floydWarshallTime = measureTime(floydWarshallWrapper, g, start, end);
            printf("Time taken for djikstra algorithm is %lf\n",dijkstraTime);
            printf("Time taken for floyd warshall algorithm is %lf\n",floydWarshallTime);
            printf("Time taken for A* search algorithm is %lf\n", aStarTime);
            break;
        case 5:
            printf("\nExiting the program. Goodbye!\n");
            break;
        default:
            printf("\nInvalid choice. Please try again.\n");
        }
    } while (choice != 5);

    return 0;
}

// Function to print the menu options
void printMenu()
{
    // ASCII art menu with color and border
    printf("\033[1;32m*********************************************************\033[0m\n");
    printf("\033[1;32m*                      \033[1;36mMenu\033[1;32m                             *\033[0m\n");
    printf("\033[1;32m*                                                       *\033[0m\n");
    printf("\033[1;32m*   \033[1;36m1. Check all possible routes available             \033[1;32m *\033[0m\n");
    printf("\033[1;32m*   \033[1;36m2. Find the minimum cost to travel and the path    \033[1;32m *\033[0m\n");
    printf("\033[1;32m*   \033[1;36m3. Check non-stop flight and display its details   \033[1;32m *\033[0m\n");
    printf("\033[1;32m*   \033[1;36m4. Compare time complexity of different algorithms   \033[1;32m *\033[0m\n");
    printf("\033[1;32m*   \033[1;36m5. Exit                                            \033[1;32m *\033[0m\n");
    printf("\033[1;32m*                                                       *\033[0m\n");
    printf("\033[1;32m*********************************************************\033[0m\n");
}

void printAirInfo(char *src, char *dest)
{
    int sz = sizeof(airinfo) / sizeof(airinfo[0]);
    bool flag = false;
    for (int i = 0; i < sz; i++)
    {
        if (strcmp(airinfo[i].src, src) == 0 && strcmp(airinfo[i].dest, dest) == 0)
        {
            printf("-----FLIGHT FOUND-----\n");
            printf("Total Distance: %s\n", airinfo[i].distance);
            printf("Airline Company: %s\n", airinfo[i].airline);
            printf("Is inflight dining available: %s", airinfo[i].food);
            printf("Total cost: %s\n", airinfo[i].cost);
            printf("------x----x----x------\n");
            flag = true;
        }
    }
    if (!flag)
        printf("No direct flight availble!\n");
}