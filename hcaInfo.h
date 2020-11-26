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
	unsigned short start_muteblock_num;    //�ϼƿ�����*0x400+0x80
	unsigned short end_muteblock_num;
};

struct compress
{
	unsigned short blocksize;      //��cbr��vbr����Ϊ0
	unsigned char data01;             // ���㷽ʽ���� v2.0����ȡֵ -> 1
	unsigned char data02;             // ���㷽ʽ���� v2.0����ȡֵ -> 15(0x0F)
	unsigned char data03;             // ���㷽ʽ���� ����ȡֵ -> 1
	unsigned char data04;             // ���㷽ʽ���� ����ȡֵ -> 0,1
	unsigned char data05;             // ���㷽ʽ���� ����ȡֵ -> 128(0x80)
	unsigned char data06;             // ���㷽ʽ���� ����ȡֵ -> 128(0x80),32(0x20)
	unsigned char data07;             // ���㷽ʽ���� ����ȡֵ -> 0,32(0x20)
	unsigned char data08;				 // ���㷽ʽ���� ����ȡֵ -> 0,8
	unsigned char reserve1;			 // ����
	unsigned char reserve2;			 // ����
	unsigned char data09;				 // ���ڼ������
};

struct loops
{
	unsigned int start;            // ѭ����ʼ��
	unsigned int end;              // ѭ��������
	unsigned short count;          // ѭ��������0x80=������
};


void Get_hcaInfo(un char* filedata, un long long filesize);
void Get_hcaInfo_nomeg(un char* filedata, un long long filesize);


