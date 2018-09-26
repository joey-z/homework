/*************************************************************************
	> File Name: tcp_clientc.c
	> Author: zoe
	> Mail: 
	> Created Time: 2018年09月25日 星期二 19时44分18秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<errno.h>
#include<unistd.h>

#define MAXSIZE 4096
#define IP "192.168.2.68"
#define SERV_PORT 8900

void sys_err(const char *ptr,int num) {
   perror(ptr);
   exit(num);
}

int main(int argc,char **argv) {
    int sockfd;
    char buf[MAXSIZE];
    struct sockaddr_in addr;

    sockfd = socket(AF_INET,SOCK_STREAM,0);
    if(sockfd < 0)
        sys_err("socket",-1);

    //bzero(&addr,sizeof(addr));

    addr.sin_family = AF_INET;
    addr.sin_port = htons(SERV_PORT);
    addr.sin_addr.s_addr = inet_addr(IP);

    if(connect(sockfd,(struct sockaddr *)&addr,sizeof(addr)) < 0)
        sys_err("connect",-2);

    const char *src = argv[1];
    int fd = open(src, O_RDONLY);
    if(fd < 0)
        sys_err("open", -3);

    while(1) {
        int len = read(fd, buf, sizeof(buf));
        if(len == 0)
            break;
        int _tmp = 0;
        while(1) {
            int ret = write(sockfd, buf + _tmp, len - _tmp);
            if(ret > 0 )
                _tmp += ret;
            if(_tmp == ret)
                break;
            if(ret < 0)
                perror("write");
                break;
        }
    }


   close(sockfd);
   return 0;

}
