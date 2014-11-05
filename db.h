#include <arpa/inet.h>
#include <stdbool.h>

#define MAX_NUM_GROUPS 100      //i.e. number of tasks
#define MAX_NUM_CLIENTS_PER_GROUP 20    //max number of clients which can execute certain task
#define MAX_NUM_GROUPS_PER_CLIENT 10
#define MAX_NUM_CLIENTS 1000

typedef struct client_db_node_ {
    unsigned long addr;
    struct client_db_node_ *next;
} client_db_node;


client_db_node ** create_db(void);
bool register_client(client_db_node **, unsigned long, int, int *);
bool delete_client(client_db_node **, unsigned long);
void print(client_db_node **, int);
