#ifndef HCACODEC_H
#define HCACODEC_H

static float hca_volume = 1.0f;

struct hcachannel
{
	int type;
	un char valueptr;
	float block[0x80];
	float base[0x80];
	un char value[0x80];
	un char scale[0x80];
	un int count;
};

class hcacodec 
{
	public:
		struct decoder
		{
			un short channelnum;
			un char* channelbuffer[16];
			un long long channelsize[16];
		};
		unsigned short get_checksum_result(unsigned char* checkdata, unsigned long long size);
		decoder* decode_16bits(un char* filedata, un long long filesize);
		void decode_step1(un char* preblock, un int block_offset, un int temp2);
};
#endif