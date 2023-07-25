#pragma once

#include "minigamebase.h"

class CMiniGameArithmetic :	public CMiniGameBase
{
public:
	CMiniGameArithmetic(void);
	CMiniGameArithmetic( CMiniGameBase* pMiniGame );
	virtual ~CMiniGameArithmetic(void);

	BOOL Excute(CUser * pUser, __MINIGAME_PACKET * pMiniGamePacket) override;

private:
	void	InitPrimeNumber();
	int		GetDivisor( int nDivided );	// �������϶� ����� ������ �������� �ǿ����ڸ� ���Ѵ�.
	std::string	MakeQuestion();
	int		Calculate( int n1, int& n2, int nOper );
	char	GetOperatorCh( int nOper );

	int m_nResult;
	int m_nCorrectCount;
	std::vector<int>	m_vecnPrimeNumber;		// operator�� ������ �� ��� �ǿ����ڸ� ������ �Ҽ� ���
};

