#pragma once

#include "ReverseVector.h"
#include "MouseCtrler.h"

struct BallPos 
{
	BallPos(int _x,int _y)
		:x(_x),y(_y)
	{
	}
	int x;
	int y;
};

class BallTracker
{
public:
	BallTracker();
	~BallTracker(void);

	void Init(int iWidth, int iHeight, MouseCtrler *pMouseCtrler);
	void OnBallPos(int x,int y,int d);
private:
	MouseCtrler *m_pMouseCtrler;
	int m_iWidth;
	int m_iHeight; 

	//摄像头中的哪一点相当于屏幕中心位置？
	std::pair<int,int> m_ptCenter;
	//小球在摄像头中移动一像素相当于在界面上移动多少像素？
	int m_iZoom;

	int m_pfs;
	std::pair<int,int> m_ScreenSize;

	ReverseVector<BallPos> m_rvecPoints;

private:
	//判断是否在iTime时间内静止不动（单位：毫秒）
	bool IsStatic(int iTime) const;
	static BallPos CalcAveragePos(const BallPos *pData,int count);
	static int Distance(const BallPos &bp1,const BallPos &bp2);
};
