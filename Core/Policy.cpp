#include <iostream>
#include "cache.h"
#include "Hash_Map.h"
#include "dll.h"
#include "queue.cpp"
#include "min_heap.cpp"

class Policies{

    dll<CacheAddress>* dl;
    HashMap<CacheAddress, CacheLine>*  h;

    //Hash Map for doubly linked list to calculate the recency of the node
    HashMap<CacheAddress, node*> hdll;

    //Hash Map for min heap to calculate usage frequency
    HashMap<ChaceAddress, int> hmh;

    queue<CacheAddress>* q;

    struct minheap_type{// For simulating cache create a structure for efficient tracking
        CacheAddress val;
        int frequency;// for keeping track of usage
        int heap_ind;// for keeping track of index;
    public:
        minheap_type(){
            frequency = 1;
            heap_ind = 0;
        }
        minheap_type(CacheAddress data){
            val = data;
            frequency = 1;
            heap_ind = 0;
        }
        // Custom operator overload for the Min Heap to correctly compare priorities
        bool operator<(const minheap_type& other) const {
            return frequency > other.frequency; // Min Heap: Lower frequency means HIGHER priority
    };

    MinHeap<minheap_type> hp;
public:
    void LRU(CacheAddress ad){
        //Does Least Recently Used
        CacheAddress val;
        CacheLine ll;
        if(!isFull(dl)){
            if(!h ->contains_key(val)){
                insertHead(dl, val);
            }
            else{
                insertHead(dl, val);
                h -> insertKey_Val(val, ll);
            }
        }
        if(isFull(dl)){
           evict();
        }
    }

    void LFU(CacheAddress data){
            minheap_type d(data);
            d.ind = hp.size();
            hp.push(d);
            if(!hmh.contains_key(data))
                insertKey_Val(data, d.ind);
            else if(hmh.contains_key(data)){
                    hp[hmp.getval(data)].frequency++;
            }
    }

    void evict(){
        //Used to delete the the memory block from both LFU and LRU Hybrid;
        h -> deleteKey(dl-> tail -> data);
        dl -> tail -> data = val;
        insertAtHeadFromTail(dl);
    }

    void FIFO(){
        CacheAddress *ad;
        CacheLine *ll;
        if(!h -> contains_key(*ad) && !isFull(q)){
            enqueue(q,*ad);
            h -> insertKey_Val(*ad, *ll );
        }
        else if(isFull(q) && !h -> contains_key(*ad)){
            dequeue(q);
            h -> insertKey_Val(*ad, *ll);
            enqueue(q, *ad);
        }
        else{
            //hit rate
        }
    }
};
