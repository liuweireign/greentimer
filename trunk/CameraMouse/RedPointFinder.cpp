//#include "stdafx.h"
#include ".\redpointfinder.h"

#define PICK_RED(rgb) (rgb%256)

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
	unsigned int uiPower = PICK_RED(*(m_ptPixels + y*m_iWidth + x));
	for (unsigned int i=1;i<r;i++)	//注意，0点已经在前面计算
	{
		//右边三点
		uiPower += PICK_RED(*(m_ptPixels + (y+r)*m_iWidth + x + r));
		uiPower += PICK_RED(*(m_ptPixels + (y-r)*m_iWidth + x + r));
		uiPower += PICK_RED(*(m_ptPixels + y*m_iWidth + x + r));

		//左边三点
		uiPower += PICK_RED(*(m_ptPixels + (y+r)*m_iWidth + x - r));
		uiPower += PICK_RED(*(m_ptPixels + (y-r)*m_iWidth + x - r));
		uiPower += PICK_RED(*(m_ptPixels + y*m_iWidth + x - r));

		//中间两点（最中间那一点前面已经计算过了）
		uiPower += PICK_RED(*(m_ptPixels + (y+r)*m_iWidth + x));
		uiPower += PICK_RED(*(m_ptPixels + (y-r)*m_iWidth + x));
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

		int i = pos%m_iWidth;
		int j = pos/m_iWidth;
		//ATLASSERT((i*m_iWidth+j)==pos);
		unsigned int iPower = CalcPointPower(i,j,r);
		if (iPower==0)
		{
			continue;
		}
		if (iPower>iMaxPower)	//发现能量更强的点，旧的点全部落选
		{
			iMaxPower=iPower;
			vecPointOut.clear();
			vecPointOut.push_back(j*m_iWidth + i);
		}
		else if (iPower==iMaxPower)	//不分输赢，同时入选
		{
			vecPointOut.push_back(pos);
		}
	}
	return vecPointOut.size();
}

int RedPointFinder::GetPowerMost(unsigned int atLeastPower )
{
	unsigned int iMaxPower = 0;
	vector<unsigned int> vecPoints;

	//首先扫描一遍，找出以2为半径时的最强点
	for (unsigned int i=2;i<m_iWidth-2;i++)
	{
		for (unsigned int j=2;j<m_iHeight-2;j++)
		{
			if (0==*(m_ptPixels + i*m_iWidth + j))
			{
				continue;
			}
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
			int iPos = j*m_iWidth + i;
			vecPoints.push_back(iPos);
		}
	}
	if (vecPoints.empty())
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
	if (vecPoints.size()>1)
	{
		return -1;
	}
	return vecPoints[0];
}