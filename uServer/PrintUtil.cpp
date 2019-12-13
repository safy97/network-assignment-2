//
// Created by mostafa on 06/12/2019.
//
#include <iostream>
#include <stdio.h>
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
struct ack_packet {
    uint16_t cksum; /* Optional bonus part */
    uint16_t len;
    uint32_t ackno;
};
void printPacket(packet p,string s){
    cout << "-------------------\n";
    cout << s << "\n";
    printf("seqno: %d\n",p.seqno);
    cout << "-------------------\n";
}
void printPacketAck(ack_packet p , string s){
    cout << "-------------------\n";
    cout << s << "\n";
    printf("seqno: %d\n",p.ackno);
    cout << "-------------------\n";
}
void printState(double cwnd, double threshold, string s){
    cout << "-------------------\n";
    cout << s << "\n";
    printf("cwnd: %f, threshold: %f\n", cwnd,threshold);
    cout << "-------------------\n";
}
void makeAnalysis(double cwnd , int seqno){
    std::ofstream outfile;
    outfile.open("analysis.txt", std::ios_base::app); // append instead of overwrite
    outfile << cwnd;
    outfile << ", ";
    outfile << seqno;
    outfile << "\n";
}
void makeAnalysis2(double cwnd ,double threshold, string s){
    std::ofstream outfile;
    outfile.open("analysis.txt", std::ios_base::app); // append instead of overwrite
    outfile << cwnd <<", "<< threshold <<", "<< s <<"\n";
}


