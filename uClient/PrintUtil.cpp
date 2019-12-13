//
// Created by mostafa on 06/12/2019.
//
#include <iostream>
#include <stdio.h>
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
    cout << s << "\n";
    printf("seqno: %d\n",p.seqno);
    for(int i=0;i<p.len;i++){
        printf("%c",p.data[i]);
    }
    printf("\n");
}

