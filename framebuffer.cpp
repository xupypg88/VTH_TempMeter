/*
        
*/
#include <iostream>
#include <sstream>
#include <cstring>
#include <linux/input.h>
#include <string>
#include <fcntl.h>
#include <inttypes.h>
#include <unistd.h>	/* close() */
#include <sys/stat.h>	/* S_IXXX flags */
#include <stdlib.h>
#include <linux/fb.h>
#include <sys/types.h>	
#include <cstdio>
#include <sys/mman.h>
//#include "font_8x8.c"


#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"
#define BITMAPV4HEADER 108
#define BITMAPV5HEADER 124
#define BMPDATAOFFSET 0xA
#define BMPINFO 0xE
#define BMPWIDTH 0x12
#define BMPHEIGHT 0x16
#define BMPBITCOUNT 0x1C

	static struct fb_fix_screeninfo fix;
	static struct fb_var_screeninfo orig_var;
	static struct fb_var_screeninfo var;
	char *fbp = 0;
	char *fbpback= 0;
	int fb=0;
	long int screensize = 0;
	
struct ptColor {
	unsigned char R,
	G, 
	B;
};

struct BITMAPFILEHEADER {
	int bfOffBits,
	biSize,		//Size BMPINFO struct - type of bmp
	biWidth,
	biHeight,
	biBitCount;
};

unsigned int read_int16 (int fd, int offset) {
	unsigned int num;
	lseek(fd, offset ,SEEK_SET);
	read(fd, &num, 2);
	return num;
}

unsigned int read_int (int fd, int offset) {
	unsigned int num;
	lseek(fd, offset ,SEEK_SET);
	read(fd, &num, 4);
	return num;
}

int read_pixel24 (int fd) {
	unsigned int num;
	read(fd, &num, 3);
	return num;
}

unsigned int read_pixel32 (int fd) {
	unsigned int num;
	read(fd, &num, 4);
	return num = (num >> 8);
}

struct BITMAPFILEHEADER init_bmp(char * path) {
	struct BITMAPFILEHEADER header;
	
	int fd = open(path, O_RDWR);
	
	header.bfOffBits = read_int(fd, BMPDATAOFFSET);
	header.biSize = read_int(fd, BMPINFO);
	header.biWidth = read_int(fd, BMPWIDTH);
	header.biHeight = read_int(fd, BMPHEIGHT);
	header.biBitCount = read_int16(fd, BMPBITCOUNT);
	
	close(fd);
	
	return header;
}

void put_pixel_int(int x, int y, int c)
{
	//if((c << 24) != 0x00) {
        unsigned int pix_offset;

        // calculate the pixel's byte offset inside the buffer
        pix_offset = x*4 + y * fix.line_length;

        // write 'two bytes at once'
        *((unsigned int*)(fbpback + pix_offset)) = c;
      // }
}

int draw_bmp(int x, int y, char * image) {
	
	struct BITMAPFILEHEADER header = init_bmp(image);

	int fd = open(image, O_RDWR);
	int xpos, ypos;
	
	if((header.biHeight + y) > 479)
		y = 479 - header.biHeight;
	if((header.biWidth + x) > 799)
		x = 800 - header.biWidth;
	
	lseek(fd, header.bfOffBits, SEEK_SET); //move to data start
	
	
	if (header.biBitCount == 24) {
		//printf("printing 24bit\n");
		for ( ypos = header.biHeight; ypos > 0; ypos--) {
			for ( xpos = 0; xpos < header.biWidth; xpos++) {
				put_pixel_int( xpos + x, ypos + y - 1,read_pixel24(fd));
			}
		}
	} else {
		//printf("printing 32bit\n");
		for ( ypos = header.biHeight; ypos > 0; ypos--) {
				for ( xpos = 0; xpos < header.biWidth; xpos++) {
					put_pixel_int( xpos + x, ypos + y, read_pixel32(fd));
				}
		}
	}
	
	//memcpy(fbp, fbpback, 1536000);
	
	close(fd);
}

void refreshfb() {
	memcpy(fbp, fbpback, 1536000);
}

void put_pixel_32bpp(int x, int y, int r, int g, int b)
{
	
        unsigned int pix_offset;
        unsigned int c;

        // calculate the pixel's byte offset inside the buffer
        pix_offset = x*4 + y * fix.line_length;

        //some magic to work out the color
        c = (r << 16) + (g << 8) + b;

        // write 'two bytes at once'
        *((unsigned int*)(fbp + pix_offset)) = c;
}

int getColor(int r, int g, int b) {   //Make color 32 int from 3 ints or chars of RGB 
	unsigned int color;
	
	color = (r << 16) + (g << 8) + b;
	return color;
}


void drawSquare(int x, int y,int width, int height, int c)
{
	printf( "Start drawing square...\n");

	int h = 0;
	int w = 0;
	
	for ( w = 0; w< width; w++)
		for ( h = 0; h< height; h++)
			put_pixel_int( w+(x-2), h+(y-2) , c);

}

void clearBuffer(int xres, int yres) {
        memset(fbp, 0, xres*yres*4);
}

int framebufferInitialize(int *xres, int *yres)
{
	char *fbdevice = "/dev/fb0" ;  //main buffer - LCD screen

	fb = open(fbdevice, O_RDWR);
	if (fb == -1) {
		perror("open fbdevice");
	return -1;
	}

	if (ioctl(fb, FBIOGET_FSCREENINFO, &fix) < 0) {
		perror("ioctl FBIOGET_FSCREENINFO");
		close(fb);
	return -1;
	}

	if (ioctl(fb, FBIOGET_VSCREENINFO, &var) < 0) {
		perror("ioctl FBIOGET_VSCREENINFO");
	close(fb);
	return -1;
	}

	printf("Original %dx%d, %dbpp\n", var.xres, var.yres, 
         var.bits_per_pixel );

	memcpy(&orig_var, &var, sizeof(struct fb_var_screeninfo));



	printf("Framebuffer %s%s%s resolution;\n",KYEL, fbdevice, KWHT);

  	printf("%dx%d, %d bpp\n\n\n", var.xres, var.yres, var.bits_per_pixel );

	// maping
	// map back framebuffer to user memory 
	screensize = fix.smem_len;
    fbpback = (char*) malloc(screensize);
    // map framebuffer to user memory                 
	fbp = (char*)mmap(0, 
                    screensize, 
                    PROT_READ | PROT_WRITE, 
                    MAP_SHARED, 
                    fb, 0);
	/*if (fbp == -1) {
	printf("Failed to mmap.\n");
	}*/
	
	*xres = var.xres;
	*yres = var.yres;
	//clear framebuffer for debug
	//clearBuffer(var.xres, var.yres);
	
	printf("Init done!\n");

}

void closeFramebuffer()
{
	munmap(fbp, screensize);
	if (ioctl(fb, FBIOPUT_VSCREENINFO, &orig_var)) {
		printf("Error re-setting variable information.\n");
	}
	close(fb);
}
