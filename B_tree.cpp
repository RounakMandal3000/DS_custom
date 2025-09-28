#include <iostream>
#include <vector>
using namespace std;

#define M 2
template <typename T>
class B_Tree{
public:
	struct node{
		T keys[2*M-1];
		struct node* children[2*M];
		bool is_leaf;
		int num_keys;
	};
    node* root_;
    B_Tree(){
        root_ = nullptr;
    }
	node* createnode(){
		node* temp = new node;
		for(int i=0;i<2*M;i++) temp->children[i]=nullptr;
		temp->num_keys=0;
		temp->is_leaf=true;
		return temp;
	}
	node* search(node* root, T key){
		if(root==nullptr) return nullptr;
		int i;
		for(i=0;i<2*M-1;i++){
			if(root->keys[i]>=key) break;
		}
		if(i<root->num_keys && root->keys[i]==key) return root;
		if(root->is_leaf) return nullptr;
		return search(root->children[i], key);
	}

    
    void split_child(node* parent, int idx){
		node* child = parent->children[idx];
		node* new_child = new node;
        new_child->is_leaf = child->is_leaf;
		int mid = M-1;
		for(int i=0;i<mid;i++){
            new_child->keys[i] = child->keys[i+M];
            child->keys[i+M] = 0;
        }
        new_child->num_keys = M-1;
        child->num_keys = M-1;

        if(child->is_leaf == false){
            for(int i=0;i<M;i++){
                new_child->children[i] = child->children[i+M];
                child->children[i+M] = nullptr;
            }
        }
        for(int i=parent->num_keys;i>idx;i--){
            parent->children[i+1] = parent->children[i];
        }
        parent->children[idx+1] = new_child;
        for(int i=parent->num_keys-1;i>=idx;i--){
            parent->keys[i+1] = parent->keys[i];
        }
        parent->keys[idx] = child->keys[mid];
        child->keys[mid] = 0;
        parent->num_keys++;

	}
	void insert_non_full(node* root, int key){
        if(root->is_leaf){
            int i;
            for(i=root->num_keys-1;i>=0 && root->keys[i]>key;i--){
                root->keys[i+1] = root->keys[i];
            }
	        root->keys[i+1] = key;
            root->num_keys++;
        }
        else{
        	cout<<key<<" ->" ;
            int i=root->num_keys-1;
            while (i >= 0 && root->keys[i] > key){
                i--;
            }
            i++;
            if(root->children[i]->num_keys == 2*M-1){
            	cout<<root->keys[root->num_keys-1]<<endl;
                split_child(root, i);
                if (root->keys[i] < key){
                    i++;
                }
            }
            insert_non_full(root->children[i], key);

        }
    }
    void insert(node* root, T key){
        if(root == nullptr){
            root = createnode();
            root->keys[0] = key;
            root->num_keys = 1;
            root_ = root;
        }
        else if(root->num_keys == 2*M-1){
            node* new_root = createnode();
            new_root->is_leaf = false;
            new_root->children[0] = root;
            split_child(new_root, 0);
            int i = 0;
            if(new_root->keys[0] < key){
                i++;
            }

            insert_non_full(new_root->children[i], key);
            root_ = new_root;
        }
        else{
            insert_non_full(root, key);

        }
    }
    void traverse(node *root) {
        if (root != nullptr) {
            int i;
            if(!root->is_leaf){
        	    for(i=0;i<root->num_keys;i++){
	                traverse(root->children[i]);
            	}
        	}
            for (i = 0; i < root->num_keys; i++) {
                printf("%d ", root->keys[i]);
            }
            if(!root->is_leaf)
	            traverse(root->children[i]);
        }
    }
};
int main() {
    B_Tree<int> btree;

    vector<int> keys = {10, 20, 5, 6, 12, 30, 7, 17};
    for (int k : keys) {
        btree.insert(btree.root_, k);
    }
    printf("In-order traversal of the B-tree: ");
    btree.traverse(btree.root_);	
    printf("\n");

    return 0;
}