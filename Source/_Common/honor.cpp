#include "stdafx.h"
#include "..\_Common\honor.h"


CTitleManager::CTitleManager()
{
	m_mapMonster.clear();
	m_mapItem.clear();
	m_mapEtc.clear();
	m_mapAll.clear();
	m_nCurrentTitleCount = 0;
#ifdef __CLIENT
	m_vecEarned.clear();
#endif	// __CLIENT
}


CTitleManager::~CTitleManager()
{

}

CTitleManager* CTitleManager::Instance( void )
{
	static CTitleManager tManager;
	return &tManager;
}

BOOL	CTitleManager::LoadTitle(LPCTSTR lpszFileName)
{
	HonorData	TempData;
	CScript		script;


	if( script.Load( lpszFileName ) == FALSE )
		return FALSE;

	script.tok = 0;

	TempData.Init();
	TempData.nID		= script.GetNumber(); // ID

	while(script.tok != FINISHED)
	{
		
		TempData.nLGrouping = script.GetNumber();	// ��з�
		TempData.nSGrouping = script.GetNumber();	// �Һз�
		TempData.nNeed		= script.GetNumber();	// ȹ�� ����
		script.GetToken();
		TempData.strTitle	= script.Token;	// Ÿ��Ʋ �̸�
		switch(TempData.nLGrouping)
		{
			case HI_HUNT_MONSTER:
				if(m_mapMonster.size() < MONSTER_TITLE_MAX)
				{
					m_mapMonster.emplace(TempData.nSGrouping, TempData);
					m_mapAll.emplace(TempData.nID, TempData);
				}
				else
				{
					Error("Too many title about monster!");
					return FALSE;
				}
				break;

			case HI_USE_ITEM:
				if(m_mapItem.size() < MONSTER_TITLE_MAX)
				{
					m_mapItem.emplace(TempData.nSGrouping, TempData);
					m_mapAll.emplace(TempData.nID, TempData);
				}
				else
				{
					Error("Too many title about item!");
					return FALSE;
				}
				break;

			default:
				m_mapEtc.emplace(TempData.nSGrouping, TempData);
				m_mapAll.emplace(TempData.nID, TempData);
				break;
		};
		m_nCurrentTitleCount++;
		TempData.Init();
		TempData.nID		= script.GetNumber(); // ID
	}
	
	return TRUE;
}
char	*CTitleManager::GetTitle(int nId)		// Ÿ��Ʋ�� ��ȯ
{
	if(nId == -1) return NULL; 
	//int nGroup = GetIdxType(nId);
	auto iter = m_mapAll.find(nId);	// ���� ���̵�� �˻�
	if(iter != m_mapAll.end()) 
	{
		return (LPSTR)(LPCSTR)iter->second.strTitle;
	}

	return NULL;
}

int	CTitleManager::GetNeedCount(int nId, int nGroup)		// ȹ�氡�� ī��Ʈ ����
{
	switch(nGroup)
	{
		case HI_HUNT_MONSTER: {
			const auto iter = m_mapMonster.find(nId); // ���� ���̵�� �˻�
			if (iter != m_mapMonster.end()) {
				return iter->second.nNeed;
			} else
				return -1;
			break;
		}

		case HI_USE_ITEM: {
			const auto iter = m_mapItem.find(nId);	// ������ ���̵�� �˻�
			if (iter != m_mapItem.end()) {
				return iter->second.nNeed;
			} else
				return -1;
			break;
		}

		case -1: {		// ���δ� �˻�
			const auto iter = m_mapAll.find(nId);	// ���� ���̵�� �˻�
			if (iter != m_mapAll.end()) {
				return iter->second.nNeed;
			} else
				return -1;
			break;
		}

		default: {
			const auto iter = m_mapEtc.find(nId);	// ������ ���̵�� �˻�
			if (iter != m_mapEtc.end()) {
				return iter->second.nNeed;
			} else
				return -1;
			break;
		}
	};
	return -1;
}

HonorData*		CTitleManager::GetHonorDataByID(int nId,int nGroup)
{
	std::map<int, HonorData>::iterator iter;

	switch(nGroup)
	{
		case HI_HUNT_MONSTER:
			iter = m_mapMonster.find(nId); // ���� ���̵�� �˻�
			if(iter != m_mapMonster.end()) 
			{
				return &iter->second;
			}
			else
				return NULL;
			break;

		case HI_USE_ITEM:
			iter = m_mapItem.find(nId);	// ������ ���̵�� �˻�
			if(iter != m_mapItem.end()) 
			{
				return &iter->second;
			}
			else
				return NULL;
			break;

		default:
			iter = m_mapEtc.find(nId);	// ������ ���̵�� �˻�
			if(iter != m_mapEtc.end()) 
			{
				return &iter->second;
			}
			else
				return NULL;
			break;
	};
	return NULL;
}
int	CTitleManager::GetIdx(int nId, int nGroup)		// 
{

	std::map<int, HonorData>::iterator iter;

	switch(nGroup)
	{
		case HI_HUNT_MONSTER:
			iter = m_mapMonster.find(nId); // ���� ���̵�� �˻�
			if(iter != m_mapMonster.end()) 
			{
				return iter->second.nID;
			}
			else
				return -1;
			break;

		case HI_USE_ITEM:
			iter = m_mapItem.find(nId);	// ������ ���̵�� �˻�
			if(iter != m_mapItem.end()) 
			{
				return iter->second.nID;
			}
			else
				return -1;
			break;

		default:
			iter = m_mapEtc.find(nId);	// ������ ���̵�� �˻�
			if(iter != m_mapEtc.end()) 
			{
				return iter->second.nID;
			}
			else
				return -1;
			break;
	};
	return -1;
}


int		CTitleManager::GetIdxType(int nId)
{
	const auto iter = m_mapAll.find(nId); // ���� ���̵�� �˻�

	if(iter != m_mapAll.end()) 
	{
		return iter->second.nLGrouping;
	}
	
	return -1;
}


int CTitleManager::GetIdxByName(LPCTSTR chName)		// 
{
	for(auto iter = m_mapAll.begin(); iter != m_mapAll.end(); ++iter) // �׿ܴ� 1��1 Ž��..������ �󸶾ȵǼ� ���� ������..
	{
		HonorData	TempData = iter->second;
		if(lstrcmp(TempData.strTitle,chName) == 0 )
           return TempData.nID;
	};
	return -1;
	
}
int     CTitleManager::GetIdxByGroup(int nLarge,int nSmall)
{
	for( auto iter = m_mapEtc.begin(); iter != m_mapEtc.end(); ++iter )
	{
		HonorData	TempData = iter->second;
		if( TempData.nLGrouping == nLarge && TempData.nSGrouping == nSmall  )
			return TempData.nID;
	}
	return -1;
}	
#ifdef __CLIENT
BOOL	CTitleManager::IsEarned(int nId)
{
	for(auto iter = m_vecEarned.begin(); iter != m_vecEarned.end(); ++iter)
	{
		if(iter->nId == nId) return TRUE;
	}
	return FALSE;
}

void	CTitleManager::AddEarned(int nId)
{
	EarnedTitle Temp;
	Temp.nId		= nId;
	Temp.strTitle	= GetTitle(nId); 
	m_vecEarned.push_back(Temp);
}

BOOL	CTitleManager::RemoveEarned(int nId)
{
	for(auto iter = m_vecEarned.begin(); iter != m_vecEarned.end(); ++iter)
	{
		if(iter->nId == nId)
		{
			m_vecEarned.erase(iter);
			return TRUE;
		}
	}
	return FALSE;
}

void	CTitleManager::InitEarned()
{
	m_vecEarned.clear();
}

#endif	// __CLIENT

