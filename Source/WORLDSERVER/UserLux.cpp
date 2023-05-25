#include "stdafx.h"
#include "defineText.h"
#include "User.h"
#include "DPDatabaseClient.h"
#include "DPSrvr.h"
#include "WorldMng.h"

#ifndef __MEM_TRACE
#ifdef _DEBUG
#define new new( __FILE__, __LINE__ )
#endif
#endif	// __MEM_TRACE

#include "UserMacro.h"

#ifdef __XUZHU
int g_nCC = 0;
#endif

// tmMaxEscape : Ż���� ��Ÿ��
void CUser::AddEscape(DWORD tmMaxEscape) {
	SendSnapshotNoTarget<SNAPSHOTTYPE_DO_ESCAPE, DWORD>(tmMaxEscape);
}

// �׼�����Ʈ ����.
void CUser::AddSetActionPoint(int nAP) {
	SendSnapshotNoTarget<SNAPSHOTTYPE_SETACTIONPOINT, int>(nAP);
}

void CUser::AddSetTarget(OBJID idTarget) {
	SendSnapshotNoTarget<SNAPSHOTTYPE_SETTARGET, OBJID>(idTarget);
}

// ----------------------------------------------------------------------------------
//
//   CUserMng Lux
//
// ----------------------------------------------------------------------------------


void	CUserMng::AddCreateSkillEffect(CMover * pAttacker, OBJID idTarget, DWORD dwSkill, DWORD dwLevel) {
	BroadcastAround<SNAPSHOTTYPE_ACTIVESKILL, OBJID, DWORD, DWORD>(pAttacker,
		idTarget, dwSkill, dwLevel
	);
}

// pMover�� ���� �Ȱ��� ���� ����Ŭ�� �˸�.
void	CUserMng::AddSetStun( CMover *pMover, int nMaxTime ) {
	BroadcastAround<SNAPSHOTTYPE_SETSTUN, int>(pMover, nMaxTime);
}

void	CUserMng::AddDoApplySkill(CCtrl * pCtrl, OBJID idTarget, DWORD dwSkill, DWORD dwLevel)
{
	BroadcastAround<SNAPSHOTTYPE_DOAPPLYUSESKILL, OBJID, DWORD, DWORD>(pCtrl,
		idTarget, dwSkill, dwLevel
	);
}

// SendActMsg�� ���� �����鿡�� ���� .
void	CUserMng::AddSendActMsg(CMover * pMover, OBJMSG dwMsg, int nParam1, int nParam2, int nParam3)
{
	BroadcastAround<SNAPSHOTTYPE_SENDACTMSG, int, BOOL, int, int, int>(pMover,
		dwMsg, pMover->IsFly(), nParam1, nParam2, nParam3
	);
}

// Mover�� ����������� �и��� �ϴ�.
// pMover : �и��� ����
// vPos : �и��� ���������� vPos - ��Ȯ�� ����ȭ������ ��ǥ�� �������� �ʿ��ϴ�.
// fAngle : �и��� ���������� Angle - 
// fPushAngle : �̴� ����.
// fPower : �̴� ��.
void	CUserMng::AddPushPower( CMover *pMover, D3DXVECTOR3 vPos, FLOAT fAngle, FLOAT fPushAngle, FLOAT fPower )
{
	if( (pMover->m_pActMover->GetState() & OBJSTA_DMG_FLY_ALL) || pMover->m_pActMover->GetState() & OBJSTA_STUN ) // ������ �ö������̸� ����
		return;

	BroadcastAround<SNAPSHOTTYPE_PUSHPOWER, D3DXVECTOR3, FLOAT, FLOAT, FLOAT>(pMover,
		vPos, fAngle, fPushAngle, fPower
	);
}

void	CUserMng::AddRemoveSkillInfluence(CMover * pMover, WORD wType, WORD wID) {
	BroadcastAround<SNAPSHOTTYPE_REMOVESKILLINFULENCE, WORD, WORD>(pMover, wType, wID);
}

void CUserMng::AddSetPosAngle(CCtrl * pCtrl, const D3DXVECTOR3 & vPos, FLOAT fAngle) {
	BroadcastAround<SNAPSHOTTYPE_SETPOSANGLE, D3DXVECTOR3, FLOAT>(pCtrl, vPos, fAngle);
}

