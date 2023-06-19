#ifndef _TOOLTIP_H_
#define _TOOLTIP_H_

enum 
{
	FIRST_TT,
//	SECOND_TT,
//	THIRD_TT,
	MAX_TT
};

class CToolTip
{
	BOOL        m_bEnable       ;
	BOOL        m_bPutToolTip   ;
	BOOL        m_bReadyToopTip ;
	DWORD       m_dwToolTipId   ;
	CEditString m_strToolTip    ;
	CTimer      m_timerToopTip  ;
	CPoint      m_ptToolTip     ;
	CPoint      m_ptOldToolTip  ;
	CRect       m_rect          ;
	CRect       m_rectRender    ;
	int         m_nPosition     ;
	BOOL        m_bToolTip      ;
	int         m_nAlpha        ;
	CTexture	m_apTextureToolTip[ MAX_TT * 9 ];
	int			m_nAdded;
	int			m_nAddedJewel[5];
	int			m_nSlot;
	CTexture*	m_pUltimateTexture;
	CTexture*	m_pJewelBgTexture;

#ifndef __IMPROVE_MAP_SYSTEM
	int			m_nMonInfoCnt;
	DWORD		m_pDwMonId[5];
#endif // __IMPROVE_MAP_SYSTEM
	int			m_nSubToolTipFlag;
	CRect		m_nRevisedRect;
	int			m_nSubToolTipNumber;
#ifdef __IMPROVE_MAP_SYSTEM
	vector< DWORD > m_vecMapMonsterID;
#endif // __IMPROVE_MAP_SYSTEM
public:
	CToolTip();
	~CToolTip();
	void SetPosition( int nPos ) { m_nPosition = nPos; }
	void SetEnable( BOOL bEnable ) { m_bEnable = bEnable; }
	BOOL GetEnable() { return m_bEnable; }
	void CancelToolTip();

	void PutToolTip( DWORD dwToolTipId, CEditString& string, CRect rect, CPoint pt, int nToolTipPos = 0 );
	void PutToolTip( DWORD dwToolTipId, CString& string, CRect rect, CPoint pt, int nToolTipPos = 0 );
	void PutToolTip( DWORD dwToolTipId, LPCTSTR lpszString, CRect rect, CPoint pt, int nToolTipPos = 0 );	
	void PutToolTipEx( DWORD dwToolTipId, CEditString& string, CRect rect, CPoint pt, int nToolTipPos = 0, int nSubToolTipFlag = 0 );
	void Process( CPoint pt, C2DRender* p2DRender );
	void Paint( C2DRender* p2DRender );
	void Delete();
	void InitTexture();
	void SetUltimateToolTip(const CItemElem & pItemBase);
#ifndef __IMPROVE_MAP_SYSTEM
	void SetWorldMapMonsterInfo(int nMonCnt, DWORD* pDwMonId);
#endif // __IMPROVE_MAP_SYSTEM
#ifdef __IMPROVE_MAP_SYSTEM
	void ResizeMapMonsterToolTip( void );
	void InsertMonsterID( DWORD dwMonsterID );
#endif // __IMPROVE_MAP_SYSTEM
	[[nodiscard]] CPoint GetPointToolTip() const { return m_ptToolTip; }
	[[nodiscard]] const CRect & GetRect() const { return m_rect; }
	void SetRenderRect( const CRect& rectRender );
	[[nodiscard]] const CRect & GetRenderRect()  const { return m_rectRender; }
	[[nodiscard]] const CRect & GetRevisedRect() const { return m_nRevisedRect; }
	void SetSubToolTipNumber( int nSubToolTipNumber );
	[[nodiscard]] int GetSubToolTipNumber() const { return m_nSubToolTipNumber; }
	[[nodiscard]] bool GetReadyToolTipSwitch() const { return m_bReadyToopTip; }
};
/*
#define TOOLTIP_COMBATMODE   1  // "����/��ȭ\n����Ű[C]"
#define TOOLTIP_INVENTORY    2  // "����ǰ\n����Ű[I]"
#define TOOLTIP_OPTION       3  // "����\n����Ű[ALT-X]"
#define TOOLTIP_STATUS       4  // "�ɷ�ġ����\n����Ű[S]"

#define TOOLTIP_EQUIPMENT    5  // "�������\n����Ű[E]"
#define TOOLTIP_KNOWLEDGE    6  // "���Ļ��\n����Ű[K]"
#define TOOLTIP_LOOK         7  // "�ڼ�������\n����Ű[L]"

#define TOOLTIP_SLOT         8  // "������ ����"  
#define TOOLTIP_SLOT_F1      9  // "1�� ���� ����\n����Ű[F1]"
#define TOOLTIP_SLOT_F2      10 // "2�� ���� ����\n����Ű[F2]"
#define TOOLTIP_SLOT_F3      11 // "3�� ���� ����\n����Ű[F3]"

#define TOOLTIP_CHAT         12 // "ä�� �Է�â\n����Ű[ENTER]"
#define TOOLTIP_CHAT1        14 // "�Ϲ� ä��"
#define TOOLTIP_CHAT2        15 // "�ӼӸ�"
#define TOOLTIP_CHAT3        16 // 

#define TOOLTIP_MESSAGE      17 // "�޽��� ���â"
*/
#endif