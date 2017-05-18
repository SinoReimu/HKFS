#include <iostream>
#include "includes/fs.h"
#include "includes/utils.h"

using namespace std;

string curDir;

void help(){
	cout << "At least one paramter required" << endl;
	cout << "fsTerm [imgFile]" << endl;
}

void handle(string command) {
	vector<string> *comm = new vector<string>;
	split(command, COMMAND_SPLIT, comm);
	vector<string> ncomm = *comm;
	if(ncomm[0] == "exit") {
		cout << "bye~" << endl;
		exit(0);
	} else if (ncomm[0] == "help") {
		cout << " format : format the disk area" << endl;
		cout << " mkdir [dirName]: create a directory in current directory" << endl;
		cout << " ls: list all file in current directory" << endl;
		cout << " cd [dirName]: move current directory to another one " << endl;
		cout << " touch [fileName]: create a file in current directory" << endl;
		cout << " rm [fileName]: remove file with the fileName" << endl;
		cout << " write [fileName] [content]: write content to file with the fileName" << endl;
		cout << " read [fileName]: print the content of file with the fileName" << endl;
	} else if(ncomm[0] == "mkdir") {
		cout << "size:" + ncomm.size() << endl;
	} else 
		cout << "unKnown Command" << endl;
	
}

void loop() {
	cout << "Sino's file System terminal," << endl;
	cout << "you can type \"help\" to view all command" << endl;
	cout << "and \"exit\" for exit program" << endl;
	char buffer[40];
	
	while(1) {
		cout << "/ > ";
		cin.getline(buffer, 40);
		handle(string(buffer));
	}
}

int main(int argc, char ** argv) {

	if(argc <= 1) {
		help();
		return 1;
	}
	
	fstream file = init_disk(argv[1]);
	
	if(!file) {
		cout << "termFile open error" << endl;
		return 1;
	}
	
	curDir = "/";
	loop();
	
	return 0;
}
