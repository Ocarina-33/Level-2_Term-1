#include <bits/stdc++.h>
#include "color.hpp"
using namespace std;
typedef long long ll;

struct node {
    int id;
    string name;
    int stock;
    int color;                          // 0 is red, 1 is black
    node *par, *left, *right;

    node(int v=0, string n="", int s=0, int c=0){
        id=v;
        name = n;
        stock = s;
        color = c;
        par = left = right = nullptr;
    }

};

class rbtree {
    node *root;
    int size;
    ofstream* fout;

    public :
    rbtree(ofstream *out=nullptr){
        root = nullptr;
        size=0;
        this->fout = out;
    }
    ~rbtree(){
        size = 0;
        clear(root);
    }

    
void addItem(int id, string name, int stock) {
        size++;                                     
        //  if tree is empty then create new root
        if (!root) {
            node *n = new node(id, name, stock);
            n->color = 1;                            // root is always black
            root = n;
            return;
        }
        
        node *cur = root;
        while (true) {
            if (cur->id == id) {
                // update 
                cur->name = name;
                cur->stock = stock;
                size--;
                return;
            }
            if (id < cur->id) {
                if (cur->left) {                                  // id is less than cur->id so to go left if exists
                    cur = cur->left;
                } else {
                    // insert as left child
                    node *n = new node(id, name, stock);
                    cur->left = n;
                    n->par = cur;
                    addFix(n);                                    // fix red-black violations
                    return;
                }
            } 

            else {     
                if (cur->right) {
                    cur = cur->right;
                } else {
                    // insert as right child
                    node *n = new node(id, name, stock);
                    cur->right = n;
                    n->par = cur;
                    addFix(n);
                    return;
                }
            }
        }
    }



     // need to fix after adding items
    void addFix(node *n) {
        // Case A: if n is root
        if (!n->par) {
            root = n;
            n->color = 1;
            return;
        }
        // as n is not a root so parent exists
        node *p = n->par;

        // Case B: if parent is black
        if (p->color == 1) return;

        node *gp = p->par;                                       // grandparent
        node *un;                                                // uncle
        if(p == gp->left){
            un = gp->right;
        }
        else{
            un = gp->left;
        }       

        // Case 1: uncle is red
        if (un && un->color == 0) {
            p->color = 1;
            un->color = 1;
            gp->color = 0;
            addFix(gp);
            return;
        }

        // Case 2: uncle is black or null
        // Case 2.1: Zig-zag (inside)
        if (p == gp->left && n == p->right) {
            // left-right case
            leftRotate(p);

            addFix(p);
            return;
        } 
        else if (p == gp->right && n == p->left) {
            // right-left case
            rightRotate(p);
            addFix(p);
            return;
        }

        // Case 2.2: Straight line 
        p->color = 1;
        gp->color = 0;
        if (n == p->left) {
            // left-left
            rightRotate(gp);
        } else {
            // right-right
            leftRotate(gp);
        }
    }

    void remove(node *n) {
        if (!n) return;
        node *cur = n;
        int curCol = cur->color;
        node *d = nullptr, *dpar = nullptr;

        // Case 1: no left child 
        if (!n->left) {
            d = n->right;
            dpar = n->par;
            if (!dpar) root = d;
            else if (n == dpar->left) dpar->left = d;
            else dpar->right = d;
            if (d) d->par = dpar;
            delete n;
            if (curCol == 1) removeFix(d, dpar);
            size--;
            return;
        }
        // Case 2: no right child 
        else if (!n->right) {
            d = n->left;
            dpar = n->par;
            if (!dpar) root = d;
            else if (n == dpar->right) dpar->right = d;
            else dpar->left = d;
            if (d) d->par = dpar;
            delete n;
            if (curCol == 1) removeFix(d, dpar);
            size--;
            return;
        }
        // Case 3: two children 
        else {
            cur = n->right;
            while (cur->left) cur = cur->left;
            curCol = cur->color;
            n->id = cur->id;
            n->stock = cur->stock;
            n->name = cur->name;
            remove(cur);                            // recursively removing
        }
    }

        // fix after deletion
    void removeFix(node *n, node *nPar) {
        while ((n != root) && (!n || n->color == 1)) {
            if (n == nPar->left) {
                node *sib = nPar->right;
                // Case 1: sibling is red
                if (sib && sib->color == 0) {
                    sib->color = 1;
                    nPar->color = 0;
                    leftRotate(nPar);
                    sib = nPar->right;
                }
                // Case 2: sibling black, both nephews black
                if ((!sib->left || sib->left->color == 1) && (!sib->right || sib->right->color == 1)) {
                    if (sib) sib->color = 0;
                    n = nPar;
                    nPar = nPar->par;
                } 
                else {
                    // Case 3: sibling black, left child red, right child black
                    if (!sib->right || sib->right->color == 1) {
                        if (sib->left) sib->left->color = 1;
                        sib->color = 0;
                        rightRotate(sib);
                        sib = nPar->right;
                    }
                    // Case 4: sibling black, right child red
                    sib->color = nPar->color;
                    nPar->color = 1;
                    if (sib->right) sib->right->color = 1;
                    leftRotate(nPar);
                    n = root;
                    break;
                }
            } 
            else {
                node *sib = nPar->left;
                // Case 1: sibling is red
                if (sib && sib->color == 0) {
                    sib->color = 1;
                    nPar->color = 0;
                    rightRotate(nPar);
                    sib = nPar->left;
                }
                // Case 2: sibling black, both nephews black
                if ((!sib->left || sib->left->color == 1) &&
                    (!sib->right || sib->right->color == 1)) {
                    if (sib) sib->color = 0;
                    n = nPar;
                    nPar = nPar->par;
                } 
                else {
                    // Case 3: sibling black, right child red, left child black

                    if (!sib->left || sib->left->color == 1) {
                        if (sib->right) sib->right->color = 1;
                        sib->color = 0;
                        leftRotate(sib);
                        sib = nPar->left;
                    }
                    // Case 4: sibling black, left child red

                    sib->color = nPar->color;
                    nPar->color = 1;
                    if (sib->left) sib->left->color = 1;
                    rightRotate(nPar);
                    n = root;
                    break;
                }
            }
        }
        if (n) n->color = 1;
    }



    node* find(int id){
        if(!root){
            return nullptr;
        }

        node *cur = root;
        while(cur){
            if(cur->id == id){
                return cur;
            }
            if(cur->id > id){
               
                cur = cur->left;
               
            }
            else{   
                cur = cur->right;
            }
        }
        return cur;
        
    }


    void leftRotate( node *n ){
        node *r = n->right;
        if(!r) return;

        n->right = r->left;
        if(r->left) r->left->par = n;

        if(n->par){
            if(n == n->par->left){
                n->par->left = r;
            }
            else n->par->right = r;
        }
        else root = r;

        r->par = n->par;
        r->left = n;
        n->par = r;
    }


    void rightRotate( node * n ){
        node *r = n->left;
        if(!r) return;

        n->left = r->right;
        if(r->right) r->right->par = n;

        if(n->par){
            if(n == n->par->right){
                n->par->right = r;
            }
            else n->par->left = r;
        }
        else root = r;

        r->par = n->par;
        n->par = r;
        r->right = n;
    }


    void clear(){
        size = 0;
        clear(root);
        root = nullptr;
    }
    void clear(node *n ){
        if(!n) return;
        clear(n->left);
        clear(n->right);
        delete n;
    }

    int getSize(){
        return size;
    }

    void inorder(){
        inorder(root);
    }
    void inorder(node *source){
        if(!source) return;
        inorder(source->left);
        string s = to_string(source->id)+"=> "+ source->name+" ("+ to_string(source->stock)+")\n";
        if(source->color==0){
            cout<<dye::red_on_black(s);
            *fout<<dye::red_on_black(s);
        }
        else{
            cout<<dye::black_on_black(s);
            *fout<<dye::black_on_black(s);           
        }
        inorder(source->right);
    }

    void printTree(){
        printTree(root);
        cout << endl;
        *fout << endl;
    }


    void printTree(node* root){
        if(!root) return;

        string s = to_string(root->id)+"_"+root->name;
        if(root->color==0){
            cout<<dye::red_on_black(s);
            if(fout) *fout<<dye::red_on_black(s);
        }
        else{
            cout<<dye::black_on_black(s);
            if(fout) *fout<<dye::black_on_black(s);
        }
        if(!root->left and !root->right) return;
        cout<<"(";
        if(fout) *fout << "(";
        printTree(root->left);
        cout<<",";
        if(fout) *fout << ",";
        printTree(root->right);
        cout<<")";
        if(fout) *fout <<")";
    }


};


class InventorySystem{
    rbtree rbt;
    ofstream *fout;

    public:
    InventorySystem(ofstream* out = nullptr){
        this->fout = out;
        rbt = rbtree(out);
    }
    void AddItem(int id, string name, int stock){
        rbt.addItem(id, name, stock);
    }
    void BuyItem(int id, int total){
        node* n = rbt.find(id);
        if(!n) return;
        if(total>=n->stock) rbt.remove(n);
        else n->stock-=total;
    }
    void CheckItem(int id){
        node *n = rbt.find(id);
        if(!n){
            cout << "Not Available"<<endl;
            if(fout) *fout << "Not Available"<<endl;
        }
        else{
            cout << "Stock left: " << n->stock<<endl;
            if(fout) *fout << "Stock left: " << n->stock<<endl;
        }
    }
    void ClearInventory(){
        if(rbt.getSize() == 0){
            cout<<"Unsuccesful"<<endl;
            if(fout) *fout<<"Unsuccesful"<<endl;
        }
        else{
            rbt.clear();
            cout<<"Succesful"<<endl;
            if(fout) *fout<<"Succesful"<<endl;
        }
    }
    void InventorySize (){
       cout<< rbt.getSize()<<endl;
       if(fout) *fout<<rbt.getSize()<<endl;
    }
    void Empty(){
        if(rbt.getSize() == 0){
            cout<<"yes"<<endl;
            if(fout) *fout<<"yes"<<endl;
        }
        else{
            cout<<"no"<<endl;
            if(fout) *fout<<"no"<<endl;
        }
    }
    void ListInventory(){
        rbt.inorder();
    }
    void printTree(){
        rbt.printTree();
    }
};



int main()
{   
    ifstream fin("input.txt");
    ofstream fout("output.txt");
    InventorySystem inv(&fout);

    string str;

    while(fin >> str){

    if (str == "AI"){
        int id;
        string name;
        int stock;
        fin >> id;
        fin.ignore(); 
        getline(fin, name, '"'); // discard up to first quote
        getline(fin, name, '"'); // read the quoted product name
        fin >> stock;  
        inv.AddItem(id,name,stock);
        inv.printTree();     
    } 

    else if (str == "CI"){
        int id;
        fin>> id;
        inv.CheckItem(id);
    }
    else if(str == "BI"){
        int id ,t;
        fin>>id>>t;
        inv.BuyItem(id,t);
        inv.printTree();
    }
    else if(str == "Em"){
        inv.Empty();
    }

    else if(str== "S"){
        inv.InventorySize();
    }
    else if(str == "Itr"){
        inv.ListInventory();
    }
    else if(str== "Clr"){
        inv.ClearInventory();
    }
    else break;

}

    
    
    

}