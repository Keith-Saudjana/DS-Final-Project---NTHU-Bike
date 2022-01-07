#ifndef FEE_HPP_
#define FEE_HPP_

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

//Fee Information Management
class Fee{
    public:
    int Electric_Fee[2];
    int Lady_Fee[2];
    int Road_Fee[2];
    int waiting_fee;
    float switching_discount;
    int transfer_fee;
    public:

    Fee(){
        ifstream cin("fee.txt");
        string Bike_Type;
        int Discounted_Fee, Regular_Fee;
        for(int i = 0; i < 3; i++){
            cin >> Bike_Type >> Discounted_Fee >> Regular_Fee;
            if(Bike_Type == "electric"){
                Electric_Fee[0] = Discounted_Fee;
                Electric_Fee[1] = Regular_Fee;
            }
            else if(Bike_Type == "lady"){
                Lady_Fee[0] = Discounted_Fee;
                Lady_Fee[1] = Regular_Fee;
            }
            else if(Bike_Type == "road"){
                Road_Fee[0] = Discounted_Fee;
                Road_Fee[1] = Regular_Fee;
            }
        }
        cin >> this->waiting_fee >> this->switching_discount >> this->transfer_fee;
    }
};

ostream& operator<<(ostream& os, const Fee& rhs) {
    cout << rhs.Electric_Fee[0] << " " << rhs.Electric_Fee[1] << endl;
    cout << rhs.Lady_Fee[0] << " " << rhs.Lady_Fee[1] << endl;
    cout << rhs.Road_Fee[0] << " " << rhs.Road_Fee[1] << endl;
    cout << rhs.waiting_fee << endl;
    cout << rhs.switching_discount << endl;
    cout << rhs.transfer_fee << endl;
    return os;
}

#endif