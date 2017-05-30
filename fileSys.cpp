#include <iostream>
#include "includes/fs.h"
#include "includes/utils.h"
#include "includes/function.h"

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
	cout << endl; //split command and output information
	if(ncomm[0] == "exit") {
		imgFile.seekg(0);
		imgFile.write((char*)super, sizeof(SuperBlock));
		imgFile.close();
		cout << "bye~" << endl;
		exit(0);
	} else if (ncomm[0] == "help") {
		cout << "format : format the disk area" << endl;
		cout << "mkdir [dirName] [mode(a append/else overwrite)]: create a directory in current directory" << endl;
		cout << "ls: list all file in current directory" << endl;
		cout << "cd [dirName]: move current directory to another one " << endl;
		cout << "touch [fileName]: create a file in current directory" << endl;
		cout << "rm [fileName]: remove file with the fileName" << endl;
		cout << "write [fileName] [content]: write content to file with the fileName" << endl;
		cout << "read [fileName]: print the content of file with the fileName" << endl;
		cout << "diag view filesystem details info" << endl;
	} else if (ncomm[0] == "diag"){
		cout << "SuperBlock:" << endl;
		cout << "    firstBlock:" << super->firstBlock<<endl;
		cout << "    firstFCB:" << super->firstFCB<<endl;
		cout << "    free FCB:" << super->freeFCBCount<<endl;
		cout << "    free Block:" << super->freeBlockCount<<endl;
		cout << endl << "reading first 10 FCB info" << endl;
		for(int i=0; i<10; i++) {
			int d;
			FCB *fcb = new FCB;
			imgFile.seekp(getFCBAddressFromNum(i));
			imgFile.read((char*)fcb, sizeof(FCB));
			cout << "FCB"<<i<<endl;
			cout << "    used:" << (d=fcb->used)<<endl;
			cout << "    name:" << fcb->filename <<endl;
			cout << "    isDir:" << (d=fcb->isDir)<<endl;
		}

		cout << endl << "reading first 10 Block info" << endl;
		for(int i=0; i<10; i++) {
			int d;
			BLOCK *block = new BLOCK;
			imgFile.seekp(getBlockAddressFromNum(i));
			imgFile.read((char*)block, sizeof(BLOCK));
			cout << "BLOCK"<<i<<endl;
			cout << "    used:" << (d=block->used) <<endl;
		}
	} else if(ncomm[0] == "format") {
		format();
	} else if(ncomm[0] == "mkdir") {
		if(ncomm.size()==1) 
			cout << "Command mkdir required a paramter [dirName]" << endl;
		else 
			mkdir(curDir, ncomm[1]);
	} else if(ncomm[0] == "ls") {
		ls(curDir);
	} else if(ncomm[0] == "cd") {
		if(ncomm.size()==1) 
			cout << "Command cd required a paramter [dirName]" << endl;
		else 
			curDir = cd(curDir, ncomm[1]);
	} else if(ncomm[0] == "touch") {
		cout << ncomm.size() << endl;
		if(ncomm.size()==1) 
			cout << "Command touch required a paramter [fileName]" << endl;
		else 
			touch(curDir, ncomm[1]);
	} else if(ncomm[0] == "rm") {
		cout << ncomm.size() << endl;
		if(ncomm.size()==1) 
			cout << "Command rm required a paramter [fileName]" << endl;
		else 
			rm(curDir, ncomm[1]);
	} else if(ncomm[0] == "write") {
		cout << ncomm.size() << endl;
		if(ncomm.size()<=2) 
			cout << "Command write required two paramter [fileName] [content]" << endl;
		else {
			string mode = "";
			if(ncomm.size()>3) mode = ncomm[3]; 
			write(curDir, ncomm[1], ncomm[2], mode);
		}
	} else if(ncomm[0] == "read") {
		cout << ncomm.size() << endl;
		if(ncomm.size()==1) 
			cout << "Command read required a paramter [fileName]" << endl;
		else 
			read(curDir, ncomm[1]);
	} else 
		cout << "unKnown Command" << endl;
	cout << endl; //split
}

void loop() {
	cout << "Sino's file System terminal," << endl;
	cout << "you can type \"help\" to view all command" << endl;
	cout << "and \"exit\" for exit program" << endl;
	char buffer[40];
	
	while(1) {
		cout << curDir << " > ";
		cin.getline(buffer, 40);
		handle(string(buffer));
	}
}

int main(int argc, char ** argv) {

	if(argc <= 1) {
		help();
		return 1;
	}
	
	init_disk(argv[1]);
	
	if(!imgFile) {
		cout << "termFile open error" << endl;
		return 1;
	}
	
	curDir = "/";
	loop();
	
	return 0;
}
