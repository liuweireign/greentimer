#include ".\httppost.h"

httpPost::httpPost(void)
: post(NULL)
, last(NULL)
{
}

httpPost::~httpPost(void)
{
}

void httpPost::doPost(void)
{
}

// 为post增加一个field
CURLFORMcode httpPost::addField(const char* name, const char* value, const char* mime_type=NULL)
{
	return CURLFORMcode();
}
