#include <ev.h>
#include <sys/fcntl.h>
#include "../include/event_manager.h"

static struct ev_loop * l_thread[3];
static ev_io in_w[10];
static ev_timer t_w[3];

extern void keep_receiving(EV_P_ ev_io *, int);

typedef struct{
    int thread_no;
    int fd;
}thread_data;

void assign_fd(int fd){
    //TBD .. write the code !!!
    static int count =-1;
    count++;
    fcntl(fd, F_SETFL, O_NONBLOCK); 
    ev_io_init(&in_w[count],&keep_receiving,fd,EV_READ);
    //ev_io_start(l_thread[fd%3],&in_w[count]);
    ev_io_start(l_thread[0],&in_w[count]);
}

void timer_cb(EV_P_ ev_timer * w, int event){}

void init_thread(void *arg){
    thread_data *data = (thread_data *)arg;
    l_thread[data->thread_no] = ev_loop_new(0);
    ev_timer_init(&t_w[data->thread_no],&timer_cb,100.0,100.0);
    ev_timer_start(l_thread[data->thread_no],&t_w[data->thread_no]);

    ev_run(l_thread[data->thread_no],0);
    //TBD .. close all the sock and destroy the ev data
}
