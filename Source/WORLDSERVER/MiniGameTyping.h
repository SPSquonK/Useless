#pragma once

#include "minigamebase.h"

class CMiniGameTyping :	public CMiniGameBase
{
public:
	CMiniGameTyping(void);
	CMiniGameTyping( CMiniGameBase* pMiniGame );
	virtual ~CMiniGameTyping(void);

	BOOL Excute(CUser * pUser, __MINIGAME_PACKET * pMiniGamePacket) override;

private:
	void	MakeQuestion();	// ������ �����.

	std::vector<std::string>	m_vecTyping;	// ������� ������ ���� ���
};

