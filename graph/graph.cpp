#include<bits/stdc++.h>

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
    printf("charset=\"UTF-8\";\n");
    char letters[] = { 'a', 'b', 'c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z' };
    for (int v = 0; v < V; ++v)
    {
        cout << letters[v];
        for (auto x : adj[v])
           cout << " -> " << letters[x];
        printf(";\n");
    }
    printf("}\n");


}
 
// Driver code
int main()
{
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