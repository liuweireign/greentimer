#pragma once

class BlockFinder
{
public:
	bool Init(int iWidth,int iHeight);

	//找到黄点
	bool FindPoint(BYTE *pData,int &x,int &y,int &d);

	//////////////////////////////////////////////////////////////////////////
	//以下为辅助函数
	//设置点颜色
	void SetPointColor(BYTE * pBuffer,int iRow, int iCol,COLORREF color);
	//取某一点的颜色
	inline COLORREF GetPointColor(BYTE * pBuffer,int iRow, int iCol);
	//计算在半径d的范围内，颜色的平均值（R,G,B分开计算）
	inline COLORREF GetAvgPointColor(BYTE * pBuffer,int iRow, int iCol, int d);
	//绘制矩形
	void DrawRect(BYTE * pBuffer, int iCol,int iRow,int d,COLORREF color);

	//获取点(iCol,iRow)所在的色块的中心位置、半径
	void GetColorBlock(BYTE * pBuffer, int iCol,int iRow,int &x,int &y,int &d);
	//沿着指定的方向前进，一直到这种颜色的边界，返回其长度
	//direct代表x，y方向步进的长度。一般应当为正负1或0。
	int GetColorLen(BYTE * pBuffer, int iCol,int iRow, int directX,int directY);
	void GetColorBorder(BYTE * pBuffer, int iCol,int iRow, int directX,int directY,int &x,int &y);
	//两种颜色之间的区别程度
	static int GetColorDistance(COLORREF c1,COLORREF c2);

private:
	int m_iWidth;
	int m_iHeight;
};
