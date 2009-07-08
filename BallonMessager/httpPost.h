#pragma once

#include <curl/curl.h>

class httpPost
{
public:
	httpPost(void);
	~httpPost(void);
	void doPost(void);
private:
	// post所用的头指针,用于curl_easy_setopt(curl, CURLOPT_HTTPPOST, post)的第三个参数
	struct curl_httppost*post;
	// post链表的尾指针,用于增加插入数据的速度
	struct curl_httppost*last;
public:
	// 为post增加一个field
	CURLFORMcode addField(const char* name, const char* value, const char* mime_type);
};
