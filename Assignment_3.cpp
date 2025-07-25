#include <iostream>
#include <vector>
#include <queue>
#include <stack>
using namespace std;

struct Cell {
    int x, y;
};

bool isValid(int x, int y, int n, int m, vector<vector<int>>& maze, vector<vector<bool>>& visited) {
    return (x >= 0 && y >= 0 && x < n && y < m && maze[x][y] == 0 && !visited[x][y]);
}

bool dfsUtil(int x, int y, int n, int m, vector<vector<int>>& maze, vector<vector<bool>>& visited, vector<Cell>& path) {
    if (x == n - 1 && y == m - 1) {
        path.push_back({x, y});
        return true;
    }

    visited[x][y] = true;
    path.push_back({x, y});

    int dx[] = {0, 1, 0, -1};  // R, D, L, U
    int dy[] = {1, 0, -1, 0};

    for (int dir = 0; dir < 4; ++dir) {
        int newX = x + dx[dir];
        int newY = y + dy[dir];

        if (isValid(newX, newY, n, m, maze, visited)) {
            if (dfsUtil(newX, newY, n, m, maze, visited, path)) {
                return true;
            }
        }
    }

    path.pop_back();
    return false;
}

void solveDFS(vector<vector<int>>& maze) {
    int n = maze.size(), m = maze[0].size();
    vector<vector<bool>> visited(n, vector<bool>(m, false));
    vector<Cell> path;

    if (dfsUtil(0, 0, n, m, maze, visited, path)) {
        cout << "Path found using DFS:\n";
        for (auto cell : path)
            cout << "(" << cell.x << "," << cell.y << ") ";
        cout << "\n";
    } else {
        cout << "No path found using DFS.\n";
    }
}

void solveBFS(vector<vector<int>>& maze) {
    int n = maze.size(), m = maze[0].size();
    vector<vector<bool>> visited(n, vector<bool>(m, false));
    vector<vector<Cell>> parent(n, vector<Cell>(m, {-1, -1}));
    queue<Cell> q;

    q.push({0, 0});
    visited[0][0] = true;

    int dx[] = {0, 1, 0, -1};  // R, D, L, U
    int dy[] = {1, 0, -1, 0};

    bool found = false;

    while (!q.empty()) {
        Cell curr = q.front(); q.pop();
        if (curr.x == n - 1 && curr.y == m - 1) {
            found = true;
            break;
        }

        for (int dir = 0; dir < 4; ++dir) {
            int newX = curr.x + dx[dir];
            int newY = curr.y + dy[dir];

            if (isValid(newX, newY, n, m, maze, visited)) {
                visited[newX][newY] = true;
                q.push({newX, newY});
                parent[newX][newY] = curr;
            }
        }
    }

    if (found) {
        vector<Cell> path;
        Cell curr = {n - 1, m - 1};
        while (!(curr.x == 0 && curr.y == 0)) {
            path.push_back(curr);
            curr = parent[curr.x][curr.y];
        }
        path.push_back({0, 0});
        reverse(path.begin(), path.end());

        cout << "Path found using BFS:\n";
        for (auto cell : path)
            cout << "(" << cell.x << "," << cell.y << ") ";
        cout << "\n";
    } else {
        cout << "No path found using BFS.\n";
    }
}

void inputMaze(vector<vector<int>>& maze, int n, int m) {
    cout << "Enter the maze matrix (0=open, 1=blocked):\n";
    maze.resize(n, vector<int>(m));
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < m; ++j)
            cin >> maze[i][j];
}

int main() {
    int n, m;
    vector<vector<int>> maze;

    cout << "Enter maze dimensions (rows cols): ";
    cin >> n >> m;

    inputMaze(maze, n, m);

    int choice;
    do {
        cout << "\nMaze Solver Menu:\n";
        cout << "1. Solve using DFS\n";
        cout << "2. Solve using BFS\n";
        cout << "3. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            solveDFS(maze);
            break;
        case 2:
            solveBFS(maze);
            break;
        case 3:
            cout << "Exiting...\n";
            break;
        default:
            cout << "Invalid choice! Try again.\n";
        }
    } while (choice != 3);

    return 0;
}
