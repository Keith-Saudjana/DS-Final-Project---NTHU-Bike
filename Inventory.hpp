#ifndef INVENTORY_HPP_
#define INVENTORY_HPP_


#include <iostream>
#include <fstream>
#include <string>

//#include "Node.hpp"
#include "MinHeap.hpp"

using namespace std;

//Station Information Management
class Inventory{
    public:
    int station_number;
    int electric;
    int lady;
    int road;

    // Node *electric_inv;
    // Node *lady_inv;
    // Node *road_inv;

    MinHeap *electric_inv;
    MinHeap *lady_inv;
    MinHeap *road_inv;

    Inventory(){
        this->station_number = 0;
        this->electric = 0;
        electric_inv = NULL;
        this->lady = 0;
        lady_inv = NULL;
        this->road = 0;
        road_inv = NULL;
    }

    Inventory(int S, int E, int L, int R){
        this->station_number = S;
        this->electric = E;
        this->electric_inv = new MinHeap;
        for(int i = 0; i < E; i ++) electric_inv->Insert(S*100 + i);
        this->lady = L;
        this->lady_inv = new MinHeap;
        for(int i = 0; i < L; i ++) lady_inv->Insert(S*100 + i);
        this->road = R;
        this->road_inv = new MinHeap;
        for(int i = 0; i < R; i ++) road_inv->Insert(S*100 + i);
    }

    void TestPrint(ostream& os){
        int *E = new int[electric_inv->Heap_Size];
        for(int i = 0; i < electric_inv->Heap_Size; i++){
            E[i] = electric_inv->Heap[i];
        }
        for(int i = 0; i < electric_inv->Heap_Size; i++){
            for(int j = i+1; j < electric_inv->Heap_Size; j++){
                if(E[i] > E[j]){
                    int temp = E[i];
                    E[i] = E[j];
                    E[j] = temp;
                }
            }
        }

        for(int i = 0; i < electric_inv->Heap_Size; i++){
            os << E[i] << " ";
        }
        os << "\n";

    }
};

ostream& operator<<(ostream& os, const Inventory& rhs) {
    os << rhs.station_number << ":" << "\n";
    //os << rhs.electric_inv->Heap_Size << " " << rhs.lady_inv->Heap_Size << " " << rhs.road_inv->Heap_Size << " "<< "\n";
    MinHeap *E = rhs.electric_inv;
    os << "electric: ";
    while(E->Heap_Size != 0){
        int Temp = E->GetBike();
        os << Temp;
        if(E->Heap_Size != 0) os << " "; 
    }
    os << "\n";

    MinHeap *L = rhs.lady_inv;
    os << "lady: ";
    while(L->Heap_Size != 0){
        int Temp = L->GetBike();
        os << Temp;
        if(L->Heap_Size != 0) os << " "; 
    }
    os << "\n";

    MinHeap *R = rhs.road_inv;
    os << "road: ";
    while(R->Heap_Size != 0){
        int Temp = R->GetBike();
        os << Temp;
        if(R->Heap_Size != 0) os << " "; 
    }
    return os;
}

#endif