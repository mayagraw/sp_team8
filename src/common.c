#include "common.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdbool.h>
#include <ctype.h>

char CTRL_MAGIC_START_STR[] = "$$@@";
char CTRL_MAGIC_END_STR[] = "##@@";
char DATA_MAGIC_START_STR[] = "!!@@";
char DATA_MAGIC_END_STR[] = "##!!";

int CTRL_LEN = 4; // length of CTRL_MAGIC_START_STR and CTRL_MAGIC_END_STR
int DATA_LEN = 4; // length of DATA_MAGIC_START_STR and DATA_MAGIC_END_STR

void validate_ip_str(char * str){
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
    strcpy(data_str,DATA_MAGIC_START_STR);
    strcat(data_str,str);
    strcat(data_str,DATA_MAGIC_END_STR);
    return 0;
}

int make_ctrl_msg(char* str,char* ctrl_len){
    int len=strlen(str);
    if(!len) return -1;
    char temp_str[MAX_CTRL_MSG_LEN + 1]="";
    memset(ctrl_len,'0',MAX_CTRL_MSG_LEN);
    strcpy(ctrl_len,CTRL_MAGIC_START_STR);
    if((len + CTRL_LEN + CTRL_LEN) < MAX_CTRL_MSG_LEN){
        append0(str,temp_str,(MAX_CTRL_MSG_LEN - CTRL_LEN - CTRL_LEN-len-1));
        strcat(ctrl_len,temp_str);
    } else {
        strncat(ctrl_len,str,MAX_CTRL_MSG_LEN - CTRL_LEN - CTRL_LEN);
    }
    strcat(ctrl_len,CTRL_MAGIC_END_STR);
    return 0;
}

        
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

int verify_ctrl_msg(char *input,char *op){
    int len=strlen(input);
    int flag=0;
    if(!len) return flag;
    if(len<=(CTRL_LEN + CTRL_LEN)){
        return flag;
    }
    if(strncmp(input,CTRL_MAGIC_START_STR,CTRL_LEN)==0){
        if(strncmp(&input[len-CTRL_LEN],CTRL_MAGIC_END_STR,CTRL_LEN)==0){
            strncpy(op,&input[CTRL_LEN],len-CTRL_LEN-CTRL_LEN);
            flag=1;
        }
    }
    return flag;
}

int verify_data_msg(char *input,char *op){
    int len=strlen(input);
    int flag=0;
    if(!len) return flag;
    if(len<=(DATA_LEN + DATA_LEN)){
        return flag;
    }
    if(strncmp(input,DATA_MAGIC_START_STR,DATA_LEN)==0){
        if(strncmp(&input[len-DATA_LEN],DATA_MAGIC_END_STR,DATA_LEN)==0){
            strncpy(op,&input[DATA_LEN],len-DATA_LEN-DATA_LEN);
            flag=1;
        }
    }
    return flag;
}
