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
	list<pair<int, T>>* table;
	const float max_load_factor = 0.75;
	string strategy = "mul"; // or "mul"
public:
	bool isEmpty() const;
	int hashfunction(int key);
	void insert(int key, T val);
	void remove(int key);
	T search(int key);
	void printtable();
	void rehash();
	float find_load_factor();
	Hash_Table() {
		table = new list<pair<int, T>>[hash_groups];
	}
	~Hash_Table() {  // always call destructor to avoid memory leak for pointers
		delete[] table;
	}
};

template <typename T>
bool Hash_Table<T>::isEmpty() const{
	int sum=0;
	for(int i=0;i<hash_groups;i++){
		sum += table[i].size();
	}
	return (sum==0);
}

template <typename T>
int Hash_Table<T>::hashfunction(int key){
	if(strategy=="mod")
		return key % hash_groups;
	else if(strategy=="mul"){
		long long a = key;
		long long b = (1ll<<32)/(1.6180339887);
		a = (a*b) % (1ll<<32);
		long long shift = 32 - static_cast<long long>(log2(hash_groups));
		a = a >> shift;
		return a;
	}
	else
		return -1;
}

template <typename T>
void Hash_Table<T>::insert(int key, T val){
	int has_val = hashfunction(key);
	auto &cell = table[has_val];
	auto b_itr = begin(cell);
	bool key_exists = false;
	for(b_itr=begin(cell); b_itr!=end(cell);b_itr++){
		if(b_itr->first == key){
			key_exists = true;
			b_itr->second = val;
			cout << "[WARNING] Key exists. Value replaced." << endl;
			break;
		}
	}
	if(!key_exists){
		cell.push_back({key,val});
	}
	float load_factor = find_load_factor();
	if(load_factor>max_load_factor) rehash();
	return;
}

template <typename T>
void Hash_Table<T>::remove(int key){
	int has_val = hashfunction(key);
	auto &cell = table[has_val];
	auto b_itr = begin(cell);
	bool key_exists = false;
	for(b_itr=begin(cell); b_itr!=end(cell);b_itr++){
		if(b_itr->first == key){
			key_exists = true;
			b_itr = cell.erase(b_itr);
			cout << "[INFO] Pair removed." << endl;
			break;
		}
	}
	if(!key_exists){
		cout << "[WARNING] Key not found. Pair not removed." << endl;
	}
	return;
}

template <typename T>
T Hash_Table<T>::search(int key){
	int hash_val = hashfunction(key);
	auto &cell = table[hash_val];
	auto b_itr = begin(cell);
	bool key_exists = false;
	for(b_itr=begin(cell); b_itr!=end(cell);b_itr++){
		if(b_itr->first == key){
			key_exists = true;
			return b_itr->second;
		}
	}
	return T();
}

template <typename T>
void Hash_Table<T>::printtable(){
	for(int i=0;i<hash_groups;i++){
		if(table[i].size()==0) continue;
		auto b_itr = table[i].begin();
		for(b_itr=table[i].begin();b_itr!=table[i].end();b_itr++){
			cout << "[INFO] Key: " << b_itr->first << " Value: " << b_itr->second << endl;
		}
	}
	return;
}

template <typename T>
float Hash_Table<T>::find_load_factor(){
	int total_elements = 0;
	for(int i=0;i<hash_groups;i++){
		total_elements += table[i].size();
	}

	return (float)total_elements / hash_groups;
}

template <typename T>
void Hash_Table<T>::rehash(){
	cout<<"REHASHING"<<endl;
	float load_factor = find_load_factor();
	if(load_factor > max_load_factor){
		int new_hash_groups = hash_groups*2;
		hash_groups = new_hash_groups;
		list<pair<int, T>>* new_table = new list<pair<int, T>>[hash_groups];
		for(int i=0;i<hash_groups/2;i++){
			for(auto itr = table[i].begin(); itr!=table[i].end(); itr++){
				int new_hash = hashfunction(itr->first);
				new_table[new_hash].push_back({itr->first, itr->second});
			}
		}
		delete[] table;
		table = new_table;
	}
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