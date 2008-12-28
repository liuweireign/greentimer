#pragma once
#include <string>

class DBSetting
{
public:
	DBSetting(const std::string &strDBName,const std::string &strTableName);
	virtual ~DBSetting(void);

	bool SetValue(long lKey,long lValue);
	bool GetValue(long lKey,long &lValue);
	bool SetValue(long lKey,const std::string &lValue);
	bool GetValue(long lKey,std::string &strValue);
private:
	std::string m_strDB;
	std::string m_strTableName;
};
