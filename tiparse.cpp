#include <stdlib.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <linux/input.h>
#include <string>
#include <list>
#include <fcntl.h>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <inttypes.h>
#include <unistd.h>	/* close() */
#include <sys/stat.h>	/* S_IXXX flags */
#include <sys/types.h>

#include <cstring>


using namespace std;

class position {
	public:
		int x,y;
};

class tag {
	public:
		tag() {
			this->active = false;
		}
		bool self_end, active; //if have some cmds than active - like link
		position pos;
		string id;
};

list<tag> active_objects = list<tag>();

class img_tag : public tag {
	img_tag() : tag() 
	{ }
	public:
		int width, height;
};

string trim(string str)
{
    while(str[0] == ' ' || str[0] == '\t')
		str = str.substr(1,strlen(&str[0])-1);
    return str;
}
	
string read_page(string path) {
	string str,tmp;
	ifstream infile("page.html");
	while(!infile.eof()) {
		getline(infile, tmp);
		str += trim(tmp);
	}
	infile.close();
	return str;
}

string read_opt(string str, string optname) {
	int  pos = str.find('"', str.find(optname)) + 1;
	str = str.substr(pos, str.find('"', pos) - pos);
	return str;
}

string parse_string(string str) {
	str = str.substr(str.find("<") + 1, str.find(">") - 1);
	//while(!infile.eof()) {
	//	list.add() = str.substr(strstr(str, "<img"), strstr(str,"/>"));
	//}
	return str;
}

list<string> parse_strings(string str) {
	list<string> liststr;
	list<string>::iterator it;
	it = liststr.begin();
	while(str.find("<")) {
		//string tmp = str.substr(str.find(str, "<") + 1, str.find(">") - 1);
		//liststr.insert(it, tmp);
		it++;
		str = str.substr(str.find(">") + 1, str.length() - 1);
	}
	return liststr;
}

//structure of object
		//id
		//type			#################
		//xstartpos		#start			#
		//ystartpos		#				#
		//xendpos		#			 end#
		//yendpos		#################
		
//function to recognize type of tag

//function to get location

//funtcion to get id

//function to get type - static or dynamic

