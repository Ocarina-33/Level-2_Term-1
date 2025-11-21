#include<bits/stdc++.h>
using namespace std;
const int INF = INT32_MAX;

vector<int> cap;
vector<pair<int,int>> g[100005];
vector<int> cost;
vector<int> dist;





int main(){
    int n,m,f;
    cin>>n>>m>>f;
    cap.resize(n);
    for(int i=0;i<n;i++){
        cin>>cap[i];
    }

    for(int i=0;i<m;i++){
        int v1,v2,wt;
        cin>>v1>>v2>>wt;
        v1--;
        v2--;
        g[v1].push_back({v2,wt});
    }
    int k;
    cin>>k;
    cost.resize(1000000);
    dist.resize(n,INF);

     
    set<pair<int,int>> st;

    dist[0] = 0;
    st.insert({0,0}); //dist,vertex


    while(!st.empty()){
        auto node = *st.begin();

        int dist_v= node.first;
        int v=node.second;
        st.erase(st.begin());

        for( auto child : g[v]){
            int child_v = child.first;
            int wt = child.second;

            if(dist[child_v] >dist[v]+wt){
                st.erase({dist[child_v],child_v});
                dist[child_v] = dist[v]+wt;
                st.insert({dist[child_v],child_v});

            }
        }
    }

    multiset<pair<int,int>> s;
        for(int i =0;i<n;i++){
            s.insert({dist[i],i});
        }

        // for(auto x:s){
        //     cout<<x.first<<" "<<x.second<<" \n";
        // }

        int i=0;
        for(auto x :s){
            for(int j=0;j<cap[x.second];j++){
                cost[i] = dist[x.second];
                i++;
            }
        }
        for(int j=0;j<k;j++){
            cout<<cost[j]+f<<" ";
        }



            //  int i=0;
            // for(auto x :s){
            //     for(int j=0;j<cap[x.second];j++){
            //         cost[i] = x.first;
            //         i++;
            //         if(i==k) break;
            //     }
            // }

            // for(int i=0;i<k;i++){
            //     cout<<cost[i]<<" ";
            // }
        



















}