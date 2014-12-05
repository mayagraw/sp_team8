#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include "../include/socket.h"
#include "../include/common.h"
//TBD remove cyclic dependencies (socket.h is included in common.c and vice versa)
#include "../include/msgq.h"
#include "../include/event_manager.h"

//TBD remove this, add the required header
extern void add_client_to_db(int,struct sockaddr_in *);

typedef struct {
    struct sockaddr_in * client;
    int socket_id;
} client_info;


void keep_accepting(void){
    //keep 'accept' in a while loop
    //add this client to the database
    int socket_desc , new_socket , c;
    struct sockaddr_in server , client;
    client_info info;

    //Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
        ERR();
    }

    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( 8888 );

    //Bind
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        ERR();
        return;
    }

    //Listen
    listen(socket_desc , 3);

    //Accept and incoming connection
    c = sizeof(struct sockaddr_in);
    while( (new_socket = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c)) )
    {
        //no need to spawn a new thread here.Add the client to the DB and add the fd to the receiving list.
        //TBD
        add_client_to_db(new_socket,&client);
        assign_fd(new_socket);
        
    }

    if (new_socket<0)
    {
        ERRSTR("accept failed");
    }
}

void keep_receiving(EV_P_ ev_io *w, int event){
    int read_size=0,nxt_len;
    int sock= w->fd;
    unsigned char buffer[2000]="";
    mheader r_msg;

    while( (read_size = recv(sock , buffer , MAX_CTRL_MSG_LEN , 0)) > 0 )
    {
        if(read_size != MAX_CTRL_MSG_LEN ){
            //Discard the packet
            continue;
        }
        if(!xtract_mheader(buffer,&r_msg)){
            printf("\nxtract_mheader failed");
            break;
        }
        //TBD .. tke action based on the type
        memset(buffer,'\0',2001);
    }
    
    if(read_size == 0){
        //inorderly shutdown performed by the client, ungraceful exist
        //TBD
    }

}

void keep_sending(void){
    //TBD 
    // if the sending fails due to self resolvable issue, enqueue the msg again. ---- is this right ? 
    // we can ignore the failure cases for now
    msgq_out msg;
    int len,total,r,done;
    while(1){
        //continuously check the out queue for msgs
        //these are blocking calls
        if(recv_msg_out((void*)&msg,&len) != -1){
            total = msg.len; // total size of data to be sent
            r=0;done=0;
            while (r<total){
                r = pwrite(msg.fd,msg.data,total - done, done);
                if(r==-1){
                    ERR();
                    break;
                }
                done+=r;
            }
        }
    }
}

void close_fd(int fd){
    //TBD .. any other action 
    close(fd);
}
