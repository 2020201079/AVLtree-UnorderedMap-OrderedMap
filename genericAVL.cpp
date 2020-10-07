#include<iostream>
#include<string>

using namespace std;

class Student{
    public:
        string name;
        int age;
        Student(){}; // should have a default constructor
        Student(string name,int age){
            this->name=name;
            this->age=age;
        }
        bool operator<(const Student& s) { // must overload < and >
            return age<s.age;
        }
        bool operator>(const Student& s) {
            return age>s.age;
        }
};
// must overload << operator for new class
std::ostream &operator<<(std::ostream &os, Student const &s) { 
    return os <<s.name<<" "<<s.age;
}

template<typename T>
class Greater{
    public:
    bool operator() ( T& x, T& y){
        return x>y;
    }
};

template<typename T>
class Less{
    public:
    bool operator() ( T& x, T& y){
        return (x<y);
    }
};

template<typename T>
class Node{
    public:
        Node* left;
        Node* right;
        T data;
        int count;
        Node(T data){
            this->left = nullptr;
            this->right = nullptr;
            this->data = data;
            this->count = 1; 
        }
};

template<typename T>
Node<T>* getPredecessor(Node<T>* root){
    if(root == nullptr){
        throw runtime_error("called getPredecessor on a null ptr ");
        return nullptr;
    }
    else if(root->left == nullptr)
        return nullptr;
    else{
        Node<T>* temp = root->left;
        while(temp->right != nullptr)
            temp = temp->right;
        return temp;
    }
}

template<typename T>
Node<T>* getSuccessor(Node<T>* root){
    if(root == nullptr){
        throw runtime_error("called getSuccessor on a null ptr ");
        return nullptr;
    }
    else if(root->right == nullptr)
        return nullptr;
    else{
        Node<T>* temp = root->right;
        while(temp->left != nullptr)
            temp = temp->left;
        return temp;
    }
}

template<typename T,typename Comp=Less<T>>
class AVLTree{
    private:
        Comp comp; 
        Node<T>* llRotation(Node<T>* root);
        Node<T>* lrRotation(Node<T>* root);
        Node<T>* rrRotation(Node<T>* root);
        Node<T>* rlRotation(Node<T>* root);
        Node<T>* insertionHelper(Node<T>* root,T x);
        Node<T>* delHelper(Node<T>* root,T x);
        void printInorderHelper(Node<T>* root);
        int balFactor(Node<T>* root);
        Node<T>* balance(Node<T>*root); // passing the element which is just inserted
        Node<T>* searchHelper(Node<T>* root,T x);
        void upperBoundHelper(Node<T>* root,T x,Node<T>* &ub);
    public:
        Node<T>* root;
        AVLTree(){
            root = nullptr;
        }
        void insert(T x); // implement it later just define all methods here
        void del(T x);
        bool search(T x);
        int countOccurrences(T x);
        T lowerBound(T x);
        T upperBound(T x);
        T closestElement(T x);
        T kthLargestElement(T k);
        int numberOfElementsInRange(T a,T b);
        void printInorder();
        int height(Node<T>* root);
};

template<typename T,typename Comp>
T AVLTree<T,Comp>::lowerBound(T x){
    Node<T>* lb = searchHelper(root,x);
    if(lb == nullptr){
        return upperBound(x);
    }
    else
        return lb->data;
}

template<typename T,typename Comp>
void AVLTree<T,Comp>::upperBoundHelper(Node<T>* root,T x,Node<T>* &ub){
    if(root==nullptr){
        return;
    }
    else if(comp(x,root->data)){
        ub=root;
        upperBoundHelper(root->left,x,ub);
    }
    else{
        upperBoundHelper(root->right,x,ub);
    }
}

template<typename T,typename Comp>
T AVLTree<T,Comp>::upperBound(T x){
    Node<T>* ub = nullptr;
    upperBoundHelper(root,x,ub);
    if(ub==nullptr){
        cout<<"No upper bound "<<endl;
        return -1;
    }
    else{
        return ub->data;
    }
}

template<typename T,typename Comp>
int AVLTree<T,Comp>::countOccurrences(T x){
    Node<T>* currNode = searchHelper(root,x);
    if(currNode == nullptr)
        return 0;
    else
        return currNode->count;
}

template<typename T,typename Comp>
Node<T>* AVLTree<T,Comp>::searchHelper(Node<T>* root,T x){
    if(root == nullptr){
        return nullptr;
    }
    else if(comp(x,root->data)){
        return searchHelper(root->left,x);
    }
    else if(comp(root->data,x)){
        return searchHelper(root->right,x);
    }
    return root;
}
template<typename T,typename Comp>
bool AVLTree<T,Comp>::search(T x){
    Node<T>* result= searchHelper(root,x);
    if(result==nullptr)
        return false;
    return true;
}

template<typename T,typename Comp>
void AVLTree<T,Comp>::del(T x){
    Node<T>* currNode = searchHelper(root,x);
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
template<typename T,typename Comp>
Node<T>* AVLTree<T,Comp>::delHelper(Node<T>* root,T x){
    if(root == nullptr){
        cout<<x<<" does not exists in tree"<<endl;
        return root;
    }
    //else if(x > root->data){
    else if(comp(root->data,x)){
        //cout<<" right "<<endl;
        root->right = delHelper(root->right,x);
        root = balance(root);
    }
    //else if(x < root->data){
    else if(comp(x,root->data)){
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
            Node<T>* temp = root->left;
            free(root);
            return temp;
        }
        else if(root->right != nullptr && root->left == nullptr){
            Node<T>* temp = root->right;
            free(root);
            return temp;
        }
        else{ // case where both children are present
            Node<T>* pred = getPredecessor(root);
            root->data = pred->data;
            root->left = delHelper(root->left,pred->data);
            root = balance(root);
        }
    }
    return root;
}

template<typename T,typename Comp>
void AVLTree<T,Comp>::insert(T x){
    Node<T>* currNode = searchHelper(root,x);
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
template<typename T,typename Comp>
Node<T>* AVLTree<T,Comp>::insertionHelper(Node<T>* root,T x)
{
    if(root == nullptr){
        root = new Node<T>(x);
        return root;
    }
    //else if(x<root->data){
    else if(comp(x,root->data)){
        //insert left
        root->left = insertionHelper(root->left,x);
        root = balance(root);
    }
    //else if(x>root->data){
    else if(comp(root->data,x)){
        //insert right
        root->right = insertionHelper(root->right,x);
        root = balance(root);
    }
    return root;
}

template<typename T,typename Comp>
int AVLTree<T,Comp>::balFactor(Node<T>* root){
    int hl = height(root->left);
    int hr = height(root->right);
    return hl-hr;
}

template<typename T,typename Comp>
Node<T>* AVLTree<T,Comp>::balance(Node<T>* root){
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

template<typename T,typename Comp>
Node<T>* AVLTree<T,Comp>::rrRotation(Node<T>* root){
    //cout<<"rr is called with root as "<<root->data<<endl;
    Node<T>* temp = root->right;
    root->right = temp->left;
    temp->left = root;
    return temp;
}

template<typename T,typename Comp>
Node<T>* AVLTree<T,Comp>::rlRotation(Node<T>* root){
    //cout<<"rl is called with root as "<<root->data<<endl;
    Node<T>* temp1 = root->right->left;
    Node<T>* temp2 = root->right;
    temp2->left = temp1->right;
    root->right = temp1->left;
    temp1->left = root;
    temp1->right = temp2;
    return temp1;
}

template<typename T,typename Comp>
Node<T>* AVLTree<T,Comp>::llRotation(Node<T>* root){
    //cout<<"ll is called with root as "<<root->data<<endl;
    Node<T>* temp = root->left;
    root->left = temp->right;
    temp->right = root;
    return temp;
}

template<typename T,typename Comp>
Node<T>* AVLTree<T,Comp>::lrRotation(Node<T>* root){
    //cout<<"lr is called with root as "<<root->data<<endl;
    Node<T>* temp1 = root->left->right; // have to return this finally
    Node<T>* temp2 = root->left;
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

template<typename T,typename Comp>
int AVLTree<T,Comp>::height(Node<T>* root){
    if(root == nullptr)
        return 0;
    int hr = height(root->right);
    int hl = height(root->left);
    return 1+max(hr,hl);
}

template<typename T,typename Comp>
void AVLTree<T,Comp>::printInorderHelper(Node<T>* root){
    if(root == nullptr){
        return;
    }
    else{
        printInorderHelper(root->left);
        for(int i=0;i<root->count;i++)
            cout<<root->data<<" ";
        printInorderHelper(root->right);
    }
}
template<typename T,typename Comp>
void AVLTree<T,Comp>::printInorder(){
    printInorderHelper(root);
}

int main(){
/*    
    AVLTree<Student> a = AVLTree<Student>();
    a.insert(Student("varun",60));
    a.insert(Student("utkarsh",50));
    a.insert(Student("anchal",45));
    a.insert(Student("Pandu",25));
    a.insert(Student("Jaanu",24));
    a.insert(Student("Sireesha",70));
    a.insert(Student("Nishanth",55));
    a.insert(Student("Sachin",42));
*/
    AVLTree<string> a = AVLTree<string>();
    a.insert("Varun");
    a.insert("Utkarsh");
    a.insert("Anchal");
    a.insert("Pandu");
    a.insert("Jaanu");
    a.insert("Sireesha");
    a.insert("Nishanth");
    a.insert("Sachin");

    a.printInorder();
    cout<<endl;
    cout<<endl;
    
    AVLTree<float> treeFloat = AVLTree<float>();
    for(int i=1;i<11;i++){
        treeFloat.insert(i+i*0.2);
    }
    for(int i=1;i<11;i++){
        treeFloat.insert(i+i*0.2);
    }
    treeFloat.printInorder();
    cout<<endl;
    for(int i=1;i<11;i+=2){
        treeFloat.del(i+i*0.2);
    }
    treeFloat.printInorder();
    cout<<endl;
    cout<<"3 lb :"<< treeFloat.lowerBound(3)<<" ub :"<<treeFloat.upperBound(3)<<endl;
    cout<<"4 lb :"<< treeFloat.lowerBound(4)<<" ub :"<<treeFloat.upperBound(4)<<endl;
    cout<<"5 lb :"<< treeFloat.lowerBound(5)<<" ub :"<<treeFloat.upperBound(5)<<endl;
    cout<<"6 lb :"<< treeFloat.lowerBound(6)<<" ub :"<<treeFloat.upperBound(6)<<endl;
}