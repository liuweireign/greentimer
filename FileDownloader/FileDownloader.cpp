// TestCUrl.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <curl/curl.h>
using namespace std;

//收到进度消息，显示下载进度
int OnProgress(void *clientp, double dltotal, double
        dlnow, double ultotal, double ulnow)
{
	int iLen = dlnow*60/dltotal;

	for(int i=0;i<71;i++)
	printf("%c", 0x8);

	for(int i=0;i<=60;i++)
	{
		if (i == 0 || i==60) cout << "|";
	    else if(i<iLen) cout<<"*";
		else if(i==iLen) cout << ">";
		else cout<<"-";
	}

	printf("\t%2.2f%%", 100*dlnow/dltotal); 

    return 0;
}

//当数据到达，写入磁盘里，并且显示下载的块数（以点号表示）
size_t OnData( void *ptr, size_t size, size_t nmemb, void *stream)
{
	ofstream &ofs = *(ofstream *)stream;
	ofs.write((char *)ptr,size*nmemb);
	return size*nmemb;
}

int _tmain(int argc, _TCHAR* argv[])
{
	if (argc!=3)
	{
		cout<<"请输入url和保存地址两个参数。"<<endl;
		return 0;
	}
	char *strUrl = argv[1];
	char *strSavePath = argv[2];

	ofstream ofs(strSavePath,ios_base::binary);
	CURL *curl;
	CURLcode res;

	//curl初始化
	curl = curl_easy_init();
	if(!curl) {
		return 0;
	}

	//设置要下载的URL
	curl_easy_setopt(curl, CURLOPT_URL, strUrl);

	//设置数据到达回调函数
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, OnData);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &ofs); //这是传递给OnData的第一个参数

	//设置下载进度回调函数
	curl_easy_setopt(curl, CURLOPT_NOPROGRESS,FALSE);  //先启动此功能
	curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, OnProgress);

	cout<<"开始下载："<<strUrl<<endl;
	cout<<"保存位置："<<strSavePath<<endl;

	for(int i=0;i<=60;i++)
	{
		if (i == 0 || i==60) cout << "|";
		else if((i*10/6)%10 == 0) cout<<"+";
		else cout<<"-";
	}

	cout << endl;

	//阻塞执行下载操作（如果是在一个线程中调用，可以达到不阻塞的目的）
	res = curl_easy_perform(curl);
	if(0!=res)
	{
		return 0;
	}

	//退出CURL
	curl_easy_cleanup(curl);

	ofs.flush();
	cout<<endl;
	cout<<"下载完成："<<strSavePath<<endl;
	return 0;
}

