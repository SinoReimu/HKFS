#include <string>
#include "includes/fs.h"
#include <string.h>
#include <iostream>

using namespace std;

void format () {

	super->freeBlockCount = max_block_count;
	super->firstBlock = 0;
	BLOCK* emptyBlock = new BLOCK;
	emptyBlock->used = 0;
	imgFile.seekg(block_offset);
	for(int i=0; i<max_block_count; i++) 
		imgFile.write((char*)emptyBlock, sizeof(BLOCK));
	cout << "Reset Blocks Finished, " << max_block_count << " blocks can be use." <<endl;
	/*重置BLOCK*/
	
	super->freeFCBCount = max_fcb_count;
	super->firstFCB = 0;
	FCB* emptyFCB = new FCB;
	emptyFCB->used = 0;
	imgFile.seekg(fcb_offset);
	for(int i=0; i<max_fcb_count; i++)
		imgFile.write((char*)emptyFCB, sizeof(FCB));
	cout << "Reset FCBs Finished, " << max_fcb_count << " fcbs can be use." <<endl;
	/*重置FCB 和 超级块*/
	cout << "Reset super block over stored in address 0x0000" <<endl;
	BLOCK* info = new BLOCK;
	info->used = 1;
	
	FCB* rootFcb = new FCB;
	rootFcb->used = 1;
	strcpy(rootFcb->filename, "\\");
	rootFcb->isDir = 1;
	rootFcb->linkCount = 0;
	int fcbf = mallocFCB();
	int blockf = mallocBlock();
	rootFcb->infoBlock = blockf;
	
	imgFile.seekg(getFCBAddressFromNum(fcbf));
	imgFile.write((char*)rootFcb, sizeof(FCB));
	
	imgFile.seekg(getBlockAddressFromNum(blockf));
	imgFile.write((char*)info, sizeof(BLOCK));
	cout << "Root Dir / was built" << endl;
	cout << "format disk with this fileSys over" <<endl;
	/*建立 / FCB*/
}

void mkdir (string curDir, string dirName) {

}

void ls (string curDir) {

}

string cd (string curDir, string dirName) {
	return "";
}

void touch (string curDir, string fileName) {

}

void rm (string curDir, string fileName) {

}

void write (string curDir, string fileName, string content, string mode) {

}

void read (string curDir, string fileName) {

}
