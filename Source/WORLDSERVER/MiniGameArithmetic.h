#pragma once

#include "minigamebase.h"
#include <span>

class CMiniGameArithmetic :	public CMiniGameBase
{
public:
	CMiniGameArithmetic();

	BOOL Excute(CUser * pUser, __MINIGAME_PACKET * pMiniGamePacket) override;

private:
	int		GetDivisor( int nDivided, std::span<const int> primes );	// �������϶� ����� ������ �������� �ǿ����ڸ� ���Ѵ�.
	std::string	MakeQuestion();
	int		Calculate( int n1, int& n2, int nOper );
	char	GetOperatorCh( int nOper );

	int m_nResult;
	int m_nCorrectCount;
};

