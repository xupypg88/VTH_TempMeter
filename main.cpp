#include <linux/input.h>
#include <string>
#include <fcntl.h>
#include <cstdio>
#include <inttypes.h>
#include <unistd.h>	/* close() */
#include <sys/stat.h>	/* S_IXXX flags */
#include <sys/types.h>	
#include <iostream>
#include <sstream>
#include <cstring>
#include "nettemp.cpp"


int  xres,yres,i;
	
#include "framebuffer.cpp"

//#include "touch.h"
//#include "touch.cpp"
#include "tiparse.cpp"
#include <signal.h>

#include <time.h>

using namespace std;

void  INThandler(int sig)
{
        signal(sig, SIG_IGN);
	closeFramebuffer();
        exit(0);
}
string to_str(int num) {
	std::stringstream ss;
    ss<<num;
    std::string str;
    ss>>str;
    return str;
}
void print_digit(int x, int y, string c) {
	string path;
	path = "digits/" + c + ".bmp";
	draw_bmp(x, y, &path[0]);
}

int main(int argc, char ** argv)
{
	
	framebufferInitialize(&xres,&yres);
	
	//backup image on screen
	printf("%s\n", argv[1]);
	int ytemper;
	int xtemper;
	string currenttemp;
	
	while(1) {
		ytemper = 100;
		xtemper = 100;
		currenttemp = "-16";
		printf("Step 1\n");
		//currenttemp = get_temp();
		if(currenttemp.substr(0, 1) != "-") {
			currenttemp = "+" + currenttemp;
		}
		currenttemp += "c";
		memcpy(fbpback, fbp, 1536000); 	
		drawSquare(xtemper, ytemper, 122, 49, 1);
		for (int i=0; i < currenttemp.length(); i++) {
			print_digit(xtemper, ytemper, currenttemp.substr(i, 1));
			xtemper += 30;
		}
		refreshfb();
		sleep(3);

		printf("Step 4\n");
		memcpy(fbpback, fbp, 1536000); 
		draw_bmp(0, 0, "digits/logo.bmp");
		refreshfb();
		sleep(3);
	}
	closeFramebuffer(); 
	//cout << get_temp("172.16.168.105");
	
}


