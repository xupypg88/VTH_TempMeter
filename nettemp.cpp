#include <stdio.h>
#include <string>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>	
#include <unistd.h>

using namespace std;

struct sockaddr_in serv_addr;
char * message, server_reply[20000];
std::string astr;

string get_temp(){//char * addr) {
	
	printf("Step 2\n");
	int sockd = socket(AF_INET , SOCK_STREAM, 0);	
	
	serv_addr.sin_addr.s_addr = inet_addr("192.168.22.55");
	serv_addr.sin_port = htons(80);
	serv_addr.sin_family = AF_INET;
	
	printf("Step 3\n");
	connect(sockd, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
	
	printf("Step 4\n");
	message = "GET /index.html HTTP/1.1 \nHost: 192.168.22.55\n\n";
	printf("Sending request\n");
	send(sockd, message, strlen(message), 0);
	printf("Step 5\n");
	sleep(1);
	recv(sockd, server_reply, 20000, 0);

	printf("Step 6\n");
	string result = server_reply;
	result = result.substr(result.find("<!--#sensval1-->") + 16, result.length() - 16);

	printf("Step 7\n");
	result = result.substr(0, result.find("."));
	printf("Step 8\n");
	return result;
	//<!--#sensval1-->
}
