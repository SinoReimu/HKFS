#include <string>
#include <fstream>
#include "includes/fs.h"

using namespace std;

fstream imgFile;
SuperBlock *super;

void init_disk(string fileName) {
	imgFile = fstream(fileName, ios::binary|ios::in|ios::out);
	super = new SuperBlock;
	imgFile.seekp(0);
	imgFile.read((char*)super, sizeof(SuperBlock));
}

int getFCBAddressFromNum (int num) {
	if(num > max_fcb_count) return -1;
	return fcb_offset + num*32;
}

int getBlockAddressFromNum (int num) {
	if(num > max_block_count) return -1;
	return block_offset+block_size*num;
}

int mallocBlock() {
	int i=0;
	BLOCK *block = new BLOCK;
	for(i=super->firstBlock; i<max_block_count; i++) {
		imgFile.seekp(getBlockAddressFromNum(i));
		imgFile.read((char*)block, sizeof(BLOCK));
		if(block->used==0) {
			super->freeBlockCount = super->freeBlockCount-1;
			super->firstBlock = i+1;
			return i;
		}
	}
	return -1;
}

int mallocFCB() {
	int i=0;
	FCB *fcb = new FCB;
	for(i=super->firstFCB; i<max_fcb_count; i++) {
		imgFile.seekp(getFCBAddressFromNum(i));
		imgFile.read((char*)fcb, sizeof(FCB));
		if(fcb->used==0) {
			super->freeFCBCount = super->freeFCBCount-1;
			super->firstFCB = i+1;
			return i;
		}
	}
	return -1;
}

int releaseBlock(int num) {
	BLOCK *block = new BLOCK;
	imgFile.seekp(getBlockAddressFromNum(num));
	imgFile.read((char*)block, sizeof(BLOCK));
	if(block->used==0) return -1;
	block->used=0;
	imgFile.seekg(getBlockAddressFromNum(num));
	imgFile.write((char*)block, sizeof(BLOCK));
	if(super->firstBlock>num) super->firstBlock = num;
	super->freeBlockCount += 1;
	return 0;
}

int releaseFCB(int num) {
	FCB *block = new FCB;
	imgFile.seekp(getFCBAddressFromNum(num));
	imgFile.read((char*)block, sizeof(FCB));
	if(block->used==0) return -1;
	block->used=0;
	imgFile.seekg(getFCBAddressFromNum(num));
	imgFile.write((char*)block, sizeof(FCB));
	if(super->firstFCB>num) super->firstFCB = num;
	super->freeFCBCount += 1;
	return 0;
}

