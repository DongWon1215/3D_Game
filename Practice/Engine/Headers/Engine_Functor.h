#pragma once

// �Լ���ü : ��ġ �Լ��� ȣ���ϴ°Ͱ� ���� �������̽��� �����ϴ� ��ü.

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









