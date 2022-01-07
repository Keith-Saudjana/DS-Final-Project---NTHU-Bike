#include <iostream>
#include <fstream>
#include <string>

#include "Database.hpp"

using namespace std;


int main(int argc, char* argv[]){
    Database *DB = new Database;
    //cout << *(DB->Map);

    string action, type;
    int station, ID, time;
    ifstream cin("user.txt");

    ofstream CA("part1_response.txt");
    ofstream CB("part1_status.txt");
    // ofstream CC("part1_test.txt");
    ofstream DA("part2_response.txt");
    ofstream DB("part2_status.txt");
    // ofstream DC("part2_test.txt");

    while(cin >> action){
        if(action == "rent"){
            cin >> station >> type >> ID >> time;
            if(DB->Rent(station, type, ID, time)){
                CA << "Accept" << endl;
            }
            else CA << "Reject" << endl;
        }
        else if(action == "return"){    
            cin >> station >> ID >> time;
            DB->Return(station, ID, time);
        }
        else break;
    }
    for(int i = 1; i < DB->Stations.cur_size; i++){
        CB << *(DB->Stations[i]) << "\n";
    }
    CB << DB->Profit << "\n";

    return 0;
}