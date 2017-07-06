#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include <cstring>
#include <linux/fb.h>
#include <string>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <linux/input.h>
#include <fcntl.h>
#include <cstdio>
#include <inttypes.h>
#include <unistd.h>	/* close() */
#include <sys/stat.h>	/* S_IXXX flags */
#include <sys/types.h>	
#include <iostream>
#include <sstream>
#include <cstring>

#include <sys/socket.h>
#include <arpa/inet.h>	
#include <unistd.h>

#include "LCD.hpp"
#include "touch.hpp"

using namespace std;

LCD_Screen Screen = LCD_Screen("/dev/fb0");

void draw_digit(int x, int y, string c) {
	cout << "Opening fbdev \n";
	string path;
	path = "/mnt/web/Skins/tempmeter/" + c + ".bmp";
	BMPImage  image = BMPImage(&path[0]);
	image.Y = y;
	image.X = x;
	Screen.DrawImageDirect(&image);
}


string gettemp(string ip_addr) {

	struct sockaddr_in serv_addr;
	string message;
	char server_reply[1000];
	std::string astr;

	int sockd = socket(AF_INET , SOCK_STREAM, 0);	
	
	serv_addr.sin_addr.s_addr = inet_addr(&ip_addr[0]);
	serv_addr.sin_port = htons(80);
	serv_addr.sin_family = AF_INET;
	
	connect(sockd, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
	
	message = "GET / HTTP/1.1\r\nHost: " + ip_addr + "\r\n\r\n";
	printf("Sending request:\n%s", &message[0]);
	send(sockd, &message[0], strlen(&message[0]), 0);
	
	sleep(1);
	int count = recv(sockd, &server_reply[0], 700, 0);

	server_reply[count] = '\0';

	string result = string(server_reply);
	result = result.substr(result.find("<!--#sensval1-->") + 16, result.length() - 16);

	result = result.substr(0, result.find("."));
	//printf("Result:\n%s\n",&result[0]);
	return result;
}


int main(int argc, char ** argv) {
	
	printf("IP: %s", argv[1]);

	int ytemper;
	int xtemper;
	string currenttemp;

	while(1) {
		ytemper = 8;
		xtemper = 722;
		currenttemp = "0";
		currenttemp = gettemp(argv[1]);
		if(currenttemp.substr(0, 1) != "-") {
			currenttemp = "+" + currenttemp;
		}
		currenttemp += "c";
		printf("TEMP: %s\n", &currenttemp[0]);

		BMPImage image = BMPImage("/mnt/web/Skins/tempmeter/logo.bmp");
		image.Y = 8;
		image.X = 722;
		Screen.DrawImageDirect(&image);
		
		for (int i=0; i < currenttemp.length(); i++) {			
			printf("Printing digit %d\n" , i);
			draw_digit(xtemper, ytemper, currenttemp.substr(i, 1));
			xtemper += 18;
		}
		sleep(3);
	}
	return 0;
}

// mkdir /tmp/test; mount //192.168.3.114/VTHMount /tmp/test/ -o username=grizzly,password=luckybitch,rw ;cd /tmp/test;
