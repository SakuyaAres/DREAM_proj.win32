#pragma once
#ifndef _SUPPORT_TOOL_H_  
#define _SUPPORT_TOOL_H_  
//////////////////////////////////////////////////////////////////////////  
#include "cocos2d.h"  


//////////////////////////////////////////////////////////////////////////  

#define A2U(varString) wSupportTool::AStrToUTF8(varString).c_str() //用于显示中文(窄字符)  
#define W2U(varString) wSupportTool::WStrToUTF8(varString).c_str() //用于显示中文(宽字符)  

//////////////////////////////////////////////////////////////////////////  
using namespace std;

class wSupportTool
{
	public:
	//string to UTF8  
	static std::string AStrToUTF8(const string& src);

	//wstring to UTF8  
	static std::string WStrToUTF8(const wstring& src);

};

#endif