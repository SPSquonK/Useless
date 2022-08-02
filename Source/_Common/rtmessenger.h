#pragma once

#include "Ar.h"

enum class FriendStatus : DWORD {
	ONLINE = 0, // �⺻ : ������ 
	OFFLINE = 1, // �ڵ� : �α׿���
	ABSENT = 3, // ���� : �ڸ����
	HARDPLAY = 4, // ���� : ������ 
	EAT = 5, // ���� : �Ļ��� 
	REST = 6, // ���� : �޽���
	MOVE = 7 // ���� : �̵��� 
};

bool IsValid(FriendStatus friendStatus);

struct Friend {
	static constexpr bool Archivable = true;

	BOOL	bBlock = FALSE;
	FriendStatus dwState = FriendStatus::ONLINE;
	Friend() = default;
};

class CRTMessenger final : public	std::map<u_long, Friend>
{
public:
	static constexpr size_t MaxFriend = 200;

	void SetFriend(u_long idFriend, const Friend & pFriend = Friend());
	void RemoveFriend(u_long idFriend) { erase(idFriend); }
	Friend*	GetFriend( u_long idFriend );
	const Friend * GetFriend(u_long idFriend) const;

	void SetBlock(u_long idFriend, bool bBlock);
	[[nodiscard]] bool IsBlock(u_long idFriend) const;

	[[nodiscard]] FriendStatus GetState() const  { return m_dwState; }
	void	SetState(FriendStatus dwState) { m_dwState = dwState; }

	int	Serialize(CAr & ar);

private:
	FriendStatus m_dwState = FriendStatus::ONLINE;
};

