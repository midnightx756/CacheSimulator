#include <iostream>
#include "cache.cpp"
#include "Hash_Map.h"
#include "dll.cpp"
#include "queue.cpp"
#include "min_heap.cpp"

class Policies{
    dll* dl;
    HashMap hm;
public:
    void LRU(dll* dl, HashMap <CacheAddress, CacheLine>*h){
        //Does LRU for My simulator
        CacheAddress val;
        CacheLine ll;
        if(!isFull(dl)){
            if(h -> get_val(val) == ll){
                insertHead(dl, val);
            }
            else{
                insertHead(dl, val);
                h -> insertKey_Val(val, ll);
            }
        }
        if(isFull(dl)){
            h -> deleteKey(*(dl-> tail -> data));
            dl -> tail -> data = &val;
            insertAtHeadFromTail(dl);
        }
    }

    void FIFO(queue* q, HashMap <CacheAddress, CacheLine>* h){
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
