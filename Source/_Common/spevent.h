#pragma once 

struct EVENT_GENERIC {
	static constexpr size_t MAX_EVENT_TITLE = 100;

	int    nId          = 0; // Event number, 500 starts, max is 531
	int    nFlag        = 0; // event flag, value 1 (event number - 500) left shift
	time_t tStart       = 0;
	time_t tEnd         = 0;
	FLOAT  fExpFactor   = 1.0f;
#ifdef __ITEMDROPRATE
	FLOAT fItemDropRate = 1.0f;
#endif // __ITEMDROPRATE
	char pszTitle[MAX_EVENT_TITLE] = "";
};

class CEventItem
{
private:
	int		m_nMax;		// Maximum daily drop
	DWORD	m_adwInterval[24];
	DWORD	m_dwTimeout;
	LONG	m_lSkip;
public:
	DWORD	m_dwItemId;
	int		m_nNum;		// Max drop at once
public:

	CEventItem();
	CEventItem( DWORD dwItemId, int nMax, int nNum );

	bool IsTimeout( int nHour );		// Is it a drop cycle?
	void	Skip( LONG lSkip );		// Set number of drop ignores in drop cycle
	friend CAr & operator<<(CAr & ar, const CEventItem & self);
	friend CAr & operator>>(CAr & ar, CEventItem & self);
};

// Because the project is not loaded on the core server
// Manage objects on the trans server.

#ifdef __EVENT_0117
typedef	struct	_REGION_GENERIC		// ���� �̺�Ʈ (������, ����)�� ���� ���� ���� ����
{
	int		nLevel;	// ���� ����
	DWORD	dwWorldId;		// ���� ��ȣ
	CRespawnInfo* pi;	// ���� ����
}	REGION_GENERIC,	*PREGION_GENERIC;

class CSpawn
{
private:
	int		m_nMax;		// ���� �ִ� ����
	float	m_fRatio;	// ���� ���� ������ ��� �ְ����� ���� Ȯ��
	DWORD	m_dwInterval;	// ���� ���� ms
	DWORD	m_dwTimeout;	// ���� �ð� Ÿ�� �ƿ�
public:
	DWORD	m_dwType;	// ���� ��ü Ÿ��	
	DWORD	m_dwIndex;		// ���� ��ü �ε���
public:
// Constructions
	CSpawn();
	CSpawn( DWORD dwType, DWORD dwIndex, int nMax, float fRatio, DWORD dwInterval );
	virtual	~CSpawn()	{}
//	Operations
	BOOL	IsTimeout( void );	// ���� �ֱ� �ΰ�?
private:
	DWORD	GetMaxSpawnProbability( int nMax )
	{
		ASSERT( m_fRatio > 0 );
		return	( (DWORD)( ( nMax + ( m_fRatio - 1.0F ) * ( nMax - 1 ) / 2.0F ) * 100.0F ) );
	}
public:
	int	GetSpawnIndex( int nMax )
	{
		ASSERT( m_fRatio > 0 );
		DWORD dwProb	= xRand() % GetMaxSpawnProbability( nMax );
		float c	= ( m_fRatio - 1.0F ) / (float)nMax;
		float D	= pow( c + 2, 2 ) - 4 * c * ( 2 - dwProb / 50.0F );
		int nIndex	= 0;
		if( D > 0 )
			nIndex	= (int)( ( - ( 2 + c ) + sqrt( D ) ) / ( 2 * c ) + 1 );
		else
			nIndex	= (int)( ( - ( 2 + c ) - sqrt( D ) ) / ( 2 * c ) + 1 );
		if( nIndex < 0 || nIndex >= nMax )
		{
			// error
			nIndex	= 0;
		}
		return nIndex;
	}
};
#endif	// __EVENT_0117

#ifdef __EVENT_0117
class CRespawnInfo;
#endif	// __EVENT_0117
class CEventGeneric
{
private:
	DWORD	m_dwFlag;
	std::vector<EVENT_GENERIC> m_lspEvent;
	std::map<int, std::vector<CEventItem>> m_mapEventItemList;

#ifdef __EVENT_0117
	std::vector<REGION_GENERIC>	m_aRegionGeneric;	// ��� ������ ������ ������ ���Ϳ� �����Ѵ�.	// ���� ��ȣ, ����, ����
	std::map<int, CSpawn*>	m_mapSpawn;		// �̺�Ʈ�� Ȱ��ȭ �Ǿ��� ��, �ش� �̺�Ʈ�� ������ �����Ѵٸ� �������� ���� ���̴�.
#endif	// __EVENT_0117

public:
	CEventGeneric();
	virtual	~CEventGeneric();
//	op
	void Clear( BOOL bDestructor = TRUE );
	// ��ũ��Ʈ �ε�
	BOOL	LoadScript( LPCSTR lpFilename );

#ifdef __WORLDSERVER
	std::pair<CEventItem *, int> GetItem();
	FLOAT	GetExpFactor( void );
#ifdef __ITEMDROPRATE
	FLOAT	GetItemDropRateFactor( void );
#endif // __ITEMDROPRATE
#ifdef __EVENT_1101
	LONG	GetEventElapsed( void );
	void	CallTheRoll( void );
#endif	// __EVENT_1101

#ifdef __EVENT_0117
	void	AddSpawn( int nEvent, DWORD dwType, DWORD dwIndex, int nMax, float fRatio, DWORD dwInterval );		// ���� �̺�Ʈ ���� ���
	void	AddRegionGeneric( int nLevel, DWORD dwWorldId, CRespawnInfo* pi );	// ���� ���� ���
	void	SortRegionGeneric( void );	// ���� ���� ����
	void	Spawn( void );
#endif	// __EVENT_0117

#endif	// __WORLDSERVER
	static	CEventGeneric*	GetInstance( void );
	friend CAr & operator<<(CAr & ar, const CEventGeneric & self);
	friend CAr & operator>>(CAr & ar, CEventGeneric & self);
	DWORD	GetFlag( void )		{	return m_dwFlag;	}
	BOOL	Run( void );
	[[nodiscard]] std::span<const EVENT_GENERIC> GetEventList() const { return m_lspEvent; }
	[[nodiscard]] const EVENT_GENERIC * GetEvent(int nEvent) const;
};

