#pragma once

#include <map>
#include <list>

//	mulcom	BEGIN100420	���� ���� ������ ����.
//#define	MAX_MAILTITLE	128
#define	MAX_MAILTITLE	32
//	mulcom	END100420	���� ���� ������ ����.
#define	MAX_MAILTEXT	256

#ifdef __INTERNALSERVER
// ����
#define MAX_KEEP_MAX_DAY		7   // �ִ� �����ϼ� 5��
#define MAX_KEEP_BASIC_DAY		2   // �⺻ �����ϼ� 1��
#else //__INTERNALSERVER
// ����
#define MAX_KEEP_MAX_DAY		15  // �ִ� �����ϼ� 15��
#define MAX_KEEP_BASIC_DAY		7	// �⺻ �����ϼ� 7��
#endif //__INTERNALSERVER

class CItemElem;
#ifdef __DBSERVER
class CMailBox;
#endif	// __DBSERVER

class CMail final
{
public:
	CMail();
	CMail( u_long idSender, CItemElem* pItemElem, int nGold, char* szTitle, char* szText );
	~CMail();

	enum	{	mail,	item,	gold,	read	};

	void	Clear( void );

#ifdef __DBSERVER
	void	SetMailBox( CMailBox* pMailBox )	{	m_pMailBox	= pMailBox;	}
	CMailBox*	GetMailBox( void )	{	return m_pMailBox;	}
#endif	// __DBSERVER
	void	Serialize( CAr & ar, BOOL bData = TRUE );
public:
	u_long	m_nMail;
	u_long	m_idSender;
	CItemElem*	m_pItemElem;
	DWORD	m_nGold;
	time_t	m_tmCreate;
	BYTE	m_byRead;
	char	m_szTitle[MAX_MAILTITLE];
	char	m_szText[MAX_MAILTEXT];

	[[nodiscard]] std::pair<int, DWORD> GetMailInfo() const;

	static	u_long	s_nMail;
#ifdef __DBSERVER
private:
	CMailBox*	m_pMailBox;
#endif	// __DBSERVER
};

#ifdef __DBSERVER
class CPost;
#endif	// __DBSERVER

class CMailBox : public std::vector<CMail *>
{
public:
	static constexpr size_t SoftMaxMail = 50;

	explicit CMailBox( u_long idReceiver = 0 );
	virtual	~CMailBox();
	u_long	AddMail( CMail* pMail );
	void	Serialize( CAr & ar, BOOL bData = TRUE );

#ifdef __DBSERVER
	void	WriteMailContent( CAr & ar );
#endif	// __DBSERVER
#ifdef __WORLDSERVER
	void ReadMailContent(CAr & ar);
#endif	// __WORLDSERVER

	CMail*	GetMail( u_long nMail );
	BOOL	RemoveMail( u_long nMail );
	BOOL	RemoveMailItem( u_long nMail );
	BOOL	RemoveMailGold( u_long nMail );
	BOOL	ReadMail( u_long nMail );
	[[nodiscard]] bool IsStampedMailExists() const;
	void	Clear( void );
#ifdef __DBSERVER
	void	SetPost( CPost* pPost )	{	m_pPost	= pPost;	}
	CPost*	GetPost( void )	{	return m_pPost;	}
#endif	// __DBSERVER

#ifdef __CLIENT
	static	CMailBox*	GetInstance( void );
#endif
private:
	std::vector<CMail *>::iterator Find( u_long nMail );
#ifdef __DBSERVER
	CPost*	m_pPost;
#endif	// __DBSERVER

public:
	u_long	m_idReceiver;
#ifdef __WORLDSERVER
	enum	{	nodata	= 0,	read	= 1,	data	= 2	};
	int		m_nStatus;
#endif	// __WORLDSERVER
};

#if defined(__DBSERVER) || defined(__WORLDSERVER)
class CPost final {
public:
	void Clear();
//	Operations
	u_long	AddMail( u_long idReceiver, CMail* pMail );
	CMailBox*	GetMailBox( u_long idReceiver );
	BOOL	AddMailBox( CMailBox* pMailBox );
	void	Serialize( CAr & ar, BOOL bData = TRUE );

#ifdef __DBSERVER
	std::map< u_long, CMail* >	m_mapMail4Proc;
	CMclCritSec	m_csPost;
	void	Process( void );
#endif	// __DBSERVER

	static CPost * GetInstance();
private:
	std::map<u_long, std::unique_ptr<CMailBox>> m_mapMailBox;
};

#endif

