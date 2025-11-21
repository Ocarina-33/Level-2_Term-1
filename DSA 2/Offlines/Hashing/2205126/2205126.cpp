#include "rbtree.h"
using namespace std::chrono;

int N;
const int mod = 1e9+7;
string stringGenerated[10000];

vector<string> iString, dString, sString;

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
    vector<rbtree> trees;
    vector<string> hashTable;

    public:

    int collisionCount;
    int probeCount;
    int time;

    Hash(int n) : N(n) {
        while(!isPrime(N)) {
            N++;
        }
        hashTable.resize(N); 
        trees.resize(N);
        collisionCount =0;
        probeCount = 0;
    }

    ~Hash(){
    }
    //polynomial rolling hash
     int hash1(string s){
        ll ans=0; 
        ll gun = 1;
        for(auto c: s){
            ans = (ans + (c*gun)%mod) % mod;
            gun = (gun*29)%mod;
        }
        return ans%N;
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

    int insertProbe(string s, bool first = true){
        int hashValue;
        if(first) hashValue = hash1(s);
        else hashValue = hash2(s);
        int prime = 5;
        int i = 0;
        while(true){                
            int index = (hashValue + i * prime) % N; 
            if(hashTable[index] == "" or hashTable[index]==s){
                hashTable[index] = s;
                collisionCount+=i;
                return index;
            }
            i++;
            if(i==N) return -1;
        }
    }

    int searchProbe(string s, bool first = true){
        int hashValue;
        if(first) hashValue = hash1(s);
        else hashValue = hash2(s);
        int prime = 5;
        int i = 0;
        while(true){                
            int index = (hashValue + i * prime) % N; 
            if(hashTable[index] == ""){
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

    void deleteProbe(string s, bool first = true){
        int hashValue;
        if(first) hashValue = hash1(s);
        else hashValue = hash2(s);
        int prime = 5;
        int i = 0;
        while(true){                
            int index = (hashValue + i * prime) % N; 
            if(hashTable[index] == ""){
                return;
            }
            else if (hashTable[index] == s){
                hashTable[index] = "~";
                return;
            }
            else if (i==N){
                return;
            }
            i++;
        } 
    }

    int insertDouble(string s, bool first = true){
        int hashValue;
        int steps;
        if(first){
            hashValue = hash1(s);
            steps = hash2(s);
        }
        else{
            hashValue = hash2(s);
            steps = hash1(s);
        }
        int i = 0;
        while(true){                
            int index = (hashValue + i * steps) % N; 
            if(hashTable[index] == "" or hashTable[index]==s){
                hashTable[index] = s;
                collisionCount+=i;
                return index;
            }
            i++;
            if(i==N) return -1;
        }
    }

    int searchDouble(string s, bool first = true){
        int hashValue;
        int steps;
        if(first){
            hashValue = hash1(s);
            steps = hash2(s);
        }
        else{
            hashValue = hash2(s);
            steps = hash1(s);
        }
        int i = 0;
        while(true){                
            int index = (hashValue + i * steps) % N; 
            if(hashTable[index] == ""){
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

    void deleteDouble(string s, bool first = true){
        int hashValue;
        int steps;
        if(first){
            hashValue = hash1(s);
            steps = hash2(s);
        }
        else{
            hashValue = hash2(s);
            steps = hash1(s);
        }
        int i = 0;
        while(true){                
            int index = (hashValue + i * steps) % N; 
            if(hashTable[index] == ""){
                return;
            }
            else if (hashTable[index] == s){
                hashTable[index] = "~";
                return;
            }
            else if (i==N){
                return;
            }
            i++;
        } 
    }

    int insertChain(string s, bool first = true){
        int hashValue;
        if(first) hashValue = hash1(s);
        else hashValue = hash2(s);
        collisionCount+=trees[hashValue].getSize();
        trees[hashValue].addItem(s,s);
        return hashValue;
    }

    void deleteChain(string s, bool first = true){
        int hashValue;
        if(first) hashValue = hash1(s);
        else hashValue = hash2(s);
        trees[hashValue].remove(s);
    }

    int searchChain(string s, bool first = true){
        int hashValue;
        if(first) hashValue = hash1(s);
        else hashValue = hash2(s);
        auto nd = trees[hashValue].find(s);;
        if(nd) return hashValue;
        else return -1;
    }

};

int main(){
    freopen("output.txt", "w", stdout);

    N=8000;
    while(!isPrime(N)) {
        N++;
    }
    
    srand(time(0));   
    for(int i=0;i<10000;i++){
        int n = ( rand()%6 ) + 5;
        string s ="";
        for(int j=0;j<n;j++){
            int a = (rand()%26) + 97;
            char c =a;
            s += c;
        }
        stringGenerated[i] = s;
    }
    

    for(double alpha = 0.4;  alpha<0.93 ;  alpha+=0.1){
        cout << "Alpha = " << alpha << endl;
        int time[10][2];
        Hash h1(N), h2(N), h3(N), h4(N), h5(N), h6(N);
        int M = N*alpha;
        iString.resize(M);
        map<int,bool> dup;
        for(int i =0; i<M;){
            int id = rand()%10000;
            if(dup[id]){
                continue;
            }
            dup[id] = true;
            iString[i] = stringGenerated[id];
            i++;
        }
        

        for(int i =0 ;i<M;i++){
            h1.insertProbe(iString[i]);
            h2.insertProbe(iString[i], 0);
            h3.insertDouble(iString[i]);
            h4.insertDouble(iString[i], 0);
            h5.insertChain(iString[i]);
            h6.insertChain(iString[i], 0);
        }

        
        cout<<"Collision Count (Linear probing) hash1 : "<<h1.collisionCount<<endl;
        cout<<"Collision Count (Linear probing) hash2 : "<<h2.collisionCount<<endl;
        cout<<"Collision Count (Double Hashing) hash1 : "<<h3.collisionCount<<endl;
        cout<<"Collision Count (Double Hashing) hash2 : "<<h4.collisionCount<<endl;
        cout<<"Collision Count (Chaining) hash1 : "<<h5.collisionCount<<endl;
        cout<<"Collision Count (Chaining) hash2 : "<<h6.collisionCount<<endl;
        cout<<"\n";

        int l = M*0.1;
        dString.resize(l);
        sString.resize(l);
        for(int i =0 ;i<l;i++){
            int id = rand()%M;
            sString[i] = iString[id];
        }
// h1
{
    auto start = high_resolution_clock::now();
    for (int i = 0; i < l; i++) {
        h1.searchProbe(sString[i]);
    }
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<nanoseconds>(stop - start);
    time[1][0] = duration.count();
}

// h2
{
    auto start = high_resolution_clock::now();
    for (int i = 0; i < l; i++) {
        h2.searchProbe(sString[i], 0);
    }
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<nanoseconds>(stop - start);
    time[2][0] = duration.count();
}

// h3
{
    auto start = high_resolution_clock::now();
    for (int i = 0; i < l; i++) {
        h3.searchDouble(sString[i]);
    }
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<nanoseconds>(stop - start);
    time[3][0] = duration.count();
}

// h4
{
    auto start = high_resolution_clock::now();
    for (int i = 0; i < l; i++) {
        h4.searchDouble(sString[i], 0);
    }
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<nanoseconds>(stop - start);
    time[4][0] = duration.count();
}

// h5
{
    auto start = high_resolution_clock::now();
    for (int i = 0; i < l; i++) {
        h5.searchChain(sString[i]);
    }
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<nanoseconds>(stop - start);
    time[5][0] = duration.count();
}

// h6
{
    auto start = high_resolution_clock::now();
    for (int i = 0; i < l; i++) {
        h6.searchChain(sString[i], 0);
    }
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<nanoseconds>(stop - start);
    time[6][0] = duration.count();
}


        cout<<"Average probe Count before delete (Linear probing) hash1 : "<< (double) h1.probeCount/ (double) l <<endl;
        cout<<"Average probe Count before delete (Linear probing) hash2 : "<< (double) h2.probeCount/ (double) l <<endl;
        cout<<"Average probe Count before delete (Double Hashing) hash1 : "<< (double) h3.probeCount/ (double) l <<endl;
        cout<<"Average probe Count before delete (Double Hashing) hash2 : "<< (double) h4.probeCount/ (double) l <<endl;
        cout<<"\n";
        cout <<"Searched" << l << "elements\n";
        cout<<"Avg Search time before delete hash1: "<<time[1][0]/l << " nano " << endl;
        cout<<"Avg Search time before delete hash2: "<<time[2][0]/l << " nano " << endl;
        cout<<"Avg Search time before delete hash3: "<<time[3][0]/l << " nano " << endl;
        cout<<"Avg Search time before delete hash4: "<<time[4][0]/l << " nano " << endl;
        cout<<"Avg Search time before delete hash5: "<<time[5][0]/l << " nano " << endl;
        cout<<"Avg Search time before delete hash6: "<<time[6][0]/l << " nano " << endl;
        cout<<"\n";

        
        dup.clear();
        for(int i =0; i<l;){
            int id = rand()%M;
            if(dup[id]){
                continue;
            }
            dup[id] = true;
            dString[i] = iString[id];
            h1.deleteProbe(dString[i]);
            h2.deleteProbe(dString[i]);
            h3.deleteDouble(dString[i]);
            h4.deleteDouble(dString[i]);
            h5.deleteChain(dString[i]);
            h6.deleteChain(dString[i]);
            i++;
        }

    h1.probeCount =0, h2.probeCount =0, h3.probeCount=0, h4.probeCount=0 ;

// h1
{
    auto start = high_resolution_clock::now();
    for (int i = 0; i < l / 2; i++) {
        int id = rand() % M;
        h1.searchProbe(iString[id]);
    }
    auto stop = high_resolution_clock::now();
    time[1][1] += duration_cast<nanoseconds>(stop - start).count();
}
// h2
{
    auto start = high_resolution_clock::now();
    for (int i = 0; i < l / 2; i++) {
        int id = rand() % M;
        h2.searchProbe(iString[id], 0);
    }
    auto stop = high_resolution_clock::now();
    time[2][1] += duration_cast<nanoseconds>(stop - start).count();
}
// h3
{
    auto start = high_resolution_clock::now();
    for (int i = 0; i < l / 2; i++) {
        int id = rand() % M;
        h3.searchDouble(iString[id]);
    }
    auto stop = high_resolution_clock::now();
    time[3][1] += duration_cast<nanoseconds>(stop - start).count();
}
// h4
{
    auto start = high_resolution_clock::now();
    for (int i = 0; i < l / 2; i++) {
        int id = rand() % M;
        h4.searchDouble(iString[id], 0);
    }
    auto stop = high_resolution_clock::now();
    time[4][1] += duration_cast<nanoseconds>(stop - start).count();
}
// h5
{
    auto start = high_resolution_clock::now();
    for (int i = 0; i < l / 2; i++) {
        int id = rand() % M;
        h5.searchChain(iString[id]);
    }
    auto stop = high_resolution_clock::now();
    time[5][1] += duration_cast<nanoseconds>(stop - start).count();
}

// h6
{
    auto start = high_resolution_clock::now();
    for (int i = 0; i < l / 2; i++) {
        int id = rand() % M;
        h6.searchChain(iString[id], 0);
    }
    auto stop = high_resolution_clock::now();
    time[6][1] += duration_cast<nanoseconds>(stop - start).count();
}


// h1
{
    auto start = high_resolution_clock::now();
    for (int i = 0; i < l / 2; i++) {
        int id = rand() % l;
        h1.searchProbe(dString[id]);
    }
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<nanoseconds>(stop - start);
    time[1][1] += duration.count();
}

// h2
{
    auto start = high_resolution_clock::now();
    for (int i = 0; i < l / 2; i++) {
        int id = rand() % l;
        h2.searchProbe(dString[id], 0);
    }
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<nanoseconds>(stop - start);
    time[2][1] += duration.count();
}

// h3
{
    auto start = high_resolution_clock::now();
    for (int i = 0; i < l / 2; i++) {
        int id = rand() % l;
        h3.searchDouble(dString[id]);
    }
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<nanoseconds>(stop - start);
    time[3][1] += duration.count();
}

// h4
{
    auto start = high_resolution_clock::now();
    for (int i = 0; i < l / 2; i++) {
        int id = rand() % l;
        h4.searchDouble(dString[id], 0);
    }
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<nanoseconds>(stop - start);
    time[4][1] += duration.count();
}

// h5
{
    auto start = high_resolution_clock::now();
    for (int i = 0; i < l / 2; i++) {
        int id = rand() % l;
        h5.searchChain(dString[id]);
    }
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<nanoseconds>(stop - start);
    time[5][1] += duration.count();
}

// h6
{
    auto start = high_resolution_clock::now();
    for (int i = 0; i < l / 2; i++) {
        int id = rand() % l;
        h6.searchChain(dString[id], 0);
    }
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<nanoseconds>(stop - start);
    time[6][1] += duration.count();
}

        cout<<"Average probe Count after delete (Linear probing) hash1 : "<< (double) h1.probeCount/ (double) l <<endl;
        cout<<"Average probe Count after delete (Linear Probing) hash2 : "<< (double) h2.probeCount/ (double) l <<endl;
        cout<<"Average probe Count after delete (Double Hashing) hash1 : "<< (double) h3.probeCount/ (double) l <<endl;
        cout<<"Average probe Count after delete (Double Hashing) hash2 : "<< (double) h4.probeCount/ (double) l <<endl;
        cout<<"\n";
        cout<<"Avg Search time ater delete hash1: "<<time[1][1]/l << " nano " << endl;
        cout<<"Avg Search time ater delete hash2: "<<time[2][1]/l << " nano " << endl;
        cout<<"Avg Search time ater delete hash3: "<<time[3][1]/l << " nano " << endl;
        cout<<"Avg Search time ater delete hash4: "<<time[4][1]/l << " nano " << endl;
        cout<<"Avg Search time ater delete hash5: "<<time[5][1]/l << " nano " << endl;
        cout<<"Avg Search time ater delete hash6: "<<time[6][1]/l << " nano " << endl;
        cout<<"\n";

        cout<<"\n\n";

    }


    return 0;
}
