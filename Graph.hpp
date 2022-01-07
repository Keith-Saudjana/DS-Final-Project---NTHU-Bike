#ifndef GRAPH_HPP_
#define GRAPH_HPP_

#include <iostream>
#include <fstream>
#include <string>

#include "Vector.hpp"
#include "Edge.hpp"

using namespace std;

#define INF 9999999

class Graph{
    public:
    int Nodes, Paths;
    vector<Edge> Edges;
    int **dist;

    Graph(){
        ifstream cin("map.txt");
        int s1, s2, dist;
        this->Paths = 0;
        this->Nodes = -1;
        while(cin >> s1 >> s2 >> dist){
            Edges.push_back(Edge(s1, s2, dist));
            this->Paths++;
            this->Nodes = max(this->Nodes, s1);
            this->Nodes = max(this->Nodes, s2);
        }
        floydWarshall();
    }

    void floydWarshall(){
        int N = this->Nodes;
        this->dist =  new int*[N];
        for(int i = 0; i < N; i++)
            this->dist[i] = new int[N];
        
        for(int i = 0; i < N; i++){
            for (int j = 0; j < N; j++){
                if(i == j) dist[i][j] = 0;
                else dist[i][j] = INF;
            }
        }

        for(int i = 0; i < this->Paths; i ++){
            int U = this->Edges[i].u;
            int V = this->Edges[i].v;
            int W = this->Edges[i].weight;
            dist[U-1][V-1] = W;
            dist[V-1][U-1] = W;
        }

        for (int k = 0; k < N; k++) {
            // Pick all vertices as source one by one
            for (int i = 0; i < N; i++) {
                // Pick all vertices as destination for the
                // above picked source
                for (int j = 0; j < N; j++) {
                    // If vertex k is on the shortest path from
                    // i to j, then update the value of
                    // dist[i][j]
                    if (dist[i][j] > (dist[i][k] + dist[k][j]) && (dist[k][j] != INF && dist[i][k] != INF))
                        dist[i][j] = dist[i][k] + dist[k][j];
                }
            }
        }
    }

};

ostream& operator<<(ostream& os, const Graph& rhs) {
    //cout << rhs.Edges << endl;
    for(int i = 0; i < rhs.Nodes; i++){
        for (int j = 0; j < rhs.Nodes; j++){
            cout << rhs.dist[i][j] << " ";
        }
        cout << endl;
    }
    return os;
}

#endif
