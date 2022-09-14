#include "..\Headers\AnimationControl.h"




CAnimationControl::CAnimationControl(LPD3DXANIMATIONCONTROLLER pAniCtrl)
	: m_pAniCtrl(pAniCtrl)
{
	Safe_AddRef(m_pAniCtrl);
}

CAnimationControl::CAnimationControl(const CAnimationControl & rhs)
	: m_pAniCtrl(nullptr)
{
	rhs.m_pAniCtrl->CloneAnimationController(rhs.m_pAniCtrl->GetMaxNumAnimationOutputs(), rhs.m_pAniCtrl->GetMaxNumAnimationSets(), rhs.m_pAniCtrl->GetMaxNumTracks(), rhs.m_pAniCtrl->GetMaxNumEvents(), &m_pAniCtrl);
}

HRESULT CAnimationControl::Ready_AnimationCtrl()
{
	return S_OK;
}

HRESULT CAnimationControl::SetUp_AnimationSet(_uint iIndex)
{
	if (nullptr == m_pAniCtrl)
		return E_FAIL;

	if (m_iOldAnimationIndex == iIndex)
		return NOERROR;

	m_iNewTrack = m_iCurrentTrack == 0 ? 1 : 0;

	LPD3DXANIMATIONSET		pAS = nullptr;

	m_pAniCtrl->GetAnimationSet(iIndex, &pAS);

	m_pAniCtrl->SetTrackAnimationSet(m_iNewTrack, pAS);

	m_Period = pAS->GetPeriod() - 0.09;

	Safe_Release(pAS);

	m_pAniCtrl->UnkeyAllTrackEvents(m_iCurrentTrack);
	m_pAniCtrl->UnkeyAllTrackEvents(m_iNewTrack);

	m_pAniCtrl->KeyTrackEnable(m_iCurrentTrack, FALSE, m_TimeAcc + 0.2);
	m_pAniCtrl->KeyTrackSpeed(m_iCurrentTrack, 1.f, m_TimeAcc, 0.2, D3DXTRANSITION_LINEAR);
	m_pAniCtrl->KeyTrackWeight(m_iCurrentTrack, 0.1f, m_TimeAcc, 0.2, D3DXTRANSITION_LINEAR);

	m_pAniCtrl->SetTrackEnable(m_iNewTrack, TRUE);
	m_pAniCtrl->KeyTrackSpeed(m_iNewTrack, 1.f, m_TimeAcc, 0.2, D3DXTRANSITION_LINEAR);
	m_pAniCtrl->KeyTrackWeight(m_iNewTrack, 0.9f, m_TimeAcc, 0.2, D3DXTRANSITION_LINEAR);

	m_iCurrentTrack = m_iNewTrack;
	m_TimeAcc = 0.0;
	m_pAniCtrl->ResetTime();
	m_pAniCtrl->SetTrackPosition(m_iCurrentTrack, 0.0);

	m_iOldAnimationIndex = iIndex;

	return NOERROR;
}

HRESULT CAnimationControl::Play_AnimationSet(_double TimeDelta)
{
	if (nullptr == m_pAniCtrl)
		return E_FAIL;

	m_pAniCtrl->AdvanceTime(TimeDelta, nullptr);

	m_TimeAcc += TimeDelta;

	return S_OK;
}

_bool CAnimationControl::is_Finished(_double pTime) const
{
	if (nullptr == m_pAniCtrl)
		return false;

	D3DXTRACK_DESC		TrackDesc;

	m_pAniCtrl->GetTrackDesc(m_iCurrentTrack, &TrackDesc);

	if (m_Period + pTime <= TrackDesc.Position)
		return true;

	return false;
}

CAnimationControl * CAnimationControl::Create(LPD3DXANIMATIONCONTROLLER pAniCtrl)
{
	CAnimationControl* pInstance = new CAnimationControl(pAniCtrl);

	if (FAILED(pInstance->Ready_AnimationCtrl()))
	{
		MSG_BOX("Failed while Creating CAnimationControl");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CAnimationControl * CAnimationControl::Clone_AnimationCtrl()
{
	return new CAnimationControl(*this);
}

void CAnimationControl::Free()
{
	Safe_Release(m_pAniCtrl);
}
