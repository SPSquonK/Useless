#pragma once

#include <map>
#include <boost/container/flat_set.hpp>
#include <string>

#include "defineHonor.h"

struct HonorData {
	int		nID = -1;			// �ε���
	int		nLGrouping = 0;		// ��з�
	int		nSGrouping = 0;		// �Һз�
	int		nNeed = 0;			// Ÿ��Ʋ ȹ�� ����
	int		nValue = 0;			// ���� ��ġ
	CString	strTitle;		// Ÿ��Ʋ��
};

class CTitleManager
{

private:
	std::map<int, HonorData>		m_mapMonster;
	std::map<int, HonorData>		m_mapItem;
	std::map<int, HonorData>		m_mapEtc;
	std::map<int, HonorData>		m_mapAll;

public:
	CTitleManager();
	static	CTitleManager*	Instance();

	int		m_nCurrentTitleCount;
	BOOL	LoadTitle(LPCTSTR lpszFileName);
	const char*	GetTitle(int nId);
	int		GetNeedCount(int nId,int nGroup);
	int		GetIdxType(int nId);
	int		GetIdx(int nId,int nGroup);
	HonorData*		GetHonorDataByID(int nId,int nGroup);
	int     GetIdxByName(LPCTSTR chName);	
	int     GetIdxByGroup(int nLarge,int nSmall);	

#ifdef __CLIENT
	// ȹ���� Ÿ��Ʋ �ε���
	boost::container::flat_set<int> m_vecEarned;

	// Updates `m_vecEarned` for honor nId considering the current advancement is
	// nCurrentCount. Return true if the player just earned the title.
	bool UpdateEarned(int nId, int nCurrentCount);

	void	InitEarned();
#endif	// __CLIENT

};





