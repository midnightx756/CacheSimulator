#include <iostream>
#include "cache.h"
#include "Hash_Map.h"
#include "dll.h"
#include "queue.h"
#include "ll.h"
#include "min_heap.cpp"

using namespace std;

class Policies{
    int CacheSize;//Cache Memory size

    ll<CacheAddress>*  l1;
    queue<CacheAddress>* q;
    dll<CacheAddress>* dl;
    HashMap<CacheAddress, CacheLine>*  h;

    //Hash Map for doubly linked list to calculate the recency of the node
    HashMap<CacheAddress, node<CacheAddress>* > hdll;

    //Hash Map for min heap to calculate usage frequency
    HashMap<CacheAddress, int> hmh;

    struct minheap_type{// For simulating cache create a structure for efficient tracking
        CacheAddress val;
        int frequency;// for keeping track of usage
        int ind;// for keeping track of index;
    public:
        minheap_type(){
            frequency = 1;
            ind = 0;
        }
        minheap_type(CacheAddress data){
            val = data;
            frequency = 1;
            ind = 0;
        }
        // Custom operator overload for the Min Heap to correctly compare priorities
        bool operator<(minheap_type& other){
            return frequency > other.frequency; // Min Heap: Lower frequency means HIGHER priority
        }
    };

    MinHeap<minheap_type> hp;

public:
    Policies(int s){
        CacheSize = s;
        q = new queue<CacheAddress>(s);
        dl = new dll<CacheAddress>(s);
        l1= nullptr;
    }

    void LRU(CacheAddress ad, CacheLine l){
        //Does Least Recently Used
        CacheAddress val;
        CacheLine ll;
        if(!isFull(dl)){
            if(!h ->contains_key(val)){
                insertHead(dl, val);
            }
            else{
                insertHead(dl, val);
                h -> insertKey_Val(val, l);
            }
        }
        if(isFull(dl)){
            h -> deleteKey(dl-> tail -> data);
            dl -> tail -> data = val;
            insertAtHeadFromTail(dl);
        }
    }

    void LFU_NonHybrid(CacheAddress data, CacheLine l){
            minheap_type d(data);
            d.ind = hp.size();

            if(hmh.contains_key(data)){
                hp[hmh.get_val(data)].frequency++;
                hp.update_node_position(hmh.get_val(data));
            }
            else if(d.ind != CacheSize && !hmh.contains_key(data)){
                hmh.insertKey_Val(data, hmh.get_val(data));
                hp.push(d);
            }
            else{
                d = hp.top();
                hp.pop();
                hmh.deleteKey(d.val);
            }
    }


    void FIFO(CacheAddress ad, CacheLine l){
        if(!in(l1, ad) && !isFull(q)){
            enqueue(q,ad);
            insertHeadll(l1, ad);
        }
        else if(isFull(q) && !in(l1, ad)){
            deleteNode(l1, Front(q));
            dequeue(q);
            enqueue(q, ad);
            insertHeadll(l1, ad);
        }
        else{
            //hit rate
        }
    }
};
