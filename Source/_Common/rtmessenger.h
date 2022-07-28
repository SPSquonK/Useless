#pragma once

#include "Ar.h"

#define FRS_ONLINE		0	// �⺻ : ������ 
#define FRS_OFFLINE		1	// �ڵ� : �α׿���
#define FRS_BLOCK	2	// ���� : ���� 
#define FRS_ABSENT	3	// ���� : �ڸ����
#define FRS_HARDPLAY	4	// ���� : ������ 
#define FRS_EAT		5	// ���� : �Ļ��� 
#define FRS_REST	6	// ���� : �޽���
#define FRS_MOVE	7	// ���� : �̵��� 
#define FRS_DIE		8	// �ڵ� : ��� 
#define FRS_DANGER		9	// �ڵ� : ����
#define FRS_OFFLINEBLOCK	10
#define	FRS_AUTOABSENT		11	// �ڵ� : �ڵ� �ڸ����	
#define MAX_FRIENDSTAT		12	// �ƽ���

#define MAX_FRIEND		200 // �ƽ��� ģ�� ��� Ƚ��

typedef struct	_Friend
{
	BOOL	bBlock;
	DWORD	dwState;
	_Friend()
	{
		bBlock	= FALSE;
		dwState		= 0;
	}
}	Friend;

class CRTMessenger	: public	std::map<u_long, Friend>
{
public:
	CRTMessenger();
	virtual	~CRTMessenger();
//
//	void	SetFriend( u_long idFriend, BOOL bBlock, DWORD dwState );
	void	SetFriend( u_long idFriend, Friend * pFriend );
	void	RemoveFriend( u_long idFriend )		{	erase( idFriend );	}
	Friend*	GetFriend( u_long idFriend );

	void	SetBlock( u_long idFriend, BOOL bBlock );
	BOOL	IsBlock( u_long idFriend );

	DWORD	GetState( void )	{	return m_dwState;	}
	void	SetState( DWORD dwState )	{	m_dwState	= dwState;	}

//	void	Serialize( CAr & ar );
	int	Serialize( CAr & ar );

	CRTMessenger &	operator =( CRTMessenger & rRTMessenger );
private:
	DWORD	m_dwState;
};

