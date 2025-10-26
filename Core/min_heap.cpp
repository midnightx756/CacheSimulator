#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;
template <typename T>
class MinHeap{
        vector<T>heap;

        int parent(int i){
            return (i-1)/2;
        }

        int left(int i){
            return 2*i + 1;
        }

        int right(int i){
            return 2*i + 2;
        }

        void heapifyup(int i){
            while(i > 0 && heap[parent(i)] > heap[i]){
                swap(heap[parent(i)], heap[i]);
                i = parent(i);
            }
        }

        void heapifydown(int i){
            int l = left(i);
            int r = right(i);
            int s = i;

            if(l < heap.size() && heap[l] < heap[s]){
                s = l;
            }

            if(r < heap.size() && heap[r] < heap[s]){
                s = r;
            }

            if(s != i){
                swap(heap[i], heap[s]);
                heapifydown(s);
            }
        }
public:
        void push(const T& value){
            heap.push_back(value);
            heapifyup(heap.size() - 1);
        }

        const T& top() const{
            if(empty()){
                throw out_of_range("Heap is empty.");
            }
            return heap.front();
        }

        void pop(){
            if(empty()){
                return;
            }
            if(heap.size() == 1){
                heap.pop_back();
                return;
            }
            swap(heap[0], heap.back());
            heap.pop_back();
            heapifydown(0);
        }

        bool empty(){
            return heap.empty();
        }

        size_t size() const{
            return heap.size();
        }

        void update_node_position(int index) {
            heapifyup(index);
            heapifydown(index);
        }

        const T& operator[](int index){
            return heap[index];
        }
};
