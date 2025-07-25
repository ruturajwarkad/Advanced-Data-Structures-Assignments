#include <iostream>
#include <vector>
#include <limits>
#include <queue>
#include <string>
using namespace std;

#define INF INT_MAX

struct Node {
    int id;
    int cost;

    bool operator>(const Node& other) const {
        return cost > other.cost;
    }
};

void dijkstra(vector<vector<pair<int, int>>>& graph, int src, vector<int>& dist, vector<int>& parent) {
    int V = graph.size();
    dist.assign(V, INF);
    parent.assign(V, -1);
    dist[src] = 0;

    priority_queue<Node, vector<Node>, greater<Node>> pq;
    pq.push({src, 0});

    while (!pq.empty()) {
        Node u = pq.top(); pq.pop();

        for (auto& neighbor : graph[u.id]) {
            int v = neighbor.first;
            int weight = neighbor.second;

            if (dist[u.id] + weight < dist[v]) {
                dist[v] = dist[u.id] + weight;
                parent[v] = u.id;
                pq.push({v, dist[v]});
            }
        }
    }
}

void printPath(int target, vector<int>& parent, vector<string>& names) {
    if (target == -1) return;
    printPath(parent[target], parent, names);
    cout << names[target];
    if (parent[target] != -1) cout << " -> ";
}

int main() {
    int V = 6; // Total locations
    vector<string> locationNames = {
        "Supplier", "Warehouse A", "Warehouse B", "Distribution Center", "Retail Store A", "Retail Store B"
    };

    vector<vector<pair<int, int>>> graph(V);

    // Add edges: graph[from].push_back({to, cost});
    graph[0].push_back({1, 10}); // Supplier -> Warehouse A
    graph[0].push_back({2, 15}); // Supplier -> Warehouse B
    graph[1].push_back({3, 12}); // Warehouse A -> Distribution Center
    graph[2].push_back({3, 10}); // Warehouse B -> Distribution Center
    graph[3].push_back({4, 5});  // Distribution Center -> Retail Store A
    graph[3].push_back({5, 7});  // Distribution Center -> Retail Store B

    // Also make it undirected if transport works both ways
    graph[1].push_back({0, 10});
    graph[2].push_back({0, 15});
    graph[3].push_back({1, 12});
    graph[3].push_back({2, 10});
    graph[4].push_back({3, 5});
    graph[5].push_back({3, 7});

    int source;
    cout << "Supply Chain Locations:\n";
    for (int i = 0; i < V; ++i)
        cout << i << ": " << locationNames[i] << endl;

    cout << "\nEnter source location index (e.g., 0 for Supplier): ";
    cin >> source;

    vector<int> dist, parent;
    dijkstra(graph, source, dist, parent);

    cout << "\nShortest Paths from " << locationNames[source] << ":\n";
    for (int i = 0; i < V; ++i) {
        if (i == source) continue;
        cout << "- To " << locationNames[i] << " [Distance: ";
        if (dist[i] == INF) {
            cout << "Unreachable]\n";
        } else {
            cout << dist[i] << "]: Path: ";
            printPath(i, parent, locationNames);
            cout << endl;
        }
    }

    return 0;
}
