#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
int main(void)
{
    int ret;
    int fd = 0;
    char buf[2] = {0};
    char buf1[2] = {0};
    char* path = "./test.txt";
    fd = open(path,O_RDONLY);
    if(fd<0)
    {
        perror("open failed:");
    }
    ret = read(fd,buf,2);
    printf("%d,%s\n",ret,buf);

    ret = read(fd,buf1,2);
    printf("%d,%s\n",ret,buf1);

    memset(buf,0,sizeof(buf));
    ret = read(fd,buf,2);
    printf("%d,%s\n",ret,buf);
    // 如果在读取过程中，不使用lseek调整fd的位置，那么就会一直递增下去

    // SEEK_SET 每次执行 都是 从文件头开始的
    // lseek(fd,2,SEEK_SET);
    // read(fd,buf,2);
    // printf("%s\n",buf);
    // lseek(fd,2,SEEK_SET);
    // read(fd,buf,2);
    // printf("%s\n",buf);
    
    // SEEK_CUR是从当前文件指针处开始的
    // lseek(fd,2,SEEK_CUR);
    // read(fd,buf,2);
    // printf("%s\n",buf);

}