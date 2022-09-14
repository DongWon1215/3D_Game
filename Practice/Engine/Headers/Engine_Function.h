#pragma once

namespace Engine
{
	template <typename T>
	void Safe_Delete(T& pPointer)
	{
		if (nullptr != pPointer)
		{
			delete pPointer;
			pPointer = nullptr;
		}	
	}

	template <typename T>
	void Safe_Delete_Array(T& pPointer)
	{
		if (nullptr != pPointer)
		{
			delete [] pPointer;
			pPointer = nullptr;
		}
	}


	// 레퍼런스 카운트로 관리되고 있는 객체를 삭제하고자 할때. 
	template <typename T>
	unsigned long Safe_Release(T& pInstance)
	{
		unsigned long	dwRefCnt = 0;

		if (nullptr != pInstance)
		{
			dwRefCnt = pInstance->Release();
			if (0 == dwRefCnt)
				pInstance = nullptr;
		}

		return dwRefCnt;
	}

	template <typename T>
	unsigned long Safe_AddRef(T& pInstance)
	{
		unsigned long	dwRefCnt = 0;

		if (nullptr != pInstance)		
			dwRefCnt = pInstance->AddRef();	

		return dwRefCnt;
	}










}
