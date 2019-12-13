//
// Created by mostafa on 05/12/2019.
//
#include <iostream>
#include <cstring>
#include <sstream>
#include <string>
#include <fstream>
using namespace std;
void readInput(char serverIp[] , unsigned short &port , char fileName[]){
    std::string line;
    ifstream infile("client.in");
    int i =0;
    while (getline(infile, line))
    {
        istringstream iss(line);
        if(i==0){
            iss >> serverIp;
            cout << "server  ip: "<<serverIp <<endl;
        }else if(i == 1){
            iss >> port;
            cout <<"port number: "<< port << endl;
        }else{
            iss >> fileName;
            cout<< "filename:  " <<fileName<<endl;
        }
        i++;
    }
}

