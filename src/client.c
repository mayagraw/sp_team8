#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdbool.h>
#include "../include/common.h"

int send_msg(int ,char*);
void close_conn(int);

int main (int argc , char *  argv[]){
    struct sockaddr_in server;
    int socket_desc;
    mheader msg={1,2,3l};
    if(argc!=2){
        printf("\nPlease enter the server address\n");
        return -1;
    }

    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if(socket_desc == -1){
        perror("\nFailed to create socket\n");
        return -1;
    }
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(argv[1]);
    server.sin_port = htons(8888);

    if((connect(socket_desc,(struct sockaddr *)&server,sizeof(server))) == -1){
        printf("\nFailed to connect to the server %s\n",argv[1]);
        return -1;
    }
    send_msg(socket_desc,&msg);
    close_conn(socket_desc);
    return 0;
}

int send_msg(int socket_desc,mheader* msg){
    char out_str[MAX_CTRL_MSG_LEN] = "";
    if(!msg){return -1;}
    make_mheader_str(msg,out_str);
    if(write(socket_desc,out_str,MAX_CTRL_MSG_LEN)== -1 ){
        ERRSTR("Send failed");
        return -1;
    }
}

void close_conn(int socket_desc){
    close(socket_desc);
}
