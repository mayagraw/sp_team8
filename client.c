#include "common.h"

int send_msg(int ,char*);
void close_conn(int);

int main (int argc , char *  argv[]){
    struct sockaddr_in server;
    int socket_desc;
    char * msg = "Hey!!!! Hello there";
    int a;
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
    send_msg(socket_desc,msg);
    close_conn(socket_desc);
    return 0;
}

int send_msg(int socket_desc,char* msg){
    char msg1[MAX_CTRL_MSG_LEN] = "";
    char buffer[MAX_CTRL_MSG_LEN] = "";
    if(!msg){return -1;}
    sprintf(buffer,"%d",strlen(msg)); //convert the len into a string
    make_ctrl_msg(buffer,msg1);
    if(write(socket_desc,msg1,strlen(msg1))== -1 ){
        printf("\n Send failed");
        return -1;
    }
    if(write(socket_desc,msg,strlen(msg))== -1 ){
        printf("\n Send failed");
        return -1;
    }
}

void close_conn(int socket_desc){
    char msg1[MAX_CTRL_MSG_LEN] = "";
    make_ctrl_msg("0",msg1);
    write(socket_desc,msg1,strlen(msg1));
    close(socket_desc);
}
