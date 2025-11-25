#include <iostream>
#include "linked_list.h"

int main(){
    std::cout << "Creating list\n";
    ll<int>* l = new ll<int>();
    insertHeadll(l, 10);
    insertHeadll(l, 20);
    insertHeadll(l, 30);
    std::cout << "Deleting list\n";
    delete l;
    std::cout << "Deleted list\n";
    return 0;
}
