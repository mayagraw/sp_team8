#include "../include/db.h"
#include <stdio.h>
#include <stdlib.h>

client_db_node **client_db;
int dbg_flag;

bool create_db(void)
{
    client_db = (client_db_node **)malloc(MAX_NUM_GROUPS*sizeof(client_db_node*));
    if (client_db == NULL) {
	printf("\nError in malloc");
        return false;
    }
    for (int i=0; i<MAX_NUM_GROUPS; i++) {
	client_db[i] = NULL;
    }

    return true;
}

/*
 *    Add client to client db using register_client API
 *   Input args:
 *	client_db pointer
 *	IP adrs of client ((struct sockaddr_in)client.sin_addr.s_addr)
 *	number of groups client belongs to (int)
 *	array of the groups client belongs to (int array, base adrs.)
 *   Returns TRUE on success.
 */

bool register_client(unsigned long addr, int group)
{
    client_db_node *temp;
    if (client_db[group] == NULL) {
	//If no client is added for this task, add first node!
	temp = (client_db_node *)malloc(sizeof(client_db_node));
	if (temp == NULL) {
	    printf("\nmalloc() error");
	    return false;
	}
	client_db[group] = temp;
	temp->addr = addr;
	temp->next = NULL;
    } else {
	temp = client_db[group];
	while (temp->next != NULL)
	    temp = temp->next;

	temp->next = (client_db_node *)malloc(sizeof(client_db_node));
	if (temp->next == NULL) {
	    printf("\nmalloc() error");
	    return false;
	}
	(temp->next)->addr = addr;
	(temp->next)->next = NULL;
    }
    debug("Client added to DB");

    return true;
}

bool delete_client(unsigned long client_addr)
{
    client_db_node *curr_node, *prev_node;
    bool deleted = false;
    for (int i=0; i<MAX_NUM_GROUPS; i++) {
	curr_node = client_db[i];
	if (curr_node != NULL && curr_node->addr == client_addr) {
	    //first node is to be deleted
	    client_db[i] = curr_node->next;
	    free(curr_node);
	    deleted = true;
	    continue;
	}
	while (curr_node != NULL) {
	    if (curr_node->addr == client_addr) {
		prev_node->next = curr_node->next;
		free(curr_node);
		deleted = true;
		break;
	    }
	    prev_node = curr_node;
	    curr_node = curr_node->next;
	}
    }
    return deleted;
}

void print(int group)
{
    printf("%s called grp=%d",__func__,group);
    client_db_node *node;
    node = client_db[group];
    printf("\nGROUP# %d:",group);
    while (node != NULL) {
        printf("\t%ld", node->addr);
        node = node->next;
    }

    debug("\tEND");
    return;
}

/*
 * Driver funnction for DB
 */
/*
int main()
{
    dbg_flag = DBG_FLAG;
    if (!create_db()) {
	printf("\nFailed to create client db...returning!");
	return -1;
    }
    unsigned long addr;
    int group;

    //Add dummy client 
    addr = inet_addr("1.1.1.1");
    debug("\nAddr = %ld",addr);

    for (int group=0; group<5; group++) {
	if (!register_client(addr, group)) {
	    printf("Failed to add the client to the db");
	}
	debug("\nClient added sucessfully");
    }

    //Add another client
    addr = inet_addr("1.1.1.2");
    debug("\nAddr = %ld",addr);
    for (int group=5; group<10; group++) {
	if (!register_client(addr, group)) {
	    printf("Failed to add the client to the db");
	}
	debug("\nClient added sucessfully");
    }

    //print DB
    for (int i=0; i<10; i++)	print(i);

    //Delete client
    if ( !delete_client(inet_addr("1.1.1.1")) ) {
	printf("\nNo client deleted");
    }

    //print DB
    for (int i=0; i<10; i++)    print(i);

    printf("\n");
    return 0;
}
*/
