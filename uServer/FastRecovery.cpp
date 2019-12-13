//
// Created by mostafa on 06/12/2019.
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
#define  TIMEOUTINTERVAL 1000000
void congetionAvoidance(int start , double cwnd, double threshold, int timePass,int newSeg,int dublicate,int sock,struct sockaddr_in clntAddr , packet filePackets[],int packetsSize,
                        unsigned short seed, double plp);
void slowStart(int start , double cwnd, double threshold, int timePass,int newSeg,int dublicate,int sock,struct sockaddr_in clntAddr , packet filePackets[],int packetsSize,
               unsigned short seed, double plp);
void send_packet(struct sockaddr_in clntAddr, int sock,packet p,unsigned short seed, double plp, double cwnd);
void printPacket(packet p,string s);
void printPacketAck(ack_packet p , string s);
void printState(double cwnd, double threshold, string s);
void makeAnalysis2(double cwnd ,double  threshold, string s);
void fastRecovery(int start , double cwnd, double threshold, int timePass,int newSeg,int dublicate,int sock,struct sockaddr_in clntAddr , packet filePackets[],int packetsSize,
                  unsigned short seed, double plp){
    printState(cwnd,threshold,"print state from fast recovery");
    int check = 0;
    for(int i= newSeg ; i<((int)cwnd + start) && newSeg < packetsSize ;i++){
        printPacket(filePackets[i],"packet send from fast recovery");
        send_packet(clntAddr , sock, filePackets[i],seed,plp,cwnd);
        newSeg++;
        if(check == 0){
            makeAnalysis2(cwnd,threshold,"fast recovery");
            check++;
        }
    }
    auto timerStart = high_resolution_clock::now();
    char buffer[sizeof(ack_packet)];
    unsigned  int clntAddrLen = sizeof(clntAddr);
    int dataLen =0;
    struct timeval tv;
    tv.tv_sec = (int)((TIMEOUTINTERVAL - timePass)/1000000);
    tv.tv_usec = (int)((TIMEOUTINTERVAL - timePass)%1000000);
    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);
    if ((dataLen = recvfrom(sock, buffer, (int)sizeof(ack_packet), 0,(struct sockaddr *) &clntAddr, &clntAddrLen)) < 0){
        // time out
        // retransmit missing segment
        printf("TimeOut from fast recovery \n");
        printPacket(filePackets[start],"packet send from fast recovery");
        send_packet(clntAddr, sock , filePackets[start],seed,plp,cwnd);
        threshold = cwnd / 2.0;
        cwnd =1.0;
        dublicate =0;
        timePass =0;
        makeAnalysis2(cwnd,threshold,"fast recovery time out");
        slowStart(start,cwnd,threshold,timePass,newSeg, dublicate,sock,clntAddr,filePackets,packetsSize,seed,plp);
        return;
    }
    auto timerEnd = high_resolution_clock::now();
    timePass += (int)duration_cast<microseconds>(timerEnd - timerStart).count();
    ack_packet p;
    memcpy(&p, buffer , sizeof(buffer));
    printPacketAck(p,"ack packet received from fast recovery");
    if(p.ackno > start){ // new ack
        start = p.ackno;
        if(start == packetsSize){ // reach the end of file
            return;
        } // still there are packets to send
        cwnd = threshold;
        dublicate =0;
        timePass =0;
        congetionAvoidance(start,cwnd,threshold,timePass,newSeg,dublicate,sock,clntAddr,filePackets,packetsSize,seed,plp);
    }else if(p.ackno == start){ // dublicate ack
        cwnd += 1.0;
        fastRecovery(start,cwnd,threshold,timePass,newSeg, dublicate,sock,clntAddr,filePackets,packetsSize,seed,plp);
    }else{ // handle inorder ack
        fastRecovery(start,cwnd,threshold,timePass,newSeg, dublicate,sock,clntAddr,filePackets,packetsSize,seed,plp);
    }
}
