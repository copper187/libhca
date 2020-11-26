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

#define un unsigned 

extern struct mainInfo maininf;
extern struct compress comp;
extern struct format fmt;
extern struct loops loop;

hca_crypto_data hca_crypto;

void hcacrypto::build_table(un long long* key)
{
	
	hca_crypto.key1 = key[0];
	hca_crypto.key2 = key[1];
	// build table1
	if (!hca_crypto.key1)
	{
		hca_crypto.key2--;
	}
	else
	{
		hca_crypto.key1--;
	}
	for (int i = 0; i<7; i++)
	{
		hca_crypto.Crypto_Table1[i] = hca_crypto.key1;
		hca_crypto.key1 = (hca_crypto.key1 >> 8) | (hca_crypto.key2 << 24);
		hca_crypto.key2 >>= 8;
	}
	//build table2
	hca_crypto.Crypto_Table2[0] = hca_crypto.Crypto_Table1[1];
	hca_crypto.Crypto_Table2[1] = hca_crypto.Crypto_Table1[1] ^ hca_crypto.Crypto_Table1[6];
	hca_crypto.Crypto_Table2[2] = hca_crypto.Crypto_Table1[2] ^ hca_crypto.Crypto_Table1[3];
	hca_crypto.Crypto_Table2[3] = hca_crypto.Crypto_Table1[2];
	hca_crypto.Crypto_Table2[4] = hca_crypto.Crypto_Table1[2] ^ hca_crypto.Crypto_Table1[1];
	hca_crypto.Crypto_Table2[5] = hca_crypto.Crypto_Table1[3] ^ hca_crypto.Crypto_Table1[4];
	hca_crypto.Crypto_Table2[6] = hca_crypto.Crypto_Table1[3];
	hca_crypto.Crypto_Table2[7] = hca_crypto.Crypto_Table1[3] ^ hca_crypto.Crypto_Table1[2];
	hca_crypto.Crypto_Table2[8] = hca_crypto.Crypto_Table1[4] ^ hca_crypto.Crypto_Table1[5];
	hca_crypto.Crypto_Table2[9] = hca_crypto.Crypto_Table1[4];
	hca_crypto.Crypto_Table2[10] = hca_crypto.Crypto_Table1[4] ^ hca_crypto.Crypto_Table1[3];
	hca_crypto.Crypto_Table2[11] = hca_crypto.Crypto_Table1[5] ^ hca_crypto.Crypto_Table1[6];
	hca_crypto.Crypto_Table2[12] = hca_crypto.Crypto_Table1[5];
	hca_crypto.Crypto_Table2[13] = hca_crypto.Crypto_Table1[5] ^ hca_crypto.Crypto_Table1[4];
	hca_crypto.Crypto_Table2[14] = hca_crypto.Crypto_Table1[6] ^ hca_crypto.Crypto_Table1[1];
	hca_crypto.Crypto_Table2[15] = hca_crypto.Crypto_Table1[6];
	//build table3
	un char temp_key = hca_crypto.Crypto_Table1[0];
	int mul = ((temp_key & 1) << 3) | 5;
	int add = (temp_key & 0xE) | 1;
	temp_key >>= 4;
	for (int i = 0; i<16; i++) 
	{
		temp_key = (temp_key * mul + add) & 0xF;
		hca_crypto.Crypto_Table3[i] = temp_key;
	}
	//build table5. table4 is a assist table to build table5.
	for (int i = 0; i<16; i++)
	{
		un char temp_key = hca_crypto.Crypto_Table2[i];
		int mul = ((temp_key & 1) << 3) | 5;
		int add = (temp_key & 0xE) | 1;
		temp_key >>= 4;
		for (int k = 0; k<16; k++) 
		{
			temp_key = (temp_key * mul + add) & 0xF;
			hca_crypto.Crypto_Table4[k] = temp_key;
		}
		unsigned char temp_key1 = hca_crypto.Crypto_Table3[i] << 4;
		for (int j = 0; j<16; j++)
		{
			unsigned char temp_key2 = hca_crypto.Crypto_Table4[j];
			hca_crypto.Crypto_Table5[i*16+j] = temp_key1 | temp_key2;
		}
	}
	//build table6. The main decrypt table.
	int target_subscript = 0;
	int j = 1;
	for (int i = 0; i < 256; i++)
	{
		target_subscript = (target_subscript + 0x11) & 0xFF;
		unsigned char result = hca_crypto.Crypto_Table5[target_subscript];
		if (result != 0 && result != 0xFF)
		{
			hca_crypto.Crypto_Table6[j] = result;
			j++;
		}
	}
	hca_crypto.Crypto_Table6[0] = 0;
	hca_crypto.Crypto_Table6[255] = 255;
}
void hcacrypto::decrypt(un char *filedata,un long long filesize,un long long* key)
{
	Get_hcaInfo_nomeg(filedata, filesize);
	hcacrypto::build_table(key);
	//还原标识 "HCA"
	un int decrypt_val = *(unsigned int*)filedata & 0x7f7f7f7f;
	memcpy(filedata, &decrypt_val, 4);
	//还原标识 "fmt"
	decrypt_val = *(unsigned int*)(filedata + 0x8) & 0x7f7f7f7f;
	memcpy(filedata + 0x8, &decrypt_val, 4);
	//还原标识 "comp"
	decrypt_val = *(unsigned int*)(filedata + 0x18) & 0x7f7f7f7f;
	memcpy(filedata + 0x18, &decrypt_val, 4);
	//还原标识 "loop"
	if (maininf.enable_loop == true)
	{
		decrypt_val = *(unsigned int*)(filedata + 0x28) & 0x7f7f7f7f;
		memcpy(filedata + 0x28, &decrypt_val, 4);
	}
	//还原标识 "ciph"
	if (maininf.enable_loop == true)
	{
		decrypt_val = *(unsigned int*)(filedata + 0x38) & 0x7f7f7f7f;
		memcpy(filedata + 0x38, &decrypt_val, 4);
	}
	else
	{
		decrypt_val = *(unsigned int*)(filedata + 0x28) & 0x7f7f7f7f;
		memcpy(filedata + 0x28, &decrypt_val, 4);
	}
	//还原标识 "pad"
	if (maininf.enable_loop == true)
	{
		decrypt_val = *(unsigned int*)(filedata + 0x3e) & 0x7f7f7f7f;
		memcpy(filedata + 0x3e, &decrypt_val, 4);
	}
	else
	{
		decrypt_val = *(unsigned int*)(filedata + 0x2e) & 0x7f7f7f7f;
		memcpy(filedata + 0x2e, &decrypt_val, 4);
	}
	//将加密标识符修改为未加密
	filedata[0x2d] = 0;
	//获取checksum值
	hcacodec hca_codec;
	unsigned short checksum = bswap(hca_codec.get_checksum_result(filedata, 0x5e));
	//写入checksum值
	memcpy(filedata + 0x5e, &checksum, 2);
	//解密数据段
	for (un long long i = 0; i < fmt.blocknum; i++)
	{
		for (int j = 0; j < comp.blocksize; j++)
		{
			un long long subscript = filedata[maininf.block_start_offset + (i * comp.blocksize) + j];
			memcpy(filedata + maininf.block_start_offset + (i * comp.blocksize) + j,&hca_crypto.Crypto_Table6[subscript],1);
		}
		checksum = bswap(hca_codec.get_checksum_result(filedata + maininf.block_start_offset + (i * comp.blocksize), comp.blocksize - 2));
		memcpy(filedata + maininf.block_start_offset + (i * comp.blocksize) + comp.blocksize - 2, &checksum, 2);
	}
}
void hcacrypto::encrypt(un char *filedata,un long long filesize,un long long* key)
{
	hcacrypto::build_table(key);
	
}

