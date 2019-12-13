//
// Created by mostafa on 05/12/2019.
//
#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
using namespace std;
void readInput(unsigned short &port , unsigned short &seed , double &plp){
    std::string line;
    ifstream infile("server.in");
    int i =0;
    while (getline(infile, line))
    {
        istringstream iss(line);
        if(i==0){
            iss >> port;
            cout << "port number:  "<<port <<endl;
        }else if(i == 1){
            iss >> seed;
            cout <<"seed number: "<< seed << endl;
        }else{
            iss >> plp;
            cout<< "plp:  " << (double)plp <<endl;
        }
        i++;
    }
}

