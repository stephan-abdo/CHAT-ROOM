#include "socketutil.h"

void reaceaveincomingdataonnewthread(int socketFD);

void listenandprint(int socketFD);

int main(){

    int socketFD = createTCPIpv4socket();
    char *ip ="127.0.0.1";

    struct sockaddr_in *address = createIpv4adress(ip,2000);

    int result = connect(socketFD,address, sizeof(*address));

    if(result == 0){
        printf("conection was sucessfull\n");
    }

    reaceaveincomingdataonnewthread(socketFD);

    char *name = NULL;
    size_t nameSize= 0;
    printf("please enter your name?\n");
    ssize_t  nameCount = getline(&name,&nameSize,stdin);
    name[nameCount-1]=0;

    char *line = NULL;
    size_t lineSize= 0;
    printf("You can now send messages on the chat room type EXIT to leave the chat room\n");

    char buffer[1024];

    while(true){

        sprintf(buffer,"%s : %s",name,line);
        ssize_t charcount = getline(&line,&lineSize,stdin);

        if(charcount > 0){
            if(strcmp(line,"EXIT\n")== 0){
                break;
            }
            printf("output");
            ssize_t amountsent = send(socketFD, buffer , strlen(buffer), 0);
        }
    }
    close(socketFD);
    return 0;
}

void reaceaveincomingdataonnewthread(int socketFD){
    pthread_t id;
    pthread_create(&id,NULL,listenandprint,socketFD);

}

void listenandprint(int socketFD){
    char buffer[1024];
    while(true){
        size_t amountreceaved = recv(socketFD, buffer, 1024,0);
        if(amountreceaved > 0){
            buffer[amountreceaved] = 0;
            printf("%s\n",buffer);
        }
        if(amountreceaved == 0){
            break;
        }
    }
    close(socketFD);
}