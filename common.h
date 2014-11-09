#define MAX_CTRL_MSG_LEN 15
#define MAX_DATA_MSG_LEN 999998 // max len that can sent via the ctrl msg - 1. max of 6 digits


int my_atoi(const char *);
int verify_ctrl_msg(char *,char *);
int verify_data_msg(char *,char *);
void append0(char *,char*,int);
int make_ctrl_msg(char* ,char* );
int make_data_msg(char* ,char* );
