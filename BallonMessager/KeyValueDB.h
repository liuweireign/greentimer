#pragma once

//#include "sphttpmsg.hpp"
#include "HttpServ.h"

//========================================
//读系统信息
void handlerReadSys(TReq * request, TResp * response);
//写键值
void handlerWrite(TReq * request, TResp * response);
//读键值
void handlerRead(TReq * request, TResp * response);
//日志
void handlerLog(TReq * request, TResp * response);
//日志
void handlerShowLog(TReq * request, TResp * response);
//gnugo
void handlerGnugo(TReq * request, TResp * response);
//文件服务器
void handlerFile(TReq * request, TResp * response);
//文件服务器
void handlerDir(TReq * request, TResp * response);