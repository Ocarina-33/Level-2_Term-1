#include <bits/stdc++.h>
using namespace std;

const int INF = INT_MAX / 3;
int adj[1005][1005];

int main(){
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    int n, m, k;
    cin >> n >> m >> k;

    vector<int> cost(k+1);
    vector<int> cap(n+1);

    for(int i = 1; i <= n; i++){
        cin >> cap[i];
    }

    // Initialize adjacency matrix
    for(int i = 1; i <= n; i++){
        for(int j = 1; j <= n; j++){
            adj[i][j] = INF;
        }
        adj[i][i] = 0;
    }

    // Read edges
    for(int i = 0; i < m; i++){
        int u, v, wt;
        cin >> u >> v >> wt;

        if(cap[u] == -1 || cap[v] == -1){
            adj[u][v] = adj[v][u] = INF;
        } else {
            adj[u][v] = adj[v][u] = wt;
        }
    }

    // Floyd Warshall (APSP)
    for(int k2 = 1; k2 <= n; k2++){
        for(int i = 1; i <= n; i++){
            for(int j = 1; j <= n; j++){
                adj[i][j] = min(adj[i][j], adj[i][k2] + adj[k2][j]);
            }
        }
    }

    int q;
    cin >> q;

    while(q--){
        int s;
        cin >> s;

        set<pair<int,int>> sd;

        for(int i = 1; i <= n; i++){
            sd.insert({adj[s][i], i});
        }

        int idx = 1;
        for(auto e : sd){
            int dist = e.first;
            int node = e.second;

            while(cap[node]-- > 0){
                cost[idx++] = dist;
                if(idx > k) break;
            }
            if(idx > k) break;
        }

        for(int i = 1; i <= k; i++){
            cout << cost[i] << " ";
        }
        cout << "\n";
    }
}
