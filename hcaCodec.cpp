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
#include "hcaCrypto.h"
#include "hcaInfo.h"
#include "hcaCodec.h"
#include "hcaSupportLib.h"
#include "hcaTables.h"

#define un unsigned 

using namespace std;



extern struct mainInfo maininf;
extern struct compress comp;
extern struct format fmt;
extern struct loops loop;
struct hcachannel hca_channel[16];
un char testt[4] = { 0xcd,0x22,0xf5,0x43 };
unsigned short hcacodec::get_checksum_result(unsigned char* checkdata, unsigned long long size)
{
	un short result = 0;
	for (un long long i = 0 ;i < size; i++)
	{
		result = (result << 8) ^ checksumTable[(result >> 8) ^ checkdata[i]];
	}
	return result;
}
void hcacodec::decode_step1(un char* present_block, un int block_offset, un int temp2)
{

}
hcacodec::decoder* hcacodec::decode_16bits(un char* filedata, un long long filesize)
{
	Get_hcaInfo_nomeg(filedata, filesize);
	hcacodec::decoder* wavedata;
	wavedata = (hcacodec::decoder*)&fmt.channel_num;
	//检查加密状态
	if (filedata[0x2d] == 0x38)
	{
		cout << "libhca: 此hca文件为加密媒体，请先解密！" << endl;
		return (hcacodec::decoder*) - 1;
	}
	/***************解码初始化 开始***************/
	//计算data09
	comp.data09 = ceil2(comp.data05 - (comp.data06 + comp.data07), comp.data08);
	//准备每个声道信息
	un int temp1 = fmt.channel_num / comp.data03;
	if (temp1 > 1)
	{
		switch (temp1) {
		case 2:codec_athTable[0] = 1; codec_athTable[1] = 2; break;
		case 3:codec_athTable[0] = 1; codec_athTable[1] = 2; break;
		case 4:codec_athTable[0] = 1; codec_athTable[1] = 2; if (comp.data04 == 0) { codec_athTable[2] = 1; codec_athTable[3] = 2; }break;
		case 5:codec_athTable[0] = 1; codec_athTable[1] = 2; if (comp.data04 <= 2) { codec_athTable[3] = 1; codec_athTable[4] = 2; }break;
		case 6:codec_athTable[0] = 1; codec_athTable[1] = 2; codec_athTable[4] = 1; codec_athTable[5] = 2; break;
		case 7:codec_athTable[0] = 1; codec_athTable[1] = 2; codec_athTable[4] = 1; codec_athTable[5] = 2; break;
		case 8:codec_athTable[0] = 1; codec_athTable[1] = 2; codec_athTable[4] = 1; codec_athTable[5] = 2; codec_athTable[6] = 1; codec_athTable[7] = 2; break;
		}
	}
	for (int i = 0; i < fmt.channel_num; i++)
	{
		hca_channel[i].type = codec_athTable[i];
		hca_channel[i].valueptr = comp.data06 + comp.data07;
		hca_channel[i].count = comp.data06 + ((codec_athTable[i] != 2) ? comp.data07 : 0);
	}
	/***************解码初始化 结束***************/
	/***************主解码过程 开始***************/
	//准备和读取原始块数据
	un char* present_block;
	un int block_offset;
	for (int i = 0; i < fmt.blocknum; i++)
	{
		present_block = filedata + maininf.block_start_offset + (i * comp.blocksize);
		block_offset = 0;
		int magic = GetBit(present_block , 16);
		//检查块起始标识
		if (magic != 0xFFFF)   //起始标志为固定的0xFFFF
		{
			cout << "libcbg: 错误. 解码第" << i + 1 << "数据块失败，块起始标志检查失败。Error code = 2.";
			return wavedata;
		}
		block_offset += 2;
		//检查checksum值
		if (get_checksum_result(present_block, comp.blocksize - 2) != *(un short *)present_block + comp.blocksize - 1)
		{
			cout << "libcbg: 错误. 解码第" << i + 1 << "数据块失败，检查checksum失败。Error code = 3.";
			return wavedata;
		}
		un int temp2 = GetBit(present_block + block_offset, 8);
		block_offset += 1;
		temp2 <<= 1;
		if (GetBit(present_block + block_offset, 8) >= 0x80)
		{
			temp2 += 1;
			temp2 -= temp2 - (GetBit(present_block + block_offset, 8) - 0x80);
		}
		else
		{
			temp2 += 0;
			temp2 -= temp2 - GetBit(present_block + block_offset, 8);
		}
		//解码第一阶段
		decode_step1(present_block, block_offset, temp2);
	}
	
	
	
}