#include "touch.hpp"

#define KWHT  "\x1B[37m"
#define KYEL  "\x1B[33m"

TouchScreen::TouchScreen() {
	this->Point.State = 0;
	if ((this->Fd = open("/dev/event0", O_RDONLY)) < 0) {
		this->Fd = -1;
	}
}

TouchScreen::~TouchScreen() {
	close(this->Fd);
}

void TouchScreen::GetInfo() {
	unsigned short id[4];
	unsigned long bit[EV_MAX][NBITS(KEY_MAX)];
	char name[256] = "Unknown";
	int abs[6] = {0};
	
	int version=0;
	/* ioctl() accesses the underlying driver */
	if (ioctl(this->Fd, EVIOCGVERSION, &version)) {
		perror("evdev ioctl");
	}
	
	/* the EVIOCGVERSION ioctl() returns an int */
	/* so we unpack it and display it */
	printf("evdev driver version is %d.%d.%d\n", version >> 16, (version >> 8) & 0xff, version & 0xff);
       
	ioctl(this->Fd, EVIOCGNAME(sizeof(name)), name);
	printf("Input device name: \"%s\"\n", name);

	memset(bit, 0, sizeof(bit));
	ioctl(this->Fd, EVIOCGBIT(0, EV_MAX), bit[0]);
	printf("Supported events:\n");
}

void TouchScreen::Init() {
	this->GetInfo();
}

void TouchScreen::ReadTouch() {
	int i;
        // how many bytes were read
        size_t rb;
        // the events (up to 64 at once) 
        struct input_event ev[64];

	rb=read(this->Fd,ev,sizeof(struct input_event)*64);
        for (i = 0;  i <  (rb / sizeof(struct input_event)); i++){
            if (ev[i].type ==  EV_SYN);
                //printf("SYNC\n");

			else if (ev[i].type == EV_KEY && ev[i].code == 330 && ev[i].value == 1) {
                //printf("Pressed\n");
				this->Point.State = 1;
			}
			
			else if (ev[i].type == EV_KEY && ev[i].code == 330 && ev[i].value == 0) {
                //printf("Unpressed\n");
				this->Point.State = 0;
			}
			else if (ev[i].type == EV_ABS && ev[i].code == 0 && ev[i].value > 0){
                //printf("X\n");
				this->Point.X = ev[i].value;
			}
			else if (ev[i].type == EV_ABS  && ev[i].code == 1 && ev[i].value > 0){
                //printf("Y\n");
				this->Point.Y = ev[i].value;
			}
	}
}
