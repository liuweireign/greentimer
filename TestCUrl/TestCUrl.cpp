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
    //return 0;
    //Downloader *downloader = reinterpret_cast<Downloader *>(clientp);

    //(void)ultotal;
    //(void)ulnow;

    //if (downloader->m_notifier)
    //    downloader->m_notifier->progressed(dltotal, dlnow);
	int iLen = dlnow*60/dltotal;
	for(int i=0;i<60;i++)
	{
		if(i<=iLen) cout<<"/";
		else cout<<"-";
	}
	//cout<<"downloaded:"<<(ULONG)dlnow<<"/"<<(ULONG)dltotal<<endl;
	cout<<endl;
    return 0;
}

//当数据到达，写入磁盘里，并且显示下载的块数（以点号表示）
size_t OnData( void *ptr, size_t size, size_t nmemb, void *stream)
{
	//cout<<(char *)ptr<<endl;
	/*for(int i=0;i<2*size*nmemb/512;i++)
	{
		cout<<".";
	}
	cout<<endl;*/

	ofstream &ofs = *(ofstream *)stream;
	ofs.write((char *)ptr,size*nmemb);
	return size*nmemb;
}

int _tmain(int argc, _TCHAR* argv[])
{
	ofstream ofs("c:\\aaa.jpg",ios_base::binary);
	CURL *curl;
	CURLcode res;

	curl = curl_easy_init();
	if(!curl) {
		return 0;
	}

	curl_easy_setopt(curl, CURLOPT_URL, "http://4.bp.blogspot.com/_pwYciwZqdIM/SVoxicLu_II/AAAAAAAAB38/IQZ8CfvvUsc/s1600/DSCF3837.JPG");
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, OnData);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &ofs);
	curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, OnProgress);
	curl_easy_setopt(curl, CURLOPT_NOPROGRESS,FALSE);

	cout<<"开始下载："<<"http://4.bp.blogspot.com/_pwYciwZqdIM/SVoxicLu_II/AAAAAAAAB38/IQZ8CfvvUsc/s1600/DSCF3837.JPG"<<endl;
	cout<<"保存位置："<<"c:\\aaa.jpg"<<endl;

	//执行下载操作（如果是在一个线程中调用，可以达到不阻塞的目的）
	res = curl_easy_perform(curl);
	if(0!=res)
	{
		return 0;
	}

	/* always cleanup */
	curl_easy_cleanup(curl);
	cout<<"下载完成："<<"c:\\aaa.jpg"<<endl;
	char c;
	std::cin>>c;
	return 0;
}

