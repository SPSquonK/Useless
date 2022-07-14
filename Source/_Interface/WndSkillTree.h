#pragma once

#include <vector>


class CWndSkillTreeEx : public CWndNeuz {
protected:
	int  m_nCount = 0;					//���� ������ �ش��ϴ� ��ų �� ����

	int  m_nCurSelect = -1;			//�� �Ⱦ����ִµ�?
	BOOL m_bDrag = FALSE;					//���콺�� Ŭ���ߴµ� ��ų������ ���� �� �� ��� TRUE
	int				m_nCurrSkillPoint = 0;		//���� ���� ��ų����Ʈ
	CTexturePack	m_textPackNum;			//��ų �������� ǥ�ÿ� �׸� ( 1, 2,..... max )
	DWORD			m_dwMouseSkill = 0;			//���콺�� ��ġ�� ��ų
	CWndButton * m_pWndButton[4];		//+, -, reset, finish

	MoverSkills m_skills;				//��ų ���
	std::vector<CTexture * > m_skillsTexture;

	CTexture * m_aSkillLevel[3] = { nullptr, nullptr, nullptr };
	IMAGE * m_atexJobPannel[2] = { nullptr, nullptr };
	CString		  m_strHeroSkilBg;			//����� �̹��� ���� �̸�
	int m_nJob = -1;			//class ��ȣ

	CTexture m_texGauEmptyNormal;
	CTexture m_texGauFillNormal;

	LPDIRECT3DVERTEXBUFFER9 m_pVBGauge = nullptr;

	CTexture * m_atexTopDown[2] = { nullptr, nullptr };
	int 	m_nTopDownGap = 0;
	BOOL m_bSlot[4] = { TRUE, TRUE, TRUE, TRUE };

	BOOL m_bLegend = FALSE;							//����
	CWndStatic * m_pWndHeroStatic[2] = { nullptr, nullptr };

public:
	[[nodiscard]] int GetCurrSkillPoint() const noexcept { return m_nCurrSkillPoint; }
	void	SubSkillPointDown(SKILL * lpSkill);

	int		GetCalcImagePos(int nIndex);
	[[nodiscard]] int GetCurSelect() const { return m_nCurSelect; }
	SKILL * GetFocusedItem() { return GetSkill(m_nCurSelect); }

	BOOL	GetSkillPoint(DWORD dwSkillID, CRect & rect);
	LPSKILL GetSkill(int i);
	void LoadTextureSkillicon();
	void InitItem();

	const MoverSkills & GetSkills() const { return m_skills; };

	virtual ~CWndSkillTreeEx();

	void AfterSkinTexture(LPWORD pDest, CSize size, D3DFORMAT d3dFormat = D3DFMT_A4R4G4B4);
	void SetJob(int nJob);
	virtual BOOL Process();
	virtual void OnDraw(C2DRender * p2DRender);
	virtual	void OnInitialUpdate();
	virtual BOOL Initialize(CWndBase * pWndParent = NULL, DWORD dwWndId = 0);
	virtual void OnMouseMove(UINT nFlags, CPoint point);
	virtual void OnMouseWndSurface(CPoint point);
	// message
	virtual BOOL OnChildNotify(UINT message, UINT nID, LRESULT * pLResult);
	virtual void OnLButtonUp(UINT nFlags, CPoint point);
	virtual void OnLButtonDown(UINT nFlags, CPoint point);
	virtual void OnLButtonDblClk(UINT nFlags, CPoint point);
	virtual void OnRButtonDblClk(UINT nFlags, CPoint point);
	void SetActiveSlot(int nSlot, BOOL bFlag);
	virtual void SerializeRegInfo(CAr & ar, DWORD & dwVersion);
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();

private:
	[[nodiscard]] bool IsDownPoint(const SKILL & skill) const; // Return true if the skill can be down leveled
	[[nodiscard]] bool CheckSkill(int i);
};


class CWndReSkillWarning : public CWndNeuz {
	bool	m_bParentDestroy;
public:
	CWndReSkillWarning(bool parentDestroy) : m_bParentDestroy(parentDestroy) {}
	virtual ~CWndReSkillWarning();

	virtual	void OnInitialUpdate();
	virtual BOOL Initialize(CWndBase * pWndParent = NULL, DWORD dwWndId = 0);
	virtual BOOL OnChildNotify(UINT message, UINT nID, LRESULT * pLResult);
	virtual void OnDestroy();
};

