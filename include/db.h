
#ifndef DBG 
#define DBG 
#include <arpa/inet.h>
#include <stdbool.h>

#define MAX_NUM_GROUPS 100      //i.e. number of tasks
#define MAX_NUM_CLIENTS_PER_GROUP 20    //max number of clients which can execute certain task
#define MAX_NUM_GROUPS_PER_CLIENT 10
#define MAX_NUM_CLIENTS 1000

#define DBG_FLAG 1
extern int dbg_flag;
#define debug if(dbg_flag) printf

typedef struct client_db_node_ {
    unsigned long addr;
    struct client_db_node_ *next;
} client_db_node;

extern client_db_node **client_db; //Client db global pointer

extern bool create_db(void);
extern bool register_client(unsigned long, int);
bool delete_client(unsigned long);
void print(int);
#endif
