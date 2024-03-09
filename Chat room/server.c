#include "socketutil.h"

struct acceptedsocket{
    int acceptedsocketFD;
    int error;
    struct sockaddr_in address;
    bool acceptedsucessfully;

};

struct acceptedsocket * acceptincomingconection(int serversocketFD);

void startacceptingincomingconection(int serversocketFD);

void reaceaveincomingdataonseperatethread(struct acceptedsocket *clientsocket);

void sendreacievedmessages(char *buffer,int socketFD);

void reaceaveincomingdata(int socketFD);

struct acceptedsocket acceptedsockets[10];

int acceptedsocketcount = 0;

int main(){

    int serversocketFD = createTCPIpv4socket();
    struct sockaddr_in *serveraddress = createIpv4adress("",2000);

    int result = bind(serversocketFD,serveraddress,sizeof(*serveraddress));
    if(result == 0){
        printf("socket binded sucessfully");
        fflush(stdout);

    }
    if (result == -1) {
    perror("bind");
    // or
    printf("bind failed: %s\n", strerror(errno));
    }


    startacceptingincomingconection(serversocketFD);

    shutdown(serversocketFD,SHUT_RDWR);

    return 0;
}

struct acceptedsocket * acceptincomingconection(int serversocketFD){
    struct sockaddr_in clientaddress;
    int listenresult = listen(serversocketFD,10);
    int clientaddresssize = sizeof(clientaddress);
    int clientsocketFD = accept(serversocketFD,&clientaddress,&clientaddresssize);

    struct acceptedsocket* acceptedsocket = (struct acceptedsocket*)malloc(sizeof(struct acceptedsocket));
    acceptedsocket->address = clientaddress;
    acceptedsocket->acceptedsocketFD = clientsocketFD;
    acceptedsocket->error = clientsocketFD>0;

    if(!acceptedsocket->acceptedsucessfully){
        acceptedsocket->error = clientsocketFD;
    }

    return acceptedsocket;
}

void startacceptingincomingconection(int serversocketFD){
    while(true){
        struct acceptedsocket *clientsocket =acceptincomingconection(serversocketFD);
        acceptedsockets[acceptedsocketcount++] = *clientsocket;
        reaceaveincomingdataonseperatethread(clientsocket);
        
    }
}

void reaceaveincomingdataonseperatethread(struct acceptedsocket *clientsocket){

    pthread_t id;
    pthread_create(&id,NULL,reaceaveincomingdata,clientsocket->acceptedsocketFD);

}

void sendreacievedmessages(char *buffer,int socketFD){
    for(int i = 0 ; i<acceptedsocketcount; i++){
        if(acceptedsockets[i].acceptedsocketFD != socketFD){

            send(acceptedsockets[i].acceptedsocketFD,buffer,strlen(buffer),0);

        }
    }
}

void reaceaveincomingdata(int socketFD){
    char buffer[1024];
    while(true){
        size_t amountreceaved = recv(socketFD, buffer, 1024,0);

        if(amountreceaved > 0){
            buffer[amountreceaved] = 0;
            printf("%s",buffer);

            sendreacievedmessages(buffer,socketFD);
        }
        if(amountreceaved == 0){
            break;
        }
    }
    close(socketFD);
}