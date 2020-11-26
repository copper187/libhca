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
	if (filedata[0x28] == 0x6c || filedata[0x28] == 0xec) //����loop��Ϣ
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
		cout << "HCA���ͣ��Ѽ��ܵ�HCA�ļ�" << endl;
		goto printinf;
	}
	if (filedata[0] == 0x48 && filedata[1] == 0x43 && filedata[2] == 0x41 && filedata[3] == 0x00)
	{
		cout << "HCA���ͣ�δ���ܵ�HCA�ļ�" << endl;
	}
	else
	{
		cout << "���󣬲���HCA��Ƶ�ļ�" << endl;
		return;
	}
printinf:;
	cout << "HCA�汾��V" << maininf.version << endl;
	cout << hex << "���ݿ���ʼλ�ã�0x" << maininf.block_start_offset << endl;
	cout << dec << "�ļ���С��" << maininf.filesize << " Bytes" << endl;
	cout << "��������" << (int)fmt.channel_num;//<<endl;
	if (fmt.channel_num == 2)
	{
		cout << "����������" << endl;
	}
	else
	{
		cout << endl;
	}
	cout << "�����ʣ�" << fmt.sampling_rate << "Hz" << endl;
	cout << "���ݿ�������" << fmt.blocknum << endl;
	cout << "��ʼ��������������" << (fmt.start_muteblock_num - 128) / 0x400 << endl;
	cout << "ĩβ��������������" << fmt.end_muteblock_num << endl;
	cout << "�Ƿ�Ϊѭ��������Ƶ��";
	if (maininf.enable_loop == true)
	{
		cout << "��" << endl;
		cout << "ѭ����ʼ�飺��" << loop.start << "��" << endl;
		cout << "ѭ�������飺��" << loop.end << "��" << endl;
		cout << "ѭ��������";
		if (loop.count == 0x80)
		{
			cout << "������" << endl;
		}
		else
		{
			cout << dec << loop.count << endl;
		}
	}
	else
	{
		cout << "��" << endl;
		cout << hex << "ѭ����ʼ�飺������" << endl;
		cout << "ѭ�������飺������" << endl;
		cout << "ѭ��������������" << endl;
	}
	cout << "���뷽ʽ��CBR/VBR����";
	if (comp.blocksize == 0)
	{
		cout << "VBR" << endl;
	}
	else
	{
		cout << "CBR" << endl;
	}
	cout << "ÿ���ݿ��С������CBR����";
	if (comp.blocksize == 0)
	{
		cout << "�ޣ����뷽ʽΪVBR��" << endl;
	}
	else
	{
		cout << dec << comp.blocksize << " Bytes" << endl;
	}
	cout << "���ʣ�" << maininf.bitrate << "kbps" << endl;
	int minute = maininf.audiolength / 60;
	int second = maininf.audiolength % 60;
	cout << "��Ƶ���ȣ�" << minute << "min" << second << "s" << endl;
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
	if (filedata[0x28] == 0x6c || filedata[0x28] == 0xec) //����loop��Ϣ
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

