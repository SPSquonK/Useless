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

struct Friend {
	static constexpr bool Archivable = true;

	BOOL	bBlock = FALSE;
	DWORD	dwState = 0;
	Friend() = default;
};

class CRTMessenger final : public	std::map<u_long, Friend>
{
public:
	static constexpr size_t MaxFriend = 200;

	void SetFriend(u_long idFriend, const Friend & pFriend = Friend());
	void RemoveFriend(u_long idFriend) { erase(idFriend); }
	Friend*	GetFriend( u_long idFriend );

	void SetBlock(u_long idFriend, bool bBlock);
	[[nodiscard]] bool IsBlock(u_long idFriend) const;

	[[nodiscard]] DWORD GetState() const  { return m_dwState; }
	void	SetState(DWORD dwState) { m_dwState = dwState; }

	int	Serialize(CAr & ar);

private:
	DWORD	m_dwState = 0;
};

