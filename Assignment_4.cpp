#include <iostream>
#include <vector>
#include <limits>
using namespace std;

#define INF INT_MAX

// Total departments
const int V = 5;

// Department names for output
string departments[V] = {
    "Computer Science", "Mathematics", "Physics", "Chemistry", "Administration"
};

// Function to find the vertex with the minimum key value
int minKey(vector<int>& key, vector<bool>& mstSet) {
    int min = INF, min_index;

    for (int v = 0; v < V; v++)
        if (!mstSet[v] && key[v] < min)
            min = key[v], min_index = v;

    return min_index;
}

// Prim's algorithm to find MST
void primMST(int graph[V][V]) {
    vector<int> parent(V); // Array to store MST
    vector<int> key(V, INF); // Key values used to pick minimum weight edge
    vector<bool> mstSet(V, false); // To represent set of vertices in MST

    key[0] = 0;     // Start from vertex 0
    parent[0] = -1; // Root has no parent

    for (int count = 0; count < V - 1; count++) {
        int u = minKey(key, mstSet);  // Pick the min key vertex
        mstSet[u] = true;             // Add the picked vertex to MST Set

        // Update key values of adjacent vertices
        for (int v = 0; v < V; v++)
            if (graph[u][v] && !mstSet[v] && graph[u][v] < key[v])
                parent[v] = u, key[v] = graph[u][v];
    }

    // Print the MST
    cout << "\nMinimum Spanning Tree using Prim's Algorithm:\n";
    cout << "Edge\t\tWeight\n";
    for (int i = 1; i < V; i++)
        cout << departments[parent[i]] << " - " << departments[i]
             << "\t" << graph[i][parent[i]] << " meters\n";
}

// Display adjacency matrix
void displayMatrix(int graph[V][V]) {
    cout << "\nAdjacency Matrix (Distances in meters):\n   ";
    for (int i = 0; i < V; i++)
        cout << i << " ";
    cout << "\n";

    for (int i = 0; i < V; i++) {
        cout << i << ": ";
        for (int j = 0; j < V; j++) {
            if (graph[i][j] == INF)
                cout << "- ";
            else
                cout << graph[i][j] << " ";
        }
        cout << "\n";
    }
}

int main() {
    // Sample graph: Adjacency matrix representation
    // INF = no direct connection
    int graph[V][V] = {
        {0, 10, 20, INF, 25},
        {10, 0, 5, 15, INF},
        {20, 5, 0, 10, INF},
        {INF, 15, 10, 0, 5},
        {25, INF, INF, 5, 0}
    };

    cout << "Departments in College:\n";
    for (int i = 0; i < V; i++)
        cout << i << ": " << departments[i] << endl;

    displayMatrix(graph);
    primMST(graph);

    return 0;
}
