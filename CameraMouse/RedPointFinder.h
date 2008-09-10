#pragma once

#include <map>
#include <vector>
using namespace std;

class RedPointFinder
{
public:
	//ptPixels是位图数据，从左下角开始，到右上角结束。
	RedPointFinder(unsigned int * ptPixels,unsigned int iWidth,unsigned int iHeight);
	~RedPointFinder(void);

	//计算找到能量最大的地方（可以并列）
	// atLeastPower:代表半径2能量阈值
	//返回值：
	// 大于等于0表示所找到的点的位置
	// -1表示这样的点有两个以上，无法区分
	// -2表示没有符合要求的点（没任何一个点达到阈值要求）
	int GetPowerMost(unsigned int atLeastPower);
	unsigned int CalcPointPower(unsigned int x,unsigned int y,unsigned int r);

	inline unsigned int GetPos(unsigned int x,unsigned int y)
	{
		return y*m_iHeight + x;
	}
	inline unsigned int GetX(unsigned int pos)
	{
		return pos%m_iWidth;
	}
	inline unsigned int GetY(unsigned int pos)
	{
		return pos/m_iWidth;
	}
	inline unsigned int GetColor(unsigned int x,unsigned int y)
	{
		return m_ptPixels[GetPos(x,y)];
	}
private:
	unsigned int *m_ptPixels;
	unsigned int m_iWidth;
	unsigned int m_iHeight;
private:

	//以r为半径，判断vecPoint中的哪个点最强
	int CheckPowerMost(unsigned int r,const vector<unsigned int> &vecPoints,vector<unsigned int> &vecPointOut);
};
