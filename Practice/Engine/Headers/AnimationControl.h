#pragma once
#include "Base.h"

BEGIN(Engine)
class CAnimationControl final : public CBase
{
private:
	explicit CAnimationControl(LPD3DXANIMATIONCONTROLLER pAniCtrl);
	explicit CAnimationControl(const CAnimationControl& rhs);
	virtual ~CAnimationControl() = default;
public:
	HRESULT Ready_AnimationCtrl();
	HRESULT SetUp_AnimationSet(_uint iIndex);
	HRESULT Play_AnimationSet(_double TimeDelta);
	_bool is_Finished(_double pTime) const;

public:
	_uint Get_CurrentTrack() { return m_iCurrentTrack; }

private:
	LPD3DXANIMATIONCONTROLLER	m_pAniCtrl = nullptr;
	_uint	m_iCurrentTrack = 0; // 현재 재생되는 트랙.
	_uint	m_iNewTrack = 0; // 바꿀애니메이션 셋을 가지고 있는 트랙.
	_double	m_TimeAcc = 0.0;
	_uint	m_iOldAnimationIndex = 0;
	_double	m_Period = 0.0;
public:
	static CAnimationControl* Create(LPD3DXANIMATIONCONTROLLER pAniCtrl);
	CAnimationControl* Clone_AnimationCtrl();
	virtual void Free();
};
END
