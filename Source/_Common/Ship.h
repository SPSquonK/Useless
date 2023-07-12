#pragma once

#include "Ctrl.h"
#include "..\_AIInterface\ActionShip.h"

/// ����� 
class CShip : public CCtrl
{
private:
	CMover *m_pMover;			// �� ���� ����. �����ϴ� ���.
	CActionShip m_Act;
	
protected:
	int		m_nCount;
	D3DXVECTOR3		m_vDelta;
	D3DXVECTOR3		m_vDeltaUnit, m_vAcc;		// ����, �������� ��
	FLOAT			m_fDeltaAng;
	FLOAT	m_fAccAng;		// ȸ�� ���ӵ�.
	std::vector<OBJID> m_LinkCtrl;
	
	virtual void Init( void );
	virtual void Destroy( void );
public:
	friend CActionShip;
	
	CShip();
	virtual ~CShip();
	
	CMover *GetMover( void ) { return m_pMover; }				// �� ���� ������ ������.
	void SetMover( CMover *pMover ) { m_pMover = pMover; }		// �� �����ϴ� ����� ����.
	int		SendActMsg( OBJMSG dwMsg, int nParam1 = 0, int nParam2 = 0, int nParam3 = 0 )
	{
		return m_Act.SendActMsg( dwMsg, nParam1, nParam2, nParam3 );
	}
	void AddCtrl( OBJID idCtrl );
	void RemoveCtrl( OBJID idCtrl );
	[[nodiscard]] D3DXVECTOR3 GetDelta() const { return m_vDelta; }
	[[nodiscard]] FLOAT GetDeltaAng() const { return m_fDeltaAng; }

#ifdef __CLIENT
	void Control( void );
#endif
	virtual void Process();
};

inline CShip * CObj::ToShip() {
	if (GetType() != OT_SHIP) return nullptr;
	return static_cast<CShip *>(this);
}

inline const CShip * CObj::ToShip() const {
	if (GetType() != OT_SHIP) return nullptr;
	return static_cast<const CShip *>(this);
}

// �ö󸮽� ������ ������ ���� ��
class CShipLoop : public CShip
{
private:
	
protected:
	void Init( void );
	void Destroy( void );
public:
	
	CShipLoop();
	virtual ~CShipLoop();
	
	void Process();
	
};


#ifdef __CLIENT
extern CShip *g_pShip;
#endif
