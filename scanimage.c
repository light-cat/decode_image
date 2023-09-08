#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>

#include <string.h>

// 图片类型种类
enum imagetype{
    BMP,
    JPG,
    PNG
};

// 图片结构体
struct pic_data{
    char path[100]; // 不能使用char *path;会报段错误
    enum imagetype type;
};

// 存放图片路径的数组
// 最多只能存放100张图片
struct pic_data imagesoure[100];
int image_index=0; // 图片的索引


char* path = "./images";
void scanimage(const char* path){

    DIR *dir;
    struct dirent* p;
    char base[1000]; // 存储遍历地址的数组
    // 1.首先获得一个文件夹
    if((dir = opendir(path)) == NULL){
        perror("opendir failed\n");
        exit(-1);
    }
    // 2. 然后打开一个文件夹  把值赋给  结构体 dirent
    // 3. 返回的是个dirent索引，如何根据索引找到文件的地址
    while ((p = readdir(dir)) != NULL){
        // 注意，必须加上这些  因为. ..也是文件夹
        if(!strcmp(p->d_name,".") || !strcmp(p->d_name,".."))
            continue;
        // 将遍历到的路径存放到base数组中
        // 存放东西之前，需要把这个base清零
        memset(base,0,sizeof(base));
        strcpy(base,path); // 先把根目录存放起来
        strcat(base,"/"); //  添加 / 
        strcat(base,p->d_name); // 然后再把内容添加进来
     
        if(p->d_type == DT_REG){
            strcpy(imagesoure[image_index].path, base);
            image_index++;
        }

        if(p->d_type == DT_DIR){
            scanimage(base); 
        }
        // scanimage(p->d_name); 只传输这个地址是不够的，需要传输工程的相对路径
    }
}

int main(void){
    scanimage(path);
    printf("end end end!\n");
    for(int i=0;i<sizeof(imagesoure) / sizeof(struct pic_data);i++){
        printf("%s\n",imagesoure[i].path);
    }
}