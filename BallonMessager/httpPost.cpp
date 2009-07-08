#include ".\httppost.h"

httpPost::httpPost(void)
: post(NULL)
, last(NULL)
, curl(NULL)
{
}

#ifdef _DEBUG
size_t httpPost::OnData( void *ptr, size_t size, size_t nmemb, void *stream)
{
	std::ofstream &ofs = *(std::ofstream *)stream;
	ofs.write((char *)ptr,size*nmemb);
	return size*nmemb;
}
#endif

httpPost::~httpPost(void)
{
	if (NULL != post)
		curl_formfree(post);

	if (curl)
		curl_easy_cleanup(curl);

#ifdef _DEBUG
	ofs.close();
#endif
}

bool httpPost::doPost(void)
{
	if (!curl)
	{
		return false;
	}

	CURLcode res;
	curl_easy_setopt(curl, CURLOPT_HTTPPOST, post);
	res = curl_easy_perform(curl);

	// 0 means everything was ok, non-zero means an error occurred as <curl/curl.h> defines.
	// see http://curl.haxx.se/libcurl/c/curl_easy_perform.html for details
	if (0 == res)
		return true;
	else
		return false;
}

// 为post增加一个field
CURLFORMcode httpPost::addField(const char* name, const char* value, const char* mime_type)
{
	if (NULL == mime_type)
	{
		return curl_formadd(&post, &last, CURLFORM_COPYNAME, name,
				CURLFORM_COPYCONTENTS, value, CURLFORM_END);
	}
	else
	{
		return curl_formadd(&post, &last, CURLFORM_COPYNAME, name,
               CURLFORM_COPYCONTENTS, value,
               CURLFORM_CONTENTTYPE, mime_type, CURLFORM_END);
	}
}

// initialize the post, if ok return true, otherwise return false
bool httpPost::init(void)
{
	curl = curl_easy_init();
	if (!curl)
		return false;

	curl_easy_setopt(curl, CURLOPT_POST, TRUE);
	curl_easy_setopt(curl, CURLOPT_URL,
			serverPostUrl);

#ifdef _DEBUG
	curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);

	try{
		ofs.open(debugLogFile,std::ios_base::binary);
	}catch (...)
	{
		ofs.close();
		return false;
	}
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, OnData);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &ofs); //这是传递给OnData的第一个参数

#endif

	return true;
}
