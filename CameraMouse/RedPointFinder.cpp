//#include "stdafx.h"
#include ".\redpointfinder.h"

#define PICK_RED(rgb) ((rgb)%256)

RedPointFinder::RedPointFinder(unsigned int * ptPixels,unsigned int iWidth,unsigned int iHeight)
{
	m_ptPixels = ptPixels;
	m_iWidth = iWidth;
	m_iHeight = iHeight;
}

RedPointFinder::~RedPointFinder(void)
{
}

unsigned int RedPointFinder::CalcPointPower(unsigned int x,unsigned int y,unsigned int r )
{
	//所要判断的点太靠近边缘，那么以r为半径的圆可能会超出图像边界。
	if( (abs(x)<(int)r)
		|| (abs(x-m_iWidth)<(int)r)
		|| (abs(y)<(int)r)
		|| (abs(y-m_iHeight)<(int)r) )
	{
		return 0;
	}

	//向8个方向计算其数值之和
	unsigned int uiPower = PICK_RED( GetColor(x, y));
	for (unsigned int i=1;i<r;i++)	//注意，0点已经在前面计算
	{
		//右边三点
		uiPower += PICK_RED( GetColor(x + i, y + i) );
		uiPower += PICK_RED( GetColor(x + i, y - i) );
		uiPower += PICK_RED( GetColor(x + i, y ) );

		//左边三点
		uiPower += PICK_RED( GetColor(x - i, y + i) );
		uiPower += PICK_RED( GetColor(x - i, y - i) );
		uiPower += PICK_RED( GetColor(x - i, y ) );

		//中间两点（最中间那一点前面已经计算过了）
		uiPower += PICK_RED( GetColor(x , y + i) );
		uiPower += PICK_RED( GetColor(x , y - i) );
	}

	return uiPower;
}

int RedPointFinder::CheckPowerMost(unsigned int r,const vector<unsigned int> &vecPoints,vector<unsigned int> &vecPointOut )
{
	unsigned int iMaxPower = 0;
	vector<unsigned int>::const_iterator it = vecPoints.begin();
	for (;it!=vecPoints.end();it++)
	{
		int pos = *it;

		int i = GetX(pos);
		int j = GetY(pos);
		//ATLASSERT((i*m_iWidth+j)==pos);
		unsigned int iPower = CalcPointPower(i,j,r);
		if (iPower<iMaxPower)
		{
			continue;
		}
		if (iPower>iMaxPower)	//发现能量更强的点，旧的点全部落选
		{
			iMaxPower=iPower;
			vecPointOut.clear();
		}
		vecPointOut.push_back(pos);
	}
	return vecPointOut.size();
}

int RedPointFinder::GetPowerMost(unsigned int atLeastPower )
{
	unsigned int iMaxPower = 0;
	vector<unsigned int> vecPoints;

	//首先扫描一遍，找出以2为半径时的最强点
	for(int pos=0;pos<m_iWidth*m_iHeight;pos++)
	{
		int i=GetX(pos);
		int j=GetY(pos);

		unsigned int iPower = CalcPointPower(i,j,2);
		if (iPower<atLeastPower)	//没达到阈值要求，看下一点
		{
			continue;
		}

		if (iPower>iMaxPower)	//发现能量更强的点，旧的点全部先清空
		{
			iMaxPower=iPower;
			vecPoints.clear();
		}
		vecPoints.push_back(pos);
	}

	if (vecPoints.empty())	//全都不符合条件？
	{
		//ATLASSERT(FALSE);
		return -2;
	}

	//然后，加大半径值，找到最强的那一个点
	vector<unsigned int> vecPointTmp;
	for (unsigned int r=4;r<(m_iWidth/2);r+=2)
	{
		if(CheckPowerMost(r,vecPoints,vecPointTmp)<=1)
		{
			vecPoints.swap(vecPointTmp);
			break;
		}
		vecPoints.swap(vecPointTmp);
		vecPointTmp.clear();
	}
	//if (vecPoints.size()>1)	//居然有那么多高手不分上下？
	//{
	//	return -1;
	//}
	//好，最强点只有一个了
	return vecPoints[0];
}