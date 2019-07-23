#pragma once

namespace PUBLIC
{

class MD5
{
	struct MD5Context
	{
		int buf[4];
		int bits[2];
		unsigned char in[64];
	} ctx_;

	void MD5Init(struct MD5Context *context);
	void MD5Update(struct MD5Context *context, unsigned char const *buf,
		unsigned len);
	void MD5Final(unsigned char digest[16], struct MD5Context *context);
	void MD5Transform(int buf[4], int const in[16]);
public:
	void MD5Make(unsigned char digest[16], unsigned char const *buf,
		unsigned len);
};

}

