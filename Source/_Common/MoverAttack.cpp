#include "stdafx.h"
#include "defineSound.h"
#include "defineText.h"
#include "resdata.h"
#include "defineObj.h"
#include "Party.h"
#ifdef __CLIENT			
	#include "Dialogmsg.h"
#endif

#include "eveschool.h"
#include "Mover.h"
#include "defineskill.h"

#ifdef __WORLDSERVER
	#include "user.h"
	#include "dpcoreclient.h"
	#include "dpdatabaseclient.h"
	#include "DPSrvr.h"
	#include "AttackArbiter.h"

	#include "eveschool.h"
	#include "ItemUpgrade.h"

#include <numbers>
#endif	// __WORLDSERVER


const int MIN_HR = 20;


//////////////////////////////////////////////////////////////////////////
// ENUM
//////////////////////////////////////////////////////////////////////////

/*
		cell input[2];
		cell result;

		amx_ctof( input[0] ) = (float)GetStr();
		amx_ctof( input[1] ) = (float)GetLevel();

		Call_AMX( SCRIPT_GETDAMAGEBASE, &result, 2, input );		
		nDamage = (int)amx_ctof( result );
*/
enum 
{
	SCRIPT_GETATTACKRESULT,
	SCRIPT_GETCRITICALPROBABILITY,
	SCRIPT_GETDAMAGEBASE,
	SCRIPT_ISBLOCKING,
	SCRIPT_CALCDEFENSE,
};

//////////////////////////////////////////////////////////////////////////
// GLOBAL FUNCTION
//////////////////////////////////////////////////////////////////////////

// ���� ����ġ�� ���Ѵ�. n - ����ġ�� ���ϱ� ���� �߰� ��갪 
float GetAttackSpeedPlusValue(const int n) {
	static constexpr int MAX_ATTACK_SPEED_PLUSVALUE = 18;

	static constexpr std::array<float, MAX_ATTACK_SPEED_PLUSVALUE> fPlusValue = {
		0.08f, 0.16f, 0.24f, 0.32f,	0.40f,
		0.48f, 0.56f, 0.64f, 0.72f,	0.80f,
		0.88f, 0.96f, 1.04f, 1.12f,	1.20f,
		1.30f, 1.38f, 1.50f
	};

	const int nIndex = std::clamp(n / 10, 0, MAX_ATTACK_SPEED_PLUSVALUE - 1);
	return fPlusValue[nIndex];
}

// ���������� ���� ���ݷ� �������� ��´�.
float GetChargeMultiplier( int nLevel )
{
	//TRACE("GetChargeMultiplier(%d)\n", nLevel );
	ASSERT( nLevel <= 4 );

	static float factors[5] = { 1.0f, 1.2f, 1.5f, 1.8f, 2.2f };
	if( 0 <= nLevel && nLevel <= 4 )
		return factors[ nLevel ];

	return 1.0f;
}

// ��������( nLevel )�� ���� �ϵ���ݷ� �������� ��´�.
float GetWandATKMultiplier( int nLevel )
{
	switch( nLevel )  
	{
	case 0:	return 0.6f;	
	case 1:	return 0.8f;
	case 2: return 1.05f;
	case 3: return 1.1f;
	case 4: return 1.3f;
	}
	return 1.0f;
}

//////////////////////////////////////////////////////////////////////////
// CMover ATTACK RELATED MEMBER FUNCTIONS
//////////////////////////////////////////////////////////////////////////

// ���ݿ� ���ؼ� ���ư� �� �ִ°�?
BOOL CMover::CanFlyByAttack()
{
	if( IsFlyingNPC() || m_pActMover->IsFly() )
		return FALSE;

	BOOL bFly = FALSE;
	if( (m_pActMover->GetState() & OBJSTA_DMG_FLY_ALL) == 0 )	// �������ö��� ���ȿ� �߰��� ���ư��� �ʴ´�.
	{
		MoverProp* pProp = GetProp();
		if( pProp->dwClass != RANK_SUPER && pProp->dwClass != RANK_MATERIAL && pProp->dwClass != RANK_MIDBOSS )	// ����/�ڿ� ���ʹ� ���ư��� ����.
			bFly = TRUE;
	}
	return bFly;
}
// ������ ���Ѵ�.
float CMover::GetAttackSpeed()
{
	float fSpeed = 1.0f;
	float fItem = 1.0f;

	const ItemProp* pWeaponProp = GetActiveHandItemProp();
	if( pWeaponProp )
		fItem = pWeaponProp->fAttackSpeed;

	const JobProp * pProperty = prj.jobs.GetJobProp( GetJob() ); 
	ASSERT( pProperty );

	// A = int( ĳ������ ���� + ( ������ ���� * ( 4 * ���� + ( ���� / 8 ) ) ) - 3 )
	// ���ݼӵ� = ( ( 50 / 200 - A ) / 2 ) + ����ġ 
	int A = int( pProperty->AttackSpeed + ( fItem * ( 4.0f * GetDex() + GetLevel() / 8.0f ) ) - 3.0f );
//	TRACE( "A =%d\n", A );
	if( 187.5f <= A )
		A	= (int)( 187.5f );
	fSpeed = ( ( 50.0f / (200.f - A) ) / 2.0f ) + GetAttackSpeedPlusValue( A );

	float fDstParam = GetParam( DST_ATTACKSPEED, 0 ) / 1000.0f;
	fSpeed += fDstParam;
	
	if( 0 < GetParam( DST_ATTACKSPEED_RATE, 0 ) )
		fSpeed = fSpeed + ( fSpeed * GetParam( DST_ATTACKSPEED_RATE, 0 ) / 100 );
	
	if( fSpeed < 0.1f )
		fSpeed = 0.1f;
	if( fSpeed > 2.0f )
		fSpeed = 2.0f;
	//TRACE( "%f\n", fSpeed );

	return fSpeed;
}

float CMover::GetCastingAniSpeed( void )
{
	float fSpeed	= 1.0F + GetParam( DST_SPELL_RATE, 0 ) / 100.0F;
	if( fSpeed < 0.1f )
		fSpeed = 0.1f;
	if( fSpeed > 2.0f )
		fSpeed = 2.0f;
	return fSpeed;
}

int CMover::GetCastingTime( int nCastingTime )
{
	int nTime	= nCastingTime - nCastingTime * GetParam( DST_SPELL_RATE, 0 ) / 100;
#ifdef __JEFF_11
	if( nTime < 0 )
		nTime	= 0;
#endif	// __JEFF_11
	return nTime;
}

// �����ۿ����� ���� ��� 
// bRandom: TRUE  - random ������ ��� 
//          FALSE - ��հ����� defense�� ��� 
int CMover::GetDefenseByItem( BOOL bRandom )
{
	int nValue = m_nDefenseMax - m_nDefenseMin;
	if( bRandom )
		return m_nDefenseMin + ( nValue > 0 ? xRandom( nValue ) : 0 );
	else
		return m_nDefenseMin + ( nValue / 2 );
}

int	CMover::GetShowDefense( BOOL bRandom )
{
	ATTACK_INFO info;
	memset( &info, 0x00, sizeof(info) );
	info.dwAtkFlags = AF_GENERIC;

	return CalcDefense( &info, FALSE );   // FALSE - ��� �ִ�/�ּ��� ��հ��� ��ڴ�.      
}

// Hit rating
int CMover::GetHR()
{
	if( IsPlayer() )
		return GetDex();
	else
		return GetProp()->dwHR;
}

BOOL CMover::GetAttackResult( CMover* pDefender, DWORD dwOption )
{
	if( m_dwMode & ONEKILL_MODE )
		return TRUE;
#ifdef __EVENTLUA_SPAWN
	if( IsPlayer() && pDefender->IsNPC() && prj.m_EventLua.IsEventSpawnMonster( pDefender->GetIndex() ) )
		return TRUE;
#endif // __EVENTLUA_SPAWN

	int nHitRate = 0;
	int nHR = GetHR();

	if( IsNPC() && pDefender->IsPlayer() )
	{
		nHitRate = (int)( ((nHR*1.5f) / (float)(nHR + pDefender->GetParrying())) * 2.0f *
			(GetLevel()*0.5f / (float)(GetLevel() + pDefender->GetLevel() * 0.3f )) * 100.0f );
	}
	else if( IsPlayer() && pDefender->IsNPC() )	// Player VS Monster
	{
		nHitRate = (int)(  ((nHR*1.6f) / (float)(nHR + pDefender->GetParrying())) * 1.5f * 
				   (GetLevel()*1.2f / (float)(GetLevel() + pDefender->GetLevel())) * 100.0f );

	}
	else
	{
		nHitRate = (int)( ((nHR*1.6f) / (float)(nHR + pDefender->GetParrying())) * 1.2f * 
				   (GetLevel()*1.2f / (float)(GetLevel() + pDefender->GetLevel())) * 100.0f );
	}
	nHitRate += GetAdjHitRate();

	if( nHitRate > 96 )
		nHitRate = 96;
	else if( nHitRate < MIN_HR )
		nHitRate = MIN_HR;

	return ( (int)( xRandom( 100 ) ) < nHitRate );
}

// ������ �߰� ���ݷ¸� ���Ѵ�.
int CMover::GetPlusWeaponATK( DWORD dwWeaponType ) const
{
	int nATK = 0;
	switch( dwWeaponType )
	{
	case WT_MELEE_SWD:	
		nATK = GetParam( DST_SWD_DMG, 0 );		
		break;
	case WT_MELEE_AXE:	
		nATK = GetParam( DST_AXE_DMG, 0 );		
		break;
	case WT_MELEE_KNUCKLE:
		nATK = GetParam( DST_KNUCKLE_DMG, 0 );		
		break;
	case WT_MELEE_YOYO:				// ��� 
		nATK = GetParam( DST_YOY_DMG, 0 );		
		break;
	case WT_RANGE_BOW:				// ���� 
		nATK = GetParam( DST_BOW_DMG, 0 );		
		break;
	}
	if(IsDualWeapon() )
		nATK += GetParam( DST_ONEHANDMASTER_DMG, 0 );
	else
	{
		switch( dwWeaponType )
		{
		case WT_MELEE_KNUCKLE:
			nATK += GetParam( DST_KNUCKLEMASTER_DMG, 0 );
			break;
		case WT_MELEE_YOYO:				// ��� 
			nATK += GetParam( DST_YOYOMASTER_DMG, 0 );
			break;
		case WT_RANGE_BOW:				// ���� 
			nATK += GetParam( DST_BOWMASTER_DMG, 0 );
			break;
		case WT_MELEE_SWD:	
		case WT_MELEE_AXE:	
			nATK += GetParam( DST_TWOHANDMASTER_DMG, 0 );
			break;
		}
	}

	return nATK;
}



// ������ ���� factor�� ���Ѵ�.
const JobProp JobProp::NullObject = JobProp();

const JobProp & CMover::GetJobProp() const {
	if (!IsPlayer()) return JobProp::NullObject;

	const JobProp * pProperty = prj.jobs.GetJobProp(GetJob());
	return pProperty ? *pProperty : JobProp::NullObject;
}

// ������ ���ݷ¸� ���Ѵ�.
int CMover::GetWeaponATK( DWORD dwWeaponType ) const
{
	int nATK = 0;
	switch( dwWeaponType )
	{
	case WT_MELEE_SWD:
		nATK = (int)( float(( GetStr() - 12 ) * GetJobProp().MeleeSWD) + (float(GetLevel() * 1.1f)) );
		break;
	case WT_MELEE_AXE:
		nATK = (int)( float(( GetStr() - 12 ) * GetJobProp().MeleeAXE) + (float(GetLevel() * 1.2f)) );
		break;
	case WT_MELEE_STAFF:
		nATK = (int)( float(( GetStr() - 10 ) * GetJobProp().MeleeSTAFF) + (float(GetLevel() * 1.1f)) );
		break;
	case WT_MELEE_STICK:
		nATK = (int)( float(( GetStr() - 10 ) * GetJobProp().MeleeSTICK) + (float(GetLevel() * 1.3f)) );
		break;
	case WT_MELEE_KNUCKLE:
		nATK = (int)( float( (GetStr() - 10 ) * GetJobProp().MeleeKNUCKLE) + (float(GetLevel() * 1.2f)) );
		break;
	case WT_MAGIC_WAND:
		nATK = (int)( ( GetInt() - 10 ) * GetJobProp().MagicWAND + GetLevel() * 1.2f );
		break;
	case WT_MELEE_YOYO:
		nATK = (int)( float(( GetStr() - 12 ) * GetJobProp().MeleeYOYO) + (float(GetLevel() * 1.1f)) );
		break;
	case WT_RANGE_BOW:
		nATK = (int)( (((GetDex()-14)*4.0f + (GetLevel()*1.3f) + (GetStr()*0.2f)) * 0.7f) );
		break;
	}

	nATK += GetPlusWeaponATK( dwWeaponType );	// ������ �߰� ���ݷ¸� ���Ѵ�.
	return nATK;
}


// ���ݷ�Min,Max�� ���Ѵ�.
std::pair<int, int> CMover::GetHitMinMax(const ATTACK_INFO * const pInfo) const {
	const int nParts = pInfo ? pInfo->nParts : PARTS_RWEAPON;

	if (IsInvalidObj(this))	return { 0, 0 };
	
	int min, max;

	if( IsPlayer() )
	{
		const ItemProp * pItemProp = GetActiveHandItemProp(nParts);
		if (!pItemProp) return { 0, 0 };

		min = GetParam(DST_ABILITY_MIN, pItemProp->dwAbilityMin * 2);
		max = GetParam(DST_ABILITY_MAX, pItemProp->dwAbilityMax * 2);

		int nPlus = GetWeaponATK( pItemProp->dwWeaponType ) + GetParam( DST_CHR_DMG, 0 );
		min += nPlus;
		max += nPlus;

		
		if (const CItemElem * pWeapon = GetWeaponItem(nParts)) {
			if (pWeapon->GetProp()) {
				const float f = GetItemMultiplier(pWeapon);
				min = static_cast<int>(min * f);
				max = static_cast<int>(max * f);
			}

			const int nOption = pWeapon->GetAbilityOption();
			if (nOption > 0) {
				const int nValue = (int)(pow((float)(nOption), 1.5f));
				min += nValue;
				max += nValue;
			}
		}
	} 
	else
	{
		const MoverProp * const pMoverProp = GetProp();
		min = GetParam(DST_CHR_DMG, GetParam(DST_ABILITY_MIN, pMoverProp->dwAtkMin));
		max = GetParam(DST_CHR_DMG, GetParam(DST_ABILITY_MAX, pMoverProp->dwAtkMax));
		
		DWORD dwAtk = pMoverProp->dwAtk1;
		if( pInfo )
		{
			DWORD dwValue = pInfo->nParam >> 16;
			if( dwValue != 0 && dwValue != NULL_ID )
				dwAtk = dwValue;
		}
	
		if( dwAtk != NULL_ID )		// ����ִ� ���Ⱑ ������ �� ������ min,max ������ ���Ѵ�.
		{
			if (const ItemProp * pItemProp = prj.GetItemProp(dwAtk)) {
				min += pItemProp->dwAbilityMin;
				max += pItemProp->dwAbilityMax;
			}
		}
	}

	return { min, max };
}

// ������������ ��´�.    this�� �����
float CMover::GetDEFMultiplier( ATTACK_INFO* pInfo )
{
	float fFactor = 1.0f;
	if( IsNPC() )
	{
		 fFactor *= GetProp()->m_fDefence_Rate;
	}

	// �Ƹ� ���Ʈ����Ʈ�� �´¼��� ������ 50%�� ���.
	if( pInfo->GetSkill() == SI_BLD_DOUBLE_ARMORPENETRATE )	
		fFactor *= 0.5f;

//	fFactor		*= (  ( 100.0f + (float)GetParam( DST_ADJDEF_RATE, 0 ) ) / 100.0f ) ;		// ���� ��ȯ�� ���� ���� ���� ��� ������ ���Ѵ�.
	fFactor		*= (  1.0f + (float)GetParam( DST_ADJDEF_RATE, 0 ) / 100.0f ) ;		// ���� ��ȯ�� ���� ���� ���� ��� ������ ���Ѵ�.

	return fFactor;
}

// ������ ����Ѵ�.
int CMover::CalcDefense( ATTACK_INFO* pInfo, BOOL bRandom )
{
	int nDefense = CalcDefenseCore( pInfo->pAttacker, pInfo->dwAtkFlags, bRandom );
	nDefense	= (int)( nDefense * GetDEFMultiplier( pInfo ) );
#if defined(__WORLDSERVER)
	if( IsPlayer() )
		nDefense += prj.m_EventLua.GetDefensePower();
#endif
	return nDefense;
}

// ���� ������ ����Ѵ�. ( generic�� magicskill�� ���� )
int CMover::CalcDefenseNPC( CMover* pAttacker, DWORD dwAtkFlags )
{
	float	fDefense = 0.0f;
	
	MoverProp* pProp = GetProp();
	if( dwAtkFlags & AF_MAGIC ) 
	{
		fDefense = pProp->dwResisMgic / 7.0f + 1;
	}
	else
	{
		ASSERT( (dwAtkFlags & AF_MELEESKILL) == AF_MELEESKILL );
		fDefense = pProp->dwNaturalArmor / 7.0f + 1;
	}

	return (int)fDefense;
}

// �÷��̾� ������ ����Ѵ�. ( generic, magicskill�� �������� ���� )
int CMover::CalcDefensePlayer( CMover* pAttacker, DWORD dwAtkFlags )
{	
	int nDefense = 0;
	if( pAttacker->IsPlayer() )		// �÷��̾� ���� 
	{			
		if( dwAtkFlags & AF_MAGIC )  
		{
			nDefense = (int)( float((GetInt() * 9.04f) + (GetLevel() * 35.98f) ) );
		}
		else
		{
			int nDEF = GetDefenseByItem() + GetParam( DST_ADJDEF, 0 );
			nDefense = (int)( (float(nDEF * 2.3f ) + (float(GetLevel()+(GetSta()/2)+GetDex()) / 2.8f ) - 4 + GetLevel() * 2 ) );
		}
	}
	else							// ������
	{		
		nDefense =(int)(  ((GetDefenseByItem() / 4 + GetParam( DST_ADJDEF, 0 )) + 
			(float(GetLevel()+(GetSta()/2)+GetDex()) / 2.8f ) - 4 + GetLevel() * 2 ) );
	}
#ifdef __JEFF_11
	if( nDefense < 0 )
		nDefense	= 0;
#endif	// __JEFF_11
	return nDefense;
}

// CalcDefense���� ȣ��Ǵ� sub�Լ� 
int CMover::CalcDefenseCore( CMover* pAttacker, DWORD dwAtkFlags, BOOL bRandom )
{
	if( dwAtkFlags & AF_MAGICSKILL )		// ����ó��:�Ӽ� ������ų�� ������ ���Ѵ�.
		return GetResistMagic();

	BOOL bGeneric = (dwAtkFlags & AF_GENERIC);
	if( IsPlayer() && pAttacker && pAttacker->IsPlayer() )
		bGeneric = TRUE;

	if( bGeneric )		
	{
		float fFactor = GetJobProp().FactorDef;

		int nDefense = (int)( ((((GetLevel()*2) + (GetSta()/2)) / 2.8f ) - 4) + ((GetSta()-14) * fFactor) );
		nDefense += (GetDefenseByItem( bRandom ) / 4);	// �����ۿ� ���� ���潺   
		nDefense += GetParam( DST_ADJDEF, 0 );	// ���潺 ����ġ�� �ִٸ� �װ��� ����.
#ifdef __JEFF_11
		if( nDefense < 0 )
			nDefense	= 0;
#endif	// __JEFF_11
		return nDefense;
	}

	if( IsNPC() )
		return CalcDefenseNPC( pAttacker, dwAtkFlags );
	else
		return CalcDefensePlayer( pAttacker, dwAtkFlags );
}

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
int CMover::GetCriticalProb( void )
{
	int nProb;
	nProb = (GetDex() / 10);
	nProb	= (int)( nProb *  GetJobProp().Critical );
	nProb = GetParam( DST_CHR_CHANCECRITICAL, nProb );	// ũ��Ƽ�� Ȯ���� �����ִ� ��ų���� 
#ifdef __JEFF_11
	if( nProb < 0 )
		nProb	= 0;
#endif	// __JEFF_11
	
	if( IsPlayer() )
	{
		if( m_idparty && (m_dwFlag & MVRF_CRITICAL) )	// ��Ƽ && 1ȸ ũ��Ƽ�� �ߵ�?
		{
#ifdef __WORLDSERVER
			CParty* pParty = g_PartyMng.GetParty( m_idparty );
			if( pParty && pParty->IsMember( m_idPlayer ) )
				nProb += (pParty->m_nSizeofMember / 2);	
#else // __WORLDSERVER
			if( g_Party.IsMember( m_idPlayer ) )
				nProb += (g_Party.m_nSizeofMember / 2);	
#endif // __WORLDSERVER			
			m_dwFlag &= (~MVRF_CRITICAL);
		}

	}
	return nProb;
}

#ifdef __WORLDSERVER

int CMover::CalcLinkAttackDamage( int nDamage )
{
	int nAdd = 0;
	if( IsPlayer() && m_idparty && nDamage > 0 ) // ���� ��Ƽ�� ���� ����
	{
		CParty* pParty = g_PartyMng.GetParty( m_idparty );
		if( pParty && pParty->IsMember( m_idPlayer ) )
		{
			// ��ȸ�ش��̸� ��ũ�������̸� ���常 �������� �����ش�
			if( pParty->m_nKindTroup == 1 && pParty->m_nModeTime[PARTY_LINKATTACK_MODE] && pParty->IsLeader( m_idPlayer ) )
			{
				int nJoinMember = 0; // ������ �ܿ���
				CUser* pLeader = (CUser*)this;
				CUser* pMemberUser;
				
				float fDist;
				D3DXVECTOR3	vDist;
				if( pParty->m_nModeTime[PARTY_PARSKILL_MODE] )
				{
					nJoinMember = pParty->m_nSizeofMember;
				}
				else
				{
					for( int i = 0 ; i < pParty->m_nSizeofMember ; i++ )
					{
						pMemberUser = g_UserMng.GetUserByPlayerID( pParty->m_aMember[i].m_uPlayerId );
						if( IsValidObj( (CObj*)pMemberUser ) )
						{
							if( pLeader->m_idSetTarget == pMemberUser->m_idSetTarget
								&& ( pLeader->m_idSetTarget != NULL_ID && pMemberUser->m_idSetTarget != NULL_ID ) )
							{
								vDist = pMemberUser->GetPos() - GetPos();
								fDist = D3DXVec3LengthSq( &vDist );		// ������ �ݴ»���� ������� �Ÿ�.
								if( fDist < 255.0f * 255.0f )		// xx���ͺ��� ������
									nJoinMember++; // ������ �ܿ���										
							}
						}
					}
				}
				nAdd = ( nDamage * nJoinMember / 10 ); 
			}
		}
	}
	return nAdd;
}

// ũ��Ƽ�� �����ΰ�?
BOOL CMover::IsCriticalAttack( CMover* pDefender, DWORD dwAtkFlags )
{
	if( IsSkillAttack( dwAtkFlags ) )	// ��ų�̸� critical ���� 
		return FALSE;

	return ( (int)( xRandom(100) ) < GetCriticalProb() );		
}

float CMover::GetBlockFactor( CMover* pAttacker, ATTACK_INFO* pInfo )
{
	if( IsPlayer() )
	{
		int r = xRandom( 80 );
		if( r <= 5 )
			return 1.0f;	// Blocking ����
		if( r >= 75 )
			return 0.1f;	// 10���� ������.

		float fBlockA = GetLevel() / ( (GetLevel()+pAttacker->GetLevel())*15.0f );
		float fBlockB = (GetDex()+pAttacker->GetDex()+2) * ( (GetDex()-pAttacker->GetDex()) / 800.0f );
		if( fBlockB > 10.0f )
			fBlockB = 10.0f;
		float fAdd = fBlockA + fBlockB;
		if( fAdd < 0.0f )
			fAdd = 0.0f;
		
		if( pInfo->IsRangeAttack() )	// ���Ÿ���Ÿ�϶��� 
			fAdd += GetParam( DST_BLOCK_RANGE, 0 );		// ���Ÿ����� ����ġ�� ������ ��
		else
			fAdd += GetParam( DST_BLOCK_MELEE, 0 );		// ���Ÿ��� �ƴҶ� �ٰŸ� ��������ġ�� ������ ��
		
		int nBR = (int)( ( GetDex() / 8.0f ) * GetJobProp().Blocking + fAdd );
		if( nBR < 0 )
			nBR = 0; 

		if ( nBR > r )		// R�� 6 ~ 74������ ���̸� ->  BR > R�� ��� Blocking ����
			return 0.0f;
	}
	else
	{
		int r = xRandom( 100 );
	
		if( r <= 5 )			// R�� 0 ~ 5�� ���̸� Blocking ����
			return 1.0f;
		if( r >= 95 )			// R�� 95 ~ 100�� ���̸� Blocking ����
			return 0.1f;

		int	nBR = (int)( ( GetParrying() - GetLevel() ) * 0.5f );  // GetParrying == ER
		if( nBR < 0 )
			nBR = 0; 

		if ( nBR > r )		// R�� 6 ~ 94������ ���̸� ->  BR > R�� ��� Blocking ����
			return 0.2f;	// 5�� ������.
	}
	return 1.0f;
}

BOOL CMover::IsBlocking( CMover* pAttacker )
{
	int r = xRandom( 100 );
	
	if( r <= 5 )			// R�� 0 ~ 5�� ���̸� Blocking ����
		return FALSE;
	if( r >= 95 )			// R�� 95 ~ 100�� ���̸� Blocking ����
		return TRUE;

	//���� Br = (����ER - ����LV)*0.5
	//�÷��̾� BR = ((ĳ����Dex - 30) / (2 * Lv)) + (�� + ��)
	int nBR = 0;

	if( IsPlayer() )
	{
		// �� : ������ ��� ��ġ
		// �� : �����۰� ��ų �߰�ġ�� ��(����, �����ۿ� ���ŷ ���� ������ ��������.)
		CItemElem* pShield = GetEquipItem( PARTS_SHIELD );	
		if( pShield )
		{
			nBR = ( GetDex() - 30 ) / ( 2 * GetLevel() );
			ItemProp* pProp = pShield->GetProp();
			nBR += pProp->dwblockRating;

			//�ּ� 2���� �ִ�65 ������ ���� ������
			if( nBR < 2 )
				nBR = 2;
			else if( nBR > 65 )
				nBR = 65;
		}
		else
			nBR = 0;
	}
	else
	{
		nBR = (int)( ( GetParrying() - GetLevel() ) * 0.5f );  // GetParrying == ER
		if( nBR < 0 )
			nBR = 0; 
	}

	return ( nBR > r );		// R�� 6 ~ 94������ ���̸� ->  BR > R�� ��� Blocking ����
}

const struct DamageMultiplierReductionFromDeltaCls {
	static constexpr int MAX_OVER_ATK = 16;
	std::array<float, MAX_OVER_ATK> memoized;

	DamageMultiplierReductionFromDeltaCls() {
		memoized[0] = 1.0f; // oh no, we waste sizeof(float) of memory :(
		for (int nDelta = 1; nDelta != MAX_OVER_ATK; ++nDelta) {
			const double radian = (std::numbers::pi * nDelta) / static_cast<float>(MAX_OVER_ATK * 2);
			// Sadly, std::cos is not a constexpr function
			memoized[nDelta] = static_cast<float>(std::cos(radian));
		}
	}
	
	[[nodiscard]] float operator()(const int nDelta /* > 0 */) const {
		return memoized[std::min(nDelta, MAX_OVER_ATK - 1)];
	}

} DamageMultiplierReductionFromDelta;



// this -> attacker
// ���Ǹ���(SI_PSY_PSY_SPRITBOMB) 
// �ڽ��� MP�� 90% �̻� �ϰ�� ���ݷ��� 1.5��
// �ڽ��� MP�� 50% �̻��ϰ�� ���ݷ��� 1.2��
// �ڽ��� MP�� 50% �̸��� ��� �⺻ ������
float CMover::GetDamageMultiplier( ATTACK_INFO* pInfo )
{
	float factor = 1.0f;
	int nSkill = pInfo->GetSkill(); 

	ItemProp* pSkillProp = NULL;
	AddSkillProp* pAddSkillProp = NULL;
	DWORD	dwProbability	= 0;

	if( nSkill )
	{
		pSkillProp	= prj.GetSkillProp( nSkill );		// ��ų�� ������Ƽ����
		if( pSkillProp )
		{
			pAddSkillProp	= prj.GetAddSkillProp( pSkillProp->dwSubDefine, pInfo->GetSkillLevel() );
			if( pAddSkillProp )
			{
				dwProbability	= pAddSkillProp->nProbability;
				if( pInfo->pAttacker->IsPlayer() && pInfo->pDefender->IsPlayer() )
					dwProbability	= pAddSkillProp->nProbabilityPVP;
				if( pAddSkillProp->nSkillCount > 0 )	//  �߻�ü ������ �ִ°�
					factor	/= (float)pAddSkillProp->nSkillCount;	// �߻�ü ������ŭ �������� ������ ��.
			}
		}
	}

	// ���̷�Ʈ���̸� ������ 2��, ��ũ�Ϸ��������̸� �����Ѵ�.
	if( HasBuff( BUFF_SKILL, SI_ACR_SUP_DARKILLUSION ) )
	{
		switch( nSkill )
		{
		case SI_JST_YOYO_BACKSTAB:   //�ִ� 2��
		case SI_JST_YOYO_VATALSTAB:  //�ִ� 4��
		case SI_ACR_BOW_SILENTSHOT:
			{
				factor *= 1.4f;
				break;
			}
		}
	}

	switch( nSkill )
	{
	case SI_JST_YOYO_VATALSTAB:
		if( xRandom( 100 ) < dwProbability )
			factor *= 2.0f;
		break;		
	case SI_ACR_BOW_AIMEDSHOT:
		if( xRandom( 100 ) < dwProbability )
			factor = 4.0f; // Tom: factor 4 is way better, else the shot just suxx!
		break;		
	case SI_ACR_BOW_JUNKBOW:		// ���͸� ȭ���� Ȯ���� �������� �ش�.
		if( xRandom( 100 ) > dwProbability )
			factor = 0.0f;
		break;		
 	case SI_JST_YOYO_HITOFPENYA:
		factor	*= ( pAddSkillProp->nDestData1[0] / 100.0f );
		break;		
	}
	

	if( nSkill == SI_PSY_PSY_SPRITBOMB )
	{
		int nPercent = GetManaPointPercent();
		//if( nPercent >= 90 )
		//	factor *= 1.5f;
		//else if( nPercent >= 50 )
		//	factor *= 1.2f;

		// Tom Spirit bomb dmg calculation! linear loss of dmg with mana!
		factor *= ((float)nPercent / 100) * 2.0f;
	}

	if( IsNPC() )	// ����ĿAI�� �ִ�.
	{
		int nBerserkHP = GetProp()->m_nBerserkHP;
		if( nBerserkHP > 0 && GetHitPointPercent( 100 ) <= nBerserkHP )
			factor *= GetProp()->m_fBerserkDmgMul;
	}
	else
	{
		if( pInfo->pDefender->IsPlayer() )		// �÷��̾�� �÷��̾��� ������ ������ ���� 
			factor *= 0.60f;

		if( pInfo->nParts == PARTS_LWEAPON )	// �޼��� ���ݷ��� 75���η� 
			factor *= 0.75f;
	}

	if( pInfo->pAttacker->IsPlayer() )
		if( pInfo->pDefender->GetAdjParam( DST_CHRSTATE ) & CHS_DOUBLE )		// �������� ���� �ɷȴ�
			factor *= 2.0f;

	int nDelta = pInfo->pDefender->GetLevel() - pInfo->pAttacker->GetLevel(); 	

	// ����ó��: Ŭ����ũ�� ����� ������ ���Ƽ skip
	if( pInfo->pAttacker->IsNPC() )
	{
		DWORD dwClass = pInfo->pAttacker->GetProp()->dwClass;
		switch( dwClass )
		{
		case RANK_GUARD:
		case RANK_SUPER:
//		case RANK_MIDBOSS:		�߰��������� ������ ���Ƽ�� �ִ°� ���� �ʳ� �ʹ�.
			nDelta = 0;
			break;
		}
	}

	if (nDelta > 0) {
		if (pInfo->pAttacker->IsNPC() || pInfo->pDefender->IsNPC()) {
			factor *= DamageMultiplierReductionFromDelta(nDelta);
		}
	}

	return factor;
}





// �Ϲ�(AF_MAGICSKILL�� ������) �Ӽ��������� ����Ѵ�.
int CMover::CalcPropDamage( CMover* pDefender, DWORD dwAtkFlags )
{
	int nAdd = 0;
	SAI79::ePropType type = SAI79::NO_PROP;
	int nATK = 0;

	if( IsNPC() )	// �����ڰ� NPC�� ���	
	{			
		type = GetProp()->eElementType;
		nATK = GetProp()->wElementAtk; 
	}
	else		    // �����ڰ� �÷��̾��� ���
	{		
		CItemElem* pItemElem = GetWeaponItem();
		if( pItemElem && pItemElem->m_bItemResist != SAI79::NO_PROP )
		{
			type = (SAI79::ePropType)pItemElem->m_bItemResist;
			nATK = pItemElem->m_nResistAbilityOption;
		
			if( ::GetLanguage() == LANG_THA )
			{
				float fResult = ((float)nATK+3.5f)*(0.5f+((float)nATK*0.01f));
				int nResult;
				nResult = (int)( fResult*fResult );
				nATK    = nResult;
			}
			else
				nATK *= 4;

			nATK += GetAdjParam( DST_CHR_WEAEATKCHANGE );
		}
		else
		{
			const ItemProp* pProp = GetActiveHandItemProp();
			type = pProp->eItemType;
			nATK = pProp->wItemEatk; 
		}
	}

	if( type != SAI79::NO_PROP )	// �Ӽ����� ?
	{
		nAdd = (int)( nATK  * (1.0f - pDefender->GetResist(type)) );
	}
	return nAdd;
}


// AF_MAGICSKILL �Ӽ��������� ����Ѵ�.
int CMover::GetMagicSkillPower( ATTACK_INFO* pInfo )
{
	CMover* pDefender = pInfo->pDefender; 
	int nSkill = pInfo->GetSkill();
	int nATK = GetMeleeSkillPower( pInfo );
	nATK = GetParam( DST_ADDMAGIC, nATK );		// �߰������� ����ġ.

	ItemProp* pSkillProp = prj.GetSkillProp( nSkill );		// ��ų�� ������Ƽ����
	if( !pSkillProp )
		return nATK;

	float fRatio = 0.0f;
	switch( pSkillProp->dwSpellType )
	{
	case ST_FIRE:	
		fRatio	= GetParam( DST_MASTRY_FIRE, 0 ) / 100.0f;
		nATK	= (int)( nATK + (nATK * fRatio) );
		break;
	case ST_WATER:	
		fRatio	= GetParam( DST_MASTRY_WATER, 0 ) / 100.0f;
		nATK	= (int)( nATK + (nATK * fRatio) );
		break;
	case ST_ELECTRICITY:	
		fRatio	= GetParam( DST_MASTRY_ELECTRICITY, 0 ) / 100.0f;
		nATK	= (int)( nATK + (nATK * fRatio) );
		break;
	case ST_WIND:	
		fRatio	= GetParam( DST_MASTRY_WIND, 0 ) / 100.0f;
		nATK	= (int)( nATK + (nATK * fRatio) );
		break;
	case ST_EARTH:	
		fRatio	= GetParam( DST_MASTRY_EARTH, 0 ) / 100.0f;
		nATK	= (int)( nATK + (nATK * fRatio) );
		break;
	}

	return nATK;
}


float CMover::GetMagicSkillFactor(SAI79::ePropType skillType) const {
	SAI79::ePropType itemType;

	if (const CItemElem * pWeapon = GetWeaponItem(); pWeapon && pWeapon->m_bItemResist != SAI79::NO_PROP) {
		itemType = (SAI79::ePropType)pWeapon->m_bItemResist;
	} else if (const ItemProp * pItemProp = GetActiveHandItemProp()) {
		itemType = pItemProp->eItemType;
	} else {
		return 1.0f;
	}

	if constexpr (useless_params::ElementedWeaponStillBoostsUnelementedSkills) {
		if (skillType == SAI79::NO_PROP) {
			return 1.1f;
		}
	}

	if (skillType == itemType) {
		return 1.1f;
	}

	if constexpr (!useless_params::ElementedWeaponStillBoostsUnelementedSkills) {
		if (skillType == SAI79::NO_PROP) {
			return 1.0f;
		}
	}

	const auto skillElemWeakTo = SAI79::GetElementStrongAgainst(skillType);
	if (skillType == itemType) {
		return 0.9f;
	} else {
		return 1.0f;
	}
}

int CMover::PostCalcMagicSkill(int nATK, ATTACK_INFO * pInfo) const {
	const int nSkill = pInfo->GetSkill();
	const ItemProp * pSkillProp = prj.GetSkillProp(nSkill);
	if (!pSkillProp) return 0;

	CMover * pDefender = pInfo->pDefender;

	const SAI79::ePropType skillType = pSkillProp->eItemType;

	const int nDEF = pDefender->CalcDefense(pInfo);
	if (pInfo->dwAtkFlags & AF_MAGICSKILL) {
		// ����ó��:�Ӽ� ������ų�� ������ ���Ѵ�.
		nATK = nATK - nATK * pDefender->GetParam(DST_RESIST_MAGIC_RATE, 0) / 100;
	}
	const float a = (nATK - nDEF) * (1.0f - pDefender->GetResist(skillType));

	return	(int)((int)a * GetMagicSkillFactor(skillType));
}

// �������ݷ��� �����Ѵ�.
float CMover::GetATKMultiplier( CMover* pDefender, DWORD dwAtkFlags )
{
	float fMultiplier = 1.0f;

	int nParam = GetParam( DST_ATKPOWER_RATE, 0 );		// ���ݷ� ���� ����ġ
	FLOAT fRate = 1.0f + (nParam / 100.0f);
	fMultiplier *= fRate;

	int nDstDmg = 0;
	if( IsPlayer() )
	{
		if( IsSMMode( SM_ATTACK_UP1 ) || IsSMMode( SM_ATTACK_UP ) )
			fMultiplier *= 1.2f;
		if( pDefender->IsPlayer() )
			nDstDmg = GetParam( DST_PVP_DMG, 0 );
		else
			nDstDmg = GetParam( DST_MONSTER_DMG, 0 );
	}
	else
	{
		fMultiplier *= ( prj.m_fMonsterHitRate * GetProp()->m_fAttackPower_Rate );
	}

	if( 0 < nDstDmg )
		fMultiplier += (fMultiplier * nDstDmg / 100.0f);

	return fMultiplier;
}

// GetHitPower�Լ����� ����� ATK, DEF�� ���� Factor�� ���Ѵ�.
void CMover::GetDamagePropertyFactor( CMover* pDefender, int* pnATKFactor, int* pnDEFFactor, int nParts )
{
	*pnATKFactor = 10000;	// 14�� ���ʹ� n/10000 factor�� ����
	*pnDEFFactor = 10000;	// 14�� ���ʹ� n/10000 factor�� ����

	SAI79::ePropType atkType, defType; 
	int atkLevel, defLevel;
	atkType = defType = SAI79::NO_PROP;
	atkLevel = defLevel = 0;

	if( IsPlayer() )
	{
		CItemElem* pItemElem = GetWeaponItem( nParts );
		if( pItemElem && pItemElem->m_bItemResist != SAI79::NO_PROP )
		{
			atkType = (SAI79::ePropType)pItemElem->m_bItemResist;
			atkLevel = pItemElem->m_nResistAbilityOption + GetAdjParam( DST_CHR_WEAEATKCHANGE );
		}
		else
		{
			const ItemProp* pProp = GetActiveHandItemProp( nParts );
			atkType = pProp->eItemType;
		}	
	}
	else
	{
		atkType = GetProp()->eElementType;
		atkLevel = GetProp()->wElementAtk;
	}


	if( pDefender->IsPlayer() )
	{
		CItemElem* pItemElem = pDefender->GetEquipItem( PARTS_UPPER_BODY );	//��Ʈ�� �Ӽ��� ���Ѵ�.
		if( pItemElem )
		{
			defType = (SAI79::ePropType)pItemElem->m_bItemResist;
			defLevel = pItemElem->m_nResistAbilityOption;
		}
	}
	else
	{
		defType = pDefender->GetProp()->eElementType;
		defLevel = pDefender->GetProp()->wElementAtk;
	}

	if( atkType == SAI79::NO_PROP && defType == SAI79::NO_PROP )
		return;

	enum class Relationship { NoRelation, Same, IncreasedDamaged, LoweredDamage };

	const Relationship result =
		atkType == defType ? Relationship::Same
		: SAI79::GetElementWeakTo(atkType) == defType ? Relationship::IncreasedDamaged
		: SAI79::GetElementStrongAgainst(atkType) == defType ? Relationship::LoweredDamage
		: Relationship::NoRelation;

	int nFactor = 0, nLevel = 0;
	switch( result )
	{
		case Relationship::NoRelation:
		case Relationship::Same:
			{
				if( atkLevel > 0 && defLevel == 0 )
					nFactor += CItemUpgrade::GetInstance()->GetAttributeDamageFactor( atkLevel );
				else if( atkLevel == 0 && defLevel > 0 )
					nFactor -= CItemUpgrade::GetInstance()->GetAttributeDefenseFactor( defLevel - 3 );	// ����
				else if( atkLevel > 0 && defLevel > 0 )
					nLevel = atkLevel - defLevel;
				break;
			}

		case Relationship::LoweredDamage: // ����� �켺
				nLevel = ( atkLevel - 5 ) - defLevel;
				break;
		
		case Relationship::IncreasedDamaged: // ������ �켺
				nLevel = atkLevel - ( defLevel > 5 ? defLevel - 5 : 0 );
				if( nLevel > 0 )
					nFactor += CItemUpgrade::GetInstance()->GetAttributeAddAtkDmgFactor( nLevel ); // �Ӽ����� ����ġ
				break;
	}
	
	if( nLevel != 0 )
		nFactor += ( nLevel > 0 )	?	CItemUpgrade::GetInstance()->GetAttributeDamageFactor( nLevel )
									:	-( CItemUpgrade::GetInstance()->GetAttributeDefenseFactor( -nLevel ) );	// ����
	*pnATKFactor += nFactor;
	*pnDEFFactor += nFactor;
}

// �Ϲ� ���� �������� ���Ѵ�.
int CMover::GetHitPower( ATTACK_INFO* pInfo  )
{
	int nATKFactor, nATK;
	GetDamagePropertyFactor( pInfo->pDefender, &nATKFactor, &pInfo->nDEFFactor, pInfo->nParts );

	auto [nMin, nMax] = GetHitMinMax(pInfo);

	if( IsCriticalAttack( pInfo->pDefender, pInfo->dwAtkFlags ) )
	{
		pInfo->dwAtkFlags |= AF_CRITICAL;

		float fMin = 1.1f;
		float fMax = 1.4f;
		if( GetLevel() > pInfo->pDefender->GetLevel() )  
		{
			if( pInfo->pDefender->IsNPC() )
			{
				fMin = 1.2f;
				fMax = 2.0f;
			}
			if( IsNPC() )
			{
				fMin = 1.4f;
				fMax = 1.8f;
			}
		}


		float fCriticalBonus	= 1 + (float)GetParam( DST_CRITICAL_BONUS, 0  ) / 100.0F;
#ifdef __JEFF_11
		if( fCriticalBonus < 0.1F )
			fCriticalBonus	= 0.1F;
#endif	// __JEFF_11
		nMin	= (int)( nMin * fMin * fCriticalBonus );
		nMax	= (int)( nMax * fMax * fCriticalBonus );

		BOOL bFlyByAttack = xRandom(100) < 15;
		const ItemProp* pItemProp = GetActiveHandItemProp();			//�������̳� �ݻ���� ������ ���� 
		if( pItemProp && pItemProp->dwWeaponType == WT_MELEE_YOYO || (pInfo->dwAtkFlags & AF_FORCE) )
			bFlyByAttack = FALSE;
		if(pInfo->pDefender->IsPlayer() )
			bFlyByAttack = FALSE;

		if( pInfo->pDefender->CanFlyByAttack() && bFlyByAttack )
			pInfo->dwAtkFlags |= AF_FLYING;
	}		

	nATK = xRandom( nMin, nMax );						// ���ݷ¿� ��������  
	nATK = MulDiv( nATK, nATKFactor, 10000 );				// 14�� ���ʹ� n/10000 ������ ����
	if( pInfo->IsRangeAttack() )						// �÷��̾��� ���Ÿ� ������ ��� 
		nATK	= (int)( nATK * GetChargeMultiplier( pInfo->GetChargeLevel() ) );		// ���������� ���� ����ó�� 

//	TRACE("nMin:%d nMax:%d nATK:%d nATKFactor:%d\n", nMin, nMax, nATK, nATKFactor ); 
	return nATK;
}

int CMover::PostCalcGeneric( int nATK, ATTACK_INFO* pInfo )
{
	int nDEF = pInfo->pDefender->CalcDefense( pInfo );  
	nDEF = MulDiv( nDEF, pInfo->nDEFFactor, 10000 );	// 14������ n/10000���� ����

	int nDamage = nATK - nDEF;
	if( nDamage > 0 )
	{
		float fBlock = pInfo->pDefender->GetBlockFactor( this, pInfo );
		if( fBlock < 1.0f )
		{
			pInfo->dwAtkFlags |= AF_BLOCKING;
			nDamage	= (int)( nDamage * fBlock );
		}
	}
	else
	{
		nDamage = 0;
	}

	// NPC�� ��� �ּ� �������� ���� �Ѵ�.
	if( pInfo->pAttacker->IsNPC() && pInfo->pDefender->IsPlayer() )	
	{
		int nMin = (int)( std::max( 0.f, nATK * 0.1f ) );	// ���� ���ݷ��� 10% �������� ������ �� �� �ְ� �� �ش�.
		nDamage = std::max( nDamage, nMin );
	}

	if( nDamage == 0 )
	{
		pInfo->dwAtkFlags &= ~AF_CRITICAL;
		pInfo->dwAtkFlags &= ~AF_FLYING;
	}
	return nDamage;
}

int	CMover::GetMeleeSkillPower( ATTACK_INFO* pInfo )
{
	int nSkill = pInfo->GetSkill();
	int nSkillLv = pInfo->GetSkillLevel();
	//int(������AbilityMinMax * (�����AbilityMinMax + 10) / 6 + ������AbilityMinMax)
	const ItemProp* pItemProp = GetActiveHandItemProp();			// ����ִ� ������ ������Ƽ ����
	ItemProp* pSkillProp = prj.GetSkillProp( nSkill );		// ��ų�� ������Ƽ����
	if( !pItemProp || !pSkillProp )
	{
		Error( "CMover::GetMeleeSkillPower() - nSkill : %d, Name : %s, Item : %d",
			nSkill, pInfo->pAttacker ? pInfo->pAttacker->GetName() : "xxx", pItemProp ? pItemProp->dwID : 0 );
		return 1;
	}
	AddSkillProp* pAddSkillProp = prj.GetAddSkillProp( pSkillProp->dwSubDefine, nSkillLv );
	if( !pAddSkillProp )
		return 1;

	CItemElem *pWeapon = GetWeaponItem();		// �տ��� �������� ������.
	const auto [nMin, nMax] = GetItemATKPower( *pItemProp, pWeapon ); 

	int   nReferStat = 0, nReferStat1 = 0, nReferStat2 = 0;
	switch(pSkillProp->dwReferStat1)
	{
	case DST_STR:		nReferStat1 = GetStr();		break;
	case DST_DEX:		nReferStat1 = GetDex();		break;
	case DST_INT:		nReferStat1 = GetInt();		break;
	case DST_STA:		nReferStat1 = GetSta();		break;
	}
	switch(pSkillProp->dwReferStat2)
	{
	case DST_STR:		nReferStat2 = GetStr();		break;
	case DST_DEX:		nReferStat2 = GetDex();		break;
	case DST_INT:		nReferStat2 = GetInt();		break;
	case DST_STA:		nReferStat2 = GetSta();		break;
	}
	
	if( pSkillProp->dwReferTarget1 == RT_ATTACK )
		if( pSkillProp->dwReferValue1 != NULL_ID )
			nReferStat1 = (int)( ((pSkillProp->dwReferValue1/10.0f)*nReferStat1) + (pAddSkillProp->dwSkillLvl * FLOAT(nReferStat1/50.0f)) );

	if( pSkillProp->dwReferTarget2 == RT_ATTACK )
		if( pSkillProp->dwReferValue2 != NULL_ID )
			nReferStat2 = (int)( ((pSkillProp->dwReferValue2/10.0f)*nReferStat2) + (pAddSkillProp->dwSkillLvl * FLOAT(nReferStat2/50.0f)) );

	nReferStat = nReferStat1 + nReferStat2;
	float fPowerMin, fPowerMax;

	DWORD dwSkillAbilityMin	= pAddSkillProp->dwAbilityMin;
	DWORD dwSkillAbilityMax	= pAddSkillProp->dwAbilityMax;
	if( pInfo->pAttacker->IsPlayer() && pInfo->pDefender->IsPlayer() )
	{
		dwSkillAbilityMin	= pAddSkillProp->dwAbilityMinPVP;
		dwSkillAbilityMax	= pAddSkillProp->dwAbilityMaxPVP;
	}	

	if( pWeapon )
	{
		fPowerMin = (float)( ( ( nMin + ( (int)dwSkillAbilityMin + pWeapon->GetProp()->nAddSkillMin ) * 5 + nReferStat - 20 ) * ( 16 + nSkillLv ) / 13 ) );
		fPowerMax = (float)( ( ( nMax + ( (int)dwSkillAbilityMax + pWeapon->GetProp()->nAddSkillMax ) * 5 + nReferStat - 20 ) * ( 16 + nSkillLv) / 13 ) );
	}
	else
	{
		fPowerMin = (float)( ( ( nMin + (int)dwSkillAbilityMin * 5 + nReferStat - 20 ) * ( 16 + nSkillLv ) / 13 ) );
		fPowerMax = (float)( ( ( nMax + (int)dwSkillAbilityMax * 5 + nReferStat - 20 ) * ( 16 + nSkillLv ) / 13 ) );
	}

	int nWeaponDamage = GetPlusWeaponATK( pItemProp->dwWeaponType );

	fPowerMin = (float)( GetParam( DST_CHR_DMG, (int)( fPowerMin ) ) + nWeaponDamage );
	fPowerMax = (float)( GetParam( DST_CHR_DMG, (int)( fPowerMax ) ) + nWeaponDamage );

	float fMinMax = (fPowerMax - fPowerMin) + 1;
	if( fMinMax <= 0 )
		fMinMax = 1;
	float fPower = (float)( fPowerMin + xRandom( (DWORD)( fMinMax ) ) );

	return (int)fPower;
}

//
// �ϵ�(����)���� ���������.
//
int CMover::GetMagicHitPower(const int nChargeLevel) const {
	const ItemProp * pItemProp = GetActiveHandItemProp();			// ����ִ� ������ ������Ƽ ����
	const CItemElem * pWeapon = GetWeaponItem();		// �տ��� �������� ������.

	auto [nMin, nMax] = GetItemATKPower(*pItemProp, pWeapon);

	const int nATK = GetWeaponATK( WT_MAGIC_WAND );
	nMin += nATK;
	nMax += nATK;

	int nDamage = xRandom( nMin, nMax );
	nDamage += GetParam( DST_CHR_DMG, 0 );
	nDamage	= (int)( nDamage * GetWandATKMultiplier( nChargeLevel ) );
	return nDamage;
}





// ���潺, �丮, ũ��Ƽ���� �����Ѵ�.
// nATK : �������� ���ݷ�
// nDamage : ���� ������ = ���ݷ� - ����
int CMover::ApplyDPC( int nATK, ATTACK_INFO* pInfo )
{	
	int nDamage;
	if( pInfo->CanIgnoreDEF() )
		nDamage = nATK;
	else
	{
		int nDefense = CalcDefense( pInfo );
		nDamage = nATK - nDefense;
	}

	if( nDamage < 0 )	
		nDamage = 0;

	// ũ��Ƽ�� ó��.
	if( pInfo->pAttacker->IsCriticalAttack( this, pInfo->dwAtkFlags ) )		
	{
		pInfo->dwAtkFlags |= AF_CRITICAL;

		int nChargeLevel = pInfo->GetChargeLevel();		// �ϵ�������	
		if( (pInfo->pAttacker->m_pActMover->GetState() & OBJSTA_ATK4) || 
			nChargeLevel == MAX_CHARGE_LEVEL )	// 4���ϰ��ݿ� �߻��� ũ��Ƽ��.
		{
			nDamage = (int)(nDamage * 2.6f);
			if( CanFlyByAttack() && xRandom(100) < 50 )
				pInfo->dwAtkFlags |= AF_FLYING;
		}
		else
		{
			nDamage = (int)(nDamage * 2.3f);
			if( CanFlyByAttack() && xRandom(100) < 30 )
				pInfo->dwAtkFlags |= AF_FLYING;
		}
		float fCriticalBonus	= 1 + (float)GetParam( DST_CRITICAL_BONUS, 0  ) / 100.0F;
#ifdef __JEFF_11
		if( fCriticalBonus < 0.1F )
			fCriticalBonus	= 0.1F;
#endif	// __JEFF_11
		nDamage		= (int)( nDamage * fCriticalBonus );
	}
	return nDamage;
}


void CMover::ReSetDuelTime( CMover* pAttacker, CMover* pDefender )
{
	if( pDefender->m_nDuel == 1 && pAttacker->m_nDuel == 1 
		&& pDefender->m_idDuelOther == pAttacker->GetId() && pAttacker->m_idDuelOther == pDefender->GetId() )
	{
		pDefender->m_dwTickEndDuel = ::timeGetTime() + NEXT_TICK_ENDDUEL;
		pAttacker->m_dwTickEndDuel = ::timeGetTime() + NEXT_TICK_ENDDUEL;
	}
	else if( pDefender->m_nDuel == 2 && pAttacker->m_nDuel == 2 
		&& pDefender->m_idDuelParty == pAttacker->m_idparty && pAttacker->m_idDuelParty == pDefender->m_idparty )
	{
		CMover* pLeader;
		CParty* pParty	= g_PartyMng.GetParty( pDefender->m_idparty );
		
		if( pParty )
		{
			pLeader = pParty->GetLeader();
			if( IsValidObj( pLeader ) )
				pLeader->m_dwTickEndDuel = ::timeGetTime() + NEXT_TICK_ENDDUEL;
		}
		
		CParty* pParty2		= g_PartyMng.GetParty( pAttacker->m_idDuelParty );
		if( pParty2 )
		{
			pLeader = pParty2->GetLeader();
			if( IsValidObj( pLeader ) )
				pLeader->m_dwTickEndDuel = ::timeGetTime() + NEXT_TICK_ENDDUEL;
		}
	}
}

// �����ڰ� ����Ǹ� ����ġ ���޷��� ����
void CMover::ChangeExpRatio( CMover* pAttacker, CMover* pDefender )
{
	BOOL bDiffer = FALSE;
	if( pDefender->m_idAttacker != pAttacker->GetId() )	// �����ڰ� ����Ǹ�?
	{
		if( pAttacker->m_idparty )	// �� ģ���� ��Ƽ�� �ִ°�.
		{
			CMover *pLastAttacker = prj.GetMover( pDefender->m_idAttacker );
			if( IsValidObj( (CObj*)pLastAttacker ) && pLastAttacker->m_idparty != pAttacker->m_idparty )	// �ٸ���Ƽ�� ������ ���޷� �ٲ�.
				bDiffer = TRUE;
		} else
			bDiffer = TRUE;			// �� ���������� �ƴ����� ��Ƽ�� ������ ���޷� �ٲ�.
	}
	
	if( bDiffer )	// �ٸ� ���(��Ƽ�� �ٸ�)�� ������ ���޷��� ���� 
	{
		if( pDefender->GetProp()->dwClass != RANK_SUPER )		// �������ϰ�� �� ������ ���� ����.
			pDefender->m_fExpRatio = (float)pDefender->GetHitPoint() / pDefender->GetMaxHitPoint();
	}
}

// ��ų�̳� ������ ������ ���� ��� 
void CMover::OnAttacked( CMover* pAttacker, int nDamage, BOOL bTarget, int nReflect )
{
	CMover* pDefender = this;
	if( pDefender == pAttacker )
	{
//		ASSERT( FALSE );	// �ϴ� �� ��ȹ���� �ڽ��� �����ϴ� ���� ����. 
		return;
	}

	BOOL bAddEnemy = TRUE;

	HITTYPE type = pAttacker->GetHitType( pDefender, bTarget, nReflect );
	if( type == HITTYPE_PVP || type == HITTYPE_WAR || type == HITTYPE_SCHOOL
#ifdef __JEFF_11_4
		|| type == HITTYPE_ARENA
#endif	// __JEFF_11_4
		)
		bAddEnemy = FALSE;
	
	if( bAddEnemy )		// TODO_raiders: HITTYPE_PK�� HITTYPE_GENERIC�� ���� �����ؾ� ��  
	{
		pDefender->AddEnemy(pAttacker->GetId(), nDamage);
		pAttacker->AddEnemy(pDefender->GetId(), 0);
	}

	switch( type )
	{
	case HITTYPE_PK:
		{
			if( nReflect )
				break;
			
			if( pAttacker->IsChaotic() || pDefender->IsChaotic() )
				break;

#ifdef __JEFF_11_4
			CWorld* pWorld	= pDefender->GetWorld();
			if( pWorld && pWorld->IsArena() )
				break;
#endif	// __JEFF_11_4

			if( pDefender->IsPKPink() )
				pAttacker->SetPKPink( GetTickCount() + SEC(prj.m_PKSetting.nReadyAttack) );
			else
				pAttacker->SetPKPink( GetTickCount() + SEC(prj.m_PKSetting.nGeneralAttack) );
			g_UserMng.AddPKPink( pAttacker, 1 );
		}
		break;
	case HITTYPE_PVP:
		pAttacker->ReSetDuelTime( pAttacker, pDefender );
		break;
	}

	if( pAttacker->IsPlayer() && pDefender->IsNPC() )		
	{
		if( pDefender->m_idAttacker != NULL_ID )			// �������� �ѹ� ģ�ǰ�?
		{
			if( nDamage > 0 )
				ChangeExpRatio( pAttacker, pDefender );		 
		}
		else
		{
			((CUser *)pAttacker)->AddCorrReq( pDefender );	// �������������� �� ��ǥ�ѹ� ����.
		}
		pDefender->m_idAttacker = pAttacker->GetId();		// �ǰ��ڴ� �����ڸ� ��� 
	}
	pAttacker->m_idLastHitMover = pDefender->GetId();		// PK�� ������ �Ǹ�ó���� ���ؼ� ����ص� ( NPP���� ������� ���� )
}



// ���� �����϶��� ���� ����
// return HITTYPE_GENERIC�̸� �ȵ巯��
HITTYPE	CMover::GetHitType2( CMover* pMover, BOOL bTarget, BOOL bGood )
{
	if( !IsValidObj( (CObj*)this ) || !IsValidObj( (CObj*)pMover ) )
		return HITTYPE_GENERIC;
	if( IsDie() || pMover->IsDie() )					// ��������
		return HITTYPE_FAIL;
	if( m_nDead || pMover->m_nDead )					// ������ ��������
		return HITTYPE_GENERIC;

	if( IsPlayer() && pMover->IsPlayer() )
	{

		if( m_pActMover->IsFly() || pMover->m_pActMover->IsFly() )	// ���߿� ������ �Ұ� 
			return HITTYPE_FAIL;
		if( IsPVPTarget( pMover ) )
			return HITTYPE_PVP;
		if( IsWarTarget( pMover ) )
			return HITTYPE_WAR;
		if( IsSchoolTarget( pMover ) )
			return HITTYPE_SCHOOL;
		if( IsGuildCombatTarget( pMover ) )
			return HITTYPE_GUILDCOMBAT;
#ifdef __JEFF_11_4
		if( IsArenaTarget( pMover ) )
			return HITTYPE_ARENA;
#endif	// __JEFF_11_4

		if( g_eLocal.GetState( EVE_PK ) )
		{
			if( bGood )		// ���� �����̸� ��� ���� ����
				return HITTYPE_FAIL;
			else if( GetLevel() <= prj.m_PKSetting.nLimitLevel || pMover->GetLevel() <= prj.m_PKSetting.nLimitLevel )
				return HITTYPE_FAIL;
			return HITTYPE_PK;
		}
		else
		{
			return HITTYPE_FAIL;
		}
	}
	else
	{
		return HITTYPE_GENERIC;		// NPC���Դ� ���� ������ �ټ� ����.
	}
}

// this - ������, pMover - �����  
HITTYPE	CMover::GetHitType( CMover* pMover, BOOL bTarget, int nReflect )
{
	if( !IsValidObj( (CObj*)this ) || !IsValidObj( (CObj*)pMover ) )
		return HITTYPE_FAIL;
	if( IsDie() || pMover->IsDie() )					// ��������
		return HITTYPE_FAIL;
	if( m_nDead || pMover->m_nDead )					// ������ ��������
		return HITTYPE_FAIL;

	// �ڱ� �ڽ��� �����ϴ� ��찡 �ƴ�
	// ����� ���¿��� ��α��� �ϸ� ������ ��� �ڽ��� �����ڷ� ������.
	if( this == pMover )	
		return HITTYPE_GENERIC;

	if( IsPlayer() && pMover->IsPlayer() )
	{
		if( IsGuildCombatTarget( pMover ) )
			return HITTYPE_GUILDCOMBAT;


		if( m_pActMover->IsFly() || pMover->m_pActMover->IsFly() )	// ���߿� ������ �Ұ� 
			return HITTYPE_FAIL;
		if( IsPVPTarget( pMover ) )
			return HITTYPE_PVP;
		if( IsWarTarget( pMover ) )
			return HITTYPE_WAR;
		if( IsSchoolTarget( pMover ) )
			return HITTYPE_SCHOOL;

#ifdef __JEFF_11_4
		if( IsArenaTarget( pMover ) )
			return HITTYPE_ARENA;
#endif	// __JEFF_11_4

		DWORD dwAttr	= GetPKPVPRegionAttr();		
		if( dwAttr == RA_SAFETY || dwAttr != pMover->GetPKPVPRegionAttr() )
			return HITTYPE_FAIL;


		if( IsBaseJob() || pMover->IsBaseJob() )
			return HITTYPE_FAIL;
		if( g_eLocal.GetState( EVE_GUILDWAR ) )			// ����� ���¿����� PK�Ұ� 
		{
			if (m_idWar != WarIdNone || pMover->m_idWar != WarIdNone) {
				return HITTYPE_FAIL;
			}
		}
		
		if( g_eLocal.GetState( EVE_PK ) )
		{
			if( GetLevel() <= prj.m_PKSetting.nLimitLevel || pMover->GetLevel() <= prj.m_PKSetting.nLimitLevel )
				return HITTYPE_FAIL;
			return HITTYPE_PK;
		}
		else
		{
			return HITTYPE_FAIL;
		}
	}
	else
	{
		if( IsNPC() && GetProp()->dwClass == RANK_GUARD )			// ����ó��: ����� ���߿� �ִ� �༮�� ���ݰ��� 
			return HITTYPE_GENERIC;
	
		if( pMover->IsNPC() && IsAttackAbleNPC( pMover ) == FALSE )	// bKillable, IsPeaceful, RANK_GUARD�� �˻� 
			return HITTYPE_FAIL;

		//TODO: IsFly()�� ���� 
		const bool bAttackerFly = ( IsNPC() && IsFlyingNPC() ) || ( IsPlayer() && IsFly() );
		const bool bDefenderFly = ( pMover->IsNPC() && pMover->IsFlyingNPC() ) || ( pMover->IsPlayer() && pMover->IsFly() );

		if( bAttackerFly != bDefenderFly )
			return HITTYPE_FAIL;

		return HITTYPE_GENERIC;
	}
}


#endif // __WORLDSERVER

BOOL CMover::IsPVPTarget( CMover* pMover )
{	
	BOOL bResult = FALSE;
	if( m_idDuelOther != NULL_ID && m_idDuelOther == pMover->GetId() )
		bResult = TRUE;

	if( 0 < m_idDuelParty && m_idDuelParty == pMover->m_idparty )
		bResult = TRUE;

	if( bResult && ( m_nDuelState == 1 || pMover->m_nDuelState == 1 ) )
		return TRUE;

	return FALSE;
}

#ifdef __JEFF_11_4
BOOL	CMover::IsArenaTarget( CMover* pMover )
{
	CWorld* pWorld	= GetWorld();
	if( pWorld && pWorld->IsArena() )
	{
		int dx	= (int)( pMover->GetPos().x - 490 );
		int dy	= (int)( pMover->GetPos().z - 507 );
		int d	= dx * dx + dy * dy;
		if( d > 1246 )
			return FALSE;
		dx	= (int)( GetPos().x - 490 );
		dy	= (int)( GetPos().z - 507 );
		d	= dx * dx + dy * dy;
		if( d > 1246 )
			return FALSE;
		return TRUE;
	}
	return FALSE;
}
#endif	// __JEFF_11_4

BOOL CMover::IsWarTarget( CMover* pMover )
{
	if( g_eLocal.GetState( EVE_GUILDWAR ) == 1 )
	{
		if( m_idWar != WarIdNone && m_idWar == pMover->m_idWar && m_idGuild != pMover->m_idGuild )
			return TRUE;
	}
	return FALSE;
}

BOOL CMover::IsSchoolTarget( CMover* pMover )
{
	return( g_eLocal.GetState( EVE_SCHOOL_BATTLE ) == SBS_START2 );		// '/sbstart2'
}

BOOL CMover::IsGuildCombatTarget( CMover* pMover )
{
#ifdef __WORLDSERVER
	if( g_GuildCombatMng.m_nGCState != CGuildCombat::WAR_WAR_STATE
		&& g_GuildCombat1to1Mng.m_nState != g_GuildCombat1to1Mng.GC1TO1_WAR
		)
		return FALSE;
#endif // __WORLDSERVER
	
	if( m_nGuildCombatState != 1 || pMover->m_nGuildCombatState != 1 )
		return FALSE;

	if( m_idGuild == pMover->m_idGuild )
		return FALSE;

	return TRUE;
}

// �´� ����� ���Ѵ�.
void CMover::SetDamagedMotion( CMover* pAttacker, DWORD dwAtkFlags )
{
	if((m_pActMover->GetState() & OBJSTA_DMG_FLY_ALL))			// �������ö����� �ƴҶ��� �Ʒ� ó��.
		return;

	m_pActMover->ResetState( OBJSTA_JUMP_ALL );
	if( dwAtkFlags & AF_FLYING )									// �°� ���ư��� �ϴ°�?
	{
		if( (m_pActMover->GetState() != OBJSTA_STUN) )
		{			
			m_pActMover->SetState( OBJSTA_DMG_ALL, OBJSTA_DMG_FLY );
			SetMotion( MTI_DMGFLY, ANILOOP_CONT );
			if( pAttacker )
			{ 
				pAttacker->SendActMsg( OBJMSG_STOP );					// �ö��״ٿ��� �Ͼ���� ����Ŀ�� �ϴ� ������.
				m_pActMover->DoDamageFly( pAttacker->GetAngle(), 145.0f, 0.18f );	// ����Ŀ�� ���������� ��������.
			}
		}
	} 
	else
		m_pActMover->SetState( OBJSTA_DMG_ALL, OBJSTA_DMG );
	
	DWORD	dwTmpTick = GetTickCount();
	int	nTmpSkillID = m_pActMover->GetCastingSKillID();
	if( m_pActMover->GetCastingEndTick() > dwTmpTick && ( nTmpSkillID == SI_KNT_HERO_DRAWING || nTmpSkillID == SI_RIG_HERO_RETURN ) )
	{
		m_pActMover->SetCastingEndTick(0);
		m_pActMover->ClearState();				// ���� Ŭ�����ϰ� �ٽ� ����.
	}

	if( m_pActMover->IsActJump() || m_pActMover->IsActAttack() )	// ������ or ������ �� ������ ��� �ȳ�.
		return;
	if( m_pActMover->IsAction() )		// �Ϲݾ׼� ���̸� ������ ��� �ȳ�.
		return;

	m_dwMotion = -1;
	if( m_pActMover->IsFly() )
	{
		ItemProp* pItemProp = prj.GetItemProp( m_dwRideItemIdx );
		if( pItemProp )
			SetMotion( pItemProp->dwUseMotion + MTA_FDMG1, ANILOOP_1PLAY, MOP_FIXED | MOP_NO_TRANS );
	}
	else
	{
		SetMotion( MTI_DMG1 + random( 2 ), ANILOOP_1PLAY );
	}
}


// �������� ȿ�� ���ϱ� ���͸� ���Ѵ�.
float CMover::GetItemMultiplier(const CItemElem * pItemElem) {
	if (pItemElem->IsFlag(CItemElem::expired)) return 0.0f;

	const ItemProp * const pItemProp = pItemElem->GetProp();
	ASSERT(pItemProp);

	// ˬ - 2007-08-27
	int nOption = pItemElem->GetAbilityOption();
	if (pItemProp->dwReferStat1 == WEAPON_ULTIMATE) {
		nOption = 10;
	}

	float fValue = 1.0f;
	if (nOption != 0) {
		int nValue = 100 + prj.GetExpUpItem(pItemProp->dwItemKind3, nOption);
		fValue = fValue * nValue / 100.0f;		// ���ü�ġ�� ��������.
	}

	return fValue;
}

std::pair<int, int> CMover::GetItemATKPower(const ItemProp & pItemProp, const CItemElem * pWeapon) const {
	const int nOption = pWeapon ? pWeapon->GetAbilityOption() : 0;	// ���ü�ġ 
	const int nAdd = nOption > 0 ? (int)(pow((float)(nOption), 1.5f)) : 0;
	const float f = pWeapon ? GetItemMultiplier(pWeapon) : 1.0f;

	return std::pair<int, int>(
		static_cast<int>(GetItemAbilityMin(pItemProp.dwID) * f) + nAdd,
		static_cast<int>(GetItemAbilityMax(pItemProp.dwID) * f) + nAdd
		);
}


int CMover::GetReqMp( int nReqMp )
{
	if( nReqMp == 9999999 )
		return nReqMp;

	return nReqMp - nReqMp * GetParam( DST_MP_DEC_RATE, 0 ) / 100;
}

int CMover::GetReqFp( int nReqFp )
{
	if( nReqFp == 9999999 )
		return nReqFp;

	return nReqFp - nReqFp * GetParam( DST_FP_DEC_RATE, 0 ) / 100;
}

float CMover::GetResist(SAI79::ePropType type) {
	if (SAI79::NO_PROP == type)
		return 0.0f;

	const int n = SAI79::GetResistDST(type);
	return GetResistSpell(n) / 100.0f;
}

void CMover::OnAttackSFX( OBJID	idTarget, int nMagicPower, DWORD dwSkill, int nDmgCnt, float	fDmgAngle, float fDmgPower,  DWORD dwAtkFlags, BOOL bControl )
{
#ifdef __WORLDSERVER
//	if( dwSkill == SI_MAG_FIRE_HOTAIR )		// ˬ	// 06-10-23
//		return;

	SFXHIT_INFO si	= 
		{ idTarget, nMagicPower, dwSkill, nDmgCnt, fDmgAngle, fDmgPower, dwAtkFlags, bControl };
	AddSFXInfo( idTarget, si );
#endif
}

#ifdef __WORLDSERVER
int CMover::GetSFXCount( OBJID idTarget )
{ 
	const auto it = m_mapSFXCount.find( idTarget );
	if( it != m_mapSFXCount.end() )
		return it->second.size();
	return 0;
}

void CMover::IncSFXCount( OBJID idTarget, DWORD dwSkill )
{
	auto it = m_mapSFXCount.find( idTarget );
	if( it != m_mapSFXCount.end() )
	{
		std::queue<SFXHIT_COUNT> &q	= it->second;
		SFXHIT_COUNT c	= {	dwSkill, GetTickCount()	};
		q.push( c );
	}
	else
	{
		std::queue<SFXHIT_COUNT> q;
		SFXHIT_COUNT c	= {	dwSkill, GetTickCount()	};
		q.push( c );
		m_mapSFXCount.emplace( idTarget, q );
	}
}

void CMover::AddSFXInfo( OBJID idTarget, SFXHIT_INFO& info )
{
	info.dwTickCount	= GetTickCount();

	auto it = m_mapSFXInfo.find( idTarget );
	if( it != m_mapSFXInfo.end() )
	{		
		std::queue<SFXHIT_INFO> &q = it->second;
		q.push( info );
	}
	else
	{
		std::queue<SFXHIT_INFO> q;
		q.push( info );
		m_mapSFXInfo.emplace( idTarget, q );
	}
}

void CMover::ClearSFX( OBJID idTarget )
{
	m_mapSFXInfo.erase( idTarget );
	m_mapSFXCount.erase( idTarget );
}

void CMover::RemoveSFX( OBJID idTarget, DWORD dwSkill )
{
	if( idTarget != NULL_ID )
	{
		CMover* pTarget	= prj.GetMover( idTarget );
		if( IsValidObj( pTarget	) )
			IncSFXCount( idTarget, dwSkill ); 
		else
			ClearSFX( idTarget );
	}
}


BOOL AttackBySFX( CMover* pAttacker, SFXHIT_INFO & info )
{
	CMover* pTarget	= prj.GetMover( info.idTarget );
	if( IsInvalidObj( pTarget ) )
		return FALSE;

	int	nParam = pTarget->m_pActMover->MakeParamDamage( info.nMagicPower, (int)info.dwSkill, info.nDmgCnt );
	if( pTarget->IsLive() == FALSE )
		return FALSE;

	//���Ͱ� ���Ÿ� �߻�ü�� ��쵵 ���ݼ������� �����Ͽ� ����Ѵ�.
	if( info.dwAtkFlags & AF_GENERIC )
	{						
		if( pAttacker->GetAttackResult( pTarget, 0 ) == FALSE )
			info.dwAtkFlags = AF_MISS;
	}

	if( !g_eLocal.GetState( EVE_STEAL ) && pAttacker->IsSteal( pTarget ) )
			return TRUE;

	if( IsSkillAttack( info.dwAtkFlags ) )
	{
		// ��ų ȿ���� ����.(���ο� �������� ���ԵǾ� �ִ�)
		LPSKILL pSkill = pAttacker->GetSkill( info.dwSkill );		// �������� ��ų������ �˱� ����...
		if( pSkill )
		{
			ItemProp *pSkillProp;
			AddSkillProp *pAddSkillProp;
		#ifdef __SKILL0517
			BOOL bSuccess	= pAttacker->GetSkillProp( &pSkillProp, &pAddSkillProp, info.dwSkill, pAttacker->GetSkillLevel( pSkill ), "OnSfxHit" );
		#else	// __SKILL0517
			BOOL bSuccess	= pAttacker->GetSkillProp( &pSkillProp, &pAddSkillProp, info.dwSkill, pSkill->dwLevel, "OnSfxHit" );
		#endif	// __SKILL0517
			if( bSuccess )
				pAttacker->DoApplySkill( pTarget, pSkillProp, pAddSkillProp, false, nParam, FALSE, info.bControl );
		} 
		else
			Error( "OnSfxHit : skill=%d�� ������ ����", info.dwSkill );
	} 
	else
		pTarget->m_pActMover->SendDamage( info.dwAtkFlags, pAttacker->GetId(), nParam );

	if( info.fDmgPower )	// �и��� �Ӽ��� �ִ°Ÿ�.
		pTarget->m_pActMover->m_vDeltaE = AngleToVectorXZ( info.fDmgAngle, info.fDmgPower );

	if( pTarget->IsLive() == FALSE )	// �� ������ ����� ����� ��� �Ͽ���.
		return FALSE;

	return TRUE;
}
#endif
