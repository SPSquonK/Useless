#ifndef __VERSION_COMMON_H__
#define __VERSION_COMMON_H__
#define __MAINSERVER

#if !defined( __TESTSERVER ) && !defined( __MAINSERVER )
	#define __INTERNALSERVER
#endif

#define		__SERVER				// Ŭ���̾�Ʈ �����ڵ带 �������� �ʱ� ���� define
#define		__MAP_SIZE						
#define		__S8_SERVER_PORT		// 2006�� 11�� 7�� ������Ʈ - World, Cache, CoreServer
#define		__EVE_NEWYEAR			// �ų� ȿ��

//
//#define		__HACK_0516 // �̱� ��ŷ 2��
#define		__JEFF_9_20		// ���� �ð��� ħ�� �ð����� ����

//
#define		__MA_VER11_01			// �شܷ���������Ʈ����
#define		__AUTO_NOTICE		// �ڵ� ����

// 13��
#define		__LAYER_1015		// ���� ��ü ��

// 14��
#define		__QUIZ						// ���� �̺�Ʈ �ý���

#define		__EVENTLUA_RAIN				// �帶 �̺�Ʈ -> ��Ʒ� ����
#define		__EVENTLUA_SNOW				// ���� �̺�Ʈ


#if	  defined(__INTERNALSERVER)	// ���� �繫�� �׽�Ʈ���� 
	#define		__GUILDVOTE			// ��� ��ǥ 
	#define		__HACK_0516			// �̱� ��ŷ 2��


#elif defined(__TESTSERVER)		// �ܺ� ���� �׽�Ʈ���� 

//	#define		__NOTIFYSTART0706	// ���Ŀ��� ���� �˸� ����� �̺�Ʈ��  
	#define		__GUILDVOTE			// ��� ��ǥ 
	#define		__HACK_0516			// �̱� ��ŷ 2��

#endif	// end - ���������� define 


#endif


