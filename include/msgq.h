#ifndef MSGH_CHECK
#define MSGH_CHECK
#include <sys/types.h>

#define DEF_MTYPE 1

extern int errno;

enum {Q_db=0,
    Q_out,
    Q_main,
    Q_UI,
    Q_task,
    Q_log,
    Q_max};//add the new queue before this line

typedef struct {
    long mtype;
    int len;
    char data[8000]; // MAX is 8192  // TBD optimize
} msgbuf;

extern int buf_len; 
extern int qid[Q_max];
extern key_t qkey[Q_max];
extern msgbuf buf;
extern char do_not_use_this[];

extern int send_msg_to_db(void * ,int);
extern int send_msg_to_main(void * ,int);
extern int send_msg_out(void * ,int);
extern int send_msg_to_UI(void * ,int);
extern int send_msg_to_task(void * ,int);
extern int send_msg_to_log(void * ,int);

extern int recv_msg_to_db(void * ,int *);
extern int recv_msg_to_main(void * ,int *);
extern int recv_msg_out(void * ,int *);
extern int recv_msg_to_UI(void * ,int *);
extern int recv_msg_to_task(void * ,int *);
extern int recv_msg_to_log(void * ,int *);

#endif
