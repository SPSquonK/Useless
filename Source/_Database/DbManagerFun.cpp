#include "StdAfx.h"
#include "defineObj.h"
#include "dbmanager.h"
#include "dploginsrvr.h"
#include "dpcoresrvr.h"
#include "dptrans.h"
#include "..\_Network\Objects\Obj.h"
#include "misc.h"
#include "mytrace.h"
#include "definequest.h"
#include "DbSerializer.h"

extern	BOOL CheckValidItem( DWORD dwItemId, short nNum );

#include "party.h"
#include "guild.h"
#include "guildwar.h"
#include "honor.h"

extern const int MAX_TASKBAR; 
extern const int MAX_APPLETTASKBAR; 
extern const int MAX_ITEMTASKBAR;   
extern const char NullStr[2];


#include "eveschool.h"

extern	CProject			prj;

#define	VERIFYSTRING_RETURN( lpString, lpszPlayer )	\
	if( FALSE == VerifyString( lpString, __FILE__, __LINE__, lpszPlayer ) )		return FALSE;

#define	VERIFYSTRING_BANK( lpString, lpszPlayer )	\
	if( FALSE == VerifyString( lpString, __FILE__, __LINE__, lpszPlayer ) )		return FALSE;

CTime CDbManager::GetStrTime(const char * strbuf) {
	char cYear[5] = { 0, };
	char cMonth[3] = { 0, };
	char cDay[3] = { 0, };
	char cHour[3] = { 0, };
	char cMin[3] = { 0, };

	strncpy(cYear, strbuf, 4);
	strncpy(cMonth, strbuf + 4, 2);
	strncpy(cDay, strbuf + 6, 2);
	strncpy(cHour, strbuf + 8, 2);
	strncpy(cMin, strbuf + 10, 2);

	return CTime(atoi(cYear), atoi(cMonth), atoi(cDay), atoi(cHour), atoi(cMin), 0);
}

void CDbManager::GetDBFormatStr( char* szDst, int nMaxLen, const char* szSrc )
{
	char szDigit[3] = {0, };
	int ch2;
	char* pCur	= const_cast<char*>( szSrc );
	int nLen	= strlen( pCur );
	if( nLen % 2 != 0 )
	{
		pCur[nLen-1]	= '\0';
		nLen--;
	}
	if( nLen >= nMaxLen * 2 )
	{
		Error( "GetDBFormatStr: nLen = %d, nMaxLen = %d", nLen, nMaxLen );
		*szDst	= '\0';
		return;
	}
	while( *pCur )
	{
		szDigit[0]	= pCur[0];
		szDigit[1]	= pCur[1];
		pCur	+= 2;
		sscanf( szDigit, "%2X", &ch2 );
		*szDst++	= ch2;
	}
	*szDst	= '\0';
}

void CDbManager::SetDBFormatStr( char* szDst, int nMaxLen, const char* szSrc )
{
	int nLen	= strlen( szSrc );
	if( nMaxLen < nLen * 2 + 1 )
	{
		Error( "SetDBFormatStr: nLen = %d, nMaxLen = %d", nLen, nMaxLen );
		*szDst	= '\0';
		return;
	}

	char* pCur	= szDst;
	while( *szSrc )
	{
		sprintf( pCur, "%2X",  (BYTE)*szSrc );
		pCur	+= 2;
		szSrc++;
	}
	*pCur	= '\0';
}

BOOL CDbManager::GetBank( CMover* pMover, CQuery *qry, LPDB_OVERLAPPED_PLUS, int nSlot )
{
	u_long uPlayerid	= (u_long)qry->GetInt( "m_idPlayer" );
	DWORD dwGold = qry->GetInt( "m_dwGoldBank" );

	pMover->m_idPlayerBank[nSlot] = uPlayerid;
	pMover->m_dwGoldBank[nSlot] = dwGold;

	return GetBankMover(pMover, qry, nSlot);
}

bool CDbManager::GetBankMover( CMover* pMover, CQuery *qry, int nSlot )
{
	char debugString[256];
	std::sprintf(debugString, __FUNCTION__" - Player Bank %s Slot %d", pMover->m_szName, nSlot);

	ItemContainerSerialization serialization = {
		.main = qry->GetStrPtr("m_Bank"),
		.apIndex = qry->GetStrPtr("m_apIndex_Bank"),
		.dwObjIndex = qry->GetStrPtr("m_dwObjIndex_Bank"),
		.ext = qry->GetStrPtr("m_extBank"),
		.piercing = qry->GetStrPtr("m_BankPiercing"),
		.szPet = qry->GetStrPtr("szBankPet"),

		.debugString = debugString
	};

	if (!serialization.CheckValidity()) return FALSE;
	return ReadItemContainer(pMover->m_Bank[nSlot], serialization);
}

void CDbManager::GetGuildBank( CItemContainer*  GuildBank, CQuery *qry ) {
	ItemContainerSerialization serialization = {
		.main = qry->GetStrPtr("m_GuildBank"),
		.apIndex = "$",
		.dwObjIndex = "$",
		.ext = qry->GetStrPtr("m_extGuildBank"),
		.piercing = qry->GetStrPtr("m_GuildBankPiercing"),
		.szPet = qry->GetStrPtr("szGuildBankPet"),

		.debugString = __FUNCTION__
	};

	if (!serialization.CheckValidity()) return;
	ReadItemContainer(*GuildBank, serialization);
}

BOOL CDbManager::GetPocket( CMover* pMover, CQuery* pQuery, LPDB_OVERLAPPED_PLUS lpDbOverlappedPlus ) {
	const int nPocket = pQuery->GetInt("nPocket");
	const BOOL bExpired = (BOOL)pQuery->GetInt("bExpired");

	const char * szPlayerName;
	if (lpDbOverlappedPlus) {
		szPlayerName = lpDbOverlappedPlus->AccountInfo.szPlayer;
	} else {
		szPlayerName = pMover->m_szName;
	}

	char debugString[256];
	std::sprintf(debugString, __FUNCTION__" Player %s Pocket %d", szPlayerName, nPocket);

	ItemContainerSerialization serialization = {
		.main = pQuery->GetStrPtr("szItem"),
		.apIndex = pQuery->GetStrPtr("szIndex"),
		.dwObjIndex = pQuery->GetStrPtr("szObjIndex"),
		.ext = pQuery->GetStrPtr("szExt"),
		.piercing = pQuery->GetStrPtr("szPiercing"),
		.szPet = pQuery->GetStrPtr("szPet"),

		.debugString = debugString
	};

	if (!serialization.CheckValidity()) return FALSE;

	if( serialization.main[0] != '$' || !bExpired ) {
		pMover->m_Pocket.Avail( nPocket );
		CPocket* pPocket	= pMover->m_Pocket.GetPocket( nPocket );
		pPocket->SetExpired( bExpired );
		pPocket->SetExpirationDate( (time_t)pQuery->GetInt( "tExpirationDate" ) );

		ReadItemContainer(*pPocket, serialization);
	}

	return TRUE;
}

BOOL CDbManager::GetInventory( CMover* pMover, CQuery *qry, LPDB_OVERLAPPED_PLUS lpDbOverlappedPlus )
{
	const char * szPlayerName;
	if (lpDbOverlappedPlus) {
		szPlayerName = lpDbOverlappedPlus->AccountInfo.szPlayer;
	} else {
		szPlayerName = pMover->m_szName;
	}

	char debugString[256];
	std::sprintf(debugString, __FUNCTION__" Player %s Pocket %d", szPlayerName);

	ItemContainerSerialization serialization = {
		.main = qry->GetStrPtr("m_Inventory"),
		.apIndex = qry->GetStrPtr("m_apIndex"),
		.dwObjIndex = qry->GetStrPtr("m_dwObjIndex"),
		.ext = qry->GetStrPtr("m_extInventory"),
		.piercing = qry->GetStrPtr("m_InventoryPiercing"),
		.szPet = qry->GetStrPtr("szInventoryPet"),

		.debugString = debugString
	};

	if (!serialization.CheckValidity()) return FALSE;
	return ReadItemContainer(pMover->m_Inventory, serialization) ? TRUE : FALSE;
}

std::vector<std::pair<DWORD, CPet *>> CDbManager::GetPets(const char * szPet) {
	std::vector<std::pair<DWORD, CPet *>> res;

	for (auto splitter : DBDeserialize::SplitBySlash(szPet)) {
		const bool isPet = splitter.NextBool();

		if (isPet) {
			CPet * pPet = new CPet;
			pPet->SetKind(static_cast<BYTE>(splitter.NextInt()));
			pPet->SetLevel(static_cast<BYTE>(splitter.NextInt()));
			pPet->SetExp(splitter.NextDWORD());
			pPet->SetEnergy(splitter.NextUShort());
			pPet->SetLife(splitter.NextUShort());

			for (int i = PL_D; i <= pPet->GetLevel(); i++) {
				pPet->SetAvailLevel(i, static_cast<BYTE>(splitter.NextInt()));
			}

			char szName[MAX_PET_NAME] = { 0, };
			splitter.NextStringInBuffer(szName);

			pPet->SetName(szName);

			res.emplace_back(splitter.Index(), pPet);
		}
	}

	return res;
}

void CDbManager::LoadPiercingInfo( CItemElem & itemElem, const char* szPirecingInven, int* pLocation )
{
	const ItemProp* itemProp = itemElem.GetProp();
	if( itemProp && itemProp->IsUltimate() )
	{
		itemElem.SetUltimatePiercingSize( GetIntPaFromStr( szPirecingInven, pLocation ) );
		for( int i = 0 ; i < itemElem.GetUltimatePiercingSize(); ++i )
			itemElem.SetUltimatePiercingItem( i, (DWORD)GetIntPaFromStr( szPirecingInven, pLocation ) );
	}
	itemElem.SetPiercingSize( GetIntFromStr( szPirecingInven, pLocation ) );
	for( int i = 0 ; i < itemElem.GetPiercingSize(); ++i )
		itemElem.SetPiercingItem( i, (DWORD)GetIntFromStr( szPirecingInven, pLocation ) );
	if( itemElem.IsVisPet() )
	{
		itemElem.SetVisKeepTimeSize( itemElem.GetPiercingSize() );
		for( int i=0; i<itemElem.GetPiercingSize(); i++ )
			itemElem.SetVisKeepTime( i, (time_t)GetIntFromStr( szPirecingInven, pLocation ) );
	}
}

// 얼터멋 웨폰의 경우 nPiercedSize가 얼터멋 피어싱 사이즈이고 dwItemId1 ~ 5 까지는 얼터멋 피어싱 아이템(보석)이다.
// nPiercedSize2가 일반 피어싱 사이즈이고 dwItemId6 ~ 15 까지가 일반 피어싱 아이템(카드)이다.
// 얼터멋 웨폰이 아닌 경우에는 nPiercedSize가 피어싱 사이즈이고 dwItemId1~10 까지가 피어싱 아이템(카드)이다.
// 이는 MakeQueryAddMail() 함수에도 동일하게 적용된다.
void CDbManager::GetPiercingInfoFromMail( CQuery* pQuery, CItemElem* pItemElem )
{
	int nAddCount = 1;
	if( pItemElem->GetProp() && pItemElem->GetProp()->IsUltimate() )
	{
		pItemElem->SetUltimatePiercingSize( pQuery->GetInt( "nPiercedSize" ) );
		for( int i=0; i<pItemElem->GetUltimatePiercingSize(); i++ )
		{
			char szItem[32];
			sprintf( szItem, "dwItemId%d", i + nAddCount );
			pItemElem->SetUltimatePiercingItem( i, pQuery->GetInt( szItem ) );
		}
		pItemElem->SetPiercingSize( pQuery->GetInt( "nPiercedSize2" ) );
		nAddCount = 6;
	}
	else
		pItemElem->SetPiercingSize( pQuery->GetInt( "nPiercedSize" ) );

	for( int i=0; i<pItemElem->GetPiercingSize(); i++ )
	{
		char szItem[32];
		sprintf( szItem, "dwItemId%d", i + nAddCount );
		pItemElem->SetPiercingItem( i, pQuery->GetInt( szItem ) );
	}
}

BOOL CDbManager::GetEquipment( CMover* pMover, CQuery *qry, LPDB_OVERLAPPED_PLUS lpDbOverlappedPlus )
{
	int CountStr	= 0;
	int IndexEquipment	= 0;
	char Equipment[500]		= {0,};
	qry->GetStr( "m_adwEquipment", Equipment );
	VERIFYSTRING_RETURN( Equipment, lpDbOverlappedPlus->AccountInfo.szPlayer );
	while( '$' != Equipment[CountStr] )
	{
		if( IndexEquipment < MAX_HUMAN_PARTS )
			pMover->m_aEquipInfo[IndexEquipment].dwId	= (DWORD)GetIntFromStr( Equipment, &CountStr );
		IndexEquipment++;
	}
	return TRUE;
}

BOOL CDbManager::GetTaskBar( CMover* pMover, CQuery *qry, LPDB_OVERLAPPED_PLUS lpDbOverlappedPlus )
{
	static constexpr auto ReadShortcut = [](DBDeserialize::WordSplitter & splitter) -> SHORTCUT {
		SHORTCUT retval;
		retval.m_dwShortcut = splitter.NextEnum<ShortcutType>();
		retval.m_dwId = splitter.NextDWORD();
		const int type = splitter.NextInt();
		retval.m_dwIndex = splitter.NextDWORD();
		/* m_dwUserId */ splitter.NextDWORD();
		retval.m_dwData = splitter.NextDWORD();
		if (retval.m_dwShortcut == ShortcutType::Chat) {
			splitter.NextStringInBuffer(retval.m_szString);
		} else {
			retval.m_szString[0] = '\0';
		}

		if (retval.m_dwShortcut == ShortcutType::Skill && type == 2) {
			retval.m_dwShortcut = ShortcutType::PartySkill;
		}

		return retval;
	};

	const char * AppletTaskBar = qry->GetStrPtr( "m_aSlotApplet");
	VERIFYSTRING_RETURN( AppletTaskBar, lpDbOverlappedPlus->AccountInfo.szPlayer );

	for (auto splitter : DBDeserialize::SplitBySlash(AppletTaskBar)) {
		const int nIndex = splitter.NextInt();
		pMover->m_UserTaskBar.m_aSlotApplet[nIndex] = ReadShortcut(splitter);
	}
	
	const char * ItemTaskBar = qry->GetStrPtr("m_aSlotItem");
	VERIFYSTRING_RETURN( ItemTaskBar, lpDbOverlappedPlus->AccountInfo.szPlayer );

	for (auto splitter : DBDeserialize::SplitBySlash(ItemTaskBar)) {
		const int nSlotIndex = splitter.NextInt();
		const int nIndex     = splitter.NextInt();
		pMover->m_UserTaskBar.m_aSlotItem[nSlotIndex][nIndex] = ReadShortcut(splitter);
	}
	
	const char * SkillTaskBar = qry->GetStrPtr("m_aSlotQueue");
	VERIFYSTRING_RETURN( SkillTaskBar, lpDbOverlappedPlus->AccountInfo.szPlayer );
	for (auto splitter : DBDeserialize::SplitBySlash(SkillTaskBar)) {
		const int nIndex = splitter.NextInt();
		pMover->m_UserTaskBar.m_aSlotQueue[nIndex] = ReadShortcut(splitter);
	}

	pMover->m_UserTaskBar.m_nActionPoint = qry->GetInt( "m_SkillBar" );
	return TRUE;
}

void CDbManager::GetCardCube( CMover* pMover, CQuery *qry, LPDB_OVERLAPPED_PLUS lpDbOverlappedPlus )
{
}


void CDbManager::GetBaseCharacter( CMover* pMover, CQuery *qry, LPDB_OVERLAPPED_PLUS lpDbOverlappedPlus )
{
	qry->GetStr( "m_szName", pMover->m_szName );
	pMover->m_vScale.x		= qry->GetFloat( "m_vScale_x" );
	pMover->m_dwMotion		= (DWORD)qry->GetInt( "m_dwMotion" );
	pMover->m_dwWorldID		= qry->GetInt( "dwWorldID");
	pMover->m_vPos.x		= qry->GetFloat( "m_vPos_x" );
	pMover->m_vPos.y		= qry->GetFloat( "m_vPos_y" );
	pMover->m_vPos.z		= qry->GetFloat( "m_vPos_z" );
	pMover->m_fAngle		= qry->GetFloat( "m_fAngle" );
	qry->GetStr( "m_szCharacterKey", pMover->m_szCharacterKey );
	pMover->m_nHitPoint		= (LONG)qry->GetInt( "m_nHitPoint" );
	pMover->m_nManaPoint	= (LONG)qry->GetInt( "m_nManaPoint" );
	pMover->m_nFatiguePoint		= (LONG)qry->GetInt( "m_nFatiguePoint" );
	pMover->m_dwSkinSet		= (DWORD)qry->GetInt( "m_dwSkinSet" );
	pMover->m_dwHairMesh	= (DWORD)qry->GetInt( "m_dwHairMesh" );
	pMover->m_dwHairColor	= (DWORD)qry->GetInt( "m_dwHairColor" );
	pMover->m_dwHeadMesh	= (DWORD)qry->GetInt( "m_dwHeadMesh" );
	pMover->SetSex( (BYTE)qry->GetInt( "m_dwSex" ) );
	pMover->m_dwRideItemIdx		= (DWORD)qry->GetInt( "m_dwRideItemIdx" );
	pMover->SetGold( qry->GetInt("m_dwGold") );
	pMover->m_nJob			= (LONG)qry->GetInt( "m_nJob" );
	pMover->m_idparty		= (u_long)qry->GetInt( "m_idparty" );
	pMover->m_nPKValue			= qry->GetInt( "m_nPKValue" );
	pMover->m_dwPKPropensity	= qry->GetInt( "m_dwPKPropensity" );
	pMover->m_dwPKExp			= qry->GetInt( "m_dwPKExp" );
	pMover->m_nAngelExp		= qry->GetExpInteger( "m_nAngelExp" );
	pMover->m_nAngelLevel	= qry->GetInt( "m_nAngelLevel" );
	
	pMover->m_idMurderer	= (u_long)qry->GetInt( "m_idMuerderer" );
	pMover->m_nFame			= qry->GetInt( "m_nFame" );
	
	int CountStr	= 0;
	char ActMover[50]	= {0,};
	qry->GetStr( "m_pActMover", ActMover );
	pMover->m_pActMover->m_dwState	= (DWORD)GetIntFromStr( ActMover, &CountStr );
	pMover->m_pActMover->m_dwStateFlag	= (DWORD)GetIntFromStr( ActMover, &CountStr );
	
	CountStr	= 0;
	pMover->m_nStr	= (LONG)qry->GetInt( "m_nStr" );
	pMover->m_nSta	= (LONG)qry->GetInt( "m_nSta" );
	pMover->m_nDex	= (LONG)qry->GetInt( "m_nDex" );
	pMover->m_nInt	= (LONG)qry->GetInt( "m_nInt" );
	pMover->m_nLevel	= (LONG)qry->GetInt( "m_nLevel" );
	pMover->m_nExp1		= qry->GetExpInteger( "m_nExp1" );
	pMover->m_nFuel		= qry->GetInt( "m_nFuel" );
	pMover->m_tmAccFuel	= qry->GetInt( "m_tmAccFuel" );
	
//	pMover->m_dwExpertLv	= (DWORD)qry->GetInt( "m_dwExpertLv");
	pMover->m_idMarkingWorld	= (DWORD)qry->GetInt( "m_idMarkingWorld");
	pMover->m_vMarkingPos.x		= qry->GetFloat( "m_vMarkingPos_x" );
	pMover->m_vMarkingPos.y		= qry->GetFloat( "m_vMarkingPos_y" );
	pMover->m_vMarkingPos.z		= qry->GetFloat( "m_vMarkingPos_z" );
	pMover->m_nRemainGP = (long)qry->GetInt( "m_nRemainGP" );
//	pMover->m_nRemainLP = (long)qry->GetInt( "m_nRemainLP" );
	int nTutorialState	= qry->GetInt( "m_nFlightLv" );
	pMover->SetTutorialState( nTutorialState );
	pMover->m_nFxp	= (long)qry->GetInt( "m_nFxp" );
//	pMover->m_nTxp	= (long)qry->GetInt( "m_nTxp" );
	pMover->m_dwMode	= (DWORD)qry->GetInt( "m_dwMode" );
	pMover->m_bPlayer	= 1;
	pMover->m_nDeathExp	= qry->GetExpInteger( "m_nDeathExp" );
	pMover->m_nDeathLevel = qry->GetInt( "m_nDeathLevel" );
	pMover->m_nSkillLevel	= qry->GetInt( "m_SkillLv" );
	pMover->m_nSkillPoint	= qry->GetInt( "m_SkillPoint" );
	pMover->m_nHonor	= qry->GetInt( "m_nHonor" );

}
bool	CDbManager::GetHonor( CMover* pMover, CQuery *qry, LPDB_OVERLAPPED_PLUS lpDbOverlappedPlus )
{
	// 내가 등록한 아이디 가지고 오기
	char szQuery[QUERY_SIZE]	= { 0,};
	sprintf( szQuery, "usp_Master_Select '%02d','%07d'", g_appInfo.dwSys, pMover->m_idPlayer );
	if (!qry->Exec(szQuery)) {
		WriteLog("%s, %d\t%s", __FILE__, __LINE__, szQuery);
		return false;
	}
	
	const char * serialized = qry->GetStrPtr("progress");
	
	std::ranges::fill(pMover->m_aHonorTitle, 0);

	VERIFYSTRING_RETURN(serialized, lpDbOverlappedPlus->AccountInfo.szPlayer);

	for (auto splitter : DBDeserialize::SplitBySlash(serialized)) {
		const int nIndex = splitter.NextInt();
		const int progress = splitter.NextInt();

		if (nIndex >= 0 && std::cmp_less_equal(nIndex, std::span(pMover->m_aHonorTitle).size())) {
			pMover->m_aHonorTitle[nIndex] = progress;
		} else {
			Error(__FUNCTION__": %s has the success (id=%d, progress=%d) but the maximum id is supposed to be %lu",
				lpDbOverlappedPlus->AccountInfo.szPlayer,
				nIndex, progress, 
				std::span(pMover->m_aHonorTitle).size()
				);
		}
	}

	return true;
}


BOOL CDbManager::GetSkill( CMover* pMover, CQuery *qry, LPDB_OVERLAPPED_PLUS lpDbOverlappedPlus )
{
	// 내가 등록한 아이디 가지고 오기
	char szQuery[QUERY_SIZE]	= { 0,};
	sprintf( szQuery,
		"uspLoadCharacterSkill '%02d','%07d'",
		g_appInfo.dwSys, pMover->m_idPlayer );
	if( FALSE == qry->Exec( szQuery ) )
	{
		WriteLog( "%s, %d\t%s", __FILE__, __LINE__, szQuery );
		return FALSE;
	}

	pMover->m_jobSkills.clear();

	if (qry->Fetch()) {
		const char * stringified = qry->GetStrPtr("Skill");
		int i = 0;
		while (stringified[i] != '$') {
			const DWORD skillId = static_cast<DWORD>(GetIntFromStr(stringified, &i));
			const DWORD skillLevel = static_cast<DWORD>(GetIntFromStr(stringified, &i));

			SKILL skill{ .dwSkill = skillId, .dwLevel = skillLevel };
			pMover->m_jobSkills.emplace_back(skill);
		}
	}

	return TRUE;
}

void CDbManager::GetJobLv( CMover* pMover, CQuery *qry, LPDB_OVERLAPPED_PLUS lpDbOverlappedPlus )
{
}

BOOL CDbManager::GetQuest( CMover* pMover, CQuery *qry, LPDB_OVERLAPPED_PLUS lpDbOverlappedPlus )
{
	pMover->m_quests = std::make_unique<MoverSub::Quests>();

	int CountStr	= 0;
	const char * QuestCnt = qry->GetStrPtr( "m_lpQuestCntArray");
	VERIFYSTRING_RETURN(QuestCnt, pMover->m_szName);
	while ('$' != QuestCnt[CountStr]) {
		const QUEST BufQuest = GetOneQuest(QuestCnt, &CountStr);
		pMover->m_quests->current.emplace_back(BufQuest);
	}

	CountStr	= 0;
	const char * CompleteQuest = qry->GetStrPtr("m_aCompleteQuest");
	VERIFYSTRING_RETURN(CompleteQuest, pMover->m_szName);
	while ('$' != CompleteQuest[CountStr]) {
		const WORD id = (WORD)GetIntFromStr(CompleteQuest, &CountStr);
		pMover->m_quests->completed.emplace_back(id);
	}

	CountStr = 0;
	const char * CheckedQuest = qry->GetStrPtr("m_aCheckedQuest");
	VERIFYSTRING_RETURN( CheckedQuest, pMover->m_szName );
	while( '$' != CheckedQuest[CountStr] )
	{
		const WORD id = (WORD)GetIntFromStr( CheckedQuest, &CountStr ); 
		pMover->m_quests->checked.emplace_back(id);
	}

	// 기존 것중에 완료된것을 완료 배열에 넣는다.
	constexpr auto Normalize = [](MoverSub::Quests & quests) {
		for (auto lpQuest = quests.current.begin();
			lpQuest != quests.current.end();
			) {

			if (lpQuest->m_nState == QS_END) {
				// Ensure is in completed, remove from current and checked
				const auto itCompleted = std::ranges::find(quests.completed, lpQuest->m_wId);

				if (itCompleted == quests.completed.end()) {
					quests.completed.emplace_back(lpQuest->m_wId);
				}

				const auto itChecked = std::ranges::find(quests.checked, lpQuest->m_wId);
				if (itChecked != quests.checked.end()) {
					quests.checked.erase(itChecked);
				}

				lpQuest = quests.current.erase(lpQuest);
			} else {
				// Ensure no dupes
				quests.current.erase(
					std::remove_if(
						lpQuest + 1, quests.current.end(),
						MoverSub::Quests::ById(lpQuest->m_wId)
					),
					quests.current.end()
				);

				++lpQuest;
			}
		}
	};
	
	Normalize(*pMover->m_quests);

	return TRUE;
}

BOOL CDbManager::GetSMMode( CMover* pMover, CQuery *qry, LPDB_OVERLAPPED_PLUS lpDbOverlappedPlus )
{
	int CountStr	= 0;
	int IndexSM	= 0;
	char SMCnt[3072]		= {0,};
	qry->GetStr( "m_dwSMTime", SMCnt );
	if( 0 != strcmp( SMCnt, "NULL" ) ) 
	{
		VERIFYSTRING_RETURN( SMCnt, pMover->m_szName );
		while( '$' != SMCnt[CountStr] )
		{
			pMover->m_dwSMTime[IndexSM] = (DWORD)GetIntFromStr( SMCnt, &CountStr );
			switch( IndexSM )
			{
			case SM_MAX_HP50:
				{
					pMover->m_nPlusMaxHitPoint = (LONG)GetIntFromStr( SMCnt, &CountStr );
				}
				break;
			case SM_RESIST_ATTACK_LEFT:
				{
					pMover->m_nAttackResistLeft = (BYTE)GetIntFromStr( SMCnt, &CountStr );
				}
				break;
			case SM_RESIST_ATTACK_RIGHT:
				{
					pMover->m_nAttackResistRight = (BYTE)GetIntFromStr( SMCnt, &CountStr );
				}
				break;
			case SM_RESIST_DEFENSE:
				{
					pMover->m_nDefenseResist = (BYTE)GetIntFromStr( SMCnt, &CountStr );
				}
				break;
			}
			++IndexSM;
		}			
	}
	return TRUE;
}

BOOL CDbManager::GetSKillInfluence( CMover* pMover, CQuery *qry, LPDB_OVERLAPPED_PLUS lpDbOverlappedPlus )
{
	int CountStr	= 0;
	int iSkillIndex = 0;
	char szSKillInfluence[7500]		= {0,};
	qry->GetStr( "SkillInfluence", szSKillInfluence );
	VERIFYSTRING_RETURN( szSKillInfluence, pMover->m_szName );
	while( '$' != szSKillInfluence[CountStr] )
	{
		WORD wType	= (WORD)GetIntFromStr( szSKillInfluence, &CountStr );
		WORD wId	= (WORD)GetIntFromStr( szSKillInfluence, &CountStr );
		DWORD dwLevel	= (DWORD)GetIntFromStr( szSKillInfluence, &CountStr );
		DWORD tmTotal	= GetIntFromStr( szSKillInfluence, &CountStr );
		if( wType == 0 && wId == 0 && dwLevel == 0 && tmTotal == 0 )
			break;
		IBuff* pBuff	= pMover->m_buffs.CreateBuff( wType );
		pBuff->SetId( wId );
		pBuff->SetLevel( dwLevel );
		pBuff->SetTotal( tmTotal );
		bool bResult	= pMover->m_buffs.Add( pBuff );
		ASSERT( bResult );
		
		++iSkillIndex;
	}
	return TRUE;
}

int CDbManager::GetOneItem( CItemElem* pItemElem, const char* pstrItem, int *pLocation )
{
	int IndexItem		= 0;
	IndexItem	= GetIntPaFromStr( pstrItem, pLocation );
	pItemElem->m_dwItemId	= (DWORD)GetIntPaFromStr( pstrItem, pLocation );
	BOOL bEquip	= GetIntPaFromStr( pstrItem, pLocation );
	int nTrade	= GetIntPaFromStr( pstrItem, pLocation );
	GetStrPaFromStr( pstrItem, pItemElem->m_szItemText, pLocation );
	pItemElem->m_nItemNum	= GetIntPaFromStr( pstrItem, pLocation );

	if( CheckValidItem( pItemElem->m_dwItemId, pItemElem->m_nItemNum ) == FALSE )
		pItemElem->m_nItemNum	= 1;

	pItemElem->m_nRepairNumber	= (BYTE)GetIntPaFromStr( pstrItem, pLocation );
	pItemElem->m_nHitPoint	= GetIntPaFromStr( pstrItem, pLocation );
	pItemElem->m_nRepair	= GetIntPaFromStr( pstrItem, pLocation );
	int nMaterial	= GetIntPaFromStr( pstrItem, pLocation );
	pItemElem->m_byFlag		= (BYTE)GetIntPaFromStr( pstrItem, pLocation );
	pItemElem->SetSerialNumber( GetSerialNumberPaFromStr( pstrItem, pLocation ) );
	pItemElem->SetOption( GetIntPaFromStr( pstrItem, pLocation ) );
	pItemElem->m_bItemResist	= (BYTE)GetIntPaFromStr( pstrItem, pLocation );
	pItemElem->m_nResistAbilityOption	= GetIntPaFromStr( pstrItem, pLocation );
	pItemElem->m_idGuild = (u_long)GetIntPaFromStr( pstrItem, pLocation );
	pItemElem->m_nResistSMItemId = GetIntPaFromStr( pstrItem, pLocation );
	pItemElem->m_dwObjId		= IndexItem;
	if( pItemElem->m_dwItemId == II_SYS_SYS_SCR_SEALCHARACTER )
	{
		CPlayerDataCenter::GetInstance()->m_Access.Enter();
		const char*	lpszPlayer	= CPlayerDataCenter::GetInstance()->GetPlayerString( pItemElem->m_nHitPoint );
		if( lpszPlayer != NULL)
			memcpy(pItemElem->m_szItemText,lpszPlayer,sizeof(pItemElem->m_szItemText));
		CPlayerDataCenter::GetInstance()->m_Access.Leave();
	}
	

	++*pLocation;
	return IndexItem;
}

void CDbManager::GetOneSkill( LPSKILL pSkill, char* pstrSkill, int *pLocation )
{
	(DWORD)GetIntPaFromStr( pstrSkill, pLocation );
	pSkill->dwLevel	= (DWORD)GetIntPaFromStr( pstrSkill, pLocation );
	pSkill->dwSkill	= (DWORD)GetIntPaFromStr( pstrSkill, pLocation );
	(BOOL)GetIntPaFromStr( pstrSkill, pLocation );
	
	++*pLocation;
}

QUEST CDbManager::GetOneQuest( const char* pstrQuest, int *pLocation )
{
	QUEST pQuest;
	pQuest.m_wId				= QuestId(GetIntPaFromStr( pstrQuest, pLocation ));
	pQuest.m_nState			= (BYTE)GetIntPaFromStr( pstrQuest, pLocation );
	pQuest.m_wTime				= (WORD)GetIntPaFromStr( pstrQuest, pLocation );

	pQuest.m_nKillNPCNum[0]	= (WORD)GetIntPaFromStr( pstrQuest, pLocation );	// chipi_091015 - NPC Kill Quest 갯수 확장( BYTE -> WORD )
	pQuest.m_nKillNPCNum[1]	= (WORD)GetIntPaFromStr( pstrQuest, pLocation );	// chipi_091015 - NPC Kill Quest 갯수 확장( BYTE -> WORD )
	pQuest.m_bPatrol			= (BYTE)GetIntPaFromStr( pstrQuest, pLocation );
	pQuest.m_bReserve2			= (BYTE)GetIntPaFromStr( pstrQuest, pLocation );
	pQuest.m_bReserve3			= (BYTE)GetIntPaFromStr( pstrQuest, pLocation );
	pQuest.m_bReserve4			= (BYTE)GetIntPaFromStr( pstrQuest, pLocation );
	pQuest.m_bReserve5			= (BYTE)GetIntPaFromStr( pstrQuest, pLocation );
	pQuest.m_bReserve6			= (BYTE)GetIntPaFromStr( pstrQuest, pLocation );
	pQuest.m_bReserve7			= (BYTE)GetIntPaFromStr( pstrQuest, pLocation );
	pQuest.m_bReserve8			= (BYTE)GetIntPaFromStr( pstrQuest, pLocation );

	++*pLocation;
	return pQuest;
}


BOOL CDbManager::GetRemoveItem( CQuery *pQry, int &nNo, char* pOneItem, int &nItem_Count, int &nAbilityOption, int &nItemResist, int &nResistAbilityOption, char &chState )
{
	nNo = pQry->GetInt( "m_nNo" );									// 유니크 넘버
	pQry->GetStr( "Item_Name", pOneItem );							// 이름
	
	nItem_Count	= pQry->GetInt( "Item_count" );						// 갯수
	nAbilityOption = pQry->GetInt( "m_nAbilityOption" );			// AbilityOption ( + ) Option
	chState = pQry->GetChar( "State" );
	nItemResist = pQry->GetInt( "m_bItemResist" );					// 속성값	
	nResistAbilityOption = pQry->GetInt( "m_nResistAbilityOption" );// 속성에 대한 Option값
	
	if( 0 == strcmp( pOneItem, "NULL") )
	{
		WriteLog( "Remove NULL Item_Name : %s", pOneItem );
		return FALSE;
	}

	if( chState != 'I' && chState != 'B'
#ifdef __REMOVEITEM_POCKET
		&& chState != 'P'
#endif // __REMOVEITEM_POCKET
		)
	{
		WriteLog( "Remove Not Data State : %c", chState );
		return FALSE;
	}

	
	if( nItem_Count == -100 || nItem_Count == 0 )
		nItem_Count = 1;
	
	if( nAbilityOption == -100 )
		nAbilityOption = 0;
	
	if( nItemResist == -100 )
		nItemResist = 0;
	
	if( nResistAbilityOption == -100 )
		nResistAbilityOption = 0;

	
	return TRUE;
}

BOOL CDbManager::RemovePenya( CQuery *pQry, char* szSql, CMover* pMover, int nItem_Count, int nNo, char chState )
{
	if( RemoveItemDeleteQuery( pQry, szSql, nNo ) == FALSE )
		return FALSE;

	if( chState == 'I' )	// Inventory
	{
		pMover->AddGold( -nItem_Count, FALSE );
	}
	else	// Bank
	{
		DWORD dwGoldBank	= pMover->m_dwGoldBank[pMover->m_nSlot];
		pMover->m_dwGoldBank[pMover->m_nSlot]	-= nItem_Count;
		if( dwGoldBank < pMover->m_dwGoldBank[pMover->m_nSlot] )
			pMover->m_dwGoldBank[pMover->m_nSlot]	= 0;
	}

	return TRUE;
}

SERIALNUMBER CDbManager::RemoveItem( CQuery *pQry, char* szSql, int nNo, CMover* pMover, char* szItemName, int nItemCount, int nAbilityOption, int nItemResist, int nResistAbilityOption, char chState )
{
	SERIALNUMBER iSerialNumber = 0;

	int nIndex = atoi( szItemName );
	ItemProp* pItemProp = NULL;
	if( nIndex > 0 )
		pItemProp = prj.GetItemProp( nIndex );	
	else
		pItemProp = prj.GetItemProp( szItemName );	

	if( pItemProp )
	{
		if( RemoveItemDeleteQuery( pQry, szSql, nNo ) == FALSE )
			return 0;

#ifdef __REMOVEITEM_POCKET
		switch( chState )
		{
			case 'I' :
				iSerialNumber	= RemoveItemInventory( pItemProp, pMover, nAbilityOption, nItemResist, nResistAbilityOption, nItemCount );
				break;
			
			case 'B' :
				iSerialNumber	= RemoveItemBank( pItemProp, pMover, nAbilityOption, nItemResist, nResistAbilityOption, nItemCount );
				break;

			case 'P' :
				iSerialNumber	= RemoveItemPocket( pItemProp, pMover, nAbilityOption, nItemResist, nResistAbilityOption, nItemCount );
				break;
		}
#else // __REMOVEITEM_POCKET
		if( chState == 'I' )
		{
			iSerialNumber	= RemoveItemInventory( pItemProp, pMover, nAbilityOption, nItemResist, nResistAbilityOption, nItemCount );
		}
		else
		{
			iSerialNumber	= RemoveItemBank( pItemProp, pMover, nAbilityOption, nItemResist, nResistAbilityOption, nItemCount );
		}
#endif // __REMOVEITEM_POCKET

		
		if( iSerialNumber == 0 )
			return 0;
		else
			return iSerialNumber;
	}
	else
	{	// no propItem		
		return 0;
	}
	return 0;
}

SERIALNUMBER CDbManager::RemoveItemInventory( ItemProp* pItemProp, CMover* pMover, int nAbilityOption, int nItemResist, int nResistAbilityOption, int nItemCount )
{
	SERIALNUMBER iSerialNumber = 0;
	CItemElem *pItemElem = pMover->m_Inventory.GetItem( pItemProp, nAbilityOption, nItemResist, nResistAbilityOption );
	
	if( pItemElem != NULL ) // 인벤토리에 아이템이 있음을 검사.
	{
		iSerialNumber = pItemElem->GetSerialNumber();
		if( pMover->m_Inventory.IsEquip( pItemElem->m_dwObjId ) )	// 장비를 착용하고 있는지 검사
		{
			if( !pMover->m_Inventory.UnEquip( pItemProp->dwParts ) ) // 장비 해제
			{
				WriteLog( "Not UnEquip : %s, %s ", pMover->m_szName, pItemProp->szName );
				return (SERIALNUMBER)0;
			}
			else
				pMover->m_aEquipInfo[pItemProp->dwParts].dwId	= NULL_ID;
		}

		if( pItemElem->m_nItemNum <= nItemCount )
			pMover->m_Inventory.RemoveAtId( pItemElem->m_dwObjId );
		else
			pItemElem->m_nItemNum -= nItemCount;
		
		return iSerialNumber;
	}
	else
	{
		WriteLog( "Not Inventory : %s, %s ", pMover->m_szName, pItemProp->szName );
		return 0;
	}
	return 0;
}

SERIALNUMBER CDbManager::RemoveItemBank( ItemProp* pItemProp, CMover* pMover, int nAbilityOption, int nItemResist, int nResistAbilityOption, int nItemCount )
{
	SERIALNUMBER iSerialNumber = 0;
	CItemElem *pItemElem	= pMover->m_Bank[pMover->m_nSlot].GetItem( pItemProp, nAbilityOption, nItemResist, nResistAbilityOption );

	if( pItemElem != NULL )
	{
		iSerialNumber	= pItemElem->GetSerialNumber();

		if( pItemElem->m_nItemNum <= nItemCount )
			pMover->m_Bank[pMover->m_nSlot].RemoveAtId( pItemElem->m_dwObjId );
		else
			pItemElem->m_nItemNum -= nItemCount;

		return iSerialNumber;
	}
	else
	{
		WriteLog( "item not found - bank: %s, %s ", pMover->m_szName, pItemProp->szName );
		return 0;
	}
	return 0;
}

#ifdef __REMOVEITEM_POCKET
SERIALNUMBER CDbManager::RemoveItemPocket( ItemProp* pItemProp, CMover* pMover, int nAbilityOption, int nItemResist, int nResistAbilityOption, int nItemCount )
{
	SERIALNUMBER iSerialNumber = 0;
	for( int i=0; i<MAX_POCKET; i++ )
	{
		CPocket* pPocket = pMover->m_Pocket.GetPocket( i );
		if( pPocket )
		{
			CItemElem* pItemElem = pPocket->GetItem( pItemProp, nAbilityOption, nItemResist, nResistAbilityOption );
			if( pItemElem != NULL )
			{
				iSerialNumber	= pItemElem->GetSerialNumber();

				if( pItemElem->m_nItemNum <= nItemCount )
					pPocket->RemoveAtId( pItemElem->m_dwObjId);
				else
					pItemElem->m_nItemNum -= nItemCount;

				return iSerialNumber;
			}
		}
	}

	WriteLog( "item not found - Pocket: %s, %s ", pMover->m_szName, pItemProp->szName );
	return 0;
}
#endif // __REMOVEITEM_POCKET

BOOL CDbManager::IsAbnormalPlayerData( CMover* pMover )
{
	DWORD dwId;
	int nMax	= pMover->m_Inventory.m_dwItemMax;
	int nNumberofIndex	= pMover->m_Inventory.m_dwIndexNum;

	for( int i = 0; i < nMax; i++ )
	{
		dwId	= pMover->m_Inventory.m_apIndex[i];
		if( dwId != NULL_ID )
		{
			if( pMover->m_Inventory.m_apItem[dwId].m_dwObjIndex != i )
				return TRUE;
		}
	}

	DWORD dwObjIndex;
	for( int i = 0; i < nMax; i++ )
	{
		dwObjIndex	= pMover->m_Inventory.m_apItem[i].m_dwObjIndex;
		if( dwObjIndex != NULL_ID )
		{
			if( pMover->m_Inventory.m_apIndex[dwObjIndex] != i )
				return TRUE;
		}
		if( FALSE == pMover->m_Inventory.m_apItem[i].IsEmpty() )
		{
			ItemProp* pItemProp
				= prj.GetItemProp( pMover->m_Inventory.m_apItem[i].m_dwItemId );
			if( !pItemProp	/*It have not property*/
				|| pItemProp->dwItemKind3 == IK3_VIRTUAL/*It is monster's virtual weapon*/ )
				return TRUE;
		}
	}

	DWORD adwEquipment[MAX_HUMAN_PARTS];
	memset( adwEquipment, 0xff, sizeof(DWORD) * MAX_HUMAN_PARTS );
	for( int i = nNumberofIndex; i < nMax; i++ )
	{
		dwId	= pMover->m_Inventory.m_apIndex[i];
		if( dwId != NULL_ID )
			adwEquipment[i - nNumberofIndex]	= dwId;
	}
	for( int i = 0; i < MAX_HUMAN_PARTS; i++ )
	{
		if( adwEquipment[i] != pMover->m_aEquipInfo[i].dwId )
			return TRUE;
	}

	return FALSE;
}

BOOL CDbManager::NormalizePlayerData( CMover* pMover )
{
	int nMax	= pMover->m_Inventory.m_dwItemMax;
	int nNumberofIndex	= pMover->m_Inventory.m_dwIndexNum;
	DWORD dwId;
	for( int i = 0; i < nMax; i++ )
	{
		dwId	= pMover->m_Inventory.m_apIndex[i];
		if( dwId != NULL_ID )
		{
			if( pMover->m_Inventory.m_apItem[dwId].m_dwObjIndex != i )
			{

				DWORD dwIndex	= NULL_ID;
				for( int j = 0; j < nMax; j++ )
				{	
					if( pMover->m_Inventory.m_apItem[j].m_dwObjIndex == i )
					{
						dwIndex	= j;
						break;
					}
				}
				if( dwIndex != NULL_ID )
				{
					pMover->m_Inventory.m_apIndex[i]	= dwIndex;
				}
				else
				{
					if( i >= nNumberofIndex )
					{
						pMover->m_Inventory.m_apIndex[i]	= NULL_ID;
					}
					else
					{
						int j = NULL;
						for( ; j < nMax; j++ )
						{
							if( pMover->m_Inventory.m_apItem[j].m_dwObjIndex == NULL_ID )
							{
								pMover->m_Inventory.m_apItem[j].m_dwObjIndex	= i;
								pMover->m_Inventory.m_apIndex[i]	= j;
								break;
							}
						}
						if( j == nMax ) {
							ASSERT( 0 );
							return FALSE;
						}
					}
				}
			}
		}
	}

	DWORD dwObjIndex;
	for( int i = 0; i < nMax; i++ )
	{
		dwObjIndex	= pMover->m_Inventory.m_apItem[i].m_dwObjIndex;
		if( dwObjIndex != NULL_ID )
		{
			if( pMover->m_Inventory.m_apIndex[dwObjIndex] != i )
				pMover->m_Inventory.m_apItem[i].m_dwObjIndex	= NULL_ID;
		}
	}

	DWORD adwEquipment[MAX_HUMAN_PARTS];
	memset( adwEquipment, 0xff, sizeof(DWORD) * MAX_HUMAN_PARTS );
	for( int i = nNumberofIndex; i < nMax; i++ )
	{
		dwId	= pMover->m_Inventory.m_apIndex[i];
		if( dwId != NULL_ID )
			adwEquipment[i - nNumberofIndex]	= dwId;
	}
	for( int i = 0; i < MAX_HUMAN_PARTS; i++ )
	{
		if( pMover->m_aEquipInfo[i].dwId != adwEquipment[i] )
			pMover->m_aEquipInfo[i].dwId	= adwEquipment[i];			
	}
	return TRUE;
}

BOOL CDbManager::RemoveQuest( void )
{
	// load - removequest.txt
	CScript s;
	OutputDebugString( "call BOOL CDbManager::RemoveQuest" );
	if( FALSE == s.Load( "removequest.txt" ) )
		return FALSE;
	std::vector<QuestId>	vecRemoveQuestId;
	int	nQuestId	= s.GetNumber();
	while( s.tok != FINISHED )
	{
		vecRemoveQuestId.push_back( QuestId(nQuestId) );
		nQuestId	= s.GetNumber();
	}

	char lpOutputString[256];
	char aQuest[3072]	= { 0,};
	char aCompleteQuest[1024]	= { 0,};
	char aCheckedQuest[100]	= {0, };

	CQuery* pQuery	= new CQuery;
	if( FALSE == pQuery->Connect( 3, DSN_NAME_CHARACTER01, DB_ADMIN_ID_CHARACTER01, DB_ADMIN_PASS_CHARACTER01 ) )
	{
		sprintf( lpOutputString, "fail to connect: dsn=%s, id=%s, pwd=%s", DSN_NAME_CHARACTER01, DB_ADMIN_ID_CHARACTER01, DB_ADMIN_PASS_CHARACTER01 );
		OutputDebugString( lpOutputString );
		SAFE_DELETE( pQuery );
		return FALSE;
	}
	
	char* pSql	=  new char[4296];	// aQuest + aCompleteQuest + aCheckedQuest + 100


	sprintf( pSql, "SELECT m_idPlayer, m_lpQuestCntArray, m_aCompleteQuest, m_aCheckedQuest FROM CHARACTER_TBL WHERE serverindex = '%02d'",
		g_appInfo.dwSys );

	sprintf( pSql, "SELECT m_idPlayer, m_lpQuestCntArray, m_aCompleteQuest FROM CHARACTER_TBL WHERE serverindex = '%02d'",
		g_appInfo.dwSys );
	if( FALSE == pQuery->Exec( pSql ) )
	{
		sprintf( lpOutputString, "query error: %s", pSql );
		SAFE_DELETE_ARRAY( pSql );
		SAFE_DELETE( pQuery );
		return FALSE;
	}
	while( pQuery->Fetch() )
	{
		CMover	mover;
		mover.m_idPlayer	= pQuery->GetInt( "m_idPlayer" );
		
		if( GetQuest( &mover, pQuery, NULL ) == FALSE )
		{
			SAFE_DELETE_ARRAY( pSql );
			SAFE_DELETE( pQuery );
			return FALSE;
		}

		BOOL bRemove	= FALSE;
		for (const QuestId questId : vecRemoveQuestId) {
			if (mover.RemoveQuest(questId)) {
				bRemove = TRUE;
			}
		}
		memset( aQuest, 0, sizeof(aQuest) );
		memset( aCompleteQuest, 0, sizeof(aCompleteQuest) );
		memset( aCheckedQuest, 0, sizeof(aCheckedQuest) );
		if( bRemove )
		{
			SaveQuest( &mover, aQuest, aCompleteQuest, aCheckedQuest );
			FILEOUT( "query_removequest.txt", "UPDATE CHARACTER_TBL SET m_lpQuestCntArray = '%s', m_aCompleteQuest = '%s', m_aCheckedQuest = '%s' WHERE m_idPlayer = '%07d' and serverindex = '%02d'",
				aQuest, aCompleteQuest, aCheckedQuest, mover.m_idPlayer, g_appInfo.dwSys );
		}
	}
	OutputDebugString( "RemoveQuest complete" );
	SAFE_DELETE_ARRAY( pSql );
	SAFE_DELETE( pQuery );
	return TRUE;
}

