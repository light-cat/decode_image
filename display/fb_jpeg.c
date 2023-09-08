#include<stdio.h>
#include<jpeglib.h>
#include<string.h>
#include<stdlib.h>
#include<linux/fb.h>
//想在这个 文件夹里实现 直接显示图片 
#include<fb.h>
#include<config.h>

typedef struct bgr888_color {
 unsigned char red;
 unsigned char green;
 unsigned char blue;
} __attribute__ ((packed)) bgr888_t;


extern unsigned int *screen_base; //映射后的显存基地址

extern struct fb_fix_screeninfo fb_fix;
extern struct fb_var_screeninfo fb_var;

int show_jpeg_image(const char *path)
{
    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;
    FILE *jpeg_file = NULL;
    bgr888_t *jpeg_line_buf = NULL; //行缓冲区:用于存储从 jpeg 文件中解压出来的一行图像数据
    unsigned int *fb_line_buf = NULL; //行缓冲区:用于存储写入到 LCD 显存的一行数据
    unsigned int min_h, min_w;
    unsigned int valid_bytes;
    unsigned int bytes_per_pixel = fb_var.bits_per_pixel / 8;
    unsigned int width = fb_fix.line_length / bytes_per_pixel; // 可以显示多少个像素
    unsigned int *base = screen_base;  // 将显存地址映射到进程空间
    int i;
    //绑定默认错误处理函数
    cinfo.err = jpeg_std_error(&jerr);
    //打开.jpeg/.jpg 图像文件
    jpeg_file = fopen(path, "r"); //只读方式打开
    if (NULL == jpeg_file) {
        perror("fopen error");
        return -1;
    }
    //创建 JPEG 解码对象
    jpeg_create_decompress(&cinfo);
    //指定图像文件
    jpeg_stdio_src(&cinfo, jpeg_file);
    //读取图像信息
    jpeg_read_header(&cinfo, TRUE);
    printf("jpeg 图像大小: %d*%d\n", cinfo.image_width, cinfo.image_height);
    //设置解码参数
    cinfo.out_color_space = JCS_RGB;//默认就是 JCS_RGB
    cinfo.scale_num = 1;
    cinfo.scale_denom = 2;
    //开始解码图像
    jpeg_start_decompress(&cinfo);
    //为缓冲区分配内存空间
    jpeg_line_buf = malloc(cinfo.output_components * cinfo.output_width);

    fb_line_buf = malloc(fb_fix.line_length);

    //判断图像和 LCD 屏那个的分辨率更低
    if (cinfo.output_width > fb_var.xres)
        min_w = fb_var.xres;
    else
        min_w = cinfo.output_width;
    if (cinfo.output_height > fb_var.yres)
        min_h = fb_var.yres;
    else
        min_h = cinfo.output_height;
    //读取数据
    valid_bytes = min_w * bytes_per_pixel;//一行的有效字节数 表示真正写入到 LCD 显存的一行数据的大小
    while (cinfo.output_scanline < min_h) {
        jpeg_read_scanlines(&cinfo, (unsigned char **)&jpeg_line_buf, 1);//每次读取一行数据
        //将读取到的 BGR888 数据转为 RGB888
        for (i = 0; i < min_w; i++)
         fb_line_buf[i] = (jpeg_line_buf[i].red << 16) |
         (jpeg_line_buf[i].green << 8) |
         (jpeg_line_buf[i].blue << 0);

        memcpy(base, fb_line_buf, valid_bytes); //放到 lcd中显示

        base += width;//+width 定位到 LCD 下一行显存地址的起点
    }
    //解码完成
    jpeg_finish_decompress(&cinfo); //完成解码
    jpeg_destroy_decompress(&cinfo);//销毁 JPEG 解码对象、释放资源
    //关闭文件、释放内存
    fclose(jpeg_file);
    free(fb_line_buf);
    free(jpeg_line_buf);
    return 0;
}
