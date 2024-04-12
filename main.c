#include <stdio.h>
#include <string.h>
#include"routes.c"

// Function prototypes
void printMenu();


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
            }
            else if (i == 3)
            {
                strcpy(arr[count].dest, token);
            }
            else if (i == 5)
            {
                strcpy(arr[count].weight, token);
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
            break;
        case 3:
            printf("\nExiting the program. Goodbye!\n");
            break;
        default:
            printf("\nInvalid choice. Please try again.\n");
        }
    } while (choice != 3);

    return 0;
}

// Function to print the menu options
void printMenu()
{
    // ASCII art menu with color and border
    printf("\033[1;32m******************************************************\033[0m\n");
    printf("\033[1;32m*                      \033[1;36mMenu\033[1;32m                          *\033[0m\n");
    printf("\033[1;32m*                                                    *\033[0m\n");
    printf("\033[1;32m*   \033[1;36m1. Check all possible routes available          \033[1;32m *\033[0m\n");
    printf("\033[1;32m*   \033[1;36m2. Find the minimum cost to travel and the path \033[1;32m *\033[0m\n");
    printf("\033[1;32m*   \033[1;36m3. Exit                                         \033[1;32m *\033[0m\n");
    printf("\033[1;32m*                                                    *\033[0m\n");
    printf("\033[1;32m******************************************************\033[0m\n");
}
