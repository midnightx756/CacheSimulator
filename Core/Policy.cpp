#include <iostream>
#include "cache.h"
#include "Hash_Map.h"
#include "dll.h"
#include "queue.h"
#include "min_heap.cpp"

using namespce std;
class Policies{
    int CacheSize;//Cache Memory size

     queue<CacheAddress>* q;
    dll<CacheAddress>* dl;
    HashMap<CacheAddress, CacheLine>*  h;

    //Hash Map for doubly linked list to calculate the recency of the node
    HashMap<CacheAddress, node*> hdll;

    //Hash Map for min heap to calculate usage frequency
    HashMap<ChaceAddress, int> hmh;


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
    Policies(int s){
        q = new queue(CacheSize);
        dl = new dll(CacheSize);
    }
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
            h -> deleteKey(dl-> tail -> data);
            dl -> tail -> data = val;
            insertAtHeadFromTail(dl);
        }
    }

    void LFU_NonHybrid(CacheAddress data){
            minheap_type d(data);
            d.ind = hp.size();

            if(if d.ind != CacheSize){
                if(!hmh.contains_key(data)){
                    insertKey_Val(data, d.ind);
                    hp.push(d);
                }
                else if(hmh.contains_key(data)){
                    hp[hmp.getval(data)].frequency++;
                    hp.update_node_position(hmp.getval(data));
                }
            }
            else{
                d = hp.top();
                pop();
                hmh.deleteKey(d);
            }
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
