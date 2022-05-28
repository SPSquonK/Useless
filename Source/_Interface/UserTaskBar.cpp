// UserTaskBar.cpp: implementation of the CUserTaskBar class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "UserTaskBar.h"
#ifdef __WORLDSERVER
#include "User.h"
#endif // __WORLDSERVER

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUserTaskBar::CUserTaskBar()
{
	InitTaskBar();
}

CUserTaskBar::~CUserTaskBar()
{

}

void CUserTaskBar::InitTaskBar()
{
	memset( m_aSlotApplet, 0, sizeof( m_aSlotApplet ) );
	memset( m_aSlotItem  , 0, sizeof( m_aSlotItem ) );
	memset( m_aSlotQueue , 0, sizeof( m_aSlotQueue ) );
	m_nActionPoint = 0;
	m_nUsedSkillQueue = -1;
}

void CUserTaskBar::RemoveAllSkills() {
	constexpr auto IsASkill = [](const SHORTCUT & shortcut) {
		return shortcut.m_dwShortcut == ShortcutType::Skill && shortcut.m_dwType == 2;
	};

	constexpr auto RemoveIfSkill = [](SHORTCUT & shortcut) {
		if (IsASkill(shortcut)) shortcut.Empty();
	};

	for (int i = 0; i < MAX_SLOT_APPLET; i++) {
		RemoveIfSkill(m_aSlotApplet[i]);
	}

	for (int i = 0; i < MAX_SLOT_ITEM_COUNT; i++) {
		for (int j = 0; j < MAX_SLOT_ITEM; j++) {
			RemoveIfSkill(m_aSlotItem[i][j]);
		}
	}

	// By definition, slot queue is full of skills
	memset(&m_aSlotQueue, 0, sizeof(m_aSlotQueue));
}

void CUserTaskBar::Serialize( CAr &ar )
{
	if( ar.IsStoring() )
	{
		int nCount	= 0;
		u_long uOffset	= ar.GetOffset();
		ar << nCount;
		for( int i = 0; i < MAX_SLOT_APPLET; i++ )
		{
			if( m_aSlotApplet[i].m_dwShortcut != ShortcutType::None)		// ���� ����ִ��� �˻�
			{
				ar << i << m_aSlotApplet[i];
				nCount++;
			}
		}
		int nBufSize;
		LPBYTE lpBuf	= ar.GetBuffer( &nBufSize );
		*(UNALIGNED int*)( lpBuf + uOffset )	= nCount;

		nCount	= 0;
		uOffset	= ar.GetOffset();
		ar << nCount;
		for( int i = 0; i < MAX_SLOT_ITEM_COUNT; i++ )
		{
			for( int j = 0; j < MAX_SLOT_ITEM; j++ )
			{
				if( m_aSlotItem[i][j].m_dwShortcut != ShortcutType::None)		// ���� ����ִ��� �˻�
				{
					ar << i << j;
					ar << m_aSlotItem[i][j];
					nCount++;
					
				}
			}
		}
		lpBuf	= ar.GetBuffer( &nBufSize );
		*(UNALIGNED int*)( lpBuf + uOffset )	= nCount;

		nCount	= 0;
		uOffset	= ar.GetOffset();
		ar << nCount;
		for( int i = 0; i < MAX_SLOT_QUEUE; i++ )
		{
			if( m_aSlotQueue[i].m_dwShortcut != ShortcutType::None)		// ���� ����ִ��� �˻�
			{
				ar << i;
				ar << m_aSlotQueue[i];
				nCount++;
			}
			
							// m_aSlotQueue[i]�� ��ȿ�� �����̶��
//				ar << i << m_aSlotQueue[i]�� �ʿ��� ���� ����
		}
		ar << m_nActionPoint;
		lpBuf	= ar.GetBuffer( &nBufSize );
		*(UNALIGNED int*)( lpBuf + uOffset )	= nCount;
	}
	else
	{
		memset( m_aSlotApplet, 0, sizeof(m_aSlotApplet) );
		memset( m_aSlotItem, 0, sizeof(m_aSlotItem) );
		memset( m_aSlotQueue, 0, sizeof(m_aSlotQueue) );

		int nCount, nIndex;
		ar >> nCount;	// applet count
		for( int i = 0; i < nCount; i++ )
		{
			ar >> nIndex;
			ar >> m_aSlotApplet[nIndex];
//			ar >>  m_aSlotApplet[nIndex]�� �Ʊ� ���� �ʿ��� ������ �ִ´�.
		}
		ar >> nCount;	// slot item count
		int nIndex2;
		for( int i = 0; i < nCount; i++ )
		{
			ar >> nIndex >> nIndex2;	// index
			ar >> m_aSlotItem[nIndex][nIndex2];
		}

		ar >> nCount;
		for( int i = 0; i < nCount; i++ )
		{
			ar >> nIndex;
			ar >> m_aSlotQueue[nIndex];
//			ar >> m_aSlotQueue[nIndex]�� ������ �ִ´�.
		}
		ar >> m_nActionPoint;
	}
}

#ifdef __WORLDSERVER
// ��ų�� ����� ������ ��������
void CUserTaskBar::OnEndSkillQueue( CUser *pUser )
{
	m_nUsedSkillQueue = -1;
	pUser->AddHdr( GETID( pUser ), SNAPSHOTTYPE_ENDSKILLQUEUE );
	((CMover*)pUser)->ClearCmd();
	TRACE( "\nOnEndSkillQueue\n" );
}

int	CUserTaskBar::SetNextSkill( CUser *pUser )
{
	m_nUsedSkillQueue ++;		// ť�ε��� ��������...

	int nAP = m_nActionPoint;

	if( !( pUser->IsSMMode( SM_ACTPOINT ) ) )
	{
		switch( m_nUsedSkillQueue )
		{
		case 1:	nAP -= 6;	break;		// �׼� ����Ʈ �Ҹ�. ���������� ����Ҽ��ִ��� �̸� �˾ƺ��� ����.
		case 2:	nAP -= 8;	break;
		case 3:	nAP -= 11;	break;
		case 4:	nAP -= 30;	break;
		}
	}

	LPSHORTCUT pShortcut = &m_aSlotQueue[ m_nUsedSkillQueue ];
	BOOL bResult = nAP < 0;
	if( ( m_nUsedSkillQueue >= MAX_SLOT_QUEUE) || pShortcut->IsEmpty() || (nAP < 0) )	// ť������ ������ ���ų� || ť�� ����ų� || AP�� ���ų�.
	{
		OnEndSkillQueue( pUser );
		return 0;		// ť���� ��.
	} else
	{
		if( nAP < 0 )	nAP = 0;
		m_nActionPoint = nAP;
		pUser->AddSetActionPoint( nAP );		// �׼�����Ʈ Ŭ�� ����.
		OBJID idTarget = pUser->m_idSetTarget;
		TRACE( "������ų��� �õ�%d, ", pShortcut->m_dwId );
		if( pUser->CMD_SetUseSkill( idTarget, pShortcut->m_dwId, SUT_QUEUEING ) == 0 )		// ������ ����� ����. �̵� + ��ų����� ������ ���.
		{
			TRACE( "������ų��� ���� %d, ", pShortcut->m_dwId );
			SetNextSkill( pUser );	// ��ų��뿡 �����ߴٸ� ���� ��ų ����ϵ��� �Ѿ.
		}
		return 1;
	}

	return 1;
}
#endif // __WORLDSERVER