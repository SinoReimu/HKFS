## A very simple file system 

### @ Language : C++
### @ Author : Hakurei Sino
### @ Date : 2017/5/18
### @ Description: use ext4 format to make up all the files, read and write.

Access the fileSystem by use entry application as a REPL client .
you can learn how to use inode, how to link up fcbs after you read these codes

*function.cpp 中可用的文件系统操作函数有以下几个*

+ int mallocFCB()  返回一个可用的FCB序号 如果没有可用FCB返回-1
+ int mallocBlock() 返回一个可用的BLOCK序号 如果没有可用Block返回-1
+ int releaseFCB(int Num)  传入一个FCB序号由文件系统进行回收，如果该块并没有被使用返回-1
+ int releaseBlock(int Num) 传入一个BLOCK序号由文件系统进行回收，如果该块并没有被使用返回-1
+ int getFCBAddressFromNum(int Num) 传入一个FCB序号，获得其在虚拟磁盘文件中的偏移地址
+ int getBlockAddressFromNum(int Num) 传入一个Block序号，获得其在虚拟磁盘文件中的偏移地址

*可用的常量*

+ const int block_size = 1024;
+ const int super_block_size = 16;
+ const int fcb_offset = super_block_size;
+ const int block_offset = fcb_offset + 1024*1024;
+ const int max_fcb_count = 1024*1024/32;
+ const int max_block_count = 39*1024;
