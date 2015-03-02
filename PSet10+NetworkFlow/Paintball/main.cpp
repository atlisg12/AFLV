#include <algorithm>
#include <bitset>
#include <cassert>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <string>
#include <utility>
#include <vector>
using namespace std;

#define all(o) (o).begin(), (o).end()
#define allr(o) (o).rbegin(), (o).rend()
const int INF = 2147483647;
typedef long long ll;
typedef pair<int, int> ii;
typedef vector<int> vi;
typedef vector<ii> vii;
typedef vector<vi> vvi;
typedef vector<vii> vvii;
template <class T> int size(T &x) { return x.size(); }

// assert or gtfo

struct flow_network {
	
    struct edge {
        int u, v, cap;
        edge *rev;
        bool forward;
        edge(int _u, int _v, int _cap, bool forw)
		: u(_u), v(_v), cap(_cap), forward(forw) { }
    };
	
    int n;
    vector<vector<edge*> > adj;
    flow_network(int _n) : n(_n), adj(_n) { }
	
    void add_edge(int u, int v, int cap) {
        edge *e = new edge(u, v, cap, true);
        edge *rev = new edge(v, u, 0, false);
        e->rev = rev;
        rev->rev = e;
        adj[u].push_back(e);
        adj[v].push_back(rev);
    }
	
    int augment(int s, int t) {
        vector<edge*> back(n, NULL);
        queue<int> Q;
        Q.push(s);
        back[s] = (edge*)1;
        while (!Q.empty()) {
            int u = Q.front(); Q.pop();
            for (int i = 0; i < adj[u].size(); i++) {
                int v = adj[u][i]->v;
                if (back[v] == NULL && adj[u][i]->cap > 0) {
                    back[v] = adj[u][i];
                    Q.push(v);
                }
            }
        }
		
        if (back[t] == NULL)
            return 0;
		
        stack<edge*> S;
        S.push(back[t]);
        int bneck = back[t]->cap;
        while (S.top()->u != s) {
            S.push(back[S.top()->u]);
            bneck = min(bneck, S.top()->cap);
        }
		
        while (!S.empty()) {
            S.top()->cap -= bneck;
            S.top()->rev->cap += bneck;
            S.pop();
        }
		
        return bneck;
    }
	
    int max_flow(int source, int sink) {
        int flow = 0;
        while (true) {
            int f = augment(source, sink);
            if (f == 0) {
                break;
            }
			
            flow += f;
        }
		
        return flow;
    }
};

int main()
{
    int N, M;
    scanf("%d %d", &N, &M);
	
	int SOURCE = 0,
	SINK = 1,
	LEFT = 1,
	RIGHT = LEFT + N,
	CNT = (N * 2) + 2;
	
    flow_network players(CNT);
	
    for (int i = 0; i < M; i++) {
        int a, b;
        scanf("%d %d", &a, &b);
		
        players.add_edge(LEFT + a, RIGHT + b, 1);
		
		players.add_edge(LEFT + b, RIGHT + a, 1);
		
    }
	
	for (int i = 1; i <= N; i++) {
		players.add_edge(SOURCE, LEFT + i, 1);
		players.add_edge(RIGHT + i, SINK, 1);
	}
	
	if(players.max_flow(SOURCE, SINK) == N) {
		for (int u = 1; u <= N; u++) {
			for (int i = 0; i < players.adj[LEFT + u].size(); i++) {
				if (players.adj[LEFT + u][i]->forward && players.adj[LEFT + u][i]->rev->cap == 1) {
					printf("%d\n", players.adj[LEFT + u][i]->v - RIGHT);
				}
			}
		}
	}
	else {
		printf("Impossible\n");
	}

	
    
	
    return 0;
}

