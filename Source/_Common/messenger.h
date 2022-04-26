#ifndef __MESSENGER_H__
#define	__MESSENGER_H__

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

typedef struct tagFRIEND
{
	BOOL bGroup;	// TRUE�̸� �׷�, �ƴϸ� ���� 
	DWORD dwState;	// ���� 0 �̸� ����, 1�̸� disconnect, 2�̸� �ڸ� ���.....
	DWORD dwUserId;
	BOOL  bSave;
#ifdef __CLIENT
	u_long m_uIdofMulti;
#endif // __CLIENT

	tagFRIEND()
	{
		bGroup = FALSE;
		dwState = FRS_OFFLINE;
		dwUserId = 0;
		bSave = FALSE;
#ifdef __CLIENT
		m_uIdofMulti = 100;
#endif // __CLIENT
	}
} FRIEND,* LPFRIEND;

typedef	std::map< u_long, FRIEND*>	C2FriendPtr;

class CMessenger
{
public:
	C2FriendPtr m_aFriend; // ���� ����� ������( ģ�� ��� )
	C2FriendPtr m_adifferntFriend; // ���� ����� ������
	DWORD		m_dwMyState;

public:
	BOOL		AddFriend( u_long uidPlayer );
	BOOL		AddFriend( u_long uidPlayer, DWORD dwSate, BOOL bSave = FALSE );
	void		SetSave( void );
	BOOL		RemoveFriend( u_long uidPlayer );
	BOOL		RemoveDifferntFriend( u_long uidPlayer );
	LPFRIEND	GetFriend( u_long uidPlayer );
	LPFRIEND	GetDefferntFriend( u_long uidPlayer );
	BOOL		IsFriend( u_long uidPlayer );
	BOOL		IsDefferntFriend( u_long uidPlayer );
	BOOL		AddDefferntFriend( u_long uidPlayer );
	void		Clear( void );
	void		Serialize( CAr & ar );
	DWORD		GetState() { return m_dwMyState; };
	LPFRIEND    GetAt( int nIndex );
	int			GetSize();
	CMessenger&	operator=( const CMessenger & rMessenger );

	CMessenger();
	~CMessenger();
};

#endif	//	_MESSENGER_H