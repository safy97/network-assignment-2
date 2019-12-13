//
// Created by mostafa on 05/12/2019.
//
#include <iostream>
#include <string>
#include <fstream>
using namespace std;
struct packet{
    /* Header */
    uint16_t cksum; /* Optional bonus part */
    uint16_t len;
    uint32_t seqno;
    /* Data */
    char data[500]; /* Not always 500 bytes, can be less */
};
void DieWithError(char errorMessage[]);
void readFiLe(string fileName , packet filePackets [] , int &packetsSize ){
    ifstream is (fileName, std::ifstream::binary);

    if (is) {
        // get length of file:
        is.seekg (0, is.end);
        int size = is.tellg();
        is.seekg (0, is.beg);
        char * buffer = new char [size];
        printf("Reading %d characters...",size);
        is.read (buffer,size);
        if (is)
            printf("all characters read successfully.");
        else
            printf("error: only %d could be read\n");
        is.close();
        int j =0;
        for(int i=0;i<size;i+=500){
            packet p;
            p.len = min(500,size-i);
            p.seqno = j++;
            for(int j=0;j < p.len;j++){
                p.data[j] = buffer[j+i];
            }
            filePackets[packetsSize++] = p;
        }
    }else{
        DieWithError("can't open file");
    }
}

