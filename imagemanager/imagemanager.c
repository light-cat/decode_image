#include "imagemanager.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/input.h>

#include <config.h>
#include <fb.h>

extern struct pic_data imagesoure[100];

void ts_updown(){
    struct input_event ev;
    int fd = 0;
    int ret;
    int i = 0;//图像索引
    fd = open(TOUCHDEVICE,O_RDONLY);

    if(fd<0){
        perror("open touchscreen failed:");
        exit(-1);
    }
    // 按键内容
    while(1){
     //   debug("test touchscreen\n");
        memset(&ev,0,sizeof(struct input_event));
        ret = read(fd,&ev,sizeof(struct input_event));
        if(ret < 0){
            perror("read touchscreen failed:");
            goto close;
            exit(-1);
        }
      //  debug("open test touchscreen\n");

        // 判断是不是触摸屏类型，是不是横坐标
        if((ev.type == EV_ABS) && (ev.code == ABS_MT_POSITION_X)){
            debug("touch screen is button\n");
            if(ev.value < 150){
             
                i--;
                if(i<0)
                i = 1;
            }
            if(ev.value > 600){
                i++;
                if(i>1)
                i = 0;
            }
        }
        show_jpeg_image(imagesoure[i].path);
      
    }

close:
    close(fd);
}