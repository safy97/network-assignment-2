#include <iostream>
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <thread>

using namespace std;
#define  FILENAMESIZE 30
void DieWithError(char *errorMessage);
void readInput(unsigned short &port , unsigned short &seed , double &plp);
void processRequest(struct sockaddr_in clntAddr, char fileName[], unsigned short seed , double plp);
int main() {
    int sock; /* Socket */
    struct sockaddr_in serverAddr;
    struct sockaddr_in clntAddr;
    unsigned int clntAddrLen;
    char filename[FILENAMESIZE];
    unsigned short serverPort;
    unsigned short seed;
    double plp;
    srand(seed);
    readInput(serverPort,seed,plp);
    int dataLen;
    if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
        DieWithError("socket() failed");

    memset(&serverAddr, 0, sizeof(serverAddr)); /* Zero out structure */
    serverAddr.sin_family = AF_INET; /* Internet address family */
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY); /* Any incoming interface */
    serverAddr.sin_port = htons(serverPort);

    if (bind(sock, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) < 0)
        DieWithError("bind() failed");
    while(true){
        clntAddrLen = sizeof(clntAddr);
        if ((dataLen = recvfrom(sock, filename, FILENAMESIZE, 0,(struct sockaddr *) &clntAddr, &clntAddrLen)) < 0)
            DieWithError("recvfrom() failed") ;
        filename[dataLen] = '\0';
//        printf("Handling client %s\n filename %s\n", inet_ntoa(clntAddr.sin_addr),filename) ;
//        if (sendto(sock, filename, dataLen , 0,(struct sockaddr *) &clntAddr, sizeof(clntAddr)) != dataLen)
//            DieWithError("sendto() sent a different number of bytes than expected");
        thread t(processRequest, clntAddr, filename,seed,plp);
        t.detach();
        //processRequest(clntAddr,filename,seed,plp);
   }
    return 0;
}