#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include "../include/msgq.h"
#include "../include/common.h"

int qid[Q_max];
key_t qkey[Q_max];
int buf_len = sizeof(msgbuf) - sizeof(long); // = 8000, as set in the struct


static int enqueue_msg(int q_no,void * msg,int len,int mtype){
    //basic checks, no error msg will be printed
    if(!msg){return -1;}
    int r=0;
    static msgbuf buf;
    buf.mtype = (mtype>DEF_MTYPE)?mtype:DEF_MTYPE;
    buf.len=len;
    memcpy(buf.data,msg,len); // use of 'len' is intentional

    if ( ( r = msgsnd(q_no,&msg,buf_len,IPC_NOWAIT) == -1)) {
        //sending msg failed, print error msg
        ERR();
    }
    return r;
}

static int dequeue_msg(int q_no,void * data,int *len,int mtype){
    int r=0;
    static msgbuf buf;

    if (msgrcv(q_no,&buf,buf_len,mtype,0) == -1) {
        //receiving msg failed, print error msg
        ERR();
        return -1;
    }
    memcpy(data,buf.data,buf.len);
    *len = buf.len;
    return 0;
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

int send_msg_to_db(void * data, int len){
    return enqueue_msg(qid[Q_db],data,len,DEF_MTYPE);
}
int send_msg_to_main(void * data, int len){
    return enqueue_msg(qid[Q_main],data,len,DEF_MTYPE);
}
int send_msg_out(void * data, int len){
    return enqueue_msg(qid[Q_out],data,len,DEF_MTYPE);
}
int send_msg_to_UI(void * data, int len){
    return enqueue_msg(qid[Q_UI],data,len,DEF_MTYPE);
}
int send_msg_to_task(void * data, int len){
    return enqueue_msg(qid[Q_task],data,len,DEF_MTYPE);
}
int send_msg_to_log(void * data, int len){
    return enqueue_msg(qid[Q_log],data,len,DEF_MTYPE);
}



int recv_msg_to_db(void * data, int * len){
    return dequeue_msg(qid[Q_db],data,len,DEF_MTYPE);
}
int recv_msg_to_main(void * data, int * len){
    return dequeue_msg(qid[Q_main],data,len,DEF_MTYPE);
}
int recv_msg_out(void * data, int * len){
    return dequeue_msg(qid[Q_out],data,len,DEF_MTYPE);
}
int recv_msg_to_UI(void * data, int * len){
    return dequeue_msg(qid[Q_UI],data,len,DEF_MTYPE);
}
int recv_msg_to_task(void * data, int * len){
    return dequeue_msg(qid[Q_task],data,len,DEF_MTYPE);
}
int recv_msg_to_log(void * data, int * len){
    return dequeue_msg(qid[Q_log],data,len,DEF_MTYPE);
}

/*
int main (){
    create_msgQ();
    map_id_key();
    delete_msgQ();
    return 0;
}
*/
