#pragma once

#include "minigamebase.h"

class CMiniGameTyping :	public CMiniGameBase
{
public:
	CMiniGameTyping(void);
	CMiniGameTyping( CMiniGameBase* pMiniGame );
	virtual ~CMiniGameTyping(void);

	virtual BOOL Excute( CUser* pUser, __MINIGAME_PACKET* pMiniGamePacket );

private:
	void	MakeQuestion();	// ������ �����.

	std::vector<std::string>	m_vecTyping;	// ������� ������ ���� ���
};

