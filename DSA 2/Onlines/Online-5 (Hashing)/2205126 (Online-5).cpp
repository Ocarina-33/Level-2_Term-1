#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
#define fast ios_base::sync_with_stdio(false);cin.tie(NULL);cout.tie(NULL);
//for delete its del, for empty space its -inf
const int mod = 1e9+7;
const int empty = -INT_MAX;
const int del = INT_MAX;
int n,pmax,q;

bool isPrime(int n) {
    if(n <= 1)return false;
    if(n <= 3)return true;
    if(n % 2 == 0 || n % 3 == 0)return false;
    for(int i = 5; i * i <= n; i += 6)
        if(n%i== 0 or n%(i + 2)== 0)
            return false;
    return true;
}

class Hash{

    int N; 
    
    
    public:
    
    vector<int> hashTable;

    int collisionCount;
    int probeCount;
    int time;
    int insert;
    int deleted;
    int occupied;

    Hash(int n) : N(n) {
        while(!isPrime(N)) {
            N++;
        }
        hashTable.resize(N,empty); 
        collisionCount =0;
        probeCount = 0;
        insert=0;
        deleted=0;
        occupied =0;
    }

    ~Hash(){
    }
    //polynomial rolling hash
    int hash1(int s){
        // ll ans=0; 
        // ll gun = 1;
        // for(auto c: s){
        //     ans = (ans + (c*gun)%mod) % mod;
        //     gun = (gun*29)%mod;
        // }
        // return ans%N;
        return s;

    }

    //fnv-1a hash
    int hash2(string s){
        unsigned int ans = 2166136261u; // FNV offset basis
        for(auto c: s){
            ans ^= c; // XOR with the byte
            ans *= 16777619u; // FNV prime
        }
        return ans % N;
    }

    int insertProbe(int s, bool first = true){
        int hashValue;
        if(first) hashValue = hash1(s);
        //else hashValue = hash2(s);
        int prime = 5;
        int i = 0;
        while(true){                
            int index = (hashValue + insert*insert) % N; 
            if(hashTable[index] == empty or hashTable[index]==s or hashTable[index] == del){
                hashTable[index] = s;
                occupied++;
                collisionCount+=i;
                return index;
            }
            i++;
            if(i==N) return -1;
        }
        insert++;
        if(collisionCount>pmax){
            reHashUp(hashTable);
        }

    }

    int searchProbe(int s, bool first = true){
        int hashValue;
        if(first) hashValue = hash1(s);
        //else hashValue = hash2(s);
        
        int i = 0;
        while(true){                
            int index = (hashValue + insert*insert) % N; 
            if(hashTable[index] == empty){
                probeCount+=i+1;
                return -1;
            }
            else if (hashTable[index] == s){
                probeCount+=i+1;
                return index;
            }
            else if(i==N) return -1;
            i++;
        }
    }

    int deleteProbe(int s, bool first = true){
        int hashValue;
        if(first) hashValue = hash1(s);
       // else hashValue = hash2(s);
        int prime = 5;
        int i = 0;
        while(true){                
            int index = (hashValue + i * prime) % N; 
            if(hashTable[index] == empty){
                return -1;
            }
            else if (hashTable[index] == s){
                hashTable[index] = del;
                deleted++;
                if(deleted==5){
                    double alpha = (double)occupied/(double)N;
                }
                return index;
            }
            else if (i==N){
                return -1;
            }
            i++;
        } 

    }

    void reHashUp(vector<int> &h1){
        int x = N;
            while(true) {
                x++;
                if(isPrime(x) and x>= 2*N) break;
             }
    
        
        vector<int> hcopy(x);
        for(int i=0;i<N;i++){
            if(hashTable[i]!=del) continue;
            hcopy[i] = hashTable[i];
        }
        hashTable.clear();
        hashTable.resize(x,empty);
        for(int i=0;i<N;i++){
            if(hashTable[i]==del) continue;
            hcopy[i] = hashTable[i];
        }       
    }

    void reHashDown(vector<int> &h1){

    }

};



int main()
{

  cin>>n>>pmax>>q;
  Hash h1(n);



}