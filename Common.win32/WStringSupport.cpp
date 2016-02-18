#include "WStringSupport.h"

//string to UTF8  
std::string wSupportTool::AStrToUTF8(const string& src)
{
	std::string curLocale = setlocale(LC_ALL, NULL);
	setlocale(LC_ALL, "");
	int len = src.length() + 1;
	wchar_t *str = new wchar_t[len];
	int total = mbstowcs(str, src.c_str(), len);
	str[total] = 0;
	std::wstring wstr = str;
	delete[] str;
	setlocale(LC_ALL, curLocale.c_str());
	return WStrToUTF8(wstr);
}

//wstring to UTF8  
std::string wSupportTool::WStrToUTF8(const wstring& src)
{

	std::string dest;
	for (size_t i = 0; i < src.size(); i++) {
		wchar_t w = src[i];
		if (w <= 0x7f)
			dest.push_back((char)w);
		else if (w <= 0x7ff) {
			dest.push_back(0xc0 | ((w >> 6) & 0x1f));
			dest.push_back(0x80 | (w & 0x3f));
		}
		else if (w <= 0xffff) {
			dest.push_back(0xe0 | ((w >> 12) & 0x0f));
			dest.push_back(0x80 | ((w >> 6) & 0x3f));
			dest.push_back(0x80 | (w & 0x3f));
		}
		/*else if (sizeof(wchar_t) > 2 && w <= 0x10ffff){
		dest.push_back(0xf0 | ((w >> 18)& 0x07)); // wchar_t 4-bytes situation
		dest.push_back(0x80| ((w >> 12) & 0x3f));
		dest.push_back(0x80| ((w >> 6) & 0x3f));
		dest.push_back(0x80| (w & 0x3f));
		}*/
		else
			dest.push_back('?');
	}
	return dest;
}
