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
#include "hcaInfo.h"
#include "hcaSupportLib.h"

mainInfo maininf;
compress comp;
format fmt;
loops loop;

void Get_hcaInfo(un char* filedata, un long long filesize)
{
	memcpy(&maininf.version, filedata + 4, 2);
	//	maininf.version = bswap(maininf.version);
	memcpy(&maininf.block_start_offset, filedata + 6, 2);
	maininf.block_start_offset = bswap(maininf.block_start_offset);
	maininf.filesize = filesize;
	memcpy(&fmt.channel_num, filedata + 0xc, 1);
	memcpy(&fmt.sampling_rate, filedata + 0xc, 4);
	fmt.sampling_rate &= 0xFFFFFF00;
	fmt.sampling_rate = bswap(fmt.sampling_rate);
	memcpy(&fmt.blocknum, filedata + 0x10, 4);
	fmt.blocknum = bswap(fmt.blocknum);
	memcpy(&fmt.start_muteblock_num, filedata + 0x14, 2);
	fmt.start_muteblock_num = bswap(fmt.start_muteblock_num);
	memcpy(&fmt.end_muteblock_num, filedata + 0x16, 2);
	fmt.end_muteblock_num = bswap(fmt.end_muteblock_num);
	memcpy(&comp.blocksize, filedata + 0x1c, 2);
	comp.blocksize = bswap(comp.blocksize);
	memcpy(&comp.data01, filedata + 0x1e, 1);
	memcpy(&comp.data02, filedata + 0x1f, 1);
	memcpy(&comp.data03, filedata + 0x20, 1);
	memcpy(&comp.data04, filedata + 0x21, 1);
	memcpy(&comp.data05, filedata + 0x22, 1);
	memcpy(&comp.data06, filedata + 0x23, 1);
	memcpy(&comp.data07, filedata + 0x24, 1);
	memcpy(&comp.data08, filedata + 0x25, 1);
	maininf.bitrate = fmt.sampling_rate * comp.blocksize / 128 / 1000;
	maininf.audiolength = fmt.blocknum * comp.blocksize / 1000 * 8 / maininf.bitrate;
	if (filedata[0x28] == 0x6c || filedata[0x28] == 0xec) //含有loop信息
	{
		maininf.enable_loop = true;
		memcpy(&loop.start, filedata + 0x2c, 4);
		loop.start = bswap(loop.start);
		memcpy(&loop.end, filedata + 0x30, 4);
		loop.end = bswap(loop.end);
		memcpy(&loop.count, filedata + 0x34, 2);
		loop.count = bswap(loop.count);
	}
	//system("chcp 1200");
	if (filedata[0] == 0xc8 && filedata[1] == 0xc3 && filedata[2] == 0xc1 && filedata[3] == 0x00)
	{
		cout << "HCA类型：已加密的HCA文件" << endl;
		goto printinf;
	}
	if (filedata[0] == 0x48 && filedata[1] == 0x43 && filedata[2] == 0x41 && filedata[3] == 0x00)
	{
		cout << "HCA类型：未加密的HCA文件" << endl;
	}
	else
	{
		cout << "错误，不是HCA音频文件" << endl;
		return;
	}
printinf:;
	cout << "HCA版本：V" << maininf.version << endl;
	cout << hex << "数据块起始位置：0x" << maininf.block_start_offset << endl;
	cout << dec << "文件大小：" << maininf.filesize << " Bytes" << endl;
	cout << "声道数：" << (int)fmt.channel_num;//<<endl;
	if (fmt.channel_num == 2)
	{
		cout << "（立体声）" << endl;
	}
	else
	{
		cout << endl;
	}
	cout << "采样率：" << fmt.sampling_rate << "Hz" << endl;
	cout << "数据块数量：" << fmt.blocknum << endl;
	cout << "起始段无声块数量：" << (fmt.start_muteblock_num - 128) / 0x400 << endl;
	cout << "末尾段无声块数量：" << fmt.end_muteblock_num << endl;
	cout << "是否为循环播放音频：";
	if (maininf.enable_loop == true)
	{
		cout << "是" << endl;
		cout << "循环开始块：第" << loop.start << "块" << endl;
		cout << "循环结束块：第" << loop.end << "块" << endl;
		cout << "循环次数：";
		if (loop.count == 0x80)
		{
			cout << "无限制" << endl;
		}
		else
		{
			cout << dec << loop.count << endl;
		}
	}
	else
	{
		cout << "否" << endl;
		cout << hex << "循环开始块：不适用" << endl;
		cout << "循环结束块：不适用" << endl;
		cout << "循环次数：不适用" << endl;
	}
	cout << "编码方式（CBR/VBR）：";
	if (comp.blocksize == 0)
	{
		cout << "VBR" << endl;
	}
	else
	{
		cout << "CBR" << endl;
	}
	cout << "每数据块大小（仅限CBR）：";
	if (comp.blocksize == 0)
	{
		cout << "无（编码方式为VBR）" << endl;
	}
	else
	{
		cout << dec << comp.blocksize << " Bytes" << endl;
	}
	cout << "码率：" << maininf.bitrate << "kbps" << endl;
	int minute = maininf.audiolength / 60;
	int second = maininf.audiolength % 60;
	cout << "音频长度：" << minute << "min" << second << "s" << endl;
}
void Get_hcaInfo_nomeg(un char* filedata, un long long filesize)
{
	memcpy(&maininf.version, filedata + 4, 2);
	//	maininf.version = bswap(maininf.version);
	memcpy(&maininf.block_start_offset, filedata + 6, 2);
	maininf.block_start_offset = bswap(maininf.block_start_offset);
	maininf.filesize = filesize;
	memcpy(&fmt.channel_num, filedata + 0xc, 1);
	memcpy(&fmt.sampling_rate, filedata + 0xc, 4);
	fmt.sampling_rate &= 0xFFFFFF00;
	fmt.sampling_rate = bswap(fmt.sampling_rate);
	memcpy(&fmt.blocknum, filedata + 0x10, 4);
	fmt.blocknum = bswap(fmt.blocknum);
	memcpy(&fmt.start_muteblock_num, filedata + 0x14, 2);
	fmt.start_muteblock_num = bswap(fmt.start_muteblock_num);
	memcpy(&fmt.end_muteblock_num, filedata + 0x16, 2);
	fmt.end_muteblock_num = bswap(fmt.end_muteblock_num);
	memcpy(&comp.blocksize, filedata + 0x1c, 2);
	comp.blocksize = bswap(comp.blocksize);
	memcpy(&comp.data01, filedata + 0x1e, 1);
	memcpy(&comp.data02, filedata + 0x1f, 1);
	memcpy(&comp.data03, filedata + 0x20, 1);
	memcpy(&comp.data04, filedata + 0x21, 1);
	memcpy(&comp.data05, filedata + 0x22, 1);
	memcpy(&comp.data06, filedata + 0x23, 1);
	memcpy(&comp.data07, filedata + 0x24, 1);
	memcpy(&comp.data08, filedata + 0x25, 1);
	maininf.bitrate = fmt.sampling_rate * comp.blocksize / 128 / 1000;
	maininf.audiolength = fmt.blocknum * comp.blocksize / 1000 * 8 / maininf.bitrate;
	if (filedata[0x28] == 0x6c || filedata[0x28] == 0xec) //含有loop信息
	{
		maininf.enable_loop = true;
		memcpy(&loop.start, filedata + 0x2c, 4);
		loop.start = bswap(loop.start);
		memcpy(&loop.end, filedata + 0x30, 4);
		loop.end = bswap(loop.end);
		memcpy(&loop.count, filedata + 0x34, 2);
		loop.count = bswap(loop.count);
	}
}

