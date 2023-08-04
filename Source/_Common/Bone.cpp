#include "stdafx.h"
#include <stdio.h>
#include "model.h"
#include "Bone.h"




////////////////////////////////////////////////////////////////////////////////////
///////////////
///////////////
///////////////
///////////////
///////////////
///////////////
////////////////////////////////////////////////////////////////////////////////////
CBones :: CBones()
{
	m_nID = 0;
	memset( m_szName, 0, sizeof(m_szName) );
	m_nRHandIdx = m_nLHandIdx = m_nRArmIdx = m_nLArmIdx = 0;

	D3DXMatrixIdentity( &m_mLocalRH );
	D3DXMatrixIdentity( &m_mLocalLH );
	D3DXMatrixIdentity( &m_mLocalShield );
	D3DXMatrixIdentity( &m_mLocalKnuckle );

	memset( m_vEvent, 0, sizeof(m_vEvent) );
	memset( m_nEventParentIdx, 0, sizeof(m_nEventParentIdx) );
}

//
//
//
int		CBones :: LoadBone( LPCTSTR szFileName )
{
	CResFile resFp;
	const BOOL bRet = resFp.Open( MakePath( DIR_MODEL, szFileName ), "rb" );	
	if( bRet == FALSE )		
		return	FAIL;

	int nVer;
	resFp.Read( &nVer, 4, 1 );		// version
	if( nVer < VER_BONE )
	{
		Error( "%s�� ������ %d.  �ֽŹ����� %d", szFileName, nVer, VER_BONE );
		resFp.Close();
		return FAIL;
	}

	// ID�б⸦ ����.
	resFp.Read( &m_nID, 4, 1 );

	if( strlen( szFileName ) > sizeof(m_szName) )
	{
		Error( "%s : file name is too long", szFileName );
		resFp.Close();
		return FAIL;
	}

	// ���ϸ� ī��
	strcpy( m_szName, szFileName );

	int		nNumBone;
	resFp.Read( &nNumBone, 4, 1 );			// �� ���� ����
	const int m_nMaxBone = nNumBone;
	m_pBones = sqktd::dynamic_array<BONE>(nNumBone);			// �� ���� ��ŭ �Ҵ�
	memset( m_pBones.get(), 0, sizeof(BONE) * nNumBone);		// zero clear

	for( int i = 0; i < nNumBone; i ++ )
	{
		int nLen;
		resFp.Read( &nLen, 4, 1 );		// �� ���Ա���
		resFp.Read( m_pBones[i].m_szName,		nLen, 1 );		// bone node �̸� ����
		resFp.Read( &m_pBones[i].m_mTM,			sizeof(D3DXMATRIX), 1 );			// World(Object) TM
		resFp.Read( &m_pBones[i].m_mInverseTM,	sizeof(D3DXMATRIX), 1 );			// Inv NODE TM
		resFp.Read( &m_pBones[i].m_mLocalTM,		sizeof(D3DXMATRIX), 1 );			// LocalTM
		resFp.Read( &m_pBones[i].m_nParentIdx,	4, 1 );								// parent bone index
		if( strcmpi( m_pBones[i].m_szName + 6, "r hand") == 0 )	// �θ��̸��� R Hand��...
			m_nRHandIdx = i;
		if( strcmpi( m_pBones[i].m_szName + 6, "l hand") == 0 )	// �θ��̸��� L Hand��...
			m_nLHandIdx = i;
		if( strcmpi(m_pBones[i].m_szName + 6, "l forearm") == 0 )	// �θ��̸��� L ForeArm��...
			m_nLArmIdx = i;
		if( strcmpi(m_pBones[i].m_szName + 6, "r forearm") == 0 )	// �θ��̸��� R ForeArm��...
			m_nRArmIdx = i;
	}

	resFp.Read( &m_bSendVS, 4, 1 );
	
	// ������ ������� ��ġ�� ���
	resFp.Read( &m_mLocalRH, sizeof(D3DXMATRIX), 1 );
	resFp.Read( &m_mLocalShield, sizeof(D3DXMATRIX), 1 );
	resFp.Read( &m_mLocalKnuckle, sizeof(D3DXMATRIX), 1 );
	
	// m_vEvent�� �߰��� ����.
//	int		nParentIdx[4];
	if( nVer == 5 )
	{
		resFp.Read( m_vEvent, sizeof(D3DXVECTOR3) * 4, 1 );
		resFp.Read( m_nEventParentIdx, sizeof(int) * 4, 1 );
	} else
	if( nVer >= 6 )
	{
		resFp.Read( m_vEvent, sizeof(D3DXVECTOR3) * MAX_MDL_EVENT, 1 );
		resFp.Read( m_nEventParentIdx, sizeof(int) * MAX_MDL_EVENT, 1 );
	}

	if( nVer == 7 )
	{
		resFp.Read( &m_mLocalLH, sizeof(D3DXMATRIX), 1 );
	}

	resFp.Close();

	// �θ� �����͸� ����
	for( int i = 0; i < nNumBone; i ++ )
	{
		if( m_pBones[i].m_nParentIdx == -1 )			// �θ� ������ �θ������ʹ� ��
			m_pBones[i].m_pParent = NULL;
		else
			m_pBones[i].m_pParent = &m_pBones[ m_pBones[i].m_nParentIdx ];
	}

	return SUCCESS;
}







////////////////////////////////////////////////////////////////////////////////////
///////////////
///////////////
///////////////
///////////////
///////////////
///////////////
////////////////////////////////////////////////////////////////////////////////////

CBonesMng		g_BonesMng;

// When this file load request comes in, the file
// is read and loaded into memory.
// When loading, the duplicate returns a pointer to
// the already loaded bone.
const CBones * CBonesMng::LoadBone(LPCTSTR szFileName) {
	char sFile[MAX_PATH] = { 0, };
	strcpy(sFile, szFileName);
	strlwr(sFile);

	if (const auto i = m_mapBones.find(sFile); i != m_mapBones.end()) {
		return i->second.get();
	}

	CBones * pBones = new CBones;
	if (pBones->LoadBone(szFileName) == FAIL) {
		safe_delete(pBones);
		return nullptr;
	}

	m_mapBones.emplace(sFile, pBones);
	return pBones;
}

////////////////////////////////////////////////////////////////////////////////////
///////////////
///////////////
///////////////
///////////////
///////////////
///////////////
////////////////////////////////////////////////////////////////////////////////////
CMotionMng		g_MotionMng;

CMotion * CMotionMng::LoadMotion(LPCTSTR szFileName) {
	char sFile[MAX_PATH] = { 0, };
	std::strcpy(sFile, szFileName);
	strlwr(sFile);

	if (const auto i = m_mapMotions.find(sFile); i != m_mapMotions.end()) {
		return i->second.get();
	}

	CMotion * pMotion = new CMotion;
	if (pMotion->LoadMotion(szFileName) == FAIL) {
		safe_delete(pMotion);
		return nullptr;
	}

	m_mapMotions.emplace(sFile, pMotion);
	return pMotion;
}


////////////////////////////////////////////////////////////////////////////////////
///////////////
///////////////
///////////////
///////////////
///////////////
///////////////
////////////////////////////////////////////////////////////////////////////////////

CMotion :: CMotion()
{
	m_pBoneFrame = NULL;
	m_fPerSlerp = 0.5f;
	m_nMaxEvent = 0;
	m_pBoneInfo = NULL;
	memset(m_vEvent, 0, sizeof(m_vEvent));
	m_pAttr = NULL;
	m_pBoneInfo = NULL;
}

CMotion :: ~CMotion()
{
	SAFE_DELETE_ARRAY(m_pAttr);
	if (m_pBoneFrame) {
		for (int i = 0; i < m_nMaxBone; i++)
			m_pBoneFrame[i].m_pFrame = NULL;
	}
	SAFE_DELETE_ARRAY(m_pBoneFrame);
	SAFE_DELETE_ARRAY(m_pBoneInfo);
}

//
//
//
int		CMotion :: LoadMotion( LPCTSTR szFileName )
{
	int		nNumBone;
	int		nNumFrame;
//	int		nNumSize;
//	int		i, j;
	int		nVer;

	CResFile resFp;
	BOOL bRet = resFp.Open( MakePath( DIR_MODEL, szFileName ), "rb" );
	if( bRet == FALSE )	
	{
		LPCTSTR szStr = Error( "%s : ã�� �� ����", szFileName );
		//ADDERRORMSG( szStr );
		resFp.Close();
		return FAIL;
	}

	resFp.Read( &nVer, 4, 1 );		// version
	if( nVer != VER_MOTION )
	{
		Error( "%s�� ������ %d.  �ֽŹ����� %d", szFileName, nVer, VER_MOTION );
		resFp.Close();
		return FAIL;
	}

	// ID�б⸦ ����.
	resFp.Read( &m_nID, 4, 1 );

	if( strlen( szFileName ) > sizeof(m_szName) )
	{
		Error( "%s : file name is too long", szFileName );
		resFp.Close();
		return FAIL;
	}

	// ���ϸ� ī��
	strcpy( m_szName, szFileName );
	
	resFp.Read( &m_fPerSlerp, sizeof(float), 1 );		// 
	resFp.Seek( 32, SEEK_CUR );		// reserved

	resFp.Read( &nNumBone, 4, 1 );			// ���� ���� ����
	resFp.Read( &nNumFrame, 4, 1 );		// �ִϸ��̼� ������ ���� ����
	m_nMaxFrame = nNumFrame;
	m_nMaxBone = nNumBone;

	if( nNumFrame <= 0 )
	{
		Error( "CMotion::LoadMotion : %s read MaxFrame = %d", szFileName, nNumFrame );
		resFp.Close();
		return FAIL;
	}

	// Path: skip it as it is never used
	int nTemp;
	resFp.Read( &nTemp, 4, 1 );
	if (nTemp) {
		resFp.Seek(sizeof(D3DXVECTOR3) * nNumFrame, SEEK_CUR);
	}

	//
	// TM_Animation ����Ÿ ����.
	ReadTM( &resFp, nNumBone, nNumFrame );
	
	// ������ �Ӽ� ����.
	resFp.Read( m_pAttr, sizeof(MOTION_ATTR) * nNumFrame, 1 );

	resFp.Read( &m_nMaxEvent, 4, 1 );	// �̺�Ʈ ��ǥ ����
	if( m_nMaxEvent > 0 )
		resFp.Read( m_vEvent, sizeof(D3DXVECTOR3) * m_nMaxEvent, 1 );
	

	resFp.Close();

	return SUCCESS;
}

//
//  Motion�� core�κ��� ����. �ܺο��� �ܵ� ȣ�� ����.
//
void	CMotion :: ReadTM( CResFile *file, int nNumBone, int nNumFrame )
{

	m_nMaxBone = nNumBone;		// LoadMotion()���� �ҷ����ٸ� �̺κ��� �ʿ������ ReadTM�� ���� �Ҹ� ���� ������ �̰� �ʿ��ϴ�.
	m_nMaxFrame = nNumFrame;

	//--- ���뱸������ ����.
	m_pBoneInfo = new BONE[ nNumBone ];			// �� ���� ��ŭ �Ҵ�
	memset( m_pBoneInfo, 0, sizeof(BONE) * nNumBone );		// zero clear
	
	
	for(int i = 0; i < nNumBone; i ++ )
	{
		int nLen; file->Read( &nLen, 4, 1 );
		if( nLen > 32 )		
			Error("CMotion::ReadTM - %s bonename is too long", m_szName );

		file->Read( m_pBoneInfo[i].m_szName,	nLen, 1 );		// bone node �̸�
		file->Read( &m_pBoneInfo[i].m_mInverseTM,	sizeof(D3DXMATRIX), 1 );			// Inv NODE TM
		file->Read( &m_pBoneInfo[i].m_mLocalTM,		sizeof(D3DXMATRIX), 1 );			// LocalTM
		file->Read( &m_pBoneInfo[i].m_nParentIdx,	4, 1 );								// parent bone index
	}
	
	// �θ� �����͸� ����
	for( int i = 0; i < nNumBone; i ++ )
	{
		if( m_pBoneInfo[i].m_nParentIdx == -1 )			// �θ� ������ �θ������ʹ� ��
			m_pBoneInfo[i].m_pParent = NULL;
		else
			m_pBoneInfo[i].m_pParent = &m_pBoneInfo[ m_pBoneInfo[i].m_nParentIdx ];
	}
	
	int		nNumSize;
	file->Read( &nNumSize, 4, 1 );			// ������ ������ ���� - �޸� Ǯ ������
	//--- ��� ����.
	m_pMotion		= std::make_unique<TM_ANIMATION[]>(nNumSize);		// �޸� Ǯ
	m_pBoneFrame	= new BONE_FRAME[ nNumBone ];
	m_pAttr			= new MOTION_ATTR[ nNumFrame ];
	memset( m_pAttr, 0, sizeof(MOTION_ATTR) * nNumFrame );	// nNumSize���µ� nNumFrame�� �´°� ����.
	
	TM_ANIMATION	*p = m_pMotion.get();

	// ���� �� ��ŭ ����
	for( int i = 0; i < nNumBone; i ++ )
	{
		int nFrame; file->Read( &nFrame, 4, 1 );
		if( nFrame == 1 )		// 1�̸� ���� ���뿡 ������ ����
		{
			m_pBoneFrame[i].m_pFrame = p;
			file->Read( m_pBoneFrame[i].m_pFrame, sizeof(TM_ANIMATION) * nNumFrame, 1 );		// �ѹ濡 �о������.
			p += nNumFrame;
		} else {
			// ���� ���뿡 ������ ����
			file->Read( &(m_pBoneFrame[i].m_mLocalTM), sizeof(D3DXMATRIX), 1 );			// �������� ������ LocalTM�� �а�
			m_pBoneFrame[i].m_pFrame = NULL;
			// m_mLocalTM�� �־����Ƿ� �޸� Ǯ���� �����ʿ� ����.
		}
	}
	
	if (m_pMotion.get() + nNumSize != p) {
		Error("%s : frame size error", m_szName);
	}
}

//
// ������ �ִϸ��̼�
// 
void	CMotion :: AnimateBone( D3DXMATRIX *pmUpdateBone, const CMotion *pMotionOld, float fFrameCurrent, int nNextFrame, int nFrameOld, BOOL bMotionTrans, float fBlendWeight ) const
{

	D3DXQUATERNION		qSlerp;
	D3DXVECTOR3			vSlerp;

	D3DXMATRIX	m1, m2;

	if( pMotionOld == NULL )	// pMotionOld-> �Ҷ� ������ ������ �ȳ�����.
		pMotionOld = this;

	// ������ ���� Slerp ���.
	const int nCurrFrame = (int)fFrameCurrent;				// �Ҽ��� ������ �����θ�..
	const float fSlp = fFrameCurrent - (float)nCurrFrame;	// �Ҽ��� �κи� ������
	/////////////////////////////

	// �ִϸ��̼� ������ �ִ� ���
	int i = m_nMaxBone;

	const BONE_FRAME * pBoneFrame	  = m_pBoneFrame;
	const BONE_FRAME * pBoneFrameOld = pMotionOld->m_pBoneFrame;		// ���� �������� // ���������� ���� ����
	const BONE * pBone = m_pBoneInfo;		// �� ���� ���� ���� ������.
	// pmUpdateBone

	D3DXMATRIX * pMatBones = pmUpdateBone;			// ������Ʈ ��Ʈ���� ����Ʈ ���� ������

	// ���� ������ŭ ���� ����
	while( i-- )
	{
		const TM_ANIMATION * pFrameOld = NULL;				// ��������� ������
		if( pBoneFrameOld->m_pFrame )
			pFrameOld = &pBoneFrameOld->m_pFrame[nFrameOld];		// �ϴ��� 0���� �Ѵ�.  ��������� nCurrFrame�� ����ϰ� �־�� �Ѵ�.
		else
			pFrameOld = NULL;		// �ϴ��� 0���� �Ѵ�.  ��������� nCurrFrame�� ����ϰ� �־�� �Ѵ�.
		if( pBoneFrame->m_pFrame )		// �� ���뿡 �������� ������
		{
			const TM_ANIMATION * pFrame = &pBoneFrame->m_pFrame[nCurrFrame];		// ���� ������ // ���� ������ ������ ����
			const TM_ANIMATION * pNext  = &pBoneFrame->m_pFrame[nNextFrame];		// ���� ������ // ���� ������ ������ ����

			D3DXQuaternionSlerp( &qSlerp, &pFrame->m_qRot, &pNext->m_qRot, fSlp );	// ������ ���ʹϾ� ����
			D3DXVec3Lerp( &vSlerp, &pFrame->m_vPos, &pNext->m_vPos, fSlp );		// ������ Pos���� ����

			if( bMotionTrans )	// ���� ��ǰ� ���� �ؾ� �ϴ°�
			{
				float	fBlendSlp = fBlendWeight;
				if( pFrameOld )
				{
					D3DXQuaternionSlerp( &qSlerp, &pFrameOld->m_qRot, &qSlerp, fBlendSlp );	// ������ǰ� ���� �������� ����
					D3DXVec3Lerp( &vSlerp, &pFrameOld->m_vPos, &vSlerp, fBlendSlp );		// ������ Pos���� ����
				} else
				{
					D3DXQUATERNION	qLocal;
					D3DXVECTOR3		vLocal;
					const D3DXMATRIX		*pmLocalTM = &pBoneFrameOld->m_mLocalTM;
					D3DXQuaternionRotationMatrix( &qLocal, pmLocalTM );
					vLocal.x = pmLocalTM->_41;
					vLocal.y = pmLocalTM->_42;
					vLocal.z = pmLocalTM->_43;
					D3DXQuaternionSlerp( &qSlerp, &qLocal, &qSlerp, fBlendSlp );	// ������ǰ� ���� �������� ����
					D3DXVec3Lerp( &vSlerp, &vLocal, &vSlerp, fBlendSlp );		// ������ Pos���� ����
				}
			}

			// matAniTM���
			// �̵���� ����
			D3DXMatrixTranslation( &m1,	 vSlerp.x,  vSlerp.y,  vSlerp.z );
		
			// ���ʹϿ� ������ ȸ��Ű���� ȸ����ķ� ����
			D3DXMatrixRotationQuaternion( &m2, &qSlerp );
			m2 *= m1;		// �̵���� X ȸ����� = �ִ����
			
			// WorldTM = LocalTM * ParentTM;
			// ���� ��ȯ�� �����Ʈ������ ����� �־ Render()���� SetTransfrom��.
		} else
		// �� ���뿡 �������� ������.
		{
			if( bMotionTrans )	// ���� ��ǰ� ���� �ؾ� �ϴ°�
			{
				float	fBlendSlp = fBlendWeight;
				D3DXQUATERNION	qLocal;
				D3DXVECTOR3		vLocal;
				const D3DXMATRIX		*pmLocalTM = &pBoneFrame->m_mLocalTM;
				D3DXQuaternionRotationMatrix( &qLocal, pmLocalTM );
				vLocal.x = pmLocalTM->_41;
				vLocal.y = pmLocalTM->_42;
				vLocal.z = pmLocalTM->_43;	// ������������ LocalTM�� ���ʹϿ°� ���ͷ� ����.

				if( pFrameOld )
				{
					D3DXQuaternionSlerp( &qSlerp, &pFrameOld->m_qRot, &qLocal, fBlendSlp );	// ������ǰ� ���� �������� ����
					D3DXVec3Lerp( &vSlerp, &pFrameOld->m_vPos, &vLocal, fBlendSlp );		// ������ Pos���� ����
				} else
				{
					D3DXQUATERNION	qLocal2;
					D3DXVECTOR3		vLocal2;
					const D3DXMATRIX		*pmLocalTM2 = &pBoneFrameOld->m_mLocalTM;
					D3DXQuaternionRotationMatrix( &qLocal2, pmLocalTM2 );
					vLocal2.x = pmLocalTM2->_41;
					vLocal2.y = pmLocalTM2->_42;
					vLocal2.z = pmLocalTM2->_43;
					D3DXQuaternionSlerp( &qSlerp, &qLocal2, &qLocal, fBlendSlp );	// ������ǰ� ���� �������� ����
					D3DXVec3Lerp( &vSlerp, &vLocal2, &vLocal, fBlendSlp );		// ������ Pos���� ����
				}
				D3DXMatrixTranslation( &m1,	 vSlerp.x,  vSlerp.y,  vSlerp.z );
			
				// ���ʹϿ� ������ ȸ��Ű���� ȸ����ķ� ����
				D3DXMatrixRotationQuaternion( &m2, &qSlerp );
				m2 *= m1;			// �̵���� X ȸ����� = �ִ����
			} else
				m2 = pBoneFrame->m_mLocalTM;

		}
		
		if( pBone->m_pParent )
			m2 *= pMatBones[ pBone->m_nParentIdx ];		// �θ� ���ٸ� ��Ʈ�ε� �׵���İ� ���� �ʿ� ��� �����ߴ�.



		*pmUpdateBone = m2;		// �� ������ ���� ���� ��ȯ ��Ʈ����

		pBoneFrame ++;
		pBoneFrameOld ++;
		pBone ++;					// �� ++
		pmUpdateBone ++;			// �� Ʈ������ ��Ʈ���� ++
	}
}

