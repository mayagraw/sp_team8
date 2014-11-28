#ifndef MSGH_CHECK
#define MSGH_CHECK

enum {Q_db=0,
    Q_out,
    Q_main,
    Q_UI,
    Q_task,
    Q_log,
    Q_max};//add the new queue before this line

int qid[Q_max];
key_t qkey[Q_max];

#define DEF_MTYPE 1;
typedef struct {
    long mtype;
    void * data;
} msgbuf;

int buf_len = sizeof(msgbuf) - sizeof(long);

char do_not_use_this[2048];
#define ERR() perror((sprintf(do_not_use_this,"Error(%d) in %s::%s::%d",errno,__FILE__,__func__,__LINE__),do_not_use_this));                                     

extern int send_msg_to_db(void * data);
extern int send_msg_to_main(void * data);
extern int send_msg_out(void * data);
extern int send_msg_to_UI(void * data);
extern int send_msg_to_task(void * data);
extern int send_msg_to_log(void * data);

#endif
