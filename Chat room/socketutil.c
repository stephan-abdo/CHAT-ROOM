#include "socketutil.h"

int createTCPIpv4socket(){
    return socket(AF_INET,SOCK_STREAM,0);
}

struct sockaddr_in *createIpv4adress(char *ip, int port){
    struct sockaddr_in *address = (struct sockaddr_in*)malloc(sizeof(struct sockaddr_in));
    address->sin_port = htons(port);
    address->sin_family = AF_INET;
    address->sin_addr.s_addr;

    if(strlen(ip) == 0){
        address->sin_addr.s_addr = INADDR_ANY;
    }
    else{
        inet_pton(AF_INET,ip,&address->sin_addr.s_addr);
    }

    return address;
}




