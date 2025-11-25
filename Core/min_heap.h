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

        int  heapifyup(int i){
            while(i > 0 && heap[parent(i)] > heap[i]){
                swap(heap[parent(i)], heap[i]);
                i = parent(i);
            }
            return i;
        }

    int heapifydown(int i) {
        int s = i; 
        while (true) {
            size_t l = left(i);
            size_t r = right(i);
            s = i;
            if (l < heap.size() && heap[l].frequency < heap[s].frequency) {
                s = l;
            }

            if (r < heap.size() && heap[r].frequency < heap[s].frequency) {
                s = r;
            }

            if (s != i) {
                swap(heap[i], heap[s]);
                i = s;
            } else {
                break;
            }
        }
        return i;
    }
public:
        int push(const T& value){
            heap.push_back(value);
            return heapifyup(heap.size() - 1);
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

        bool empty()const{
            return heap.empty();
        }

        size_t size() const{
            return heap.size();
        }

        int update_node_position(int index) {
            int r = heapifyup(index);
            return heapifydown(index);
        }

        T& operator[](int index){
            return heap[index];
        }

        const T& operator[](int index)const{
            return heap[index];
        }
};
