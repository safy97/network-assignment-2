//
// Created by mostafa on 05/12/2019.
//
#include <iostream>
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <chrono>
using namespace std;
using namespace std::chrono;
#define  PACKETSSIZE 6000
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
#define  TIMEOUTINTERVAL 999999
unsigned  short port = 7000;
void DieWithError(char errorMessage[]);
void readFiLe(string fileName , packet filePackets [] , int &packetsSize );
void slowStart(int start , double cwnd, double threshold, int timePass,int newSeg,int dublicate,int sock,struct sockaddr_in clntAddr , packet filePackets[],int packetsSize,
               unsigned short seed , double plp);
int checkPlp(unsigned short seed , double plp);
void printPacket(packet p,string s);
void makeAnalysis(double cwnd , int seqno);
int createSocket(){
    //insert mutex tbd
    unsigned short serverPort = port;
    port++;
    // insert mutex tbd
    int sock;
    if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
        DieWithError("socket() failed");
    struct sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr)); /* Zero out structure */
    serverAddr.sin_family = AF_INET; /* Internet address family */
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY); /* Any incoming interface */
    serverAddr.sin_port = htons(serverPort);
    if (bind(sock, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) < 0)
        DieWithError("bind() failed");
    return sock;
}
void send_packet(struct sockaddr_in clntAddr, int sock,packet p, unsigned short seed, double plp , double cwnd ){
    //makeAnalysis(cwnd , p.seqno);
    if(checkPlp(seed,plp)) {
        printPacket(p,"packet loss");
        return;
    }
    int packetSize = sizeof(p);
    char buff [packetSize];
    memcpy(buff, &p, packetSize);
    if (sendto(sock, buff , packetSize , 0,(struct sockaddr *) &clntAddr, sizeof(clntAddr)) != packetSize)
            DieWithError("sendto() sent a different number of bytes than expected");
}
void processRequest(struct sockaddr_in clntAddr, char fileName[], unsigned short seed , double plp){
    int sock = createSocket();
    packet filePackets [PACKETSSIZE];
    int packetsSize =0;
    sendto(sock, "" , 0 , 0,(struct sockaddr *) &clntAddr, sizeof(clntAddr)); // confirm reading file name
    readFiLe(fileName ,filePackets, packetsSize);
    printf("packets Size: %d\n",packetsSize);
    slowStart(0,1.0,10.0,0,0,0,sock,clntAddr,filePackets,packetsSize,seed , plp);
    sendto(sock, "" , 0 , 0,(struct sockaddr *) &clntAddr, sizeof(clntAddr)); // close the socket
    close(sock);
    printf("socket closed successfully\n");
}
