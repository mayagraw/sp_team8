#include "../include/common.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdbool.h>

int send_msg(int ,char*);
void close_conn(int);

int main (int argc , char *  argv[]){
    struct sockaddr_in server;
    int socket_desc;
    char msg[MAX_DATA_MSG_LEN+1] = "";
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
    while(1){
        printf("\nEnter a msg to be sent (max len %d):",MAX_DATA_MSG_LEN);
        fgets(msg,MAX_DATA_MSG_LEN,stdin);
        //printf("read by scanf %d\n",scanf("%MAX_DATA_MSG_LEN[^'\n']s",msg));
        //printf("read by scanf %d\n",scanf("%s",msg));
        validate_ip_str(msg);
        if(strlen(msg)){
            send_msg(socket_desc,msg);
        } else {
            close_conn(socket_desc);
            break;
        }
    }
    return 0;
}

int send_msg(int socket_desc,char* msg){
    char msg1[MAX_CTRL_MSG_LEN] = "";
    char data_msg[MAX_DATA_MSG_LEN] = "";
    char buffer[MAX_CTRL_MSG_LEN] = "";
    if(!msg){return -1;}
    sprintf(buffer,"%lu",(unsigned long)strlen(msg)); //convert the len into a string
    make_ctrl_msg(buffer,msg1);
    if(write(socket_desc,msg1,strlen(msg1))== -1 ){
        printf("\n Send failed");
        return -1;
    }
    make_data_msg(msg,data_msg);
    if(write(socket_desc,data_msg,strlen(data_msg))== -1 ){
        printf("\n Send failed");
        return -1;
    } else { return 1;}
}

void close_conn(int socket_desc){
    char msg1[MAX_CTRL_MSG_LEN] = "";
    make_ctrl_msg("0",msg1);
    write(socket_desc,msg1,strlen(msg1));
    close(socket_desc);
}
