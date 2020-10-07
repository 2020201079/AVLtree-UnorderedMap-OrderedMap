#include<iostream>
#include<string>

using namespace std;

template<typename T,typename V>
class Node{
    public:
        Node<T,V>* left;
        Node<T,V>* right;
        T key;
        V data;
        Node(){}
        Node(T key,V data){
            this->left = nullptr;
            this->right = nullptr;
            this->data = data;
            this->key = key;
        }
};

template<typename T,typename V>
Node<T,V>* getPredecessor(Node<T,V>* root){
    if(root == nullptr){
        throw runtime_error("called getPredecessor on a null ptr ");
        return nullptr;
    }
    else if(root->left == nullptr)
        return nullptr;
    else{
        Node<T,V>* temp = root->left;
        while(temp->right != nullptr)
            temp = temp->right;
        return temp;
    }
}

template<typename T,typename V>
Node<T,V>* getSuccessor(Node<T,V>* root){
    if(root == nullptr){
        throw runtime_error("called getSuccessor on a null ptr ");
        return nullptr;
    }
    else if(root->right == nullptr)
        return nullptr;
    else{
        Node<T,V>* temp = root->right;
        while(temp->left != nullptr)
            temp = temp->left;
        return temp;
    }
}

template<typename T,typename V>
class AVLTree{
    private:
        Node<T,V>* llRotation(Node<T,V>* root);
        Node<T,V>* lrRotation(Node<T,V>* root);
        Node<T,V>* rrRotation(Node<T,V>* root);
        Node<T,V>* rlRotation(Node<T,V>* root);
        Node<T,V>* insertionHelper(Node<T,V>* root,T key,V data);
        Node<T,V>* delHelper(Node<T,V>* root,T key);
        void printInorderHelper(Node<T,V>* root);
        int balFactor(Node<T,V>* root);
        Node<T,V>* balance(Node<T,V>* root); // passing the element which is just inserted
        Node<T,V>* searchHelper(Node<T,V>* root,T key);
        void clearHelper(Node<T,V>* root);
        int noOfKeys;
    public:
        Node<T,V>* root;
        AVLTree(){
            root = nullptr;
            noOfKeys=0;
        }
        int size();
        void insert(T key,V data); // implement it later just define all methods here
        void erase(T key);
        bool find(T key);
        void printInorder();
        int height(Node<T,V>* root);
        void clear();
        V& operator[](T key);
};

template<typename T,typename V>
int AVLTree<T,V>::size(){
    return noOfKeys;
}

template<typename T,typename V>
Node<T,V>* AVLTree<T,V>::searchHelper(Node<T,V>* root,T key){
    if(root == nullptr){
        return nullptr;
    }
    else if(key<root->key){
        return searchHelper(root->left,key);
    }
    else if(key>root->key){
        return searchHelper(root->right,key);
    }
    return root;
}
template<typename T,typename V>
bool AVLTree<T,V>::find(T key){
    Node<T,V>* result= searchHelper(root,key);
    if(result==nullptr)
        return false;
    return true;
}

template<typename T,typename V>
void AVLTree<T,V>::erase(T key){
    Node<T,V>* currNode = searchHelper(root,key);
    if(currNode == nullptr){
        cout<<key<<" is not present in tree to delete"<<endl;
        return;
    }
    else{
        root = delHelper(root,key);
        noOfKeys--;
        //cout<<"deleted"<<x<<endl;
    }
}
template<typename T,typename V>
Node<T,V>* AVLTree<T,V>::delHelper(Node<T,V>* root,T key){
    if(root == nullptr){
        cout<<key<<" does not exists in tree"<<endl;
        return root;
    }
    else if(key > root->key){
        //cout<<" right "<<endl;
        root->right = delHelper(root->right,key);
        root = balance(root);
    }
    else if(key < root->key){
        //cout<<" left "<<endl;
        root->left = delHelper(root->left,key);
        root = balance(root);
    }
    else{ // found the node to del
        //cout<<" found "<<endl;
        if(root->left == nullptr && root->right == nullptr){
            free(root);
            return nullptr;
        }
        else if(root->right == nullptr && root->left !=nullptr){
            //cout<<"entered right null left not null "<<endl;
            Node<T,V>* temp = root->left;
            free(root);
            return temp;
        }
        else if(root->right != nullptr && root->left == nullptr){
            Node<T,V>* temp = root->right;
            free(root);
            return temp;
        }
        else{ // case where both children are present
            Node<T,V>* pred = getPredecessor(root);
            root->data = pred->data;
            root->key = pred->key;
            root->left = delHelper(root->left,pred->key);
            root = balance(root);
        }
    }
    return root;
}

template<typename T,typename V>
void AVLTree<T,V>::insert(T key,V data){
    Node<T,V>* currNode = searchHelper(root,key);
    if(currNode != nullptr){
        currNode->data = data;
        return;
    }
    else{
        root = insertionHelper(root,key,data);
        noOfKeys++;
        //cout<<"Inserted "<<x<<endl;
    }
}

template<typename T,typename V>
Node<T,V>* AVLTree<T,V>::insertionHelper(Node<T,V>* root,T key,V data)
{
    if(root == nullptr){
        root = new Node<T,V>(key,data); // ---------------check this later ----------------
        return root;
    }
    else if(key<root->key){
        //insert left
        root->left = insertionHelper(root->left,key,data);
        root = balance(root);
    }
    else if(key>root->key){
        //insert right
        root->right = insertionHelper(root->right,key,data);
        root = balance(root);
    }
    return root;
}

template<typename T,typename V>
int AVLTree<T,V>::balFactor(Node<T,V>* root){
    int hl = height(root->left);
    int hr = height(root->right);
    return hl-hr;
}

template<typename T,typename V>
Node<T,V>* AVLTree<T,V>::balance(Node<T,V>* root){
    int bf = balFactor(root);
    if(bf>1){
        if(balFactor(root->left)>0){
            root = llRotation(root);
        }
        else{
            root = lrRotation(root);
        }
    }
    else if(bf<-1){
        if(balFactor(root->right)>0){
            root = rlRotation(root);
        }
        else{
            root = rrRotation(root);
        }
    }
    return root;
}

template<typename T,typename V>
Node<T,V>* AVLTree<T,V>::rrRotation(Node<T,V>* root){
    //cout<<"rr is called with root as "<<root->data<<endl;
    Node<T,V>* temp = root->right;
    root->right = temp->left;
    temp->left = root;
    return temp;
}

template<typename T,typename V>
Node<T,V>* AVLTree<T,V>::rlRotation(Node<T,V>* root){
    //cout<<"rl is called with root as "<<root->data<<endl;
    Node<T,V>* temp1 = root->right->left;
    Node<T,V>* temp2 = root->right;
    temp2->left = temp1->right;
    root->right = temp1->left;
    temp1->left = root;
    temp1->right = temp2;
    return temp1;
}

template<typename T,typename V>
Node<T,V>* AVLTree<T,V>::llRotation(Node<T,V>* root){
    //cout<<"ll is called with root as "<<root->data<<endl;
    Node<T,V>* temp = root->left;
    root->left = temp->right;
    temp->right = root;
    return temp;
}

template<typename T,typename V>
Node<T,V>* AVLTree<T,V>::lrRotation(Node<T,V>* root){
    //cout<<"lr is called with root as "<<root->data<<endl;
    Node<T,V>* temp1 = root->left->right; // have to return this finally
    Node<T,V>* temp2 = root->left;
    temp2->right = temp1->left;
    root->left = temp1->right;
    temp1->left = temp2;
    temp1->right = root;
    return temp1;
}

int max(int a,int b){
    if(a>b)
        return a;
    return b;
}

template<typename T,typename V>
int AVLTree<T,V>::height(Node<T,V>* root){
    if(root == nullptr)
        return 0;
    int hr = height(root->right);
    int hl = height(root->left);
    return 1+max(hr,hl);
}

template<typename T,typename V>
void AVLTree<T,V>::printInorderHelper(Node<T,V>* root){
    if(root == nullptr){
        return;
    }
    else{
        printInorderHelper(root->left);
        //cout<<root->data<<":"<<predValue<<" ";
        cout<<root->key<<":"<<root->data<<" ";
        printInorderHelper(root->right);
    }
}
template<typename T,typename V>
void AVLTree<T,V>::printInorder(){
    printInorderHelper(root);
}

template<typename T,typename V>
void AVLTree<T,V>::clearHelper(Node<T,V>* root){
    if(root == nullptr){
        return;
    }
    else{
        clearHelper(root->left);
        clearHelper(root->right);
        free(root);
    }
}
template<typename T,typename V>
void AVLTree<T,V>::clear(){
    Node<T,V>* temp = root;
    clearHelper(temp);
    root = nullptr;
}

template<typename T,typename V>
V& AVLTree<T,V>::operator[](T key){
    Node<T,V>* currNode = searchHelper(root,key);
    if(currNode != nullptr){ // key already present
        return currNode->data;
    }
    else{
        //Node<T,V>* newNode = new Node<T,V>;
        //newNode->key = key;
        //insert(newNode->key,newNode->data);
        insert(key,{});
        Node<T,V>* insertedNode = searchHelper(root,key);
        return insertedNode->data;
    }
}

int main(){
    AVLTree<float,string> a = AVLTree<float,string>();
    int n = 10;
    for (int i = 1; i <= n; i ++) a.insert(i+0.2,to_string(i));
    a.printInorder();
    cout << endl;
    for (int i = 1; i <= n; i +=2) a.insert(i,to_string(i*2));
    a.printInorder();
    cout << endl;
    cout<<"size "<<a.size()<<endl;
    for(int i=1; i<=n; i++)
        cout<<a.find(i)<<" ";
    cout<<endl;

    for(int i=1; i<=n; i+=3)
        a.erase(i);

    a.printInorder();
    cout<<"size "<<a.size()<<endl;

    a.clear();
    cout<<"cleared a"<<endl;
    a.printInorder();

    a[1] = "hello";
    a[1] = "hello updated";
    a[2] = "hello world 2";
    a.printInorder();
}
