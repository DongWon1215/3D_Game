#pragma once

// 함수객체 : 마치 함수를 호출하는것과 같은 인터페이스로 동작하는 객체.

class CTag_Finder
{
public:
	CTag_Finder(const wchar_t* pString)
		: m_pString(pString)
	{

	}

public:
	template<typename T>
	bool operator () (T& Pair)
	{
		return !lstrcmp(m_pString, Pair.first);			
	}
private:
	const wchar_t* m_pString = nullptr;	
};









