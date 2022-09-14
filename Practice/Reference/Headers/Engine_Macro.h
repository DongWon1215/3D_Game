#pragma once

#define NO_COPY(ClassName)						\
private:										\
	ClassName(const ClassName&);				\
	ClassName& operator=(const ClassName&);

#define DECLARE_SINGLETON(ClassName)			\
		NO_COPY(ClassName)						\
public:											\
	static ClassName* GetInstance();			\
	unsigned long DestroyInstance();			\
private:										\
	static ClassName* m_pInstance;

#define IMPLEMENT_SINGLETON(ClassName)			\
ClassName* ClassName::m_pInstance = nullptr;	\
ClassName* ClassName::GetInstance()				\
{												\
	if(nullptr == m_pInstance)					\
		m_pInstance = new ClassName;			\
	return m_pInstance;							\
}												\
unsigned long ClassName::DestroyInstance()		\
{												\
	unsigned long	dwRefCnt = 0;				\
	if(nullptr != m_pInstance)					\
	{											\
		dwRefCnt = m_pInstance->Release();		\
		if(0 == dwRefCnt)						\
			m_pInstance = nullptr;				\
	}											\
	return dwRefCnt;							\
}


#define MSG_BOX(MESSAGE) MessageBox(0, TEXT(MESSAGE), L"System Error", MB_OK)

#define BEGIN(NAMESPACE) namespace NAMESPACE {
#define END }
#define USING(NAMESPACE) using namespace NAMESPACE;


#ifdef ENGINE_EXPORTS 

#define ENGINE_DLL _declspec(dllexport)

#else

#define ENGINE_DLL _declspec(dllimport)

#endif