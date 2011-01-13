// testhttpsync.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "httpSync.h"
#include "httpPost.h"
int login(const char *strUserName, const char *strPwd){
	httpPost hp("http://note.memeshu.com:8080/login.n");
	hp.init();

	/*
	lemail:***
	lpassword:***
	client:client
	rember:on
	url:null
	*/
	hp.addField("lemail","tantianzuo@gmail.com");
	hp.addField("lpassword","51211765");
	hp.addField("client","client");
	hp.addField("rember","on");
	hp.addField("url","");
	hp.doPost();
	std::string strRes = hp.getResult();
	ATLTRACE(strRes.c_str());
	return 0;
}

//int addnote(const std::map<std::string,std::string> &mapData){
int addnote(const char *strTitle, const char *strContent,const char *strUrl="", const char *strTags=""){
	httpPost hp("http://note.memeshu.com:8080/note/");
	hp.init();
	/*
	Method:post
	url:/note/?
	参数：
		 t=add
		 at=ajax
		 title=        #标题。必填
		 category=    #1、文章  2、url书签
		 tags=       #标签，使用逗号（,）分隔
		 content=   #文章内容
		 url=        #书签url                  url和content至少填一个
	*/     
	hp.addField("t","add");
	hp.addField("at","ajax");
	hp.addField("title",strTitle);
	hp.addField("category","1");
	hp.addField("tags",strTags);
	hp.addField("content",strContent);
	hp.addField("url",strUrl);
	hp.doPost();
	std::string strRes = hp.getResult();
	ATLTRACE(strRes.c_str());
	return 0;
}

int main(int argc, _TCHAR* argv[])
{
	//httpPost hp("http://jingguangyin.appspot.com/TestPost");
	//login("tantianzuo@gmail.com","51211765");

	addnote("test","hello world!");


	//std::map<std::string,std::string> mapData;
	//mapData["data"] = "hello,world";
	//post(mapData);

	return 0;
}

