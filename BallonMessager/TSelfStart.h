#pragma once
#include <string>

class TSelfStart
{
public:
	TSelfStart(const std::string &strName,const std::string &strPath);
	~TSelfStart(void);

	bool SetSelfStart();
	bool IsSelfStart();
	bool RemoveSelfStart();
private:
	std::string m_strPath;
	std::string m_strName;
};
