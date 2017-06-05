#ifndef FS_H 
#define FS_H

#include <fstream>
#include <string>

using namespace std;

// 100KB
// 文件夹的infoBlock 记载的是子文件的fcb号码

//一个 fcb 32B 一个 block 1024B

// 1M fcb area 256*1024个 fcb 
// 共40M 
// 39M 39*1024个block

//dir 0 info Block 存 512个fcb 或者 512个block 的编址

//fcb 0 root 


struct SuperBlock {
	int freeBlockCount;
	int freeFCBCount;
	int firstBlock;
	int firstFCB;
};

struct FCB {
	char used;
	char filename[18];
	char isDir;
	int fileSize;
	int linkCount;
	int infoBlock;
};


// next 16位 64K个 BLOCK = 64MB 个
struct BLOCK {
	char used; //0 if not used 1 if used
	char data[1023];
};


extern fstream imgFile;
extern SuperBlock *super;

const int block_size = 1024;
const int super_block_size = 16;
const int fcb_offset = super_block_size;
const int block_offset = fcb_offset + 1024*1024;
const int max_fcb_count = 1024*1024/32;
const int max_block_count = 39*1024;

void init_disk(string fileName);

int getFCBAddressFromNum (int num);
int getBlockAddressFromNum (int num);
int mallocBlock();
int mallocFCB();
int releaseBlock(int num);
int releaseFCB(int num);
int getFCBAddressFromDir(string curDir, int num = 0);
#endif
