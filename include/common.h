#ifndef COMMONH_CHECK
#define COMMONH_CHECK
#include "../include/socket.h"

char do_not_use_this[2048];
#define ERR() perror((sprintf(do_not_use_this,"Error(%d) in %s::%s::%d",errno,__FILE__,__func__,__LINE__),do_not_use_this));                                     
#define ERRSTR(A) perror((sprintf(do_not_use_this,"Error(%d) in %s::%s::%d\n\t\t%s",errno,__FILE__,__func__,__LINE__,A),do_not_use_this));                                     
#define PRI(A) printf("\n%s %s=%d",__func__,#A,(A))

#define MAX_DATA_MSG_LEN 999998 // max len that can sent via the ctrl msg - 1. max of 6 digits

typedef unsigned int uint;
typedef unsigned long int ulong;
typedef unsigned char uchar;

extern const int MAGIC_LEN;
extern const int MAX_CTRL_MSG_LEN;

void deserialize_mheader(uchar *,mheader *);
void serialize_mheader(mheader *,uchar *);
int xtract_mheader(uchar *,mheader *);

/*
int verify_data_msg(char *,char *);
void append0(char *,char*,int);
int make_ctrl_msg(char* ,char* );
int make_data_msg(char* ,char* );
void validate_snd_str(char * );
int my_atoi(const char *);
*/



#endif
