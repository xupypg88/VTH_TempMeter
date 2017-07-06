#include "LCD.hpp"

using namespace std;


LCD_Screen::LCD_Screen(string dev_name) {
	this->Device = open(&dev_name[0], O_RDWR);
	
	ioctl(this->Device, FBIOGET_VSCREENINFO, &this->vinfo);
	ioctl(this->Device, FBIOGET_FSCREENINFO, &this->finfo);
		
	this->BufferSize = this->vinfo.xres * this->vinfo.yres * this->vinfo.bits_per_pixel/8;
	
	this->Buffer = (char *)malloc(this->BufferSize);
	this->Plane = (char *)mmap(0, this->BufferSize, PROT_READ | PROT_WRITE, MAP_SHARED, this->Device, (off_t)0);
	
	//SetBkLight(9);
}

LCD_Screen::~LCD_Screen() {
	munmap(this->Plane,this->BufferSize);
	close(this->Device);
}
	
void LCD_Screen::SetBkLight(int lev) {
	int fd = open("/proc/driver/period",O_WRONLY);
	string level = intStr(lev);
	write(fd, &level[0], sizeof(level));
	close(fd);
}

int LCD_Screen::GetWidth() {
	return this->vinfo.xres;
}

int LCD_Screen::GetHeight() {
	return this->vinfo.yres;
}
		
bool LCD_Screen::Clear() {
	memset(&this->Plane[0], BACKCOLOR, this->BufferSize);
}

int LCD_Screen::MakeColor(char r, char g, char b) {
	return 255 << 24 | r << 16 | g << 8 | b << 0;
}

void LCD_Screen::DrawPoint(int x, int y, int color) {
	int xy = (x + y * 800)*4;
	*((unsigned int *)(this->Buffer + xy)) = color;
}

void LCD_Screen::DrawImage(BMPImage * image) { 
	int xpos = 0;
	int ix = 0;	
	
	if( image->X < 0 )
		image->X = 0;
	if( image->Y < 0)
		image->Y = 0;
	
	if(image->X + image->Width > this->vinfo.xres)
		image->X = this->vinfo.xres - image->Width;
	
	if(image->Y + image->Height > this->vinfo.yres)
		image->Y = this->vinfo.yres - image->Height;
		
	for (int iy = image->Height; iy > 0; iy--) {
		for (ix = 0; ix < image->Width; ix++) {
			int xy = (ix + image->X + ( iy + image->Y) * 800)*4;
			*(unsigned int *)(this->Buffer + xy) = ((*(int *)(image->Buffer + ((image->Height - iy-1)*image->Width+ix)*4 ) >> 8) & 0xFFFFFF) | (0xFF << 24);
		}
		xpos += ix;
	}
	
}

void LCD_Screen::DrawImageDirect(BMPImage * image) { 
	int xpos = 0;
	int ix = 0;	
	
	if( image->X < 0 )
		image->X = 0;
	if( image->Y < 0)
		image->Y = 0;
	
	if(image->X + image->Width > this->vinfo.xres)
		image->X = this->vinfo.xres - image->Width;
	
	if(image->Y + image->Height > this->vinfo.yres)
		image->Y = this->vinfo.yres - image->Height;
		
	for (int iy = image->Height; iy > 0; iy--) {
		for (ix = 0; ix < image->Width; ix++) {
			int xy = (ix + image->X + ( iy + image->Y) * 800)*4;
			*(unsigned int *)(this->Plane + xy) = ((*(int *)(image->Buffer + ((image->Height - iy-1)*image->Width+ix)*4 ) >> 8) & 0xFFFFFF) | (0xFF << 24);
		}
		xpos += ix;
	}
	
}

void LCD_Screen::Flip() {
	memcpy(&this->Plane[0],&this->Buffer[0],this->BufferSize);
	memset(&this->Buffer[0], BACKCOLOR, this->BufferSize);
}

