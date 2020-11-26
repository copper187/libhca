#pragma once

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <queue>
#include <vector>
#include <cstring>
#include <algorithm>
#include <thread>
#include <mutex>
#include <fstream>
//#include "hcaCrypto.h"
#include "hcaSupportLib.h"

#define un unsigned 

using namespace std;

struct mainInfo
{
	unsigned short version;
	unsigned short block_start_offset;
	unsigned long long filesize;
	unsigned int bitrate;
	unsigned int audiolength;
	bool enable_loop = false;
};

struct format
{
	unsigned char channel_num;
	unsigned int sampling_rate;
	unsigned int blocknum;
	unsigned short start_muteblock_num;    //合计块数量*0x400+0x80
	unsigned short end_muteblock_num;
};

struct compress
{
	unsigned short blocksize;      //仅cbr，vbr此项为0
	unsigned char data01;             // 计算方式不明 v2.0可能取值 -> 1
	unsigned char data02;             // 计算方式不明 v2.0可能取值 -> 15(0x0F)
	unsigned char data03;             // 计算方式不明 可能取值 -> 1
	unsigned char data04;             // 计算方式不明 可能取值 -> 0,1
	unsigned char data05;             // 计算方式不明 可能取值 -> 128(0x80)
	unsigned char data06;             // 计算方式不明 可能取值 -> 128(0x80),32(0x20)
	unsigned char data07;             // 计算方式不明 可能取值 -> 0,32(0x20)
	unsigned char data08;				 // 计算方式不明 可能取值 -> 0,8
	unsigned char reserve1;			 // 保留
	unsigned char reserve2;			 // 保留
	unsigned char data09;				 // 后期计算求出
};

struct loops
{
	unsigned int start;            // 循环开始块
	unsigned int end;              // 循环结束块
	unsigned short count;          // 循环次数，0x80=无限制
};


void Get_hcaInfo(un char* filedata, un long long filesize);
void Get_hcaInfo_nomeg(un char* filedata, un long long filesize);


