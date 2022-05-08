#pragma once

#include <map>
#include <vector>
#include <string>



#include "defineHonor.h"

typedef struct HonorData
{
	int		nID;			// �ε���
	int		nLGrouping;		// ��з�
	int		nSGrouping;		// �Һз�
	int		nNeed;			// Ÿ��Ʋ ȹ�� ����
	int		nValue;			// ���� ��ġ
	CString	strTitle;		// Ÿ��Ʋ��
	HonorData() {Init();};
	~HonorData() {};
	void Init() {nID = -1;nLGrouping =  nSGrouping = nNeed = nValue = 0; strTitle.Empty(); };
}PHonorData, *PPHonorData;

#ifdef __CLIENT
struct EarnedTitle
{
	int	nId;
	CString	strTitle;
};
#endif	// __CLIENT

class CTitleManager
{

private:
	std::map<int, HonorData>		m_mapMonster;
	std::map<int, HonorData>		m_mapItem;
	std::map<int, HonorData>		m_mapEtc;
	std::map<int, HonorData>		m_mapAll;

public:
	CTitleManager();
	~CTitleManager();
	static	CTitleManager*	Instance();

	int		m_nCurrentTitleCount;
	BOOL	LoadTitle(LPCTSTR lpszFileName);
	char*	GetTitle(int nId);
	int		GetNeedCount(int nId,int nGroup);
	int		GetIdxType(int nId);
	int		GetIdx(int nId,int nGroup);
	HonorData*		GetHonorDataByID(int nId,int nGroup);
	int     GetIdxByName(LPCTSTR chName);	
	int     GetIdxByGroup(int nLarge,int nSmall);	

#ifdef __CLIENT
	// ȹ���� Ÿ��Ʋ �ε���
	std::vector<EarnedTitle>	m_vecEarned;

	BOOL	IsEarned(int nId);
	void	AddEarned(int nId);
	BOOL	RemoveEarned(int nId);
	void	InitEarned();
#endif	// __CLIENT

};





