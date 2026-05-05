/*
- BFS: Uses '#pragma omp parallel for' to process nodes level-by-level.
- DFS: Uses '#pragma omp task' to explore branches in parallel.
*/

#include <iostream>
#include <vector>
#include <omp.h>

using namespace std;

// Graph represented using an Adjacency List
vector<int> adj[100];
bool visited[100];

// Parallel Breadth First Search
void bfs(int start, int n) {
    for(int i = 0; i < n; i++) visited[i] = false;
    vector<int> q;

    q.push_back(start);
    visited[start] = true;

    cout << "BFS: ";
    while(!q.empty()) {
        vector<int> next_level;

        // Parallelize the current level
        #pragma omp parallel for
        for(int i = 0; i < q.size(); i++) {
            int u = q[i];
            
            #pragma omp critical
            cout << u << " ";

            for(int v : adj[u]) {
                #pragma omp critical
                if(!visited[v]) {
                    visited[v] = true;
                    next_level.push_back(v);
                }
            }
        }
        q = next_level; 
    }
    cout << endl;
}

// Parallel Depth First Search
void dfs(int u) {
    #pragma omp critical
    {
        if(!visited[u]) {
            visited[u] = true;
            cout << u << " ";
        }
    }

    for(int v : adj[u]) {
        // If not visited, spawn a new task for the neighbor
        #pragma omp task
        if(!visited[v]) dfs(v);
    }
}

int main() {
    int n, e, u, v, start;
    cout << "Enter number of vertices and edges: ";
    cin >> n >> e;

    cout << "Enter " << e << " edges (u v):" << endl;
    for(int i = 0; i < e; i++) {
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    cout << "Enter starting vertex: ";
    cin >> start;

    // Run BFS
    bfs(start, n);

    // Run DFS
    for(int i = 0; i < n; i++) visited[i] = false;
    cout << "DFS: ";
    #pragma omp parallel
    {
        #pragma omp single
        dfs(start);
    }
    cout << endl;

    return 0;
}
