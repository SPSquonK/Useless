#ifndef __VERSION_COMMON_H__
#define __VERSION_COMMON_H__ 
#define __MAINSERVER

#if !defined( __TESTSERVER ) && !defined( __MAINSERVER )
	#define __INTERNALSERVER
#endif

#define		D3DDEVICE	nullptr
#define		__SERVER						// Ŭ���̾�Ʈ �����ڵ带 �������� �ʱ� ���� define
#define		__X15
#define		__VERIFY_LOOP041010
//#define		__PROFILE_RUN
#define		__CPU_UTILDOWN_060502			// CPU ���� �����۾� 
#define		__SLIDE_060502					// ������ ������ ���� ���� 
#define		__S8_SERVER_PORT				// 2006�� 11�� 7�� ������Ʈ - World, Cache, CoreServer
#define		__EVENT_1101					// ��ý �̺�Ʈ
// �ؿ� 6�� ���� ����
#define		__TRAFIC_1222					// �ߺ� ��Ŷ ���� ����
#define		__S_SERVER_UNIFY				// ���� ���� Neuz, World

// �ؿ� 7�� ���� ����
#define		__BUGFIX_0326					// ���� ���� ������ ���

// 8.5��
#define		__INFINITE_0227

#define		__LANG_IME_0327					// ������ imeǥ�� â ��� ����
// 9��
#define		__TRADESYS						// ��ȯ ������ ��ũ��Ʈ(�̺�Ʈ, ����Ʈ, ...) Neuz, World
#define		__EVE_BALLOON					//ǳ�� �̺�Ʈ ����

#define		__EVENT_1101_2					// ��ý �̺�Ʈ 64��Ʈ ����
#define		__S_RECOMMEND_EVE				// ��õ �̺�Ʈ Neuz, Trans, World
#define		__EVE_MINIGAME					// �̺�Ʈ �̴ϰ��� 4��, Neuz, World

#define		__ANGEL_LOG						// ���� ���� �α�
#define		__EXP_ANGELEXP_LOG				// ����ġ, ���� ����ġ �α� ���� CHARACTER_TBL �÷� �߰�
#define		__S_ADD_EXP						// EXP_S ������ ����

#define		__S_ADD_RESTATE					// ���ȭ ������ ����( ������Ʈ ��, ��ø, ü��, ���� )


// 11��

#define		__EVENTLUA_COUPON			// ���� �̺�Ʈ
#define		__NOLIMIT_RIDE_ITEM			// ���� ���� ���� ����ü ������		// ������ ����
#define		__NPC_BUFF					// NPC�� ���� �����ޱ�
//	#define		__REFLECTDMG_AFTER			// �ݻ絥������ ���߿� ����


#define		__JEFF_11					// ���� ����� �ִ� �������� �޾���� ���ϴ� �������� AI ������ �ʿ�
#define		__SYS_TICKET				// �����
#define		__HACK_1023					// ���ҽ� ���� // ���� �ӵ� // ���� ���� �ӵ�

#define		__INVALID_LOGIN_0320		// ������ �Ϸ� ���� ���� ������� ���� ����(���� ����)

#define		__JEFF_9_20					// ���� �ð��� ħ�� �ð����� ����

#define		__EVENTLUA_GIFT				// Ư�� ������ �������� ������ ����

#define		__JEFF_11_4					// �Ʒ���
#define		__JEFF_11_6					// �ٺ� ����

#define		__DST_GIFTBOX				// ���� �����ۿ� ����Ʈ �ڽ� ��� �߰�
#define		__EVENT_MONSTER				// �̺�Ʈ ����(WorldServer)

#define		__CHIPI_DYO					// NPC�� Ư������������ ����� �� �ְ� character.inc�� ���� 
#define		__CHIPI_ITEMUPDATE_080804	// ������ ��� g_uKey�� 101�� ����. �׷��� ����...

#define		__VM_0820
//#define		__VM_0819	// ���� �޸� ���� ã��

// 12��	
//	#define 	__ANGEL_NODIE				// ĳ���Ͱ� ����ص� ������ ������� �ʴ´�.
//	#define		__PET_0519	// �� ����

// 13��
//	#define		__EXT_ENCHANT				// ���� Ȯ��(�Ӽ�, �Ϲ�)
//	#define		__HOUSING					// �Ͽ�¡ �ý���

#define		__LAYER_1015		// ���� ��ü ��
#define		__LAYER_1020		// __LAYER_1015 �׽�Ʈ
#define		__LAYER_1021		// __LAYER_1015 ������
#define		__AZRIA_1023	// ����� �� ����

#define		__OCCUPATION_SHOPITEM	// ���ɱ�� ���� ���� ���� ������

#define		__SYNC_1217		// �񵿱� ����(�ʸ��� ����)
#define		__SPEED_SYNC_0108		// ResetDestParam speed ���� �������� ���̴°� ���� Ÿ������ �ʱ⽺�ǵ�񵿱⵵ ���� ���� 
#define		__SYS_ITEMTRANSY			// ������ Ʈ������ �ý��ۿ��� ����

#define		__EVENTLUA_CHEEREXP		// ��� �̺�Ʈ - ���� ����ġ ����

#define		__FUNNY_COIN			// �۴� ����

#define		__MAP_SECURITY				// ������ ������ �� �˻�

// 14��
//	#define		__SMELT_SAFETY				// 14�� ��������
//	#define		__ANGEL_EXPERIENCE			// 14�� ���� ���� ����(����ġ ����)
//	#define		__EQUIP_BIND				// ���������� ����� �ͼ�
//	#define		__EXT_ATTRIBUTE				// �Ӽ����� ���� Ȯ�� �� ����
//	#define		__NEW_ITEM_LIMIT_LEVEL		// ������ ���뷹�� ��Ģ ����(MASTER, HERO�� ���� ������ �ް� ���� Ŭ���� ����� ������ ���� ����)
//	#define		__BALLOON_CODE_IMPROVEMENT	// ǳ�� ���� �ڵ� ����
	#define		__QUIZ						// ���� �̺�Ʈ �ý���

#define		__BUFF_TOGIFT				// ���� �ð� ����Ǹ� ������ �����ϴ� �ý���(IK2_BUFF_TOGIFT)
#define		__EVENTLUA_SPAWN			// ������ �� ���� ���� �̺�Ʈ
#define		__EVENTLUA_KEEPCONNECT		// ���� ���� ������ ���� �̺�Ʈ

#define		__PERIN_BUY_BUG				// ��� �ݺ����� ���� Ȯ�ο� �ڵ�

#define		__ERROR_LOG_TO_DB		// ���� �α� �ý���

#define		__EVENTLUA_RAIN				// �帶 �̺�Ʈ -> ��Ʒ� ����

#define		__EVENTLUA_SNOW				// ���� �̺�Ʈ

#define		__ADD_RESTATE_LOW			// ���ȭ ������ ����( ������Ʈ �ϱ� ��, ��ø, ü��, ���� )


// 15��
//	#define		__TELEPORTER				// 15�� �ڷ����� 
//	#define		__IMPROVE_SYSTEM_VER15		// 15�� �ý��� ��������
//	#define		__DYNAMIC_MPU				// ���氡���� MPU
//	#define		__REACTIVATE_EATPET			// ������ ������ �����Ÿ� �̻� �־����� ���ȯ
//	end15th


	#define		__FORCE_KILL_SERVER

	#define		__SHOP_COST_RATE			// ���� ���� ����

//	#define		__ITEMTRANSY_PENYA			//	��ķ� ������ Ʈ���� �����ϰ� ����.
	#define		__PROTECT_AWAKE				//	���� ��ȣ�� �η縶��.


#if	  defined(__INTERNALSERVER)	// ���� �繫�� �׽�Ʈ���� 
//	#define		__VERIFY_MEMPOOL
	#define		__GUILDVOTE					// ��� ��ǥ 
	#define		__IAOBJ0622					// ��� ��ǥ ��ǥ ����	// ����, ����
	#define		__SKILL0517
//	#define		__S_NEW_SKILL_2				// ��ų ���� ��Ŷ ���� Neuz, World, Trans
	#define		__Y_CASTING_SKIP			// �ɽ��� ���� ��ų �ߵ� - Neuz, World
	#define		__YAIMONSTER_EX				// AI��� �߰� - Neuz, World
	#define		__Y_PATROL					// ��ȸ�ϱ� - Neuz, World
	#define		__V060721_TEXTDRAG			// ���ڿ� ȿ���ֱ�
	#define		__Y_BEAUTY_SHOP_CHARGE
//	#define		__LUASCRIPT060908			// lua ��ũ��Ʈ����( ������ ) 
	#define		__Y_FLAG_SKILL_BUFF			// ��, ���� ������ ��۹��� ����... Neuz, World
//	#define		__EVENT_FALL				// ��ǳ �̺�Ʈ - �𵨱�ü Neuz, World
//	#define		__JEFF_VER_8				// 8�� �۾�
//	#define		__JHMA_VER_8_1				// 8�� ���ӳ�����ӱ���	Neuz, World 
//	#define		__JHMA_VER_8_2				// 8�� ���ӳ��������ǸŰ�������Ǯ��	Neuz, World
//	#define		__JHMA_VER_8_5				// 8�� ��ų����ġ�ٿ��	Neuz, World
//	#define		__JHMA_VER_8_7				// 8�� ������� ������� PVP�����ϰ���   Neuz, World
//	#define		__JHMA_VER_8_5_1			// 8.5�� ��� ������ų ����ȿ�� �Ұ��� ���� World
//	#define		__JHMA_VER_8_5_2			// 8.5�� ������� �α����� ���������ʰ� ����  World
//	#define		__CSC_VER8_3				// 8�� Buffâ ����. Neuz, World
//	#define		__CSC_VER8_4				// 8�� ����, �������� ���� Neuz, World
//	#define		__CSC_VER8_5				// 8�� ���� ��ȯ Neuz, World, Trans
//	#define		__CSC_VER8_6				// ��� ��ɾ� statall �߰� World
	#define		__Y_MAX_LEVEL_8				// ���� 120���� ����... Neuz, World, Trans
	#define		__Y_HAIR_BUG_FIX
	#define		__EVENT_0117				// ���� �̺�Ʈ
//	#define		__NEWYEARDAY_EVENT_COUPON	// �� �̺�Ʈ - ����, ���� �����̿�� ����.
	#define		__TRAFIC_1215
//	#define     __Y_NEW_ENCHANT				// ���� ������ ����, Neuz, World

//	#define		__INVALID_LOGIN_0320		// ������ �Ϸ� ���� ���� ������� ���� ����(���� ����)
	#define		__BUGFIX_0326				// ���� ���� �̺�Ʈ ������ ��� ����
// 10��
// 10��
	
//	#define		__JEFF_9					// 9, 10�� �߰� �۾�
	#define		__HACK_0516					// �̱� ��ŷ 2��

//	#define		__FLYBYATTACK0608			// 9th FlyByAttack edit
//	#define		__PVPDEMAGE0608				// 9th PVP DEMAGE edit
//	#define		__BLADELWEAPON0608			// 9th ���̵� ��տ� ���⸦ ���� �� �߰� �ɼ��� �����տ� ��� �ִ� ������ �͸� ������ �ǵ��� ����
//	#define		__METEONYKER_0608
//	#define		__Y_DRAGON_FIRE
//	#define		__CSC_VER9_5				// 9�� ���ð��� (�߰� �˱� ���)

	#define		__GLOBAL_COUNT_0705			// CTime::GetTimer

//	#define		__AI_0711					// ���׿���Ŀ AI ����
//	#define		__HACK_0720					// ���ҽ� ��ŷ
//11
//	#define		__JEFF_11					// ���� ����� �ִ� �������� �޾���� ���ϴ� �������� AI ������ �ʿ�
	
//	#define		__SYS_TICKET				// �����
//	#define		__HACK_1023					// ���ҽ� ���� // ���� �ӵ� // ���� ���� �ӵ�


	#define		__JEFF_11_1
	#define		__JEFF_11_3					// ����/Ŭ���̾�Ʈ ���ҽ� �и�
//	#define		__JEFF_11_4					// �Ʒ���

//	#define		__JEFF_11_5					// �޸� ����

//	#define		__DST_GIFTBOX				// ���� �����ۿ� ����Ʈ �ڽ� ��� �߰�

	#define		__OPT_MEM_0811
//	#define		__MEM_TRACE
	#define		__NEW_PROFILE		// ���ο� �������Ϸ� ����

	#define		__VTN_TIMELIMIT				// ��Ʈ�� �÷��� �ð� ����

	#define		__PROTECT_AWAKE				//	���� ��ȣ�� �η縶��.


// error : inserted by coan: "#define __VER 16" differently redefines -D symbol at B:\Useless\Source\WORLDSERVER\VersionCommon.h(319)

	#define		__GUILD_HOUSE_MIDDLE		// ����Ͽ콺 ����

	#define		__MOVER_STATE_EFFECT		// ���� ���°� ��ȭ�� ���� ����Ʈ ����

	#define		__NEW_ITEM_VARUNA			// �� ���� �ý���(�ٷ糪)


#elif defined(__TESTSERVER)		// �ܺ� ���� �׽�Ʈ���� 
	
	#define		__GUILDVOTE					// ��� ��ǥ 
//	#define		__SKILL0517					// ��ų ���� �Ķ����
	#define		__Y_CASTING_SKIP			// �ɽ��� ���� ��ų �ߵ� - Neuz, World
	#define		__YAIMONSTER_EX				// AI��� �߰� - Neuz, World
	#define		__Y_PATROL					// ��ȸ�ϱ� - Neuz, World
	#define		__V060721_TEXTDRAG			// ���ڿ� ȿ���ֱ�
	#define		__Y_BEAUTY_SHOP_CHARGE
	#define		__Y_FLAG_SKILL_BUFF			// ��, ���� ������ ��۹��� ����... Neuz, World
//	#define		__EVENT_FALL				// ��ǳ �̺�Ʈ - �𵨱�ü Neuz, World
	#define		__Y_MAX_LEVEL_8				// ���� 120���� ����... Neuz, World, Trans
	#define		__TRAFIC_1215

	// 10��

//	#define		__CSC_VER9_2				// 9�� ����â ���� ���� Neuz, World

	#define		__HACK_0516					// �̱� ��ŷ 2��

//	#define		__FLYBYATTACK0608			// 9th FlyByAttack edit
//	#define		__PVPDEMAGE0608				// 9th PVP DEMAGE edit
//	#define		__BLADELWEAPON0608			// 9th ���̵� ��տ� ���⸦ ���� �� �߰� �ɼ��� �����տ� ��� �ִ� ������ �͸� ������ �ǵ��� ����

//	#define		__Y_DRAGON_FIRE
//	#define		__EVENTLUA					// �̺�Ʈ (��� ��ũ��Ʈ ����) - World, Trans, Neuz
//	#define		__CSC_VER9_5				// 9�� ���ð��� (�߰� �˱� ���)

//	#define		__METEONYKER_0608
//	#define		__AI_0711					// ���׿���Ŀ AI ����
//	#define		__JEFF_9					// 9, 10�� �߰� �۾�
	#define		__GLOBAL_COUNT_0705			// CTime::GetTimer

//	#define		__JEFF_11					// ���� ����� �ִ� �������� �޾���� ���ϴ� �������� AI ������ �ʿ�
//	#define		__SYS_TICKET				// �����
//	#define		__HACK_1023					// ���ҽ� ���� // ���� �ӵ� // ���� ���� �ӵ�

//	#undef	__VER
//	#define	__VER	11						// 11��

	#define		__JEFF_11_1
	#define		__JEFF_11_3					// ����/Ŭ���̾�Ʈ ���ҽ� �и�
//	#define		__JEFF_11_4					// �Ʒ���
//	#define		__JEFF_11_5					// �޸� ����


	#define		__OPT_MEM_0811
//	#define		__MEM_TRACE
	
	#define		__NEW_PROFILE		// ���ο� �������Ϸ� ����

	#define		__GUILD_HOUSE_MIDDLE		// ����Ͽ콺 ����

#elif defined(__MAINSERVER)	// �ܺ� ����
 
	#define		__ON_ERROR
	#define		__IDC
	#define		__Y_BEAUTY_SHOP_CHARGE		// ��Ƽ��, ���λ��� ���������ȭ..Neuz, World
	#define		__TRAFIC_1215
//	#define		__EVENT_FALL		// ��ǳ

//	#define     __Y_NEW_ENCHANT				// ���� ������ ����, Neuz, World

	#define		__JEFF_11_1
	#define		__JEFF_11_3		// ����/Ŭ���̾�Ʈ ���ҽ� �и�

	#define		__EVENT_0117				// propEvent.inc spawn

	#define		__OPT_MEM_0811
//	#define		__MEM_TRACE
	//#define		__NEW_PROFILE		// ���ο� �������Ϸ� ����
	#define		__GLOBAL_COUNT_0705			// CTime::GetTimer

#endif	// end - ���������� define 

// �ӽ� - ����� ���� ���� 
	#define		__REMOVE_SCIRPT_060712		 

#endif