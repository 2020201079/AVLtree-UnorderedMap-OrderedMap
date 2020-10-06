#include<iostream>
#include<typeinfo>
#include<string>
#include<sstream>   

using namespace std;

#define TABLE_SIZE 257 // selected a prime number just bigger than 256. now hash function range from 0,1,...256
#define STRING_SIZE 20 // max string size can be 20

template<typename T,typename V>
class Node{
    public:
        T key;
        V data;
        Node<T,V>* next; // need for collisions and linked list.
        Node(T key,V data){
            this->key = key;
            this->data = data;
            this->next = nullptr;
        }
};

template<typename T,typename V>
class unorderedMap{
    private:
        int randInt[STRING_SIZE];
        int hashFunction(T key);
        Node<T,V>* arr[TABLE_SIZE]{nullptr};
        void insertInArray(int index,Node<T,V>* currNode);
    public:
        unorderedMap(){
            for(int i=0;i<STRING_SIZE;i++){
                randInt[i] = rand()%TABLE_SIZE; // saves rand integers for each instance of umap
            }
        }
        void insert(T key,V value);
        void erase(T key);
        bool find(T key);
        V map(T key);
        void print();
        V& operator[](T i);
};

template<typename T,typename V>
void unorderedMap<T,V>::erase(T key){
    int index=hashFunction(key);
    if(arr[index]==nullptr){
        cout<<key<<"is not present "<<endl;
        return;
    }
    else{
        arr[index] = deleteNode(arr[index],key);
    }
}

template<typename T,typename U>
Node<T,U>* deleteNode(Node<T,U>* root,T key){
    if(root->key == key ){
        auto par = root->next;
        free(root);
        return par;
    }
    auto par = root;
    auto temp = root->next;
    while(temp != nullptr){
        if(temp->key == key){
            par->next = temp->next;
            free(temp);
            break;
        }
        else{
            temp = temp->next;
            par = par->next;
        }
    }
    return root;
}

template<typename T,typename V>
bool unorderedMap<T,V>::find(T key){
    int index = hashFunction(key);
    if(arr[index] == nullptr)
        return false;
    return true;
}

template<typename T,typename V>
void unorderedMap<T,V>::print(){
    for(int i=0;i<TABLE_SIZE;i++){
        if(arr[i] != nullptr){
            Node<T,V>* temp= arr[i];
            while(temp!=nullptr){
                cout<<temp->key<<":"<<temp->data<<" ";
                temp = temp->next;
            }
            cout<<endl;
        }
    }
}

template<typename T,typename V>
void unorderedMap<T,V>::insertInArray(int index,Node<T,V>* currNode){
    if(arr[index] == nullptr){
        arr[index] = currNode;
        return;
    }
    else{
        auto temp = arr[index];
        while(temp->next != nullptr){
            temp=temp->next;
        }
        temp->next = currNode;
    }
}

template<typename T,typename V>
void unorderedMap<T,V>::insert(T key,V value){
    int index = hashFunction(key);
    Node<T,V>* node = new Node<T,V>(key,value);
    insertInArray(index,node);
    //cout<<"inserted "<<key<<":"<<value<<endl;
}

template<typename T,typename V>
int unorderedMap<T,V>::hashFunction(T key){
    ostringstream keyStream;
    keyStream<<key;
    string keyString = keyStream.str();
    if(keyString.size() > 20){
        cout<<"Key size should be less than 20";
        return -1;
    }
    int ans = 0;
    int i=0;
    for(char c: keyString){
        ans += c*randInt[i];
        ans = ans % TABLE_SIZE;
        i++;
    }
    return ans;
}

template<typename T,typename V>
V& unorderedMap<T,V>::operator[](T key){
    int index=hashFunction(key);
    if(arr[index] != nullptr){
        auto root = arr[index];
        while(root!= nullptr){
            if(root->key == key){
                return root->data;
            }
            root = root->next;
        }
    }
    throw runtime_error("Key does not exists");
    return arr[index]->data;
}

int main(){
    auto umap = unorderedMap<double,string>();
    umap.insert(2.34,"rupees");
    //umap.insert(4.560,"yen");
    umap.insert(4.5600,"rmb");
    umap.insert(7.32,"dollar");
    umap.insert(95.3,"pound");
    umap.print();

    cout<<"[4.56]"<<umap[4.56]<<endl;
    cout<<"[95.3]"<<umap[95.3]<<endl;
    cout<<"[1]"<<umap[1]<<endl;
     
}