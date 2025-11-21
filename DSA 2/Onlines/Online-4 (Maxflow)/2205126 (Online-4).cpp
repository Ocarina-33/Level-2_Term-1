#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
#define fast ios_base::sync_with_stdio(false);cin.tie(NULL);cout.tie(NULL);

const int N = 510;
vector<int> adj[N];
ll cap[N][N];
int par[N];

struct person{
 int exp,age, cr_record, origin, wpf, tl , la;
};


bool bfs(int s, int t){
    memset(par,-1,sizeof(par));
    queue<int> q;
    par[s] = -2;    
    q.push(s);

    while(!q.empty()){
        int u = q.front(); q.pop();
        for(auto v : adj[u])
        {
            if(par[v]==-1 and cap[u][v]>0){
                par[v] = u;
                q.push(v);
                if(v==t) return true;
            }
        }
    }

    return false;
}



ll maxflow(int s, int t){
    ll max_flow=0;
    while(bfs(s,t)){
        ll min_flow = LLONG_MAX;
        int cur = t;
        while(par[cur]!=-2){
            min_flow = min(min_flow , cap[par[cur]][cur]);
            cur = par[cur];
        }
        max_flow+=min_flow;

        cur = t;
        while(par[cur]!=-2){
            cap[par[cur]][cur]-=min_flow;
            cap[cur][par[cur]]+= min_flow;
            cur = par[cur];
        }
    }
    return max_flow;

}



int main()
{
    int t;
    cin>>t;
    for(int i=1;i<=t;i++){
        int m,n;
        cin>>m>>n;
        vector<person> gang_mem(m);
        vector<person> partner(n);
        for(int i=1;i<=m;i++){
            cin>>gang_mem[i].exp>>gang_mem[i].age>>gang_mem[i].cr_record>>gang_mem[i].origin>>gang_mem[i].wpf>>gang_mem[i].tl>>gang_mem[i].la;
        }
        
        
        for(int i=1;i<=n;i++){
            cin>>partner[i].exp>>partner[i].age>>partner[i].cr_record>>partner[i].origin>>partner[i].wpf>>partner[i].tl>>partner[i].la;
        }

        int source =0;
        int sink = m+n+1;
        //source to gang
        for(int i=1;i<=m;i++){
            adj[source].push_back(i);
            adj[i].push_back(source);
            cap[source][i] =1;
        }

        //partner to sink
        for(int i=1;i<=n;i++){
            int u = i+m;
            adj[u].push_back(sink);
            adj[sink].push_back(u);
            cap[u][sink] =1;
        }

        // gang to partner

        for(int i=1;i<=m;i++){ //gang
            for(int j=1;j<=n;j++){  //partner

                if( (abs(gang_mem[i].exp - partner[j].exp)<=12) and (abs(gang_mem[i].age - partner[j].age)<=5)
                 and ((gang_mem[i].cr_record == partner[i].cr_record))
                 and (gang_mem[i].origin == partner[j].origin) and ((gang_mem[i].tl + partner[j].tl)>=10)
                 and ()
             )




            }
        }

    }
    return 0;
}