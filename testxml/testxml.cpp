// testxml.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <tinyxml.h>
#include <string>

using namespace std;
int _tmain(int argc, _TCHAR* argv[])
{
	const char * xmlStr = "<Result Action=\"add\"></Result>";

	TiXmlDocument doc("testxml.xml");
	doc.Parse( xmlStr );

	TiXmlNode* node = 0;
	node = doc.RootElement();
	if (node)
	{
		TiXmlElement* p_elmt = node->ToElement();
		string s = p_elmt->GetText();

		p_elmt->SetAttribute("newattribe", "value");

		cout << "text: " << s << endl;
		cout << "action: " << p_elmt->Attribute("Action") << endl;
		cout << "newattribute: " << p_elmt->Attribute("newattribe") << endl;

		const char * str = p_elmt->Attribute("aa");
		if (!str)
			cout << "error !" << endl;


		//node = node->FirstChildElement();
		//while (node)
		//{
		//	TiXmlElement *elmt = node->ToElement();
  //          
		//	s = elmt->Attribute("priority");
		//	cout << s << endl;

		//	s = elmt->GetText();

		//	cout << s << endl;

		//	/*elmt->SetAttribute("newattribute", "the value of newattribute");

		//	s = elmt->Attribute("newattribute");
		//	cout << s << endl;*/

		//	node = node->NextSibling();
		//}
	}

	doc.SaveFile();

	system("pause");

	return 0;
}

