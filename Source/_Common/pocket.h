#ifndef __POCKET_H__
#define	__POCKET_H__

#if defined(__WORLDSERVER) || defined(__CLIENT)
#include "item.h"
#endif	// __WORLDSERVER || __CLIENT

#include "ar.h"

class CPocket : public CItemContainer
{
public:
// Constructions
	CPocket();
	virtual	~CPocket();

// Attributes
	void	SetExpired( BOOL bExpired )	{	m_bExpired	= bExpired;		}
	BOOL	IsExpired( void ) const	{	return m_bExpired;	}
	void	SetExpirationDate( time_t tExpirationDate )	{	m_tExpirationDate	= tExpirationDate;		}
	time_t	GetExpirationDate( void )	{	return m_tExpirationDate;	}
	void	Copy( CPocket & rPocket );
// Operations
	friend CAr & operator<<(CAr & ar, const CPocket & self);
	friend CAr & operator>>(CAr & ar, CPocket & self);

private:
	BOOL	m_bExpired;
	time_t	m_tExpirationDate;
};

#define	MAX_POCKET	3
#define	POCKET_SLOT_SIZE_0	6
#define	POCKET_SLOT_SIZE_1_2	24

class CPocketController
{
public:
// Constructions
	CPocketController();
	virtual	~CPocketController();
#ifdef __WORLDSERVER
	void	Init( CMover* pMover )	{	m_pMover	= pMover;	}
#endif	// __WORLDSERVER
	void	Clear( void );
// Operations
	CItemElem*	GetAtId( int nPocket, int nItem, BOOL bExpiration = TRUE );	

	BOOL	Add( int nPocket, CItemElem*pItemElem );
	BOOL	RemoveAtId( int nPocket, int nItem, int nNum );		// if nNum < 0, remove all.
	int		RemoveItem( DWORD dwItemId, int nNum );
	
	int		GetEmptyCount( void );
	BOOL	IsAllClean( void );
	int		GetCount( DWORD dwItemId );
	
	BOOL	IsAvailable( int nPocket, BOOL bExpiration = TRUE ) const
		{	return( m_apPocket[nPocket] && ( !bExpiration || !m_apPocket[nPocket]->IsExpired() ) );		}
	void	Avail( int nPocket, time_t t = 0 );

	enum	{	expired, avail,	};
	void	SetAttribute( int nAttribute, int nPocket, int nData );
	time_t		GetAvailable( int nPocket );

	void	Copy( CPocketController & rPocketController );
	friend CAr & operator<<(CAr & ar, const CPocketController & self);
	friend CAr & operator>>(CAr & ar, CPocketController & self);

#ifdef __WORLDSERVER
	void	ProcessExpiring( void );
#endif	// __WORLDSERVER
	CPocket*	GetPocket( int nPocket );

#ifdef __CLIENT
	CItemElem*	GetAt( int nPocket, int nIndex );
#endif	// __CLIENT

private:
#ifdef __WORLDSERVER
	CMover*	m_pMover;
#endif	// __WORLDSERVER
	CPocket*	m_apPocket[MAX_POCKET];
};
#endif	// __POCKET_H__