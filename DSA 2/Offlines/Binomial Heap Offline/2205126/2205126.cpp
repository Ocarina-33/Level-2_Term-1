#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

const int N =32;
const int INF = INT_MAX;


struct node{
    int val;
    int degree;
    vector<node*> children;
    node* parent;

    node(int key=0){
        val = key;
        degree = 0;
        parent = nullptr;
        children.resize(N);
        for(int i=0;i<N;i++){
            children[i] = nullptr;
        }
    }
    ~node(){
        for(int i=0;i<N;i++){
            if(children[i]){
                children[i]->parent = nullptr;
            } 
        }
    }
};

map<int,node*> node_map;


 void changeValInMap(int oldVal, int newVal, node* n) {
    auto it = node_map.find(oldVal);
    if (it != node_map.end() && it->second == n) {
        node_map.erase(it);
    }

    node_map[newVal] = n;
}

struct tree{
    node* root;
    tree(node* r = nullptr){
        root = r;
    }
    ~tree(){
        delete root;
    }
};

tree* mergeTree(tree* x, tree* y){
    if(!x) return new tree(y->root);
    if(!y) return new tree(x->root);

    if(x->root->val < y->root->val){
        y->root->parent = x->root;
        x->root->children[x->root->degree] = y->root;
        x->root->degree++;
        return new tree(x->root);
    }
    else{
        x->root->parent = y->root;
        y->root->children[y->root->degree] = x->root;
        y->root->degree++;
        return new tree(y->root);        
    }
}

struct BinomialHeap{
    vector<tree*> trees;

    BinomialHeap(tree* t =nullptr){
        trees.resize(N);
        for(int i=0;i<N;i++){
            trees[i] = nullptr;
        }
        if(t) trees[t->root->degree] = t;
    }

    ~BinomialHeap(){
        for(auto t: trees){
            delete t;
        }
    }

    BinomialHeap(const BinomialHeap&) = delete;
    BinomialHeap& operator= (const BinomialHeap&) = delete;

    void merge(BinomialHeap& h){
        tree* carry = nullptr;
        for(int i=0;i<N;i++){
            if(!trees[i] && !h.trees[i]){
                trees[i] = carry;
                carry = nullptr;
            }
            else if(!h.trees[i]){
                if(carry){
                    carry = mergeTree(carry,trees[i]);
                    trees[i] = nullptr;
                }
            }

            else if(!trees[i]){
                if(carry){
                    carry = mergeTree(carry,h.trees[i]);
                }
                else{
                    trees[i] = h.trees[i];
                }
            }

            else{
                if(carry){
                    carry = mergeTree(carry,h.trees[i]);
                }
                else{
                    carry = mergeTree ( trees[i], h.trees[i]);
                    trees[i] = nullptr;
                }
            }
            h.trees[i] =nullptr;
        }

    }

    node* insert(int val){
        node* newnode = new node(val);
        tree* newtree = new tree(newnode);
        BinomialHeap heap(newtree);

        merge(heap);
        node_map[val] = newnode;
        return newnode;
    }

    int getMin(){
        int mn =INF;
        for(auto tree: trees){
            if(tree && tree->root->val <mn){
                mn = tree->root->val;
            }
        }
        return mn;
    }

    int extractMin(){
        int mn =getMin();
        if(mn == INF) return mn;

        int index =-1;

        for(int i=0;i<N;i++){
            if(trees[i] and trees[i]->root->val == mn){
                index = i;
                break;
            }
        }

        if (index == -1) return mn;

        node* n = trees[index]->root;
        trees[index] = nullptr;
        BinomialHeap h;

         auto it = node_map.find(n->val);
         if (it != node_map.end() && it->second == n) {
               node_map.erase(it);
         }

        for(int i=0;i<n->degree;i++){
            if(n->children[i]){
                n->children[i]->parent = nullptr;
                tree* t = new tree(n->children[i]);
                BinomialHeap newHeap(t);
                h.merge( newHeap);
                node_map[n->children[i]->val] = n->children[i];
            }
        } 

        delete n;
        merge(h);;
        return mn;

    }

    node* decreaseKey(int oldKey, int newKey){
        auto it = node_map.find(oldKey);
        if (it == node_map.end()) return nullptr; 

        node* curr = it->second;

        
        if (newKey >= oldKey) return nullptr; 

    
        curr->val = newKey;
        changeValInMap(oldKey, newKey, curr);


        node* parent = curr->parent;
        while (parent && curr->val < parent->val) {
            
            swap(curr->val, parent->val);

            
            changeValInMap(curr->val, parent->val, curr);
            changeValInMap(parent->val, curr->val, parent);

            
            curr = parent;
            parent = curr->parent;
        }

        return curr;
    }

    bool empty(){
        for(auto t : trees){
            if(t) return false;
        }

        return true;
    }

    void removeKey(int val) {
        if (!node_map.count(val)) return;

        decreaseKey(val, -INF);
        extractMin();  
    }

    void print(ostream& out) {
    out << "Printing Binomial Heap. . ." << endl;
    cout << "Printing Binomial Heap. . ." << endl;

    for (int i = 0; i < N; i++) {
        if (!trees[i]) continue;

        out << "Binomial Tree, B" << i << endl;
        cout << "Binomial Tree, B" << i << endl;

        queue<pair<node*, int>> q;
        q.push(make_pair(trees[i]->root, 0));
        vector<vector<int>> levels;

        while (!q.empty()) {
            pair<node*, int> front = q.front();
            q.pop();

            node* cur = front.first;
            int lvl = front.second;

            if (levels.size() <= lvl) levels.push_back(vector<int>());
            levels[lvl].push_back(cur->val);

            for (int j = 0; j < cur->degree; j++) {
                if (cur->children[j]) {
                    q.push(make_pair(cur->children[j], lvl + 1));
                }
            }
        }

        for (int l = 0; l < levels.size(); l++) {
            out << "Level " << l << ":";
            cout << "Level " << l << ":";

            for (int k = levels[l].size() - 1; k >= 0; k--) {
                out << " " << levels[l][k];
                cout << " " << levels[l][k];
            }

            out << endl;
            cout << endl;
        }
    }
}


};

int main() {
    char c;
    BinomialHeap h;

    ifstream fin("input.txt");
    ofstream fout("output.txt");

    if (!fin || !fout) {
        cerr << "Error opening files!" << endl;
        return 1;
    }

    while (fin >> c && c != 'X') {
        if (c == 'I') {
            int x;
            fin >> x;
            h.insert(x);
        }
        else if (c == 'F') {
            if (h.empty()) {
                cout << "Heap is empty" << endl;
                fout << "Heap is empty" << endl;
            } else {
                int minVal = h.getMin();
                cout << "Find Min returned: " << minVal << endl;
                fout << "Find Min returned: " << minVal << endl;
            }
        }
        else if (c == 'E') {
            if (h.empty()) {
                cout << "Heap is empty" << endl;
                fout << "Heap is empty" << endl;
            } else {
                int minVal = h.extractMin();
                cout << "Extract Min returned: " << minVal << endl;
                fout << "Extract Min returned: " << minVal << endl;
            }
        }
        else if (c == 'D') {
            int x, n;
            fin >> x >> n;
            h.decreaseKey(x, n);
        }
        else if (c == 'R') {
            int x;
            fin >> x;
            h.removeKey(x);
        }
        else if (c == 'P') {
            h.print(fout); 
        }
    }

    fin.close();
    fout.close();
    return 0;
}
