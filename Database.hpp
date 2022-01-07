#ifndef DATABASE_HPP_
#define DATABASE_HPP_

#include <iostream>
#include <fstream>
#include <string>

#include "Inventory.hpp"
#include "AVL.hpp"
#include "Graph.hpp"
#include "Fee.hpp"

using namespace std;

//Central Management
class Database{
    public:
    vector<Inventory*> Stations;
    AVL *Root;
    Graph *Map;
    Fee *Fees;
    
    long long Profit;

    Database(){
        ifstream cin("station.txt");
        int S, E, L, R;
        
        Stations.push_back(new Inventory(0, 0, 0, 0)); //Station 0
        while(cin >> S >> E >> L >> R){
            Stations.push_back(new Inventory(S, E, L, R));
        }
        Root = NULL;
        Map = new Graph;
        Fees = new Fee;

        Profit = 0;
    }

    //Rental Functions
    int Rent(int S, string Bike_Type, int IN_ID, int T){
        int Bike_Num = Check_Available(S, Bike_Type);
        if(Bike_Num > 0){
            Root = Root->insert(Root, new User(S, Bike_Type, IN_ID, Bike_Num, T));
            return 1;
        }
        return 0;
    }

    int Check_Available(int S, string TYP){
        if(TYP == "electric"){
            if(Stations[S]->electric > 0)
                return Rent_Electric(Stations[S]);
            else return -1;
        }
        else if(TYP == "lady"){
            if(Stations[S]->lady > 0)
                return Rent_Lady(Stations[S]);
            else return -1;
        }
        else if (TYP == "road"){
            if(Stations[S]->road > 0)
                return Rent_Road(Stations[S]);
            else return -1;
        }
        return -1;
    }

    int Rent_Electric(Inventory *Station){
        int Bike_Num = Station->electric_inv->GetBike();
        Station->electric --;
        return Bike_Num;
    }

    int Rent_Lady(Inventory *Station){
        int Bike_Num = Station->lady_inv->GetBike();
        Station->lady --;
        return Bike_Num;
    }

    int Rent_Road(Inventory *Station){
        int Bike_Num = Station->road_inv->GetBike();
        Station->road --;
        return Bike_Num;
    }

    //Return Functions
    void Return(int S, int IN_ID, int T){
        AVL *N = Root->Find(Root, IN_ID);
        if(N == NULL) return;

        User User_Return = N->U;
        Root = Root->deleteNode(Root, IN_ID);
        
        int Bike_ID, station_rent, time_spent, discounted_fee, regular_fee;
        string Bike_Type;
        
        Bike_ID = User_Return.Bike_ID;
        Bike_Type = User_Return.Bike_Type;
        if(Bike_Type == "electric"){
            Return_Electric(Stations[S], Bike_ID);
            discounted_fee = Fees->Electric_Fee[0];
            regular_fee = Fees->Electric_Fee[1]; 
        }
        else if(Bike_Type == "lady"){
            Return_Lady(Stations[S], Bike_ID); 
            discounted_fee = Fees->Lady_Fee[0];
            regular_fee = Fees->Lady_Fee[1]; 
        }
        else if(Bike_Type == "road"){
            Return_Road(Stations[S], Bike_ID); 
            discounted_fee = Fees->Road_Fee[0];
            regular_fee = Fees->Road_Fee[1]; 
        }
        
        station_rent = User_Return.station_rent;
        time_spent = T - User_Return.time_rent;

        if(time_spent == Map->dist[station_rent-1][S-1]) Profit += time_spent * discounted_fee;
        else Profit += time_spent * regular_fee;
    }

    void Return_Electric(Inventory *Station, int Bike_Num){
        Station->electric_inv->Insert(Bike_Num);
        //Station->Insert(&(Station->electric_inv), Bike_Num);
        Station->electric ++;
        return;
    }

    void Return_Lady(Inventory *Station, int Bike_Num){
        Station->lady_inv->Insert(Bike_Num);
        //Station->Insert(&(Station->lady_inv), Bike_Num);
        Station->lady ++;
        return;
    }

    void Return_Road(Inventory *Station, int Bike_Num){
        Station->road_inv->Insert(Bike_Num);
        //Station->Insert(&(Station->road_inv), Bike_Num);
        Station->road ++;
        return;
    }

};

ostream& operator<<(ostream& os, const Database& rhs) {
    cout << *(rhs.Fees) << endl;
    cout << *(rhs.Map) <<endl;
    cout << rhs.Stations << endl;
    return os;
}

#endif