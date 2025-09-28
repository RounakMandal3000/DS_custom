#include <iostream>
#include <vector>
using namespace std;

class Heap{
public:
	vector<int> heap;
	void Heapify(int i){
		int n=heap.size();
		int left = 2*i;
		int largest = i;
		if(left<=n && heap[largest-1]<heap[left-1]){
			largest = left;
		}
		int right = 2*i+1;
		if(right<=n && heap[largest-1]<heap[right-1]){
			largest = right;
		}
		if(largest!=i){
			swap(heap[i-1], heap[largest-1]);
			Heapify(largest);
		}
	}
	void buildheap(vector<int> A){
		int n = A.size();
		heap.resize(n);
		for(int i=0;i<n;i++) heap[i]=A[i];
		for(int i=n/2;i>=1;i--){
			Heapify(i);
		}
	}
	int pop(){
		if(heap.size()==0) return -1;
		int mx = heap[0];
		heap[0] = heap[heap.size()-1];
		heap.pop_back();
		Heapify(1);
		return mx;
	}
	int push(int key){
		heap.push_back(key);
		int i=heap.size();
		while(i>1 && heap[i/2 - 1]<key){
			heap[i-1]=heap[i/2 - 1];
			i/=2;
		}
		heap[i-1]=key;
	}
};
void testHeap() {
    Heap h;

    // Input array
    vector<int> arr = {3, 1, 6, 5, 2, 4};

    // Copy arr into h.heap before building
    h.buildheap(arr);

    cout << "Heap after buildheap: ";
    for (int x : h.heap) cout << x << " ";
    cout << endl;

    // Test push
    h.push(10);
    h.push(7);
    cout << "Heap after pushing 10 and 7: ";
    for (int x : h.heap) cout << x << " ";
    cout << endl;

    // Test pop
    cout << "Pop max: " << h.pop() << endl;
    cout << "Heap after pop: ";
    for (int x : h.heap) cout << x << " ";
    cout << endl;

    cout << "Pop max: " << h.pop() << endl;
    cout << "Heap after pop: ";
    for (int x : h.heap) cout << x << " ";
    cout << endl;
}

int main() {
    testHeap();
    return 0;
}
