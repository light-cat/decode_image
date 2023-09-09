
#ifndef  __FB__
#define  __FB__
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

int fb_open(void);
int show_jpeg_image(const char *path);


void ts_updown();
#endif // 

