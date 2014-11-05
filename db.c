#include <stdio.h>
#include <stdlib.h>
#include "db.h"

client_db_node ** create_db(void)
{
    client_db_node **client_db = (client_db_node **)malloc(MAX_NUM_GROUPS*sizeof(client_db_node*));
    if (client_db == NULL) {
	printf("\nError in malloc");
        return NULL;
    }
    for (int i=0; i<MAX_NUM_GROUPS; i++) {
	client_db[i] = NULL;
    }

    return client_db;
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

bool register_client(client_db_node **client_db, unsigned long addr, int num_groups, int *groups)
{
    client_db_node *temp;
    for (int i=0; i<num_groups; i++) {
	if (client_db[groups[i]] == NULL) {
	    //If no client is added for this task, add first node!
	    temp = (client_db_node *)malloc(sizeof(client_db_node));
	    if (temp == NULL) {
		printf("\nmalloc() error");
		return false;
	    }
	    client_db[groups[i]] = temp;
	    temp->addr = addr;
	    temp->next = NULL;
	} else {
	    temp = client_db[groups[i]];
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
    }

    return true;
}

bool delete_client(client_db_node **client_db, unsigned long client_addr)
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

void print(client_db_node **client_db, int group)
{
    client_db_node *node;
    node = client_db[group];
    printf("\nGROUP# %d:",group);
    while (node != NULL) {
	printf("\t%ld", node->addr);
	node = node->next;
    }

    printf("\nDone printing!");
    return;
}

int main()
{
    client_db_node ** client_db = create_db();
    unsigned long addr;
    int num_groups, *groups;

    //Add dummy client 
    num_groups = 5; 
    groups = (int *)malloc(num_groups*sizeof(int));
    for (int i=0; i<num_groups; i++)	groups[i] = i;
    addr = inet_addr("1.1.1.1");
    printf("\nAddr = %ld",addr);
    if (!register_client(client_db, addr, num_groups, groups)) {
	printf("Failed to add the client to the db");
    }
    printf("\nClient added sucessfully");

    //Add another client
    addr = inet_addr("1.1.1.2");
    printf("\nAddr = %ld",addr);
    if (!register_client(client_db, addr, num_groups, groups)) {
	printf("Failed to add the client to the db");
    }
    printf("\nClient added sucessfully");

    free(groups);

    //print DB
    for (int i=0; i<num_groups; i++)	print(client_db, i);

    //Delete client
    if ( !delete_client(client_db, inet_addr("1.1.1.3")) ) {
	printf("\nNo client deleted");
    }

    //print DB
    for (int i=0; i<num_groups; i++)    print(client_db, i);

    printf("\n");
    return 0;
}
