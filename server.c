#include "common.h" 
#include "db.h"
#include<pthread.h> //for threading , link with lpthread

client_db_node **client_db;
int dbg_flag;
void *connection_handler(void *);
void control_msg_recv(void *msg);

int client_index=0;

struct client_info {
    struct sockaddr_in * client;
    int socket_id;
};

int main(int argc , char *argv[])
{
    int socket_desc , new_socket , c;
    struct sockaddr_in server , client;
    char *message;
    struct client_info info;

    //Initializing the DB
    dbg_flag = DBG_FLAG;
    if (!create_db()) {
	printf("\nFailed to create client db...returning!");
	return -1;
    }
     
    //Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
        printf("Could not create socket");
    }
    
    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( 8888 );
     
    //Bind
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        puts("bind failed");
        return 1;
    }
    puts("bind done");
     
    //Listen
    listen(socket_desc , 3);
     
    //Accept and incoming connection
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);
    while( (new_socket = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c)) )
    {
        puts("Connection accepted");
         
        //Reply to the client
        message = "Hello Client , I have received your connection. And now I will assign a handler for you\n";
        //write(new_socket , message , strlen(message));
         
        pthread_t sniffer_thread;
        info.socket_id = new_socket;
        info.client = &client;
        if( pthread_create( &sniffer_thread , NULL ,  connection_handler , (void *)&info) < 0)
        {
            perror("could not create thread");
            return 1;
        }
         
        //Now join the thread , so that we dont terminate before the thread
        //pthread_join( sniffer_thread , NULL);
        puts("Handler assigned");
    }
     
    if (new_socket<0)
    {
        perror("accept failed");
        return 1;
    }
     
    return 0;
}
/*
 * This will handle connection for each client
 * */
void *connection_handler(void *arg)
{
    //Get the socket descriptor
    struct client_info * info = (struct client_info *)arg ;
    int sock = info->socket_id;
    int read_size , nxt_len;
    char *message , client_message[2000];
    char buffer[2001]="";

    //Receive a message from client
    while( (read_size = recv(sock , client_message , MAX_CTRL_MSG_LEN -1 , 0)) > 0 )
    {
        if(!verify_ctrl_msg(client_message,buffer)){
            printf("\nverify_ctrl_msg failed");
            break;
        }
        nxt_len = my_atoi(buffer);
        if(nxt_len == 0){
            printf("\nExit msg received from %s\n",inet_ntoa(info->client->sin_addr));
            break;
        }
        memset(client_message,'\0',2000);
        if((read_size = recv(sock , client_message , nxt_len , 0)) > 0){
            printf("Msg from %s:%s\n",inet_ntoa(info->client->sin_addr),client_message);
            register_client(info->client->sin_addr.s_addr,atoi(client_message));
            memset(client_message,'\0',2000);
        }
    }

    fflush(stdout);
    print(3);
     
    return 0;
} 
