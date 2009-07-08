#pragma once

#include <curl/curl.h>

#ifdef _DEBUG
#include <fstream>
const char * debugLogFile = "out.htm";
#endif

const char * serverPostUrl = "http://192.168.92.128/post.php";

class httpPost
{
public:
	httpPost(void);
	~httpPost(void);
	bool doPost(void);
private:
	// post所用的头指针,用于curl_easy_setopt(curl, CURLOPT_HTTPPOST, post)的第三个参数
	struct curl_httppost*post;
	// post链表的尾指针,用于增加插入数据的速度
	struct curl_httppost*last;

#ifdef _DEBUG
	std::ofstream ofs;

	//当数据到达，写入磁盘里，并且显示下载的块数（以点号表示）
	size_t OnData( void *ptr, size_t size, size_t nmemb, void *stream);
#endif
	
public:
	// 为post增加一个field
	CURLFORMcode addField(const char* name, const char* value, const char* mime_type=NULL);
private:
	CURL* curl;
public:
	// initialize the post, if ok return true, otherwise return false
	bool init(void);
};
