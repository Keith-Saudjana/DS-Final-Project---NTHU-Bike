#ifndef OPTIMIZEDDATABASE_HPP_
#define OPTIMIZEDDATABASE_HPP_

#include <iostream>
#include <fstream>
#include <string>

#include "Inventory.hpp"
#include "AVL.hpp"
#include "Graph.hpp"
#include "Fee.hpp"
#include "Database.hpp"

using namespace std;

//Central Management
class OptimizedDatabase{
    public:
    vector<Inventory*> Stations;
    AVL *Root;
    Graph *Map;
    Fee *Fees;
    
    long long Profit;

    OptimizedDatabase(){
        ifstream cin("station.txt");
        int S, E, L, R;
        
        Stations.push_back(new Inventory(0, 0, 0, 0)); //Station 0
        while(cin >> S >> E >> L >> R) Stations.push_back(new Inventory(S, E, L, R));
        Root = NULL;
        Map = new Graph;
        Fees = new Fee;

        Profit = 0;
    }

    //Rental Functions
    void Rent(ostream& os, int S, string Bike_Type, int IN_ID, int T){
        int Bike_Num = Get_Bike_Num(S, Bike_Type);
        if(Bike_Num > 0){
            Root = Root->insert(Root, new User(S, Bike_Type, IN_ID, Bike_Num, T));
            os << "accept\n";

            if(Check_Inventory_Status(S, Bike_Type)) Transfer(os, S, Bike_Type, T);
            return;
        }
        else if(Check_Inventory_Status(S, "electric") + Check_Inventory_Status(S, "lady") + Check_Inventory_Status(S, "road") > 0){
            string New_Bike_Type = Switch(S, Bike_Type, IN_ID, T);
            int New_Bike_Num = Get_Bike_Num(S, New_Bike_Type);
            Root = Root->insert(Root, new User(S, New_Bike_Type, IN_ID, New_Bike_Num, T));
            os << "discount " << New_Bike_Type << "\n";

            if(Check_Inventory_Status(S, New_Bike_Type)) Transfer(os, S, New_Bike_Type, T);
            return;
        }
        else if(Check_Transfer(S, Bike_Type, T)){
            Wait(S, Bike_Type, IN_ID, T);
            os << "wait\n";
            return;
        }
        os << "reject\n";
        return;
    }
    
    //Special Functions Start

    bool Check_Inventory_Status(int S, string TYP){
        //Checks if type of bike is empty and if there is a transfer on the way
        if(TYP == "electric" && Stations[S]->electric == 0 && Stations[S]->electric_transfer.state == false) return true;
        else if(TYP == "lady" && Stations[S]->lady == 0 && Stations[S]->lady_transfer.state == false) return true;
        else if (TYP == "road" && Stations[S]->road == 0 && Stations[S]->road_transfer.state == false) return true;
        return false;
    }

    bool Check_Transfer(int S, string TYP, int T){
        //Calculate average map distance from the current station
        //Currently not optimized
        if(TYP == "electric" && Stations[S]->electric_transfer.state == true && Map->average_dist[S-1] >= (Stations[S]->electric_transfer.arrival_time - T)) return true;
        else if(TYP == "lady" && Stations[S]->lady_transfer.state == true && Map->average_dist[S-1] >= (Stations[S]->lady_transfer.arrival_time - T)) return true;
        else if (TYP == "road" && Stations[S]->road_transfer.state == true && Map->average_dist[S-1] >= (Stations[S]->road_transfer.arrival_time - T)) return true;
        return false;
    }

    void Update(ostream& os, int T){
        //update waiting list
        for(int i = 1; i < Stations.cur_size; i ++){
            if(Stations[i]->electric_transfer.arrival_time <= T){
                Stations[i]->electric_transfer.state = false;
                Stations[i]->electric_transfer.arrival_time = -1;
                for(int j = 0; j < Stations[i]->electric_transfer.quantity; j++)
                    Return_Electric(Stations[i], Stations[i]->electric_transfer.Transferred_Bike[j]);
                delete Stations[i]->electric_transfer.Transferred_Bike;
            }
            if(Stations[i]->lady_transfer.arrival_time <= T){
                Stations[i]->lady_transfer.state = false;
                Stations[i]->lady_transfer.arrival_time = -1;
                for(int j = 0; j < Stations[i]->lady_transfer.quantity; j++)
                    Return_Lady(Stations[i], Stations[i]->lady_transfer.Transferred_Bike[j]);
                delete Stations[i]->lady_transfer.Transferred_Bike;
            }
            if(Stations[i]->road_transfer.arrival_time <= T){
                Stations[i]->road_transfer.state = false;
                Stations[i]->road_transfer.arrival_time = -1;
                for(int j = 0; j < Stations[i]->road_transfer.quantity; j++)
                    Return_Road(Stations[i], Stations[i]->road_transfer.Transferred_Bike[j]);
                delete Stations[i]->road_transfer.Transferred_Bike;
            }
        }
    }

    void Balancing(ostream& os){
        for(int i = 1; i < Stations.cur_size; i ++){
            if(Stations[i]->electric == 0) Transfer(os, i, "electric", 0);
            if(Stations[i]->lady == 0) Transfer(os, i, "lady", 0);
            if(Stations[i]->road == 0) Transfer(os, i, "road", 0);
        }
    }
    //ransfer 𝑠𝑗 𝑠𝑖 type quantity time
    void Transfer(ostream& os, int S, string TYP, int T){
        int Chosen_Station;
        int Bike_Quantity;
        int var = -1;
        //check if there is any time left for a transfer
        if(TYP == "electric"){
            for(int i = 1; i < Stations.cur_size; i++){
                if(S != i){
                    int station_var;
                    station_var = 100 * (Stations[i]->electric * Fees->Electric_Fee[1]) / (Map->dist[S-1][i-1] * Fees->transfer_fee);
                    if(station_var > var){
                        var = station_var;
                        Chosen_Station = i;
                        Bike_Quantity = Stations[i]->electric / 2;
                    }
                }
            }
            Stations[S]->electric_transfer.state = true;
            Stations[S]->electric_transfer.arrival_time = T + Map->dist[S-1][Chosen_Station-1];
            Stations[S]->electric_transfer.quantity = Bike_Quantity;
            Stations[S]->electric_transfer.Transferred_Bike = new int[Bike_Quantity];
            for(int i = 0; i < Bike_Quantity; i++) Stations[S]->electric_transfer.Transferred_Bike[i] = Get_Bike_Num(Chosen_Station, "electric");
            Profit -= Fees->transfer_fee * Map->dist[S-1][Chosen_Station-1];
            os << "transfer " << Chosen_Station << " " << S << " electric " << Bike_Quantity << " " << T << "\n";
        }
        else if(TYP == "lady"){
            for(int i = 1; i < Stations.cur_size; i++){
                if(S != i){
                    int station_var;
                    station_var = 100 * (Stations[i]->lady * Fees->Lady_Fee[1]) / (Map->dist[S-1][i-1] * Fees->transfer_fee);
                    if(station_var > var){
                        var = station_var;
                        Chosen_Station = i;
                        Bike_Quantity = Stations[i]->lady / 2;
                    }
                }
            }
            Stations[S]->lady_transfer.state = true;
            Stations[S]->lady_transfer.arrival_time = T + Map->dist[S-1][Chosen_Station-1];
            Stations[S]->lady_transfer.Transferred_Bike = new int[Bike_Quantity];
            Stations[S]->lady_transfer.quantity = Bike_Quantity;
            for(int i = 0; i < Bike_Quantity; i++) Stations[S]->lady_transfer.Transferred_Bike[i] = Get_Bike_Num(Chosen_Station, "lady");
            Profit -= Fees->transfer_fee * Map->dist[S-1][Chosen_Station-1];
            os << "transfer " << Chosen_Station << " " << S << " lady " << Bike_Quantity << " " << T << "\n";
        }
        else if(TYP == "road"){
            for(int i = 1; i < Stations.cur_size; i++){
                if(S != i){
                    int station_var;
                    station_var = 100 * (Stations[i]->road * Fees->Road_Fee[1]) / (Map->dist[S-1][i-1] * Fees->transfer_fee);
                    if(station_var > var){
                        var = station_var;
                        Chosen_Station = i;
                        Bike_Quantity = Stations[i]->road / 2;
                    }
                }
            }
            Stations[S]->road_transfer.state = true;
            Stations[S]->road_transfer.arrival_time = T + Map->dist[S-1][Chosen_Station-1];
            Stations[S]->road_transfer.Transferred_Bike = new int[Bike_Quantity];
            Stations[S]->road_transfer.quantity = Bike_Quantity;
            for(int i = 0; i < Bike_Quantity; i++) Stations[S]->road_transfer.Transferred_Bike[i] = Get_Bike_Num(Chosen_Station, "road");
            Profit -= Fees->transfer_fee * Map->dist[S-1][Chosen_Station-1];
            os << "transfer " << Chosen_Station << " " << S << " road " << Bike_Quantity << " " << T << "\n";
        }
        return;
    }

    void Wait(int S, string TYP, int IN_ID, int T){
        if(TYP == "electric") Stations[S]->electric_waiting_list.push_back(IN_ID, T);
        else if(TYP == "lady")Stations[S]->lady_waiting_list.push_back(IN_ID, T);
        else if (TYP == "road") Stations[S]->lady_waiting_list.push_back(IN_ID, T);
    }

    string Switch(int S, string TYP, int IN_ID, int T){
        string New_Bike_Type;
        if(TYP == "electric"){
            if(Stations[S]->lady >= Stations[S]->road) New_Bike_Type = "lady";
            else New_Bike_Type = "road";
        }
        else if(TYP == "lady"){
            if(Stations[S]->electric >= Stations[S]->road) New_Bike_Type = "electric";
            else New_Bike_Type = "road";
        }
        else if(TYP == "road"){
            if(Stations[S]->electric >= Stations[S]->lady) New_Bike_Type = "electric";
            else New_Bike_Type = "lady";
        }

        return New_Bike_Type;
    }

    //Special Functions End

    int Get_Bike_Num(int S, string TYP){
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
        Station->electric ++;
        return;
    }

    void Return_Lady(Inventory *Station, int Bike_Num){
        Station->lady_inv->Insert(Bike_Num);
        Station->lady ++;
        return;
    }

    void Return_Road(Inventory *Station, int Bike_Num){
        Station->road_inv->Insert(Bike_Num);
        Station->road ++;
        return;
    }

};

ostream& operator<<(ostream& os, const OptimizedDatabase& rhs) {
    cout << *(rhs.Fees) << endl;
    cout << *(rhs.Map) <<endl;
    cout << rhs.Stations << endl;
    return os;
}

#endif