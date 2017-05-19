#include <iostream>
#include "includes/fs.h"


int main (){
	init_disk("disk.img");
	SuperBlock *super = new SuperBlock;
	imgFile.seekp(0);
	imgFile.read((char*)super, sizeof(SuperBlock));
	cout << "SuperBlock Loaded:" << endl;
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
	return 0;
}
