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
#include <fstream>
using namespace std;
#define  PACKETSSIZE 100000
#define  BUFFERSIZE 10000000
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
packet filePackets[PACKETSSIZE];
int received[PACKETSSIZE];
char fileBuffer[BUFFERSIZE];
int packetsSize =0;
int expected = 0;
void DieWithError(char errorMessage[]);
void printPacket(packet p,string s);
void gatherData();
void processRequest(int sock, struct sockaddr_in serverAddr){
    while(true) {
        ack_packet ack_p;
        ack_p.ackno = expected;
        char ackBuffer[sizeof(ack_packet)];
        memcpy(ackBuffer, &ack_p, sizeof(ack_packet));
        char buffer[sizeof(packet)];
        struct timeval tv;
        tv.tv_sec = 0;
        tv.tv_usec = 400000;
        struct sockaddr_in serverAddrReceived;
        unsigned int servAddrLen;
        setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const char *) &tv, sizeof tv);
        servAddrLen = sizeof(serverAddrReceived);
        int receivedData =0;
        while ((receivedData = recvfrom(sock, buffer, sizeof(packet), 0, (struct sockaddr *) &serverAddrReceived, &servAddrLen)) < 0) {
            if(receivedData == 0) break;
            if (sendto(sock, ackBuffer, sizeof(ack_packet), 0, (struct sockaddr *)
                    &serverAddr, sizeof(serverAddr)) != sizeof(ack_packet))
                DieWithError("sendto() sent a different number of bytes than expected");
            printf("--------------------------\nsend ack no %d\n--------------------------\n",expected);
            servAddrLen = sizeof(serverAddrReceived);
            setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const char *) &tv, sizeof tv);
        }
        if(receivedData == 0) break;
        if (serverAddr.sin_addr.s_addr != serverAddrReceived.sin_addr.s_addr) {
            fprintf(stderr, "Error: received a packet from unknown source.\n");
            exit(1);
        }
        packet p;
        memcpy(&p, buffer, sizeof(packet));
        packetsSize = max(packetsSize, (int) p.seqno + 1);
        filePackets[p.seqno] = p;
        received[p.seqno] = 1;
        printf("--------------------------\nreceive seq no:  %d\n--------------------------\n",p.seqno);
        while (received[expected] == 1)
            expected++;
        ack_p.ackno = expected;
        memcpy(ackBuffer,&ack_p, sizeof(ack_packet));
        if (sendto(sock, ackBuffer, sizeof(ack_packet), 0, (struct sockaddr *)
                &serverAddr, sizeof(serverAddr)) != sizeof(ack_packet))
            DieWithError("sendto() sent a different number of bytes than expected");
        printf("--------------------------\nsend ack no:  %d\n--------------------------\n",expected);
    }

}
//void processRequest(int sock,char buffer[], struct sockaddr_in serverAddr){
//    while (true) {
//        packet p;
//        memcpy(&p, buffer, sizeof(packet));
//        packetsSize = max(packetsSize, (int) p.seqno + 1);
//        filePackets[p.seqno] = p;
//        received[p.seqno] = 1;
//        ack_packet ack_p;
//        while (received[expected] == 1)
//            expected++;
//        ack_p.ackno = expected;
//        char ackBuffer[sizeof(ack_packet)];
//        memcpy(ackBuffer, &ack_p, sizeof(ack_packet));
//        printf("-------------------------\nreceived seq no %d\n-------------------------\n",p.seqno);
//        if (sendto(sock, ackBuffer, sizeof(ack_packet), 0, (struct sockaddr *)
//                &serverAddr, sizeof(serverAddr)) != sizeof(ack_packet)){
//            DieWithError("sendto() sent a different number of bytes than expected");
//        }
//        struct timeval tv;
//        tv.tv_sec = 0;
//        tv.tv_usec = 500000;
//        struct sockaddr_in serverAddrReceived;
//        unsigned int servAddrLen;
//        setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const char *) &tv, sizeof tv);
//        servAddrLen = sizeof(serverAddrReceived);
//        int receivedData =0;
//        while ((receivedData = recvfrom(sock, buffer, sizeof(packet), 0, (struct sockaddr *) &serverAddrReceived, &servAddrLen)) < 0) {
//            if(receivedData == 0) break;
//            if (sendto(sock, ackBuffer, sizeof(ack_packet), 0, (struct sockaddr *)
//                    &serverAddr, sizeof(serverAddr)) != sizeof(ack_packet))
//                DieWithError("sendto() sent a different number of bytes than expected");
//            printf("send ack no %d\n",expected);
//            servAddrLen = sizeof(serverAddrReceived);
//            setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const char *) &tv, sizeof tv);
//        }
//        if(receivedData == 0) break;
//        if (serverAddr.sin_addr.s_addr != serverAddrReceived.sin_addr.s_addr) {
//            fprintf(stderr, "Error: received a packet from unknown source.\n");
//            exit(1);
//        }
//    }
//}
void gatherData(){
    int bufSize=0;
    printf("packets size: %d\n",packetsSize);
    for(int i=0;i<packetsSize;i++){
        packet p = filePackets[i];
        for(int j=0;j<p.len;j++){
            fileBuffer[bufSize] = p.data[j];
            bufSize++;
        }
    }
    for(int i=0;i<bufSize;i++){
        printf("%c",fileBuffer[i]);
    }
    printf("total buffer size : %d\n",bufSize);
    ofstream outfile ("out.txt",std::ofstream::binary);
    outfile.write (fileBuffer,bufSize);
    outfile.close();
}

