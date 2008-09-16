#pragma once
#include <vector>

template<class _Ty,
class _Ax = std::allocator<_Ty> >
class ReverseVector
{
public:
	ReverseVector(size_t iMaxSize)
	{
		m_iMaxSize = iMaxSize;
	}

	void push_back(const _Ty &val)
	{
		if (m_vecData.size()>m_iMaxSize*2)
		{
			m_vecData.erase(m_vecData.begin(), m_vecData.begin() + m_iMaxSize);
		}
		m_vecData.push_back(val);
	}
	//取倒数第iBackPos个数据的指针(zero base)
	const _Ty *GetDataPointer(size_t iBackPos) const
	{
		size_t sizeNow = m_vecData.size();
		if (sizeNow<iBackPos)
		{
			return NULL;
		}
		return &m_vecData[sizeNow-iBackPos-1];
	}

private:
	std::vector<_Ty,_Ax> m_vecData;
	size_t m_iMaxSize;
};
