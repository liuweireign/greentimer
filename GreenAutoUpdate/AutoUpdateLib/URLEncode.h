#pragma once
#include <string>

class URLEncode
{
public:
	static std::string escapeURL(const std::string &URL);
	static std::string deescapeURL(const std::string &URL);
private:
	static char dec2hexChar(short int n);
	static short int hexChar2dec(char c);
};
