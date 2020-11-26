#ifndef HCACRYPTO_H
#define HCACRYPTO_H

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

#define un unsigned 

struct hca_crypto_data
{
	un char Crypto_Table1[8];
	un char Crypto_Table2[16];
	un char Crypto_Table3[16];
	un char Crypto_Table4[16];
	un char Crypto_Table5[256];
	un char Crypto_Table6[256];
	un long long key1;
	un long long key2;
};
class hcacrypto
{
	private:
		
	public:
		void build_table(un long long* key);
		void decrypt(un char *filedata,un long long filesize,un long long* key);
		void encrypt(un char *filedata,un long long filesize,un long long* key);
};


#endif
