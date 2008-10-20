#include "StdAfx.h"
#include "BlockFinder.h"
#include <set>
#include <map>
using namespace std;

//目标颜色
const COLORREF c_colorTarget = RGB(0XFF,0XFF,0);

bool BlockFinder::Init(int iWidth,int iHeight)
{
	m_iWidth = iWidth;
	m_iHeight = iHeight;
	return true;
}

bool BlockFinder::FindPoint( BYTE *pData,int &x,int &y,int &d )
{
	int powerMAX = 0;
	set<pair<int,int> > stPoints;

	//获取候选点
	//半径为4的范围内，有哪些最强点？
	for (int i=0;i<m_iWidth;i++)
	{
		for (int j=0;j<m_iHeight;j++)
		{
			COLORREF colorTmp = GetAvgPointColor(pData,j,i,10);

			//求出黄色部分的强度
			int iPower = GetRValue(colorTmp) + GetGValue(colorTmp) 
				- 2*GetBValue(colorTmp)	//（扣除灰度部分）
				- abs(GetRValue(colorTmp) - GetGValue(colorTmp)); //红色素与黄色素要接近
			if (iPower>powerMAX)
			{
				powerMAX = iPower;
				stPoints.clear();
				stPoints.insert(make_pair(i,j));
			}
			if (iPower==powerMAX)
			{
				stPoints.insert(make_pair(i,j));
			}
		}
	}
	if (stPoints.empty())
	{
		ASSERT(FALSE);
		return false;
	}

	//从候选点中，筛选出最佳的点
	//现在是判断面积最大的那个点
	//set<pair<int,int> > stLastPoints;
	//for (d=11;d<m_iHeight/3;d++)
	//{
	//	powerMAX = 0;
	//	set<pair<int,int> >::iterator it = stPoints.begin();
	//	for (;it!=stPoints.end();it++)
	//	{
	//		x = (*it).first;
	//		y = (*it).second;
	//		COLORREF colorTmp = GetAvgPointColor(pData,x,y,d);

	//		//求出黄色部分的强度
	//		int iPower = GetRValue(colorTmp) + GetGValue(colorTmp) 
	//			- 2*GetBValue(colorTmp)	//（扣除灰度部分）
	//			- abs(GetRValue(colorTmp) - GetGValue(colorTmp)); //红色素与黄色素要接近

	//		if (iPower>powerMAX)
	//		{
	//			powerMAX = iPower;
	//			stLastPoints.clear();
	//			stLastPoints.insert(*it);
	//		}
	//		if (iPower==powerMAX)
	//		{
	//			stLastPoints.insert(*it);
	//		}
	//	}
	//	if (stLastPoints.empty())	//在范围扩大后，有些色块的均值会变小，甚至变为0以下
	//	{
	//		break;
	//	}
	//	stPoints = stLastPoints;
	//	if (stPoints.size()==1)
	//	{
	//	}
	//}
	//if (stPoints.empty())
	//{
	//	ASSERT(FALSE);
	//	return false;
	//}

	//x = (*stPoints.begin()).first;
	//y = (*stPoints.begin()).second;

	//////////////////////////////////////////////////////////////////////////
	//第二种筛选方法：计算每个块的半径
	map<int,pair<int,int> > mapBlocks;
	set<pair<int,int> >::iterator it = stPoints.begin();
	for (;it!=stPoints.end();it++)
	{
		x = (*it).first;
		y = (*it).second;

		int newX,newY,newD;
		GetColorBlock(pData,x,y,newX,newY,newD);
		mapBlocks.insert(make_pair(newD,make_pair(newX,newY)));
	}
	d = (*mapBlocks.rbegin()).first;
	x = (*mapBlocks.rbegin()).second.first;
	y = (*mapBlocks.rbegin()).second.second;
	
	return false;
}

COLORREF BlockFinder::GetPointColor( BYTE * pBuffer,int iRow, int iCol )
{
	if(iRow<0 || iRow>=m_iHeight)
	{
		return 0;
	}
	if(iCol<0 || iCol>=m_iWidth)
	{
		return 0;
	}
	//颜色是按B-G-R顺序保持的
	return RGB(
		*(pBuffer + 3*(iRow*m_iWidth + iCol) + 2)
		,*(pBuffer + 3*(iRow*m_iWidth + iCol) + 1)
		,*(pBuffer + 3*(iRow*m_iWidth + iCol) ));
}

COLORREF BlockFinder::GetAvgPointColor( BYTE * pBuffer,int iRow, int iCol, int d )
{
	COLORREF color = GetPointColor(pBuffer,iRow,iCol);
	unsigned int lRed = GetRValue(color);
	unsigned int lGreen = GetGValue(color);
	unsigned int lBlue = GetBValue(color);
	for (int i=1;i<d;i++)	//注意，0点已经在前面计算
	{
		//////////////////////////////////////////////////////////////////////////
		//red
		//右边三点
		COLORREF colorTmp = GetPointColor(pBuffer, iRow + i,iCol + i);
		lRed += GetRValue( colorTmp );
		lGreen += GetGValue(colorTmp);
		lBlue += GetBValue(colorTmp);
		colorTmp = GetPointColor(pBuffer, iRow - i,iCol + i);
		lRed += GetRValue( colorTmp );
		lGreen += GetGValue(colorTmp);
		lBlue += GetBValue(colorTmp);
		colorTmp = GetPointColor(pBuffer, iRow,iCol + i );
		lRed += GetRValue( colorTmp );
		lGreen += GetGValue(colorTmp);
		lBlue += GetBValue(colorTmp);

		//左边三点
		colorTmp = GetPointColor(pBuffer, iRow + i,iCol - i);
		lRed += GetRValue( colorTmp );
		lGreen += GetGValue(colorTmp);
		lBlue += GetBValue(colorTmp);
		colorTmp = GetPointColor(pBuffer, iRow - i,iCol - i);
		lRed += GetRValue( colorTmp );
		lGreen += GetGValue(colorTmp);
		lBlue += GetBValue(colorTmp);
		colorTmp = GetPointColor(pBuffer, iRow,iCol - i);
		lRed += GetRValue( colorTmp );
		lGreen += GetGValue(colorTmp);
		lBlue += GetBValue(colorTmp);

		//中间两点（最中间那一点前面已经计算过了）
		colorTmp = GetPointColor(pBuffer, iRow + i,iCol);
		lRed += GetRValue( colorTmp );
		lGreen += GetGValue(colorTmp);
		lBlue += GetBValue(colorTmp);
		colorTmp = GetPointColor(pBuffer, iRow - i,iCol);
		lRed += GetRValue( colorTmp );
		lGreen += GetGValue(colorTmp);
		lBlue += GetBValue(colorTmp);
	}

	int iPointCount = 8*(d-1) + 1;
	return RGB(lRed/iPointCount,lGreen/iPointCount,lBlue/iPointCount);
}

void BlockFinder::SetPointColor( BYTE * pBuffer,int iRow, int iCol,COLORREF color )
{
	if(iRow<0 || iRow>=m_iHeight)
	{
		//ASSERT(FALSE);
		return ;
	}
	if(iCol<0 || iCol>=m_iWidth)
	{
		//ASSERT(FALSE);
		return ;
	}
	*(pBuffer + 3*(iRow*m_iWidth + iCol) ) = GetBValue(color);	//RED
	*(pBuffer + 3*(iRow*m_iWidth + iCol) + 1) = GetGValue(color);	//GREEN
	*(pBuffer + 3*(iRow*m_iWidth + iCol) + 2) = GetRValue(color);//255;
}

void BlockFinder::DrawRect( BYTE * pBuffer, int iCol,int iRow,int d,COLORREF color )
{
	for(int i=0;i<d;i++)
	{
		//右边三点
		SetPointColor(pBuffer, iRow + i,iCol + i,color);
		SetPointColor(pBuffer, iRow - i,iCol + i,color);
		SetPointColor(pBuffer, iRow,iCol + i ,color);

		//左边三点
		SetPointColor(pBuffer, iRow + i,iCol - i,color);
		SetPointColor(pBuffer, iRow - i,iCol - i,color);
		SetPointColor(pBuffer, iRow,iCol - i,color);

		//中间两点（最中间那一点前面已经计算过了）
		SetPointColor(pBuffer, iRow + i,iCol,color);
		SetPointColor(pBuffer, iRow - i,iCol,color);
	}
}

int BlockFinder::GetColorLen( BYTE * pBuffer, int iCol,int iRow, int directX,int directY )
{
	if (directX==0 && directY==0)
	{
		ASSERT(FALSE);
		return 0;
	}
	const int DISTURB = 3;	//平滑系数，避免噪声的影响
	const int MAX_COLOR_DISTANCE = 10*10*3;	//颜色突变多少就认为到了色块边缘

	COLORREF colorAvg = GetAvgPointColor(pBuffer,iRow,iCol,DISTURB);

	//各色素的总值（为了计算平均值）
	unsigned int lRed = GetRValue(colorAvg);
	unsigned int lGreen = GetGValue(colorAvg);
	unsigned int lBlue = GetBValue(colorAvg);

	for (int d=1;d<m_iWidth;d++)
	{
		//计算此前多个点的平均值
		colorAvg = RGB(lRed/d,lGreen/d,lBlue/d);	

		//看新的一点是否存在突变
		COLORREF colorCurr = GetAvgPointColor(pBuffer,iRow+d*directY,iCol+d*directX,DISTURB);
		int iDistance = GetColorDistance(colorAvg,colorCurr);
		if (iDistance>MAX_COLOR_DISTANCE)
		{
			//有突变，game over
			return d - 1;
		}

		//把这一点的颜色加入到颜色总值中
		lRed += GetRValue(colorCurr);
		lGreen += GetGValue(colorCurr);
		lBlue += GetBValue(colorCurr);
	}
	return 0;
}

void BlockFinder::GetColorBorder( BYTE * pBuffer, int iCol,int iRow, int directX,int directY,int &x,int &y )
{
	int d = GetColorLen(pBuffer,iCol,iRow,directX,directY);
	x = iCol + d*directX;
	y = iRow + d*directY;
}
//static
int BlockFinder::GetColorDistance( COLORREF c1,COLORREF c2 )
{
	return (GetRValue(c1) - GetRValue(c2))*(GetRValue(c1) - GetRValue(c2)) + 
		(GetGValue(c1) - GetGValue(c2))*(GetGValue(c1) - GetGValue(c2)) + 
		(GetBValue(c1) - GetBValue(c2))*(GetBValue(c1) - GetBValue(c2));
}

void BlockFinder::GetColorBlock( BYTE * pBuffer, int iCol,int iRow,int &x,int &y,int &d )
{
	int AvgX = 0;
	int AvgY = 0;
	int tmpX,tmpY;
	
	//向八个方向取样，获取边界位置，然后取平均值，得到色块中心位置
	for (int dirctX=-1;dirctX<=1;dirctX++)
	{
		for (int dirctY=-1;dirctY<=1;dirctY++)
		{
			if (dirctX==0 && dirctY==0)
			{
				continue;
			}
			GetColorBorder(pBuffer,iCol,iRow,dirctX,dirctY,tmpX,tmpY);
			AvgX += tmpX;
			AvgY += tmpY;
		}
	}
	x = AvgX/8;
	y = AvgY/8;

	//这次从中心点开始，再次向八个方向取样，获取平均长度，作为色块的中心长度
	int r = 0;
	for (int dirctX=-1;dirctX<=1;dirctX++)
	{
		for (int dirctY=-1;dirctY<=1;dirctY++)
		{
			if (dirctX==0 && dirctY==0)
			{
				continue;
			}
			r += GetColorLen(pBuffer,iCol,iRow,dirctX,dirctY);
		}
	}
	d = r/8;
}
