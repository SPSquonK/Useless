// Exchange.h: interface for the CExchange class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EXCHANGE_H__24DC28E3_B7E1_44DE_99C9_197B1B1F4AA4__INCLUDED_)
#define AFX_EXCHANGE_H__24DC28E3_B7E1_44DE_99C9_197B1B1F4AA4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef __TRADESYS
class CExchange  
{
public:
	enum { EXCHANGE_SUCCESS, EXCHANGE_FAILED, EXCHANGE_INVENTORY_FAILED, EXCHANGE_CONDITION_FAILED };
	
	struct __ITEM
	{
		DWORD	dwItemId = NULL_ID;
		int		nItemNum = 0;
		int		nPayProb = 0;
		BYTE	byFalg = 0;
	};

	struct __POINT
	{
		BYTE	nType = 0;
		int		nPoint = 0;
	};

	struct __SET
	{
		UINT	nSetTextId  = NULL_ID;
		std::vector<int>    vecResultMsg;
		std::vector<__ITEM> vecCondItem;
		std::vector<__ITEM> vecRemoveItem;
		std::vector<__POINT> vecCondPoint;
		std::vector<__POINT> vecRemovePoint;
		std::vector<__ITEM> vecPayItem;
		int		nPayNum = 0;
	};

	typedef struct __SETLIST
	{
		std::vector<int>		vecDesciprtionId;
		std::vector<__SET>	vecSet;
	} SETLIST, *PSETLIST;

	CExchange();
	virtual ~CExchange();

	BOOL			Load_Script();					// Load Script
	std::vector<int>		GetListTextId( int nMMIId );	// �� ����Ʈ�� ���� TEXT ID ���
	std::vector<int>		GetDescId( int nMMIId );		// ��ü ���� TEXT ID ���
	std::vector<int>		GetResultMsg( int nMMIId, int nListNum );
	PSETLIST		FindExchange( int nMMIID );		
#ifdef __WORLDSERVER
	int				ResultExchange( CUser* pUser, int nMMIid, int nListNum ); // ���� ���
	BOOL			CheckCondition( CUser* pUser, int nMMIId, int nListNum ); // ���� ��
	BOOL			IsFull( CUser* pUser, std::vector<__ITEM> vecRemoveItem, std::vector<__ITEM> vecPayItem ); // �κ� ����
	std::vector<__ITEM>	GetRemoveItemList( int nMMIId, int nListNum ); // ���� ������ ����Ʈ
	std::vector<__ITEM>	GetPayItemList( int nMMIId, int nListNum ); // ���� ������ ����Ʈ
	std::vector<__POINT>	GetRemovePointList( int nMMIId, int nListNum ); // ���� ����Ʈ ����Ʈ
#endif // __WORLDSERVER
		
	std::map<int, __SETLIST> m_mapExchange;
};

#endif // __TRADESYS

#endif // !defined(AFX_EXCHANGE_H__24DC28E3_B7E1_44DE_99C9_197B1B1F4AA4__INCLUDED_)
