#ifndef NODE_HPP_
#define NODE_HPP_
#endif

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

//Bike Inventory Linked List Implementation
struct Node{
    public:
    int Bike_ID;
    Node *next;

    Node(){ 
        Bike_ID = -1;
        next = NULL;
    }

    Node(int BD){
        Bike_ID = BD;
        next = NULL;
    }
};