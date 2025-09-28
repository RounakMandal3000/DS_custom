#include <iostream>
#include <list>
#include <string>
#include <math.h>
#include <bits/stdc++.h>
using namespace std;

template <typename T>
class Hash_Table{
private:
	int hash_groups = 16;
	pair<int, T>* table;
	bool* occupied;
	bool* deleted;
	const float max_load_factor = 0.75;
	string probing_strategy = "linear"; // or "mul"
public:
	bool isEmpty() const;
	int hashfunction(int key, int i);
	void insert(int key, T val);
	void remove(int key);
	T search(int key);
	void printtable();
	void rehash();
	float find_load_factor();
	Hash_Table() {
		table = new pair<int, T>[hash_groups];
		occupied = new bool[hash_groups];
		deleted = new bool[hash_groups];
		for(int i=0;i<hash_groups;i++){
			occupied[i] = false;
			deleted[i] = false;
		}
	}
	~Hash_Table() {  // always call destructor to avoid memory leak for pointers
		delete[] table;
		delete[] occupied;
		delete[] deleted;
	}
};

template <typename T>
bool Hash_Table<T>::isEmpty() const{
	int sum=0;
	for(int i=0;i<hash_groups;i++){
		sum += (occupied[i]);
	}
	return (sum==0);
}

template <typename T>
int Hash_Table<T>::hashfunction(int key, int i){
	if(probing_strategy=="linear"){
		return (key + i) % hash_groups;
	}
	else if(probing_strategy=="quadratic"){
		int hash = (key + i*i) % hash_groups;
		return hash;
	}
	else if(probing_strategy=="double"){
		int hash1 = key % hash_groups;
		int hash2 = 1 + key % (hash_groups - 1);
		return (hash1 + i*hash2) % hash_groups;
	}
	else
		return -1;
}

template <typename T>
void Hash_Table<T>::insert(int key, T val){
	for (int i = 0; i < hash_groups; i++) {
		int probe = hashfunction(key, i+1);
		if (occupied[probe] && table[probe].first == key) {
			table[probe].second = val; // Update existing key
			cout << "[INFO] Key updated" << endl;
			return;
		}
		if (!(occupied[probe] && !deleted[probe])) {
			occupied[probe] = true;
			deleted[probe] = false;
			table[probe] = make_pair(key, val);
			cout << "[INFO] Key inserted" << endl;
			float load_factor = find_load_factor();
			if(load_factor > max_load_factor){
				rehash();
			}
			return;
		}
	}
	
	cout<<"HASH TABLE FULL"<<endl;
	return;
}

template <typename T>
void Hash_Table<T>::remove(int key){
	for (int i = 0; i < hash_groups; i++) {
		int probe = hashfunction(key, i+1);
		if (!occupied[probe] && !deleted[probe]) {
			cout << "[WARNING] Key not found" << endl;
			return;
		}
		if (occupied[probe] && table[probe].first == key) {
			deleted[probe] = true;
			occupied[probe] = false;
			cout << "[INFO] Key removed" << endl;
			return;
		}
	}
	cout<<"Key not found"<<endl;
	return;
}

template <typename T>
T Hash_Table<T>::search(int key){
	for (int i = 0; i < hash_groups; i++) {
		int probe = hashfunction(key, i+1);
		if (!occupied[probe] && !deleted[probe]) {
			return T(); // Key not found
		}
		if (occupied[probe] && table[probe].first == key) {
			return table[probe].second; // Key found
		}
	}
	return T(); // Key not found after full probe
}

template <typename T>
void Hash_Table<T>::printtable(){
	for(int i=0;i<hash_groups;i++){
		if(!occupied[i]) continue;
		cout << "[INFO] Key: " << table[i].first << " Value: " << table[i].second << endl;
	}
	return;
}

template <typename T>
float Hash_Table<T>::find_load_factor(){
	int total_elements = 0;
	for(int i=0;i<hash_groups;i++){
		total_elements += occupied[i];
	}

	return (float)total_elements / hash_groups;
}

template <typename T>
void Hash_Table<T>::rehash(){
	cout<<"REHASHING -> ";
	float load_factor = find_load_factor();
	int new_hash_groups = hash_groups*2;
	hash_groups = new_hash_groups;
	cout<<load_factor<<endl;
	pair<int, T>* new_table = new pair<int, T>[hash_groups];
	bool* new_deleted = new bool[hash_groups];
	bool* new_occupied = new bool[hash_groups];
	for(int i=0;i<hash_groups;i++){
		new_occupied[i] = false;
		new_deleted[i] = false;
	}

	for(int i=0;i<hash_groups/2;i++){
		int j=0;
		int key = table[i].first;
		T val = table[i].second;
		int hash_val = hashfunction(key, j+1);
		while(new_occupied[hash_val] && !new_deleted[hash_val]){
			j++;
			hash_val = hashfunction(key, j+1);
			if(!(new_occupied[hash_val] && !new_deleted[hash_val])){
				new_occupied[hash_val] = true;
				new_deleted[hash_val] = false;
				new_table[hash_val] = make_pair(key, val);
				break;
			}
		}
	}
	delete[] table;
	delete[] occupied;
	delete[] deleted;
	occupied = new_occupied;
	deleted = new_deleted;
	table = new_table;
}

int main() {
    cout << "--- Test Case 1: Basic insert and print ---\n";
    Hash_Table<string> ht;
    ht.insert(1, "One");
    ht.insert(2, "Two");
    ht.insert(3, "Three");
    ht.printtable();

    cout << "\n--- Test Case 2: Insert duplicate key ---\n";
    ht.insert(2, "Two Updated");
    ht.printtable();

    cout << "\n--- Test Case 3: Remove existing key ---\n";
    ht.remove(2);
    ht.printtable();

    cout << "\n--- Test Case 4: Remove non-existent key ---\n";
    ht.remove(5);

    cout << "\n--- Test Case 5: Search existing key ---\n";
    cout << ht.search(1) << endl;

    cout << "\n--- Test Case 6: Search non-existent key ---\n";
    cout << ht.search(4) << endl;

    cout << "\n--- Test Case 7: Insert enough keys to trigger rehash ---\n";
    for (int i = 4; i <= 30; i++) {
        ht.insert(i, "Val" + to_string(i));
    }
    cout << "Load factor after inserts: " << ht.find_load_factor() << endl;
    ht.printtable();

    cout << "\n--- Test Case 8: Check isEmpty() ---\n";
    Hash_Table<int> empty_ht;
    cout << (empty_ht.isEmpty() ? "Empty" : "Not empty") << endl;
    empty_ht.insert(10, 100);
    cout << (empty_ht.isEmpty() ? "Empty" : "Not empty") << endl;

    return 0;
}