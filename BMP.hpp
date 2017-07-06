#include <stdlib.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <fstream>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string>
#include <cstring>
#include <iostream>

#define BITMAPV4 108
#define BITMAPV5 124
#define BMPDATAOFFSET 0xA
#define BMPINFO 0xE
#define BMPWIDTH 0x12
#define BMPHEIGHT 0x16
#define BMPBITCOUNT 0x1C

struct BITMAPFILEHEADER {
	int bfOffBits,
	biSize,		//Size BMPINFO struct - type of bmp
	biWidth,
	biHeight,
	biBitCount;
};

class BMPImage {
		BITMAPFILEHEADER Header;
				
	public:
		int X, Y;
		char * Buffer;
		unsigned int Size;
		unsigned int Width, Height, BitCount, DataOffset;
		
		BMPImage(char * path);
		~BMPImage();
		BITMAPFILEHEADER Init(int fd);
		void Reload();
};
