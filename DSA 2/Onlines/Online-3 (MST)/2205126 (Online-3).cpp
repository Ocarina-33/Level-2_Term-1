#include <bits/stdc++.h>
#include <climits>
using namespace std;
typedef long long ll;
#define fast ios_base::sync_with_stdio(false);cin.tie(NULL);cout.tie(NULL);
const int N = 1e5;
int parent[N];
int s[N];

const int inf = INT_MAX/2;

void make(int v){
    parent[v]=v;
    s[v]=1;
}

int find(int v){
    if(v == parent[v]) return parent[v];
    return parent[v]= find(parent[v]);
}

void Union( int a, int b){
    a = find(a);
    b = find(b);
    if (a != b)
    {
        if (s[a] < s[b])
        {
            parent[a] = b;
            s[b] += s[a];
        }
        else
        {
            parent[b] = a;
            s[a] += s[b];
        }
    }
 }

int riskc[N];
int check[N];

int main()
{
    fast
    int n,m,p,k;
    cin>>n>>m >> p>>k;
    
    for(int i=0;i<k;i++){
        int x;
        cin>>x;

        riskc[x] =1;
    }
    // vector<int> r;
    int cmp = n-k;

    vector<pair<int,int>> e;
    vector<pair<int, pair<int,int> > > edges;
    
    vector<pair<int, pair<int,int> > > redges;

    for(int i=0;i<m;i++){
        int u,v,wt;
        cin>>u>>v>>wt;
        if(riskc[u]==1 or riskc[v]==1){
            redges.push_back({wt,{u,v}});
            // if(riskc[u]==1) r.push_back(u);
            // if(riskc[v]==1) r.push_back(v);
        }
        edges.push_back({wt,{u,v}});
    }
    sort(edges.begin(),edges.end());
    sort(redges.begin(),redges.end());

    for(int i=0;i<n;i++){
        make(i);
    }

    int total_cost=0;
    int T=0;

    for( auto &edge : edges){
        int wt = edge.first;
        int u = edge.second.first;
        int v = edge.second.second;
        if(riskc[u]==1 or riskc[v]==1) continue;
        if(find(u)==find(v)) continue;
        Union(u,v);
        total_cost+=wt;
        //cout<<u<<" "<<v<<endl;
        e.push_back({u,v});
        check[u]=1;
        check[v]=1;
        T++;
        cmp--;
    }
    
    for(int i=0;i<cmp-1;i++){
        for(auto &e: redges ){
                int c = e.first;
               // cout<<c<<endl;
                total_cost+=c + (p);
                check[i]=1;

            }
        
    }







    cout<<T<<endl;
    for(auto &ed : e){
        cout<<ed.first<<" "<<ed.second<<endl;
    }
    cout<<total_cost<<endl;




    return 0;
}