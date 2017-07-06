#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include <cstring>
#include <linux/fb.h>
#include <fcntl.h>
#include <unistd.h>
#include <string>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include "BMP.hpp"
#include "standard.hpp"

#define BACKCOLOR 0

using namespace std;

class LCD_Screen {	
	private:
		struct fb_fix_screeninfo finfo;
		struct fb_var_screeninfo vinfo;
		char * Buffer;
		char * Plane;
		
	public:
		int Device;
		long BufferSize;
		LCD_Screen(string dev_name);

		~LCD_Screen();
		
		int GetWidth();
		int GetHeight();
		
		void SetBkLight(int lev);
		
		bool Clear();
		
		int MakeColor(char r, char g, char b);
		
		void DrawPoint(int x, int y, int color);
		void DrawImage(BMPImage * image);
		void DrawImageDirect(BMPImage * image);
		
		void Flip();
};


class Element {
	public:
		int X, Y , Width, Height;
};
