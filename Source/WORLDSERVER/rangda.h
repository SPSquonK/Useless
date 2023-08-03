#pragma once


// ���� ���� �̺�Ʈ ���͸� �����ϴ� Ŭ����
class CRangda final
{
public:
	// ���� �̺�Ʈ ���Ͱ� ��Ÿ���� ������ ��Ÿ���� ����ü
	struct RANGDA_POS final {
		DWORD	dwWorldId;	// ��Ÿ�� �� �ִ� ���� ���� �ĺ���
		D3DXVECTOR3 vPos;	// ��Ÿ�� �� �ִ� ���� ��ǥ
	};


	CRangda( DWORD dwMonster );

	void	AddPos( const RANGDA_POS & vPos );	// ��ǥ �߰�
	void	OnTimer();	// ƽ ó��
	void	SetInterval( int nInterval )	{	m_nInterval	= nInterval;	}	// ��� ���� ����
	void	SetReplace( int nReplace )	{	m_nReplace	= nReplace;		}	// �̵� ���� ����
	void	SetActiveAttack( BOOL bActiveAttack )	{	m_bActiveAttack		= bActiveAttack;	}	// ���� ���� ����

private:
	RANGDA_POS	GetRandomPos( void );	// ���� ��ǥ ��ȯ
	void	SetObj( OBJID objid );		// �� ��ü ����
	BOOL	HavetoGenerate( void );		// ��� ������ �����ϴ°�?
	BOOL	HavetoReplace( void );		// �̵� ������ �����ϴ°�?
	void	CreateMonster( void );	// �� ��ü ����
	BOOL	IsDamaged( CMover* pMonster );	// ��ó �Ծ���?
	void	ProcessReplace( CMover* pMonster );		// �̵� ó��
	void	ProcessGenerate( void );	// ��� ó��
	CMover*		GetMonster( void );		// �� ��ü ��ȯ
private:
	const DWORD	m_dwMonster;	// ������ ���� ���� �ĺ���
	int		m_nInterval;	// ��� ����
	int		m_nReplace;		// �̵� ����
	BOOL	m_bActiveAttack;	// ���� ����
	std::vector<RANGDA_POS>	m_vvPos;	// ��Ÿ�� �� �ִ� ��ǥ ����
	int		m_nGenerateCountdown;	// ��� ��� �ð� ƽ�� 1 ����
	int		m_nReplaceCountdown;	// �̵� ��� �ð�, ƽ�� 1 ����
	BOOL	m_bReplaceable;		// �̵� ���� �÷���
	int		m_nOldPos;		// ���� ��ǥ
	OBJID	m_objid;	// �� ��ü �ĺ���
};


class CRangdaController final {
public:
	static CRangdaController * Instance();

	bool LoadScript(const char * szFile);
	void OnTimer();
	
private:
	CRangdaController() = default;
private:
	std::vector<CRangda> m_vRangda;
};

