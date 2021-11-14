#include <iostream>
#include <vector>
#include <clocale>

using namespace std;
  
// A utility function to add an edge in an
// undirected graph.
void addEdge(vector<int> adj[], int u, int v)
{
    adj[u].push_back(v);
    adj[v].push_back(u);
}
  
// A utility function to print the adjacency list
// representation of graph
void printGraph(vector<int> adj[], int V)
{
    printf("digraph {\n");
    for (int v = 0; v < V; ++v)
    {
        cout << "  " << v;
        for (auto x : adj[v])
           cout << " ->  " << x;
        printf(";\n");
    }
    printf("}\n");
}
 
// Driver code
int main(int argc, char** argv)
{
    std::setlocale(LC_ALL, "en_US.UTF-8");
    
    int V = 5;
    vector<int> adj[V];
    
    addEdge(adj, 0, 1);
    addEdge(adj, 0, 4);
    addEdge(adj, 1, 2);
    addEdge(adj, 1, 3);
    addEdge(adj, 1, 4);
    addEdge(adj, 2, 3);
    addEdge(adj, 3, 4);
    
    printGraph(adj, V);

    return 0;
}