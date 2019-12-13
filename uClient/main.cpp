#include <iostream>
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
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
void DieWithError(char *errorMessage);
void gatherData();
void readInput(char serverIp[] , unsigned short &port , char fileName[]);
void processRequest(int sock, struct sockaddr_in serverAddr);
char line1[30];
unsigned short line2;
char  line3[30];
int main() {
    readInput(line1,line2,line3);
    int sock;
    struct sockaddr_in serverAddr;
    struct sockaddr_in serverAddrReceived; // returned from receive
    unsigned short serverPort=line2;
    unsigned int servAddrLen; // returned from receive
    char *serverIP = line1;
    char *filename = line3;

    char buffer[sizeof(packet)];
    int dataLen = strlen(filename);
    int receivedDataLen;
    if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
        DieWithError( "socket () failed") ;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(serverIP);
    serverAddr.sin_port = htons(serverPort);

    if (sendto(sock, filename, dataLen, 0, (struct sockaddr *)
            &serverAddr, sizeof(serverAddr)) != dataLen)
        DieWithError("sendto() sent a different number of bytes than expected");

//    servAddrLen = sizeof(serverAddrReceived) ;
//    if ((receivedDataLen = recvfrom(sock, echoBuffer, BUFSIZE, 0,
//                                  (struct sockaddr *) &serverAddrReceived, &servAddrLen)) != dataLen)
//        DieWithError("recvfrom() failed") ;
//
//    if (serverAddr.sin_addr.s_addr != serverAddrReceived.sin_addr.s_addr)
//    {
//        fprintf(stderr,"Error: received a packet from unknown source.\n");
//        exit(1);
//    }
//    echoBuffer[receivedDataLen] = '\0' ;
//    printf("Received: %s\n", echoBuffer); /* Print the echoed arg */
//    close(sock);
//    while(true){
//        if ((receivedDataLen = recvfrom(sock, echoBuffer, BUFSIZE, 0,(struct sockaddr *) &serverAddrReceived, &servAddrLen)) < 0)
//         DieWithError("recvfrom() failed") ;
//        packet p;
//        printf("\n%d\n",receivedDataLen);
//        memcpy(&p , echoBuffer , receivedDataLen);
//        for(int i=0;i<p.len;i++){
//            cout<< p.data[i];
//        }
//        if(p.len<500){
//            cout << p.len;
//            break;
//        }
//    }
    struct timeval tv;
    tv.tv_sec = 1;
    tv.tv_usec = 0;
    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);
    servAddrLen = sizeof(serverAddrReceived) ;
    while((receivedDataLen = recvfrom(sock, buffer, sizeof(packet), 0,(struct sockaddr *) &serverAddrReceived, &servAddrLen)) != 0){
        if (sendto(sock, filename, dataLen, 0, (struct sockaddr *)
                &serverAddr, sizeof(serverAddr)) != dataLen)
            DieWithError("sendto() sent a different number of bytes than expected");
        printf("time out receive assurance of file \n");
        servAddrLen = sizeof(serverAddrReceived) ;
        setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);
    }
    printf("received first packet\n");
    processRequest(sock,serverAddrReceived);
    gatherData();
    exit(0);

}