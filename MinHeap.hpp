#ifndef MINHEAP_HPP_
#define MINHEAP_HPP_

#include <iostream>
#include <fstream>
#include <string>

#include "Vector.hpp"

using namespace std;

//MinHeap Implementation
class MinHeap{
    public:
    vector<int> Heap;
    int Heap_Size;

    MinHeap(){Heap_Size = 0;}

    int parent(int i) {return (i-1)/2;}

    int left(int i) {return (2*i + 1);}

    int right(int i) {return (2*i + 2);}

    void Insert(int BD){
        int i = this->Heap_Size;
        this->Heap_Size ++;
        Heap.push_back(BD);
        while(i != 0 && Heap[parent(i)] > Heap[i]){
            swap(&(Heap[i]), &(Heap[parent(i)]));

            i = parent(i); 
        }
    }

    int GetBike(){
        if (Heap_Size <= 0) return -1;
        if (Heap_Size == 1){
            Heap_Size--;
            Heap.pop_back();
            return Heap[0];
        }
    
        // Store the minimum value, and remove it from heap
        int root = Heap[0];
        Heap[0] = Heap[Heap_Size-1];
        Heap_Size--;
        Heap.pop_back();
        MinHeapify(0);
    
        return root;
    }

    void MinHeapify(int i){
        if(i >= Heap_Size) return;
        int l = left(i);
        int r = right(i);
        int smallest = i;
        if (l < Heap_Size && Heap[l] < Heap[i])
            smallest = l;
        if (r < Heap_Size && Heap[r] < Heap[smallest])
            smallest = r;
        if (smallest != i) {
            swap(&Heap[i], &Heap[smallest]);
            MinHeapify(smallest);
        }
    }

    void swap(int *x, int *y){
        int temp = *x;
        *x = *y;
        *y = temp;
    }

};

#endif