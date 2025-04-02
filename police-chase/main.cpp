#include <iostream>
#include <queue>
#include <vector>

using namespace std;

constexpr int MAX_N = 503, MAX_M = 1003, INF = 1e9 + 9;

vector<int> adj[MAX_N];
int cap[MAX_N][MAX_N];
bool s_part[MAX_M];

int find_augmenting_path(int s, int t, vector<int>& prev) {
    fill(prev.begin(), prev.end(), -1);
    prev[s] = 0;
    queue<pair<int, int>> q;
    q.push({s, INF});

    while (!q.empty()) {
        int v = q.front().first;
        int flow = q.front().second;
        q.pop();
        for (int u : adj[v]) {
            if (prev[u] == -1 && cap[v][u] > 0) {
                prev[u] = v;
                int new_flow = min(flow, cap[v][u]);
                if (u == t) {
                    return new_flow;
                }
                q.push({u, new_flow});
            }
        }
    }
    return 0;
}

int find_max_flow(int s, int t, int n) {
    int max_flow = 0, new_flow;
    vector<int> prev(n + 1);
    while ((new_flow = find_augmenting_path(s, t, prev)) > 0) {
        max_flow += new_flow;  // in this case new_flow = 1
        int v = t;
        while (v != s) {
            int u = prev[v];
            cap[u][v] -= new_flow;
            cap[v][u] += new_flow;
            v = u;
        }
    }
    return max_flow;
}

void find_cut_part(int v) {
    s_part[v] = true;
    for (int u : adj[v]) {
        if (!s_part[u] && cap[v][u] > 0) {
            find_cut_part(u);
        }
    }
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);

    int n, m, u, v;
    cin >> n >> m;
    for (int i = 0; i < m; i++) {
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
        cap[u][v] = 1;
        cap[v][u] = 1;
    }

    int flow = find_max_flow(1, n, n);
    find_cut_part(1); 
    cout << flow << "\n";
    for (int i = 1; i <= n; i++) {
        if (s_part[i]) {
            for (int u : adj[i]) {
                if (!s_part[u] && cap[i][u] == 0) {
                    cout << i << " " << u << "\n";
                }
            }
        }
    }

    return 0;
}
