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
#include <sys/types.h>
#include <linux/input.h>
#include <vector> 

#include <inttypes.h>
#include <sys/stat.h>	/* S_IXXX flags */
#define BITS_PER_LONG (sizeof(long) * 8)
#define NBITS(x) ((((x)-1)/BITS_PER_LONG)+1)
#define OFF(x)  ((x)%BITS_PER_LONG)
#define BIT(x)  (1UL<<OFF(x))
#define LONG(x) ((x)/BITS_PER_LONG)
#define test_bit(bit, array)	((array[LONG(bit)] >> OFF(bit)) & 1)

int openTouchScreen();
void getTouchScreenDetails(int fd, int *screenXmin,int *screenXmax,int *screenYmin,int *screenYmax);
void getTouchSample(int fd, int *rawX, int *rawY, int *rawPressure);

class TouchPoint {
	public:
		int X, Y, State;
}; 

class TouchScreen {
	int Fd;
	
	public:
		TouchScreen();
		~TouchScreen();
		
		TouchPoint Point;
		
		void Init();
		void GetInfo();
		void ReadTouch();
};
