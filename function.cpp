#include <string>
#include "includes/fs.h"
#include <string.h>
#include <iostream>

using namespace std;

void format () {

	cout << "sizeof FCB:" << sizeof(FCB) << endl;
	cout << "sizeof BLOCK:" << sizeof(BLOCK) << endl;
	super->freeBlockCount = max_block_count;
	super->firstBlock = 0;
	BLOCK* emptyBlock = new BLOCK;
	emptyBlock->used = 0;
	for(int i=0; i<max_block_count; i++) {
		imgFile.seekg(getBlockAddressFromNum(i));	
		imgFile.write((char*)emptyBlock, sizeof(BLOCK));
	}
	cout << "Reset Blocks Finished, " << max_block_count << " blocks can be use." <<endl;
	/*重置BLOCK*/
	
	super->freeFCBCount = max_fcb_count;
	super->firstFCB = 0;
	FCB* emptyFCB = new FCB;
	emptyFCB->used = 0;
	for(int i=0; i<max_fcb_count; i++) {		
		imgFile.seekg(getFCBAddressFromNum(i));
		imgFile.write((char*)emptyFCB, sizeof(FCB));
	}
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
	int idxFCBFile, idxBlockFile, idxFCBDir;
    idxFCBDir = getFCBAddressFromDir(curDir);
    FCB *fcbDir = new FCB; 
    imgFile.seekp(getFCBAddressFromNum(idxFCBDir)); 
    imgFile.read((char*)fcbDir, sizeof(FCB));
    BLOCK *blockDir = new BLOCK; 
    imgFile.seekp(getBlockAddressFromNum(fcbDir->infoBlock)); 
    imgFile.read((char*)blockDir, sizeof(BLOCK));
    int idx;
    FCB *fcb = new FCB;
    for (int j = 0, cnt = fcbDir->linkCount; j < 2 * cnt; j += 2)
    {
        idx = ((unsigned char)blockDir->data[j] << 8) + ((unsigned char)blockDir->data[j + 1]);
        imgFile.seekp(getFCBAddressFromNum(idx));
        imgFile.read((char*)fcb, sizeof(FCB));
        if (!strcmp(fcb->filename, fileName.c_str()))
        {
            cout << "Have the same name directory or file" << endl;
            return;
        } 
    }
    FCB *fcbFile = new FCB; idxFCBFile = mallocFCB(); imgFile.seekp(getFCBAddressFromNum(idxFCBFile)); imgFile.read((char*)fcbFile, sizeof(FCB));
    BLOCK *blockFile = new BLOCK; idxBlockFile = mallocBlock(); imgFile.seekp(getBlockAddressFromNum(idxBlockFile)); imgFile.read((char*)blockFile, sizeof(BLOCK));
    fcbFile->used = 1;
    for (int i = 0; i < fileName.length(); i++) fcbFile->filename[i] = fileName[i]; fcbFile->filename[fileName.length()] = 0;
    fcbFile->isDir = 1;fcbFile->fileSize = fcbFile->linkCount = 0;
    fcbFile->infoBlock = idxBlockFile;
    blockFile->used = 1;
    blockDir->data[2 * fcbDir->linkCount] = idxFCBFile >> 8;
    blockDir->data[2 * fcbDir->linkCount + 1] = idxFCBFile & 0xff;
    fcbDir->linkCount++;
    imgFile.seekg(getFCBAddressFromNum(idxFCBFile)); imgFile.write((char*)fcbFile, sizeof(FCB));
    imgFile.seekg(getBlockAddressFromNum(idxBlockFile)); imgFile.write((char*)blockFile, sizeof(BLOCK));	
    imgFile.seekg(getFCBAddressFromNum(idxFCBDir)); imgFile.write((char*)fcbDir, sizeof(FCB));
    imgFile.seekg(getBlockAddressFromNum(fcbDir->infoBlock)); imgFile.write((char*)blockDir, sizeof(BLOCK));
	cout << "option mkdir is over!" << endl;
}

void ls (string curDir) {
	cout << "doing ls" << endl;
	int idxFCBFile, idxBlockFile, idxFCBDir;
    idxFCBDir = getFCBAddressFromDir(curDir);
    FCB *fcbDir = new FCB; 
    imgFile.seekp(getFCBAddressFromNum(idxFCBDir)); 
    imgFile.read((char*)fcbDir, sizeof(FCB));
    BLOCK *blockDir = new BLOCK; 
    imgFile.seekp(getBlockAddressFromNum(fcbDir->infoBlock)); 
    imgFile.read((char*)blockDir, sizeof(BLOCK));
    int idx;
    FCB *fcb = new FCB;
    for (int j = 0, cnt = fcbDir->linkCount; j < 2 * cnt; j += 2)
    {
        idx = ((unsigned char)blockDir->data[j] << 8) + ((unsigned char)blockDir->data[j + 1]);
        imgFile.seekp(getFCBAddressFromNum(idx));
        imgFile.read((char*)fcb, sizeof(FCB));
		if(fcb->isDir = 1){
				cout << fcb->filename << "  ........type:dir" << endl;
		}
		else{
				cout << fcb->filename << "  ........type:file" << endl;
		}
    }
	cout << "option ls is over!" << endl;
}

string cd(string curDir, string dirName)
{
    FCB *fcb = new FCB;
    if (dirName[0] == '/')
    {
        int idx = getFCBAddressFromDir(dirName);
        if (idx == -1) {cout << "No such directory: " << dirName << endl; return curDir;}
        string newDir = dirName;
        if (newDir.length() != 1 && newDir[newDir.length() - 1] == '/') return newDir.substr(0, newDir.length() - 1);
        return newDir;
    }
    else
    {
        string newDir = curDir + (curDir[curDir.length() - 1] == '/' ? "" : "/") + dirName;
        int idx = getFCBAddressFromDir(newDir);
        if (idx == -1) {cout << "No such directory: " << dirName << endl; return curDir;}
        if (newDir.length() != 1 && newDir[newDir.length() - 1] == '/') return newDir.substr(0, newDir.length() - 1);
        return newDir;
    }
}

void touch(string curDir, string fileName)
{
    int idxFCBFile, idxBlockFile, idxFCBDir;
    idxFCBDir = getFCBAddressFromDir(curDir);
    FCB *fcbDir = new FCB; 
    imgFile.seekp(getFCBAddressFromNum(idxFCBDir)); 
    imgFile.read((char*)fcbDir, sizeof(FCB));
    BLOCK *blockDir = new BLOCK; 
    imgFile.seekp(getBlockAddressFromNum(fcbDir->infoBlock)); 
    imgFile.read((char*)blockDir, sizeof(BLOCK));
    int idx;
    FCB *fcb = new FCB;
    for (int j = 0, cnt = fcbDir->linkCount; j < 2 * cnt; j += 2)
    {
        idx = ((unsigned char)blockDir->data[j] << 8) + ((unsigned char)blockDir->data[j + 1]);
        imgFile.seekp(getFCBAddressFromNum(idx));
        imgFile.read((char*)fcb, sizeof(FCB));
        if (!strcmp(fcb->filename, fileName.c_str()))
        {
            cout << "Have the same name directory or file" << endl;
            return;
        } 
    }
    FCB *fcbFile = new FCB; idxFCBFile = mallocFCB(); imgFile.seekp(getFCBAddressFromNum(idxFCBFile)); imgFile.read((char*)fcbFile, sizeof(FCB));
    BLOCK *blockFile = new BLOCK; idxBlockFile = mallocBlock(); imgFile.seekp(getBlockAddressFromNum(idxBlockFile)); imgFile.read((char*)blockFile, sizeof(BLOCK));
    fcbFile->used = 1;
    for (int i = 0; i < fileName.length(); i++) fcbFile->filename[i] = fileName[i]; fcbFile->filename[fileName.length()] = 0;
    fcbFile->isDir = fcbFile->fileSize = fcbFile->linkCount = 0;
    fcbFile->infoBlock = idxBlockFile;
    blockFile->used = 1;
    blockDir->data[2 * fcbDir->linkCount] = idxFCBFile >> 8;
    blockDir->data[2 * fcbDir->linkCount + 1] = idxFCBFile & 0xff;
    fcbDir->linkCount++;
    imgFile.seekg(getFCBAddressFromNum(idxFCBFile)); imgFile.write((char*)fcbFile, sizeof(FCB));
    imgFile.seekg(getBlockAddressFromNum(idxBlockFile)); imgFile.write((char*)blockFile, sizeof(BLOCK));	
    imgFile.seekg(getFCBAddressFromNum(idxFCBDir)); imgFile.write((char*)fcbDir, sizeof(FCB));
    imgFile.seekg(getBlockAddressFromNum(fcbDir->infoBlock)); imgFile.write((char*)blockDir, sizeof(BLOCK));
}

void rm(string curDir, string fileName)
{
    int idxFCBDir, idxFCBFile;
    idxFCBDir = getFCBAddressFromDir(curDir);
    if (idxFCBDir == -1)
    {
        cout << "No such file" << endl;
        return;
    }
    FCB *fcbDir = new FCB; imgFile.seekp(getFCBAddressFromNum(idxFCBDir)); imgFile.read((char*)fcbDir, sizeof(FCB));
    BLOCK *blockDir = new BLOCK; imgFile.seekp(getBlockAddressFromNum(fcbDir->infoBlock)); imgFile.read((char*)blockDir, sizeof(BLOCK));
    FCB *fcbFile = new FCB;
    int ifFind = 0;
    for (int j = 0, cnt = fcbDir->linkCount; j < 2 * cnt; j += 2)
    {
        idxFCBFile = ((unsigned char)blockDir->data[j] << 8) + ((unsigned char)blockDir->data[j + 1]);
        imgFile.seekp(getFCBAddressFromNum(idxFCBFile));
        imgFile.read((char*)fcbFile, sizeof(FCB));
        if ((!fcbFile->isDir) && (!strcmp(fcbFile->filename, fileName.c_str())))
        {
            ifFind = 1;
            for (int i = j; i < 2 * cnt - 2; i += 2)
            {
                blockDir->data[j] = blockDir->data[j + 2];
                blockDir->data[j + 1] = blockDir->data[j + 3];
            }
            releaseBlock(fcbFile->infoBlock);
            releaseFCB(idxFCBFile);
        }
    }
    if (!ifFind) {cout << "No such file" << endl; return;}
    fcbDir->linkCount--;
    imgFile.seekg(getFCBAddressFromNum(idxFCBDir)); imgFile.write((char*)fcbDir, sizeof(FCB));
    imgFile.seekg(getBlockAddressFromNum(fcbDir->infoBlock)); imgFile.write((char*)blockDir, sizeof(BLOCK));
}

void write (string curDir, string fileName, string content, string mode) {
	int num, cnt, i, j, k, last_bl_num, last_bl_count, rest_count, m, n, block3_num, block2_num;
	num = getFCBAddressFromDir(curDir);
	if(num == -1)
	{
		cout << "No such file or directory!" << endl;
		return;
	}
	FCB *fcb1 = new FCB;
    imgFile.seekp(getFCBAddressFromNum(num));
    imgFile.read((char*)fcb1, sizeof(FCB));
	int ifFind = 0;
    BLOCK *block1 = new BLOCK;
    imgFile.seekp(getBlockAddressFromNum(fcb1->infoBlock));
    imgFile.read((char*)block1, sizeof(BLOCK));
    for (j = 0, cnt = fcb1->linkCount; j < 2 * cnt; j += 2)
    {
    	num = ((unsigned char)block1->data[j] << 8) + ((unsigned char)block1->data[j + 1]);
        imgFile.seekp(getFCBAddressFromNum(num));
        imgFile.read((char*)fcb1, sizeof(FCB));
        if ((fcb1->isDir == 0) && (!strcmp(fcb1->filename, fileName.c_str())))
		{
			ifFind = 1;
			break;
		}
    }
    if (!ifFind)
	{
		cout << "This file does not exist or is a directory!" << endl;
		return;
	}        //鎵惧埌鏂囦欢鎵€鍦‵CB
	imgFile.seekp(getBlockAddressFromNum(fcb1->infoBlock));
	imgFile.read((char*)block1, sizeof(BLOCK));
	BLOCK *block2 = new BLOCK;
	BLOCK *block3 = new BLOCK;
	if(mode == "a")
	{
		j = (fcb1->linkCount)*2-2;
		last_bl_num = ((unsigned char)block1->data[j] << 8) + ((unsigned char)block1->data[j + 1]);
		imgFile.seekp(getBlockAddressFromNum(last_bl_num));
        imgFile.read((char*)block2, sizeof(BLOCK));
		last_bl_count = fcb1->fileSize - (fcb1->linkCount - 1) * 1023;
		rest_count = 1023 - last_bl_count;
		fcb1->fileSize = fcb1->fileSize + content.length();
		if(content.length() <= rest_count)
		{
			for(i = last_bl_count, j = 0; j < content.length(); i++, j++)
			{
				block2->data[i] = content[j];
			}
			imgFile.seekg(getBlockAddressFromNum(last_bl_num));
			imgFile.write((char*)block2, sizeof(BLOCK));
			imgFile.seekg(getFCBAddressFromNum(num));
			imgFile.write((char*)fcb1, sizeof(FCB));
			return;
		}
		for(i = last_bl_count, j = 0; j < rest_count; i++, j++)
		{
			block2->data[i] = content[j];
		}
		imgFile.seekg(getBlockAddressFromNum(last_bl_num));
		imgFile.write((char*)block2, sizeof(BLOCK));
		n = (content.length() - rest_count) / 1023;		
		for(i = 0; i < n; i++)
		{
			block3_num = mallocBlock();
			imgFile.seekp(getBlockAddressFromNum(block3_num));
			imgFile.read((char*)block3, sizeof(BLOCK));
			block3->used = 1;
			block1->data[2 * fcb1->linkCount] = block3_num >> 8;
    		block1->data[2 * fcb1->linkCount + 1] = block3_num & 0xff;
			fcb1->linkCount++;
			for(j = 0; j < 1023; j++)
			{
				block3->data[j] = content[rest_count+j+i*1023];
			}
			imgFile.seekg(getBlockAddressFromNum(block3_num));
			imgFile.write((char*)block3, sizeof(BLOCK));
		}
		if((content.length() - rest_count) % 1023 == 0)
		{
			imgFile.seekg(getFCBAddressFromNum(num));
			imgFile.write((char*)fcb1, sizeof(FCB));
			imgFile.seekg(getBlockAddressFromNum(fcb1->infoBlock));
			imgFile.write((char*)block1, sizeof(BLOCK));
			return;
		}
		block3_num = mallocBlock();
		imgFile.seekp(getBlockAddressFromNum(block3_num));
		imgFile.read((char*)block3, sizeof(BLOCK));
		block3->used = 1;
		block1->data[2 * fcb1->linkCount] = block3_num >> 8;
    	block1->data[2 * fcb1->linkCount + 1] = block3_num & 0xff;
		fcb1->linkCount++;
		m = content.length() - rest_count - n*1023;
		for(i = 0, j = rest_count + n*1023; i < m; i++, j++)
		{
			block3->data[i] = content[j];
		}
		imgFile.seekg(getBlockAddressFromNum(block3_num));
		imgFile.write((char*)block3, sizeof(BLOCK));
		imgFile.seekg(getFCBAddressFromNum(num));
		imgFile.write((char*)fcb1, sizeof(FCB));
		imgFile.seekg(getBlockAddressFromNum(fcb1->infoBlock));
		imgFile.write((char*)block1, sizeof(BLOCK));
		return;
	}
	n = content.length() / 1023;
	if(content.length() <= fcb1->linkCount * 1023)
	{
		for(i = 0, j = 0; i < n; i++, j += 2)
		{
			block2_num = ((unsigned char)block1->data[j] << 8) + ((unsigned char)block1->data[j + 1]);
			imgFile.seekp(getBlockAddressFromNum(block2_num));
        	imgFile.read((char*)block2, sizeof(BLOCK));
        	for(k = 0; k < 1023; k++)
        	{
        		block2->data[k] = content[i*1023 + k];
        	}
        	imgFile.seekg(getBlockAddressFromNum(block2_num));
			imgFile.write((char*)block2, sizeof(BLOCK));
		}
		if(content.length() % 1023 == 0)
		{
			if(n == fcb1->linkCount)
			{
				fcb1->fileSize = content.length();
				imgFile.seekg(getFCBAddressFromNum(num));
				imgFile.write((char*)fcb1, sizeof(FCB));
				return;
			}
			for(k = 0, i = n * 2; k < (fcb1->linkCount - n); k++, i += 2)
			{
				block2_num = ((unsigned char)block1->data[i] << 8) + ((unsigned char)block1->data[i + 1]);
				releaseBlock(block2_num);
				fcb1->linkCount--;
			}
			fcb1->fileSize = content.length();
			imgFile.seekg(getFCBAddressFromNum(num));
			imgFile.write((char*)fcb1, sizeof(FCB));
			return;
		}
		for(k = 0, i = (n + 1) * 2; k < (fcb1->linkCount - n - 1); k++, i += 2)
		{
			block2_num = ((unsigned char)block1->data[i] << 8) + ((unsigned char)block1->data[i + 1]);
			releaseBlock(block2_num);
			fcb1->linkCount--;
		}
		block2_num = ((unsigned char)block1->data[2 * n] << 8) + ((unsigned char)block1->data[2 * n + 1]);
		imgFile.seekp(getBlockAddressFromNum(block2_num));
        imgFile.read((char*)block2, sizeof(BLOCK));
		for(i = 0, j = n * 1023; j < content.length(); i++, j++)
		{
			block2->data[i] = content[j];
		}
		fcb1->fileSize = content.length();
		imgFile.seekg(getFCBAddressFromNum(num));
		imgFile.write((char*)fcb1, sizeof(FCB));
		imgFile.seekg(getBlockAddressFromNum(block2_num));
		imgFile.write((char*)block2, sizeof(BLOCK));
		return;
	}
	for(i = 0, j = 0; i < fcb1->linkCount; i++, j += 2)
	{
		block2_num = ((unsigned char)block1->data[j] << 8) + ((unsigned char)block1->data[j + 1]);
		imgFile.seekp(getBlockAddressFromNum(block2_num));
        imgFile.read((char*)block2, sizeof(BLOCK));
        for(k = 0; k < 1023; k++)
        {
        	block2->data[k] = content[i*1023 + k];
        }
        imgFile.seekg(getBlockAddressFromNum(block2_num));
		imgFile.write((char*)block2, sizeof(BLOCK));
	}
	for(i = 0; i < n - fcb1->linkCount; i++)
	{
		block2_num = mallocBlock();
		imgFile.seekp(getBlockAddressFromNum(block2_num));
		imgFile.read((char*)block2, sizeof(BLOCK));
		block2->used = 1;
		block1->data[2 * fcb1->linkCount] = block3_num >> 8;
    	block1->data[2 * fcb1->linkCount + 1] = block3_num & 0xff;
		for(j = 0; j < 1023; j++)
		{
			block2->data[j] = content[j + fcb1->linkCount * 1023];
		}
		fcb1->linkCount++;
		imgFile.seekg(getBlockAddressFromNum(block2_num));
		imgFile.write((char*)block2, sizeof(BLOCK));
	}
	if(content.length() % 1023 == 0)
	{
		fcb1->fileSize = content.length();
		imgFile.seekg(getFCBAddressFromNum(num));
		imgFile.write((char*)fcb1, sizeof(FCB));
		imgFile.seekg(getBlockAddressFromNum(fcb1->infoBlock));
		imgFile.write((char*)block1, sizeof(BLOCK));
		return;
	}
	block2_num = mallocBlock();
	imgFile.seekp(getBlockAddressFromNum(block2_num));
	imgFile.read((char*)block2, sizeof(BLOCK));
	block2->used = 1;
	block1->data[2 * fcb1->linkCount] = block3_num >> 8;
    block1->data[2 * fcb1->linkCount + 1] = block3_num & 0xff;
	for(i = fcb1->linkCount * 1023, j = 0; i < content.length(); i++, j++)
	{
		block2->data[j] = content[i];
	}
	fcb1->linkCount++;
	imgFile.seekg(getBlockAddressFromNum(block2_num));
	imgFile.write((char*)block2, sizeof(BLOCK));
	fcb1->fileSize = content.length();
	imgFile.seekg(getFCBAddressFromNum(num));
	imgFile.write((char*)fcb1, sizeof(FCB));
	imgFile.seekg(getBlockAddressFromNum(fcb1->infoBlock));
	imgFile.write((char*)block1, sizeof(BLOCK));
	return;
}

void read (string curDir, string fileName) {
	int num, cnt, i, j, block2_num, m;
	num = getFCBAddressFromDir(curDir);
	if(num == -1)
	{
		cout << "No such file or directory!" << endl;
		return;
	}
	FCB *fcb1 = new FCB;
    imgFile.seekp(getFCBAddressFromNum(num));
    imgFile.read((char*)fcb1, sizeof(FCB));
	int ifFind = 0;
    BLOCK *block1 = new BLOCK;
    imgFile.seekp(getBlockAddressFromNum(fcb1->infoBlock));
    imgFile.read((char*)block1, sizeof(BLOCK));
    for (j = 0, cnt = fcb1->linkCount; j < 2 * cnt; j += 2)
    {
    	num = ((unsigned char)block1->data[j] << 8) + ((unsigned char)block1->data[j + 1]);
        imgFile.seekp(getFCBAddressFromNum(num));
        imgFile.read((char*)fcb1, sizeof(FCB));
        if ((fcb1->isDir == 0) && (!strcmp(fcb1->filename, fileName.c_str())))
		{
			ifFind = 1;
			break;
		}
    }
    if (!ifFind)
	{
		cout << "This file does not exist or is a directory!" << endl;
		return;
	}        //鎵惧埌鏂囦欢鎵€鍦‵CB
	imgFile.seekp(getBlockAddressFromNum(fcb1->infoBlock));
	imgFile.read((char*)block1, sizeof(BLOCK));
	BLOCK *block2 = new BLOCK;
	for(i = 0, j = 0; i < fcb1->linkCount - 1; i++, j += 2)
	{
		block2_num = ((unsigned char)block1->data[j] << 8) + ((unsigned char)block1->data[j + 1]);
		imgFile.seekp(getBlockAddressFromNum(block2_num));
        imgFile.read((char*)block2, sizeof(BLOCK));
        for(cnt = 0; cnt < 1023; cnt++)
        {
        	cout << block2->data[cnt] << endl;
        }
    }
    m = 2 * (fcb1->linkCount - 1);
    block2_num = ((unsigned char)block1->data[m] << 8) + ((unsigned char)block1->data[m + 1]);
	imgFile.seekp(getBlockAddressFromNum(block2_num));
    imgFile.read((char*)block2, sizeof(BLOCK));
    for(i = (fcb1->linkCount - 1) * 1023, j = 0; i < fcb1->fileSize; i++, j++)
    {
    	cout << block2->data[j] << endl;
    }
    return;
}

