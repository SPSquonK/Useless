#if !defined(AFX_ITEM_H__80E88B36_BD6B_449B_BE76_34F2B5B77552__INCLUDED_)
#define AFX_ITEM_H__80E88B36_BD6B_449B_BE76_34F2B5B77552__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#if defined(__DBSERVER) || defined(__CORESERVER)
#include "..\_Network\Objects\Obj.h"
#else
#include "Ctrl.h"
#include "ar.h"
#endif
#include "ProjectCmn.h"
#include "lang.h"

#include "pet.h"

#include "accessory.h"

#include "Piercing.h"

#include "ItemElem.h"

inline void CItemElem::SetExtra(int nExtra) {
	m_nExtra = nExtra;
}

inline int CItemElem::GetExtra() const noexcept {
	return m_nExtra;
}

inline CTexture * CItemElem::GetTexture() const {
#ifdef __CLIENT
	return m_pTexture;
#else
	return NULL;
#endif
}

inline int CItemElem::GetAttrOption()		// �������� +�ɼǰ��� �Ӽ�/�Ӽ��������� ���ļ� ����.
{
	int nAttr = (int)m_bItemResist;
	int nOption = m_nResistAbilityOption;
	int nRet = m_nAbilityOption;
	
	nRet |= (nAttr << 16);		// D16~D23 : �Ӽ�
	nRet |= (nOption << 24);	// D24~D31 : �Ӽ�����
	return nRet;
}

inline int CItemElem::GetItemResist( void ) const
{
	return m_bItemResist;
}

inline int CItemElem::GetResistAbilityOption( void ) const
{
	return m_nResistAbilityOption;
}

#include "SkillsOfMover.h"
#include "ItemContainer.h"




//////////////////////////////////////////////////////////////////////////
//
//
//
//
//////////////////////////////////////////////////////////////////////////
#include "mempooler.h"
typedef MemPooler<CItem> CItemPool;

/// �������� �����ϴ� ������ 
class CItem  : public CCtrl
{
private:
#ifdef __CLIENT
	float	m_fGroundY;		// �������� �����ɴ�� �� ��ġ�� Y��ǥ�� �̸� ���س���.
	D3DXVECTOR3	 m_vDelta;	// 
#endif // __CLIENT
	
public:
	CItemElem * m_pItemBase;
	u_long	m_idHolder;
	OBJID	m_idOwn;		// ���� �̾������� ���� �Ǹ��� �ִ°�.
	DWORD	m_dwDropTime;	// ����������� �ð�.
	BOOL	m_bDropMob;		// ���Ͱ� �׾ ����Ѱ��̳�?.
#ifdef __EVENT_MONSTER
	BOOL	m_IdEventMonster;	// �̺�Ʈ ���Ͱ� ����� �������ΰ�?
#endif // __EVENT_MONSTER


public:
	CItem();
	virtual ~CItem();

	void SetOwner( OBJID id );
	DWORD GetItemType() const { return 0; }
	void SetItemBase( CItemElem * pItemBase ) { m_pItemBase = pItemBase; }

	CItemElem * GetItemBase() const { return m_pItemBase; }
	ItemProp* GetProp() { return prj.GetItemProp( GetIndex() ); }
	
	virtual void Serialize( CAr & ar );
	virtual void Process();
	virtual void Render( );
	virtual void RenderName( CD3DFont* pFont, DWORD dwColor = 0xffffffff );
	virtual BOOL Read( CFileIO* pFile );
		
#ifdef __CLIENT
	void SetDelta( float fGroundY, D3DXVECTOR3 &vDelta );
#endif // __CLIENT

public:
#ifndef __VM_0820
#ifndef __MEM_TRACE
	static	CItemPool*		m_pPool;
	void*	operator new( size_t nSize )	{	return CItem::m_pPool->Alloc();	}
	void*	operator new( size_t nSize, LPCSTR lpszFileName, int nLine )	{	return CItem::m_pPool->Alloc();	}
	void	operator delete( void* lpMem )	{	CItem::m_pPool->Free( (CItem*)lpMem );	}
	void	operator delete( void* lpMem, LPCSTR lpszFileName, int nLine )	{	CItem::m_pPool->Free( (CItem*)lpMem );	}
#endif	// __MEM_TRACE
#endif	// __VM_0820
};

inline CItem * CObj::ToItem() {
	if (GetType() != OT_ITEM) return nullptr;
	return static_cast<CItem *>(this);
}

inline const CItem * CObj::ToItem() const {
	if (GetType() != OT_ITEM) return nullptr;
	return static_cast<const CItem *>(this);
}

bool IsUsableItem(const CItemElem * pItem);
bool IsUsingItem(const CItemElem * pItem);

#endif // !defined(AFX_ITEM_H__80E88B36_BD6B_449B_BE76_34F2B5B77552__INCLUDED_)

