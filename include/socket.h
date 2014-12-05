#ifndef SOCKETH_CHECK
#define SOCKETH_CHECK
#include <ev.h>
//Any change in this struct should be matched with changes in
// deserialize_mheader, serialize_mheader and corresponsing function
typedef struct{
    unsigned int ctype;
    unsigned int client_id;
    unsigned long data;
} mheader;

typedef struct{
    int fd;
    unsigned char data[8000];
    int len;
} msgq_out;

void keep_accepting(void);
void keep_receiving(EV_P_ ev_io , int);
void keep_sending(void);
void close_fd(int);
#endif
