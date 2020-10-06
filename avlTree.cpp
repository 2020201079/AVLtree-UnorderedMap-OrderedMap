#include<iostream>

using namespace std;

class Node{
    public:
        Node* left;
        Node* right;
        int data;
        int count;
        Node(int data){
            this->left = nullptr;
            this->right = nullptr;
            this->data = data;
            this->count = 1; 
        }
};

Node* getPredecessor(Node* root){
    if(root == nullptr){
        throw runtime_error("called getPredecessor on a null ptr ");
        return nullptr;
    }
    else if(root->left == nullptr)
        return nullptr;
    else{
        Node* temp = root->left;
        while(temp->right != nullptr)
            temp = temp->right;
        return temp;
    }
}

Node* getSuccessor(Node* root){
    if(root == nullptr){
        throw runtime_error("called getSuccessor on a null ptr ");
        return nullptr;
    }
    else if(root->right == nullptr)
        return nullptr;
    else{
        Node* temp = root->right;
        while(temp->left != nullptr)
            temp = temp->left;
        return temp;
    }
}

class AVLTree{
    private:
        Node* llRotation(Node* root);
        Node* lrRotation(Node* root);
        Node* rrRotation(Node* root);
        Node* rlRotation(Node* root);
        Node* insertionHelper(Node* root,int x);
        Node* delHelper(Node* root,int x);
        void printInorderHelper(Node* root);
        int balFactor(Node* root);
        Node* balance(Node*root); // passing the element which is just inserted
        Node* searchHelper(Node* root,int x);
        void upperBoundHelper(Node* root,int x,Node* &ub);
    public:
        Node* root;
        AVLTree(){
            root = nullptr;
        }
        void insert(int x); // implement it later just define all methods here
        void del(int x);
        bool search(int x);
        int countOccurrences(int x);
        int lowerBound(int x);
        int upperBound(int x);
        int closestElement(int x);
        int kthLargestElement(int k);
        int numberOfElementsInRange(int a,int b);
        void printInorder();
        int height(Node* root);
};

int AVLTree::lowerBound(int x){
    Node* lb = searchHelper(root,x);
    if(lb == nullptr){
        return upperBound(x);
    }
    else
        return lb->data;
}

void AVLTree::upperBoundHelper(Node* root,int x,Node* &ub){
    if(root==nullptr){
        return;
    }
    else if(root->data>x){
        ub=root;
        upperBoundHelper(root->left,x,ub);
    }
    else{
        upperBoundHelper(root->right,x,ub);
    }
}
int AVLTree::upperBound(int x){
    Node* ub = nullptr;
    upperBoundHelper(root,x,ub);
    if(ub==nullptr){
        cout<<"No upper bound "<<endl;
        return -1;
    }
    else{
        return ub->data;
    }
}

int AVLTree::countOccurrences(int x){
    Node* currNode = searchHelper(root,x);
    if(currNode == nullptr)
        return 0;
    else
        return currNode->count;
}

Node* AVLTree::searchHelper(Node* root,int x){
    if(root == nullptr){
        return nullptr;
    }
    else if(x<root->data){
        return searchHelper(root->left,x);
    }
    else if(x>root->data){
        return searchHelper(root->right,x);
    }
    return root;
}
bool AVLTree::search(int x){
    Node* result= searchHelper(root,x);
    if(result==nullptr)
        return false;
    return true;
}

void AVLTree::del(int x){
    Node* currNode = searchHelper(root,x);
    if(currNode == nullptr){
        cout<<x<<" is not present in tree to delete"<<endl;
        return;
    }
    else if(currNode->count >1 ){
        currNode->count--;
        //cout<<"deleted one instance of "<<x<<endl;
    }
    else{
        root = delHelper(root,x);
        //cout<<"deleted"<<x<<endl;
    }
}
Node* AVLTree::delHelper(Node* root,int x){
    if(root == nullptr){
        cout<<x<<" does not exists in tree"<<endl;
        return root;
    }
    else if(x > root->data){
        //cout<<" right "<<endl;
        root->right = delHelper(root->right,x);
        root = balance(root);
    }
    else if(x < root->data){
        //cout<<" left "<<endl;
        root->left = delHelper(root->left,x);
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
            Node* temp = root->left;
            free(root);
            return temp;
        }
        else if(root->right != nullptr && root->left == nullptr){
            Node* temp = root->right;
            free(root);
            return temp;
        }
        else{ // case where both children are present
            Node* pred = getPredecessor(root);
            root->data = pred->data;
            root->left = delHelper(root->left,pred->data);
            root = balance(root);
        }
    }
    return root;
}

void AVLTree::insert(int x){
    Node* currNode = searchHelper(root,x);
    if(currNode != nullptr){
        currNode->count++;
        //cout<<"Incremented instance of "<<x<<endl;
        return;
    }
    else{
        root = insertionHelper(root,x);
        //cout<<"Inserted "<<x<<endl;
    }
}

Node* AVLTree::insertionHelper(Node* root,int x)
{
    if(root == nullptr){
        root = new Node(x);
        return root;
    }
    else if(x<root->data){
        //insert left
        root->left = insertionHelper(root->left,x);
        root = balance(root);
    }
    else if(x>root->data){
        //insert right
        root->right = insertionHelper(root->right,x);
        root = balance(root);
    }
    return root;
}

int AVLTree::balFactor(Node* root){
    int hl = height(root->left);
    int hr = height(root->right);
    return hl-hr;
}

Node* AVLTree::balance(Node* root){
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

Node* AVLTree::rrRotation(Node* root){
    //cout<<"rr is called with root as "<<root->data<<endl;
    Node* temp = root->right;
    root->right = temp->left;
    temp->left = root;
    return temp;
}

Node* AVLTree::rlRotation(Node* root){
    //cout<<"rl is called with root as "<<root->data<<endl;
    Node* temp1 = root->right->left;
    Node* temp2 = root->right;
    temp2->left = temp1->right;
    root->right = temp1->left;
    temp1->left = root;
    temp1->right = temp2;
    return temp1;
}

Node* AVLTree::llRotation(Node* root){
    //cout<<"ll is called with root as "<<root->data<<endl;
    Node* temp = root->left;
    root->left = temp->right;
    temp->right = root;
    return temp;
}

Node* AVLTree::lrRotation(Node* root){
    //cout<<"lr is called with root as "<<root->data<<endl;
    Node* temp1 = root->left->right; // have to return this finally
    Node* temp2 = root->left;
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

int AVLTree::height(Node* root){
    if(root == nullptr)
        return 0;
    int hr = height(root->right);
    int hl = height(root->left);
    return 1+max(hr,hl);
}

void AVLTree::printInorderHelper(Node* root){
    if(root == nullptr){
        return;
    }
    else{
        printInorderHelper(root->left);
        Node* pred = getPredecessor(root);
        int predValue;
        if(pred == nullptr){
            predValue = -1;
        }
        else{
            predValue = pred->data;
        }
        cout<<root->data<<":"<<predValue<<" ";
        printInorderHelper(root->right);
    }
}
void AVLTree::printInorder(){
    printInorderHelper(root);
}

int main(){
    AVLTree tree = AVLTree();
    tree.insert(50);tree.insert(40);tree.insert(60);
    tree.insert(45);tree.insert(50);tree.insert(50);
    tree.printInorder();
    cout<<"Height is : "<<tree.height(tree.root)<<endl;
    
    cout<<"42 ub:"<<tree.upperBound(42)<<" lb:"<<tree.lowerBound(42)<<endl;
    cout<<"50 ub:"<<tree.upperBound(50)<<" lb:"<<tree.lowerBound(50)<<endl;
    cout<<"30 ub:"<<tree.upperBound(30)<<" lb:"<<tree.lowerBound(30)<<endl;
    cout<<"45 ub:"<<tree.upperBound(45)<<" lb:"<<tree.lowerBound(45)<<endl;
    cout<<"53 ub:"<<tree.upperBound(53)<<" lb:"<<tree.lowerBound(53)<<endl;
}
