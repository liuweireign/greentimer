#pragma once

#include <curl/curl.h>
#include <string>

#ifdef _DEBUG
#include <fstream>
#endif


class httpPost
{
public:
	httpPost(void);
	~httpPost(void);
	bool doPost(void);
private:
	// post所用的头指针,用于curl_easy_setopt(curl, CURLOPT_HTTPPOST, post)的第三个参数
	struct curl_httppost *m_pPost;
	// post链表的尾指针,用于增加插入数据的速度
	struct curl_httppost *m_pLast;

#ifdef _DEBUG
	std::ofstream ofs;
#endif

	//当数据到达，写入磁盘里
	static size_t OnData( void *ptr, size_t size, size_t nmemb, void *pInstance);
	
public:
	// 为post增加一个field
	bool addField(const char* name, const char* value, const char* mime_type=NULL);
private:
	CURL* m_pCurl;
public:
	// initialize the post, if ok return true, otherwise return false
	bool init(void);

	//取post之后,服务器返回的数据
	std::string getResult(void);
private:
	//post的result将写到这个buffer里面,可以通过getResult()取得
	std::string resultBuffer;
};
