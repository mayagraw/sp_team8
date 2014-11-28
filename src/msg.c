#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <stdio.h>
#include <errno.h>
#include "../include/msg.h"

msgbuf buf;
#define PRI(A) printf("\n%s %s=%d",__func__,#A,(A))

static int enqueue_msg(int q_no,msgbuf * msg){
    int r=0;
    //basic checks, no error msg will be printed
    if(!msg){return -1;}

    if ( ( r = msgsnd(q_no,msg,buf_len,IPC_NOWAIT) == -1)) {
        //sending msg failed, print error msg
        ERR();
    }
    return r;
}

int create_msgQ(void){
    int r = 0; 
    for(int count = 0 ; count < Q_max ; count ++){
        qkey[count] = ftok(".",count+10) ; 
        r = msgget(qkey[count], 0666 | IPC_CREAT);
        if(r == -1){
            ERR();
            for(int i = count-1 ; i >= 0 ; i --){
                //free the other msg queues
                if(!msgctl(qid[i], IPC_RMID, NULL))
                    ERR();
            }
            return -1;
        } else {
            qid[count]=r;
            PRI(qid[count]);
        }
    }
    buf.mtype = DEF_MTYPE;
    return 0;
}

void delete_msgQ(void){
    for(int i = 0 ; i < Q_max ; i ++){
        if(-1 == msgctl(qid[i], IPC_RMID, NULL)){
            ERR();
        }
    }
}

void map_id_key(void){
    int r = 0; 
    for(int count = 0 ; count < Q_max ; count ++){
        r = msgget(qkey[count], 0666);
        if(r == -1){
            ERR();
        } else {
            qid[count]=r;
            PRI(qid[count]);
        }
    }
}

int send_msg_to_db(void * data){
    if(!data){return -1;}
    buf.data = data;
    return enqueue_msg(qid[Q_db],&buf);
}
int send_msg_to_main(void * data){
    if(!data){return -1;}
    buf.data = data;
    return enqueue_msg(qid[Q_main],&buf);
}
int send_msg_out(void * data){
    if(!data){return -1;}
    buf.data = data;
    return enqueue_msg(qid[Q_out],&buf);
}
int send_msg_to_UI(void * data){
    if(!data){return -1;}
    buf.data = data;
    return enqueue_msg(qid[Q_UI],&buf);
}
int send_msg_to_task(void * data){
    if(!data){return -1;}
    buf.data = data;
    return enqueue_msg(qid[Q_task],&buf);
}
int send_msg_to_log(void * data){
    if(!data){return -1;}
    buf.data = data;
    return enqueue_msg(qid[Q_log],&buf);
}
/*
int main (){
    create_msgQ();
    map_id_key();
    delete_msgQ();
    return 0;
}
*/
