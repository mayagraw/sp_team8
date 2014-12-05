#include <string.h>
#include <arpa/inet.h>
#include "../include/common.h"

const char MAGIC_START_STR[] = "$$@@";
const char MAGIC_END_STR[] = "##@@";

const int MAGIC_LEN = 4; // length of MAGIC_START_STR and MAGIC_END_STR
const int MAX_CTRL_MSG_LEN = sizeof(mheader) + 2*MAGIC_LEN;

/*
   Below is struct of mheader
   struct{
   uint16_t ctype;    // 4 bytes
   uint16_t client_id;// 4 bytes 
   uint32_t data;     // 8 bytes
   } mheader;
*/


static ulong int_from_8char(uchar *ip){
    ulong op=0;
    for(int i=0;i<8;i++){
        op+=ip[i];
        if(i<7)op<<=8;
    }
    return op;
}

static void int_to_8char(ulong ip,uchar *op){
    for(int i=7;i>=0;i--){
        op[i]=ip;
        ip>>=8;
    }
}
static uint int_from_4char(uchar *ip){
    uint op=0;
    for(int i=0;i<4;i++){
        op+=ip[i];
        if(i<3)op<<=8;
    }
    return op;
}

static void int_to_4char(uint ip,uchar *op){
    for(int i=3;i>=0;i--){
        op[i]=ip;
        ip>>=8;
    }
}

void deserialize_mheader(uchar * ip,mheader *op){
    ulong tmp;
    op->ctype = ntohl(int_from_4char(ip));
    op->client_id = ntohl(int_from_4char(&ip[4]));

    tmp = int_from_8char(&ip[8]);
    //ntohl works on 4bytes, long is 8 bytes. Hence the below code.
    op->data = (ulong)ntohl((uint)tmp) + (((ulong)ntohl((uint)(tmp>>32)))<<32) ;
}

void serialize_mheader(mheader *ip,uchar *op){
    if(!op){return;}
    ulong tmp;
    int_to_4char(htonl(ip->ctype),op);
    int_to_4char(htonl(ip->client_id),&op[4]);
    //htonl works on 4bytes, long is 8 bytes. Hence the below code.
    tmp = (ulong)htonl((uint)ip->data) + (((ulong)htonl((uint)((ip->data)>>32)))<<32) ;
    int_to_8char(tmp,&op[8]);
}    

int xtract_mheader(uchar *input,mheader * op){
    const int len=MAX_CTRL_MSG_LEN;
    int flag=0;
    // we can use sizeof mheader here cos the struct alignment matches the word size of the machine, 
    // since we use only int and long,else we would have to calculate the size
    uchar data[sizeof(mheader)]; 
    if(len<=(MAGIC_LEN + MAGIC_LEN)){
        return flag;
    }
    if(strncmp((char *)input,MAGIC_START_STR,MAGIC_LEN)==0){
        if(strncmp((char *)(&input[len-MAGIC_LEN]),MAGIC_END_STR,MAGIC_LEN)==0){
            memcpy(data,&input[MAGIC_LEN],sizeof(mheader));
            deserialize_mheader(data,op);
            flag=1;
        }
    }
    return flag;
}

void make_mheader_str(mheader *ip,uchar *op){
    if(!op)return;

    memcpy(op,MAGIC_START_STR,MAGIC_LEN);
    serialize_mheader(ip,&op[4]);
    // we can use sizeof mheader here cos the struct alignment matches the word size of the machine, 
    // since we use only int and long,else we would have to calculate the size
    memcpy(&op[sizeof(mheader)+MAGIC_LEN],MAGIC_START_STR,MAGIC_LEN);
}
/*        
void append0(char *str,char* dst,int t){
    memset(dst,'0',MAX_CTRL_MSG_LEN);
    memcpy(&dst[t],str,(MAX_CTRL_MSG_LEN-t));
}
    

int my_atoi(const char * str){                                
    int len = strlen(str);                                    
    int i,value=0;
    bool read=false;                                          
    for(i=0;i<len;i++){
        if(!read){
            if(str[i]=='0'){continue;}                        
            else read=true; 
        }                                                     
        value = value*10 + str[i]-'0' ;                       
    }
    return value;
}

int verify_data_msg(char *input,char *op){
    int len=strlen(input);
    int flag=0;
    if(!len) return flag;
    if(len<=(MAGIC_LEN + MAGIC_LEN)){
        return flag;
    }
    if(strncmp(input,MAGIC_START_STR,MAGIC_LEN)==0){
        if(strncmp(&input[len-MAGIC_LEN],MAGIC_END_STR,MAGIC_LEN)==0){
            strncpy(op,&input[MAGIC_LEN],len-MAGIC_LEN-MAGIC_LEN);
            flag=1;
        }
    }
    return flag;
}

void validate_snd_str(char * str){
    if(!str)return;
    int len = strlen(str);
    if(!len)return;
    int indx1=0,indx2=0;
    while(indx2<len){
        if(isalnum(str[indx2]) || isblank(str[indx2])){
            if(indx2>indx1){
                str[indx1]=str[indx2];
            }
            indx1++;
            indx2++;
        } else {
            indx2++;
        }
    }//end of while
    str[indx1]='\0';
}
int make_data_msg(char* str,char* data_str){
    int len=strlen(str);
    if(!len) return -1;
    if(len > MAX_DATA_MSG_LEN) {
	    //printf("\n%s string len exceeds the capacity",__func__);
	    return -2;
    }
    memset(data_str,'0',MAX_DATA_MSG_LEN);
    strcpy(data_str,MAGIC_START_STR);
    strcat(data_str,str);
    strcat(data_str,MAGIC_END_STR);
    return 0;
}

int make_ctrl_msg(char *op,mheader * ip){
    const int len=sizeof(mheader);
    if(!len) return -1;
    char temp_str[MAX_CTRL_MSG_LEN + 1]="";
    //memcpy too would have worked here
    sprintf("");
    memcpy(op,MAGIC_START_STR,MAGIC_LEN);
    memcpy(&op[MAGIC_LEN],ip,len);
    memcpy(&op[len+MAGIC_LEN],MAGIC_END_STR,MAGIC_LEN);
    return 0;
}
*/
