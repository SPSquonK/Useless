// MiniGame.h: interface for the CMiniGame class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MINIGAME_H__79AC4FCB_D41A_4407_ACF3_0FDF796F2ED1__INCLUDED_)
#define AFX_MINIGAME_H__79AC4FCB_D41A_4407_ACF3_0FDF796F2ED1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef __EVE_MINIGAME

class CUser;
class CMiniGame  
{
public:
	enum { KAWIBAWIBO_WIN, KAWIBAWIBO_LOST, KAWIBAWIBO_DRAW, KAWIBAWIBO_FAILED, KAWIBAWIBO_GETITEM	};
	enum { KAWI, BAWI, BO };
	enum { ALPHABET_NOTENOUGH_MONEY = 998, ALPHABET_FAILED = 999, };
	enum { FIVESYSTEM_BETOK = 996, FIVESYSTEM_FAILED = 997, FIVESYSTEM_OVERMAX = 998, FIVESYSTEM_NOTENOUGH = 999 };

	struct __ITEMGAME
	{
		DWORD	dwItemId = 0;	// ���̵�
		int		nItemCount = 0;	// ����
		DWORD	dwPer = 0;		// �ۼ�Ʈ
	};
	
	struct __ALPHABET
	{
		int		nPublic = 1;	// ���� ���� ��ġ
		CString strWord = _T("");	// ���� �ܾ�
		CString	strQuestion = _T("");	// ����
		std::vector<__ITEMGAME>	vecItem;	// ���� ������
	};
	
	struct __KAWIBAWIBORESULT
	{
		int		nMy;
		int		nYou;
		int		nResult;
	};

	struct __KAWIBAWIBO
	{
		DWORD	dwDraw = 0;		// ��� Ȯ��
		DWORD	dwLost = 0;		// �й� Ȯ��
		DWORD	dwWin = 0;		// �¸� Ȯ��
		DWORD	dwItemId = 0;	// ���̵�
		int		nItemCount = 0;	// ������ ����
	};

	struct __FIVESYSTEM		// (����) ������ ���� ������ Ȯ��
	{
		DWORD	dwMin = 0;		// Min Ȯ��
		DWORD	dwMax = 0;		// Max Ȯ��
	};



public:
	CMiniGame();
	virtual ~CMiniGame();

	BOOL Load_Alphabet();	// ���ĺ� �ܾ� ���߱�
	BOOL Load_KawiBawiBo();	// ����, ����, ��
	BOOL Load_FiveSystem();	// ����
	BOOL Load_Reassemble(); // ���� 
	char GetCharacter( DWORD ItemId ); // ������ ID �´� ���ĺ� �� ���� ����
	int	GetKawibawiboYou( int nMy, int nResult );
#ifdef __WORLDSERVER
	int Result_Kawibawibo( CUser* pUser );
	BOOL ResultItem_Kawibawibo( CUser* pUser );
	__KAWIBAWIBO FindKawibawibo( int nWinCount );
	
	BOOL Result_Reassemble( CUser* pUser, OBJID* objItemId, int nCount );
	BOOL ResultItem_Reassemble( CUser* pUser );
	__ITEMGAME GetReassembleRandomItem();
	void OpenWnd_Reassemble( CUser* pUser );
	
	void OpenWnd_Alphabet( CUser* pUser );
	int Result_Alphabet( CUser* pUser, OBJID* objItemId, int nCount, int nQuestionID );
	BOOL ResultItem_Alphabet( CUser* pUser, int nNum );
	__ITEMGAME GetAlphabetRandomItem( int nNum );
	char GetPublicCharacter( int nQuestionNum, int nPublicAlphabet );
		
	void OpenWnd_FiveSystem( CUser* pUser );
	void DestroyWnd_FiveSystem( CUser* pUser );
	int Bet_FiveSystem( CUser* pUser, int nBetNum, int nBetPenya );
	int Result_FiveSystem( CUser* pUser );
	BOOL ResultPenya_FiveSystem( CUser* pUser, int nResult );
#endif // __WORLDSERVER
	
	int m_nBetMinPenya;				// ���� - �ּ� ���� �ݾ�
	int m_nBetMaxPenya;				// ���� - �ִ� ���� �ݾ�
	int m_nMultiple;				// ���� - ��� ���� ���
	
	int m_nPayPenya;				// ���ĺ� ���� ���
	
	int nKawiBawiBo_MaxWin;			// ���������� �ִ� ���� �ε���

	std::vector<__ALPHABET>		m_vecAlphabet;	
	std::map<int, __KAWIBAWIBO>  m_mapKawiBawiBo;
	std::map<int, __FIVESYSTEM>	m_mapFiveSystem;
	std::vector<__ITEMGAME>		m_vecReassembleItem;
	std::vector<DWORD>	m_vecReassembleCard;
	std::vector<__KAWIBAWIBORESULT> m_vecKawibawiboResult;
};

#endif // __EVE_MINIGAME
#endif // !defined(AFX_MINIGAME_H__79AC4FCB_D41A_4407_ACF3_0FDF796F2ED1__INCLUDED_)
