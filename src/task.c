#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

struct DL_Node
{
    int client_id;
};
typedef struct DL_Node Node;

struct task_Node
{
    int client_id;
};
typedef struct task_Node Client_Node;

Node* insert(int value)
{
     Node *var, *temp;
     var = (Node*)malloc(sizeof(Node));
     if (var == NULL) {
        printf("\nmalloc error");
        return NULL;
     }
     var->client_id = value;
     return var;
}

Client_Node* insert_task(int value)
{
     Client_Node *var, *temp;
     var = (Client_Node*)malloc(sizeof(Node));
     if (var == NULL) {
        printf("\nmalloc error");
        return NULL;
     }
     var->client_id = value;
     return var;
}

void delete (Node * temp)
{
    Node *var;
    if (temp!=NULL)
        free(temp);
}

void delete_task (Client_Node * temp)
{
    Client_Node *var;
    if (temp!=NULL)
        free(temp);
}

struct hash_table
{
    int client_id;
    Node * Client_Addr;
}hash_t[100];

struct task_table
{
    int task_id;
    uint client_count;
    Client_Node * Client_Addr[10];
}task_t[100];

 
void divide_send(int i)
{ 
   
   printf("\ntask id is %d",i);
   printf("\nClient count for task id %d is %d", i, task_t[i].client_count);
}


// A normal C function that is executed as a thread when its name
// is specified in pthread_create()
void *task_Thread(void *task_id)
{
    divide_send(*(int*)task_id);
    
    return NULL;
}

  
int main()
{
    pthread_t tid;
    int i;
    i=55;    
    printf("\nBefore Thread");
    task_t[55].client_count=100;
    pthread_create(&tid, NULL, task_Thread, &i);
    pthread_join(tid, NULL);
    printf("\nAfter Thread\n");
    exit(0);
}
