#ifndef USER_HPP_
#define USER_HPP_

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

//User Info
class User{
    public:
    int ID;
    int Bike_ID;
    string Bike_Type;

    int station_rent;

    int time_rent;

    User(){
        ID = -1;
        Bike_ID = -1;
        Bike_Type = "";
        station_rent = -1;
        time_rent = -1;
    }

    User(int S, string TYP, int IN_ID, int Bike_Num, int T){
        ID = IN_ID;
        Bike_ID = Bike_Num;
        Bike_Type = TYP;

        station_rent = S;
        
        time_rent = T;
    }

};

#endif
