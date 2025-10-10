#include <bits/stdc++.h>
using namespace std;

class Edge {
    public:
    int u, v, w;
};

vector<long long> dijkstra(int n, int src, const vector<vector<pair<int,int>>>& adj) {
     long long INF = 1e18;
    vector<long long> dist(n + 1, INF);
    priority_queue<pair<long long,int>, vector<pair<long long,int>>, greater<>> pq;

    dist[src] = 0;
    pq.push({0, src});

    while (!pq.empty()) {
        long long d= pq.top().first;
        int u= pq.top().second;
         pq.pop();
        if (d > dist[u]) continue;

        for (auto it : adj[u]) {
            int v=it.first;
            int w=it.second;
            if (dist[v] > dist[u] + w) {
                dist[v] = dist[u] + w;
                pq.push({dist[v], v});
            }
        }
    }
    return dist;
}

vector<string> markRoads(int node,  vector<int>& from, vector<int>& to, vector<int>& wt) {
                        int m=from.size();                
    vector<vector<pair<int,int>>> adj(node + 1);
    vector<Edge> edges(m);

    for (int i = 0; i < m; i++) {
        edges[i] = {from[i], to[i], wt[i]};
        adj[from[i]].push_back({to[i], wt[i]});
        adj[to[i]].push_back({from[i], wt[i]});
    }

 
    vector<long long> start = dijkstra(node, 1, adj);
    vector<long long> end  = dijkstra(node, node, adj);

    long long dis = start[node];

    vector<string> ans(m);
    for (int i = 0; i < m; i++) {
        int u = edges[i].u, v = edges[i].v, w = edges[i].w;
        bool ok = false;

        if (start[u] + w + end[v] == dis) ok = true;
        if (start[v] + w + end[u] == dis) ok = true;

        ans[i] = ok ? "YES" : "NO";
    }
    return ans;
}

// Example usage
int main() {
    int r_nodes = 5, r_edges = 7;
    vector<int> r_from = {1, 2, 3,1,4,3,2};
    vector<int> r_to   = {2,3,5,4,5,4,4};
    vector<int> weights= {1, 1, 1,1,2,2,4};

    vector<string> ans = markRoads(r_nodes, r_from, r_to, weights);

    for (auto &s : ans) cout << s << "\n";
    return 0;
}
