#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <unistd.h>


#include <fb.h>
#include <config.h>

unsigned int *screen_base = NULL; //映射后的显存基地址

struct fb_fix_screeninfo fb_fix;
struct fb_var_screeninfo fb_var;

//这个函数的作用就是将显存映射到进程中来
int fb_open(void)
{
	 int ret = -1;
	 int fbfd = 0;

	//  struct fb_fix_screeninfo fb_fix;
    //  struct fb_var_screeninfo fb_var;
	 // 第1步：打开设备
	 fbfd = open(FBDEVICE, O_RDWR);
	 if (fbfd < 0)
	 {
		 perror("open error");
		 return -1;
	 }
	// debug("open %s success.\n", FBDEVICE);
	 
	 // 第2步：获取设备的硬件信息
	 ret = ioctl(fbfd, FBIOGET_FSCREENINFO, &fb_fix);
	 if (ret < 0)
	 {
		 perror("ioctl");
		 return -1;
	 }
	//debug("smem_start = 0x%lx, smem_len = %u.\n", fb_fix.smem_start, fb_fix.smem_len);
	 
	 ret = ioctl(fbfd, FBIOGET_VSCREENINFO, &fb_var);
	 if (ret < 0)
	 {
		 perror("ioctl");
		 return -1;
	 }
	debug("xres = %u, yres = %u.\n", fb_var.xres, fb_var.yres);
	debug("xres_virtual = %u, yres_virtual = %u.\n", fb_var.xres_virtual, fb_var.yres_virtual);
	debug("bpp = %u.\n", fb_var.bits_per_pixel);
 
	 
	 // 第3步：进行mmap
	 unsigned long len = fb_var.xres_virtual * fb_var.yres_virtual * fb_var.bits_per_pixel / 8;
	// debug("len = %ld\n", len);
	 screen_base = mmap(NULL, len, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);
	 if (NULL == screen_base)
	 {
		 perror("mmap error");
		 return -1;
	 }
	// debug("pfb = %p.\n", pfb);

	 return 0;
	 
}