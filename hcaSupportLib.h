#ifndef HCASUPPORTLIB_H
#define HCASUPPORTLIB_H
#define un unsigned 

//big endian转little endian
//引用自Nyagamon HCADecoder v1.21
/**********start**********/
inline short bswap(short v) { short r = v & 0xFF; r <<= 8; v >>= 8; r |= v & 0xFF; return r; }
inline unsigned short bswap(unsigned short v) { unsigned short r = v & 0xFF; r <<= 8; v >>= 8; r |= v & 0xFF; return r; }
inline int bswap(int v) { int r = v & 0xFF; r <<= 8; v >>= 8; r |= v & 0xFF; r <<= 8; v >>= 8; r |= v & 0xFF; r <<= 8; v >>= 8; r |= v & 0xFF; return r; }
inline unsigned int bswap(unsigned int v) { unsigned int r = v & 0xFF; r <<= 8; v >>= 8; r |= v & 0xFF; r <<= 8; v >>= 8; r |= v & 0xFF; r <<= 8; v >>= 8; r |= v & 0xFF; return r; }
inline long long bswap(long long v) { long long r = v & 0xFF; r <<= 8; v >>= 8; r |= v & 0xFF; r <<= 8; v >>= 8; r |= v & 0xFF; r <<= 8; v >>= 8; r |= v & 0xFF; r <<= 8; v >>= 8; r |= v & 0xFF; r <<= 8; v >>= 8; r |= v & 0xFF; r <<= 8; v >>= 8; r |= v & 0xFF; r <<= 8; v >>= 8; r |= v & 0xFF; return r; }
inline unsigned long long bswap(unsigned long long v) { unsigned long long r = v & 0xFF; r <<= 8; v >>= 8; r |= v & 0xFF; r <<= 8; v >>= 8; r |= v & 0xFF; r <<= 8; v >>= 8; r |= v & 0xFF; r <<= 8; v >>= 8; r |= v & 0xFF; r <<= 8; v >>= 8; r |= v & 0xFF; r <<= 8; v >>= 8; r |= v & 0xFF; r <<= 8; v >>= 8; r |= v & 0xFF; return r; }
inline float bswap(float v) { unsigned int i = bswap(*(unsigned int *)&v); return *(float *)&i; }
inline unsigned int ceil2(unsigned int a, unsigned int b) { return (b > 0) ? (a / b + ((a % b) ? 1 : 0)) : 0; }
/**********end**********/

//以下函数由作者实现


inline un int GetBit(un char* filebuffer, un int bitlength)
{
	//support max 32 bits (unsigned int)
	un int result = 0;
	un char present_byte = 0;
	for (int i = 0; i < 4; i++)
	{
		present_byte = filebuffer[i];
		if (bitlength >= 8)
		{
			result += present_byte;
			bitlength -= 8;
			if (bitlength == 0)
			{
				return result;
			}
			result <<= bitlength;

		}
		else
		{
			present_byte >>= (8 - bitlength);
			result += present_byte;
			return result;
		}
	}

}
inline un int GetBitPlus(un char* filebuffer,un int bitlength, un short extra_offset)
{
	//support max 32 bits (unsigned int)
	un int result = 0;
	un char present_byte = 0;
	//The different between function "GetBit" and function "GetBitPlus"
	/********start********/
	un char temp = 0;
	if (bitlength >= (8 - extra_offset))
	{
		temp = (*filebuffer << extra_offset);
		temp >>= extra_offset;
		result += temp;
		bitlength -= extra_offset;
		result <<= bitlength;
		if (bitlength == 0)
		{
			return result;
		}
	}
	else
	{
		temp = (*filebuffer << extra_offset);
		temp >>= (8-bitlength);
		result += temp;
		return result;
	}
	/********end********/
	for (int i = 0; i < 4; i++)
	{
		present_byte = filebuffer[i];
		if (bitlength >= 8)
		{
			result += present_byte;
			bitlength -= 8;
			if (bitlength == 0)
			{
				return result;
			}
			result <<= bitlength;

		}
		else
		{
			present_byte >>= (8 - bitlength);
			result += present_byte;
			return result;
		}
	}

}

#endif