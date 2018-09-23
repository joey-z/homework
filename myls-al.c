/*************************************************************************
	> File Name: myls-al.c
	> Author: zoe
	> Mail: 
	> Created Time: 2018年09月22日 星期六 18时02分20秒
 ************************************************************************/
//用c语言实现ls-al功能

#include <stdio.h>
#include <time.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <unistd.h>
#include <string.h>

//需要获取隐藏文件 
//权限显示
//文件夹还是文件 文件夹中有几个文件 文件夹是蓝色 可执行文件为绿色 777文件为红色
//所有者 所属组
//c语言中日期实现 x月 
//建立时间
//文件名 
//输出顺序不同 无法输入路径
void my_err(const char *s,int line) { 
    fprintf(stderr,"line: %d ",line); 
    perror(s); 
    _exit(1);  
} 

int main() { 
    //char file_name[100]; 
    //char time[100]; 
    int u, g, o, i, s; 
    int a[3];
    //long b = 0;
    DIR * dirp;
    struct dirent * dp;
    //Dir* dir_ptr;
    //struct dirent * direntp; //目录指针
    struct passwd *psd; 
    struct group *grp; 
    struct stat buf; 

    //printf("请输入文件或目录：\n"); 
    //scanf("%s",file_name);
    //DIR * opendir(const char * file_name);
    //struct dirent * readdir(DIR *);
    dirp = opendir(".");
    while ((dp = readdir(dirp)) != NULL) {
       // b += buf.st_size;
        if(stat(dp->d_name,&buf) == -1)  
            my_err("stat",__LINE__);  
        if(S_ISLNK(buf.st_mode)) 
            printf("l"); //链接文件
        else if(S_ISREG(buf.st_mode)) 
            printf("-"); //普通文件
        else if(S_ISDIR(buf.st_mode)) 
            printf("d"); //目录
        else if(S_ISCHR(buf.st_mode)) 
            printf("c"); //字符设备
        else if(S_ISBLK(buf.st_mode)) 
            printf("b"); //块设备
        else if(S_ISFIFO(buf.st_mode)) 
            printf("p"); //管道
        else if(S_ISSOCK(buf.st_mode)) 
            printf("s"); //套接口文件 

    s=buf.st_mode; 
    o=s%8; 
    g=s/8%8; 
    u=s/8/8%8; 
    a[0]=u;
    a[1]=g;
    a[2]=o; 
    for(i=0;i<3;i++) { 
        switch(a[i]) { 
            case 0:
                printf("---");
                break; 
            case 1:
                printf("--x");
                break; 
            case 4:
                printf("r--");
                break; 
            case 2:
                printf("-w-");
                break; 
            case 3:
                printf("-wx");
                break; 
            case 5:
                printf("r-x");
                break; 
            case 6:
                printf("rw-");
                break; 
            case 7:
                printf("rwx");
            break;  
        }  
    } 

    psd=getpwuid(buf.st_uid); 
    grp=getgrgid(buf.st_gid); 
    printf("  %-4ld ",buf.st_nlink); 
    printf("%-6s",psd->pw_name); //所有者
    printf("%-6s",grp->gr_name); //所属组
    printf("%6ld ",buf.st_size); //文件大小
    printf("%.12s ",4 + ctime(&buf.st_mtime)); //时间
    //printf("%s ", time);
    if(S_ISDIR(buf.st_mode)) { 
        printf("\033[1m%.12s\33[0m\\\n", dp->d_name);
    } else if (a[1] % 2 == 0){
        printf("%.12s\n", dp->d_name);
    } else if (a[2] == 7) {
        printf("\033[41m%.12s\n\033[0m", dp->d_name);
    } else {
        printf("\033[32m%.12s\33[0m", dp->d_name);
        printf("*\n");
    }
    }
//printf("总用量 %ld", b);
    (void) closedir(dirp);
    return 0;
}


