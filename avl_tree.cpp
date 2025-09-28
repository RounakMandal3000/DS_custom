#include <iostream>
using namespace std;

template <typename T>
class AVLTree{
public:
    struct node{
        T data;
        node* left, * right;
        int balance_factor;
    };
    node* root;
    node* make_node(int val){
        node* new_node = new node;
        new_node->left = nullptr;
        new_node->right = nullptr;
        new_node->data=val;
        new_node->balance_factor = 0;
        return new_node;
    }
    AVLTree(){
        root = nullptr;
    }
    
    int height(node* t){
        if(t==nullptr) return 0;
        int left_height = height(t->left);
        int right_height = height(t->right);
        return 1 + max(left_height, right_height);
    }
    
    node* balance_right(node* t){
        if(t==nullptr) return NULL;
        node* left = t->left;
        t->left = left->right;
        left->right = t;
        return left;
    }
    node* balance_left(node* t){
        if(t==nullptr) return NULL;
        node* right = t->right;
        t->right = right->left;
        right->left = t;
        return right;
    }
    node* insert(node* root, T value){
        if(root==nullptr){
            node* new_node = make_node(value);
            root = new_node;
            return root;
        }
        if(root->data < value){
            root->right = insert(root->right, value);
        }
        else if(root->data > value){ // difference between set and multiset
            root->left = insert(root->left, value);
        }
        root->balance_factor = height(root->left) - height(root->right);
        if(root->balance_factor==2){
            if(root->left->balance_factor>=0){
                root = balance_right(root);
            }
            else if(root->left->balance_factor==-1){
                root->left = balance_left(root->left);
                root = balance_right(root);
            }
        }
        else if(root->balance_factor==-2){
            if(root->right->balance_factor<=0){
                root = balance_left(root);
            }
            else if(root->right->balance_factor==1){
                root->right = balance_right(root->right);
                root = balance_left(root);
            }
        }
        root->balance_factor = height(root->left) - height(root->right);
        return root;
    }
    node* erase(node* root, T val){
        if(root==nullptr) return root;
        if(root->data < val){
            root->right = erase(root->right, val);
        }
        else if(root->data > val){
            root->left = erase(root->left, val);
        }
        else{
            if(root->left==nullptr && root->right==nullptr){
                delete root;
                return nullptr;
            }
            else if(root->left==nullptr){
                node* temp = root->right;
                delete root;
                return temp;
            }
            else if(root->right==nullptr){
                node* temp = root->left;
                delete root;
                return temp;
            }
            else{
                node* temp = root->right;
                while(temp->left!=nullptr){
                    temp = temp->left;
                }
                root->data = temp->data;
                root->right = erase(root->right, temp->data);
            }
        }
        root->balance_factor = height(root->left) - height(root->right);
        if(root->balance_factor==2){
            cout<<root->balance_factor<<endl;
            cout<<root->left->balance_factor<<endl;
            if(root->left->balance_factor>=0){
                root = balance_right(root);
            }
            else if(root->left->balance_factor==-1){
                root->left = balance_left(root->left);
                root = balance_right(root);
            }
        }
        else if(root->balance_factor==-2){
            if(root->right->balance_factor<=0){
                root = balance_left(root);
            }
            else if(root->right->balance_factor==1){
                root->right = balance_right(root->right);
                root = balance_left(root);
            }
        }
        root->balance_factor = height(root->left) - height(root->right);
        return root;
    }
    void preOrder(node *root) {
        if (root != nullptr) {
            cout << root->data << " ";
            preOrder(root->left);
            preOrder(root->right);
        }
    }  
};

int main() {
    AVLTree<int> tree;

    // // Constructing tree given in the 
    // // above figure
    tree.root = tree.insert(tree.root, 9);
    tree.root = tree.insert(tree.root, 5);
    tree.root = tree.insert(tree.root, 10);
    tree.root = tree.insert(tree.root, 0);
    tree.root = tree.insert(tree.root, 6);
    tree.root = tree.insert(tree.root, 11);
    tree.root = tree.insert(tree.root, -1);
    tree.root = tree.insert(tree.root, 1);
    tree.root = tree.insert(tree.root, 2);

    cout << "Preorder traversal of the "
            "constructed AVL tree is \n";
    tree.preOrder(tree.root);
    cout<<endl;
    tree.root = tree.erase(tree.root, 10);

    cout << "\nPreorder traversal after"
            " deletion of 10 \n";
    tree.preOrder(tree.root);

    return 0;
}