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
        unsigned int time;// To calculate time for LFU
    public:
        minheap_type(){
            frequency = 1;
            ind = 0;
            time = 0;
        }
        minheap_type(CacheAddress data){
            val = data;
            frequency = 1;
            ind = 0;
            time = 0;
        }
          minheap_type(CacheAddress data, unsigned int t){
            val = data;
            frequency = 1;
            ind = 0;
            time = t;
        }
        // Custom operator overload for the Min Heap to correctly compare priorities
        bool operator>(minheap_type& other){
            if(frequency != other.frequency){
            return frequency > other.frequency;
            } // Min Heap: Lower frequency means HIGHER priority

            return time > other.time;
        }
    };

    MinHeap<minheap_type> hp;
    unsigned int counter = 0;

public:
    Policies(int s){
        CacheSize = s;
        q = new queue<CacheAddress>(s);
        dl = new dll<CacheAddress>(s);
        l1= nullptr;
    }

    int LRU(CacheAddress ad, CacheLine l){
        node<CacheAddress>* chn;
       if(hdll.contains_key(ad)){
            chn = hdll.get_val(ad);
            if(dl -> head == dl -> tail)
                return 1;
            if(!chn -> next)
                insertAtHeadFromTail(dl);
            else if(chn -> next && chn -> prev){
                chn -> next -> prev = chn -> prev;
                chn -> prev -> next = chn -> next;
                chn -> next = dl -> head;
                chn -> prev = nullptr;
                dl -> head -> prev = chn;
                dl -> head = chn;
            }
            return 1;
       }

       if(isFull(dl)){
            hdll.deleteKey(dl -> tail -> data);
            deleteTail(dl);
       }

       insertHead(dl, ad);
       hdll.insertKey_Val(ad, dl -> head);
       return 0;
    }

    int LFU_NonHybrid(CacheAddress data, CacheLine l){
           

            if(hmh.contains_key(data)){
                int i = hmh.get_val(data);
                hp[i].frequency++;
                int in = hp.update_node_position(i);
                hmh.deleteKey(data);
                hmh.insertKey_Val(data, in);
                return 1;
            }

            minheap_type d(data);
            d.ind = hp.size();
            d.time = ++counter;

            if(d.ind == CacheSize){
                minheap_type nb = hp.top();
                hp.pop();
                hmh.deleteKey(nb.val);
            }

            int i = hp.push(d);
            hmh.insertKey_Val(data, i); 
            
    }


    int FIFO(CacheAddress ad, CacheLine l){
        if(!in(l1, ad) && !isFull(q)){
            enqueue(q,ad);
            insertHeadll(l1, ad);
            return 0;
        }
        else if(isFull(q) && !in(l1, ad)){
            deleteNode(l1, Front(q));
            dequeue(q);
            enqueue(q, ad);
            insertHeadll(l1, ad);
            return 0;
        }
        else{
            return 1;
        }
    }
};
