#include "BMP.hpp"
#include <cstdio>
#include <iostream>
 
#include <cstdlib>
using namespace std;

unsigned int read_int(int fd, int offset, int size) {
	unsigned int num;
	lseek(fd, offset ,SEEK_SET);
	read(fd, &num, size);
	return num;
}

BMPImage::BMPImage(char * path){  //Reads file and loads to buffer
	int fd = open(&path[0], O_RDONLY);
	this->Header = this->Init(fd);
	
	this->Buffer = (char *)malloc(this->Size*(this->BitCount/8));
	
	lseek(fd, this->DataOffset, SEEK_SET);  //Go to data start
	
	read(fd, &this->Buffer[0], this->Size*(this->BitCount/8));				//read data to buffer
	close(fd);
	
	this->X = this->Y = 0;
}

BMPImage::~BMPImage(){
	munmap(this->Buffer,this->Size*(this->BitCount/8));
}

BITMAPFILEHEADER BMPImage::Init(int fd){ //Fills struct of header
	struct BITMAPFILEHEADER header;
	if(fd > 0) {			
		header.biSize = read_int(fd, BMPINFO, 4);
		header.bfOffBits = read_int(fd, BMPDATAOFFSET, 4);
		header.biWidth = read_int(fd, BMPWIDTH, 4);
		header.biHeight = read_int(fd, BMPHEIGHT, 4);
		header.biBitCount = read_int(fd, BMPBITCOUNT, 2);			
	}
	this->Size = header.biWidth*header.biHeight;
	this->DataOffset = header.bfOffBits;
	this->BitCount = header.biBitCount;
	this->Width = header.biWidth;
	this->Height = header.biHeight;		
	
	return header;
}
