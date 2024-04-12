#include <stdio.h>
#include <string.h>

// Function prototypes
void printMenu();
void checkDirectFlight(char source[], char destination[]);
void findMinimumCost(char source[], char destination[]);

int main() {
    char source[50], destination[50];
    int choice;

    // ASCII art welcome banner with color and border
    printf("\033[1;33m********************************************\033[0m\n");
    printf("\033[1;33m*\033[0m                                          \033[1;33m*\033[0m\n");
    printf("\033[1;33m*                \033[1;34mWelcome to the\033[1;33m            *\033[0m\n");
    printf("\033[1;33m*      \033[1;34mOptimal Flight Routing System\033[1;33m       *\033[0m\n");
    printf("\033[1;33m*\033[0m                                          \033[1;33m*\033[0m\n");
    printf("\033[1;33m********************************************\033[0m\n");

    printf("\n=> Enter your source city: ");
    fgets(source, sizeof(source), stdin);
    source[strcspn(source, "\n")] = 0;

    printf("=> Enter your destination city: ");
    fgets(destination, sizeof(destination), stdin);
    destination[strcspn(destination, "\n")] = 0;

    do {
        printMenu();
        printf("=> Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                checkDirectFlight(source, destination);
                break;
            case 2:
                findMinimumCost(source, destination);
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
void printMenu() {
    // ASCII art menu with color and border
    printf("\033[1;32m******************************************************\033[0m\n");
    printf("\033[1;32m*                      \033[1;36mMenu\033[1;32m                          *\033[0m\n");
    printf("\033[1;32m*                                                    *\033[0m\n");
    printf("\033[1;32m*   \033[1;36m1. Check if there is a direct flight available  \033[1;32m *\033[0m\n");
    printf("\033[1;32m*   \033[1;36m2. Find the minimum cost to travel and the path \033[1;32m *\033[0m\n");
    printf("\033[1;32m*   \033[1;36m3. Exit                                         \033[1;32m *\033[0m\n");
    printf("\033[1;32m*                                                    *\033[0m\n");
    printf("\033[1;32m******************************************************\033[0m\n");
}

// Placeholder function to simulate checking for a direct flight
void checkDirectFlight(char source[], char destination[]) {
    printf("\nChecking if there is a direct flight between %s and %s...\n", source, destination);
    // Code to check for direct flight
    printf("Direct flight is available from %s to %s.\n", source, destination);
}

// Placeholder function to simulate finding the minimum cost and path
void findMinimumCost(char source[], char destination[]) {
    printf("\nFinding the minimum cost to travel from %s to %s...\n", source, destination);
    // Code to find minimum cost
    printf("The minimum cost to travel from %s to %s is $500.\n", source, destination);
    printf("The path is: %s -> City1 -> City2 -> %s\n", source, destination);
}
