# Optimal Flight Routing System

This project implements three different algorithms - Dijkstra's algorithm, Floyd-Warshall algorithm, and A* algorithm - to find the least cost between flights for traveling.

## Overview

In this repository, you'll find implementations of three popular algorithms used for finding the least cost between flights to facilitate travel:

1. **Dijkstra's Algorithm**: A graph search algorithm that finds the shortest path between nodes in a weighted graph.
2. **Floyd-Warshall Algorithm**: An algorithm for finding shortest paths in a weighted graph with positive or negative edge weights (but with no negative cycles). It is used to find the shortest paths between all pairs of nodes in a weighted graph.
3. **A* (A-Star) Algorithm**: A heuristic search algorithm that finds a path from a given initial node to a given goal node by using a cost-plus-heuristic function.

## functionalities

1. Checks all possible paths available.
2. Minimum Cost to travel (Using all three algorithms).
3. Checks if there is direct flight available , If available display its details such as distance , cost , airline-company , availibilty of inflight dining availble or not , rating of flight , no. of ratings . 
4. Give rating of flight . this rating changes and stores it even if you close the program.
5. Compares time between all three algorithms.



## Contributors 

This is group project with sahilpatil1804 and Sujal-424 . but if you want to add some more functionalities to this project feel free to contribute.

## Usage

To get started with using this project, follow these steps:

1. Clone the repository to your local machine using git clone command
2. To run the program , make sure you are in right directory .
3. i) if you are on windows -> gcc main.c<br>
                            -> ./a.exe<br>
   ii) if you are on linux -> gcc main.c -lm
                           -> ./a.out

