#include "stdafx.h"
#include "UserTaskBar.h"
#ifdef __WORLDSERVER
#include "User.h"
#endif // __WORLDSERVER
#ifdef __CLIENT
#include "DPClient.h"
#endif

#include <ranges>
#include <numeric>

CTaskbar::CTaskbar() {
	SHORTCUT emptyShortcut;
	emptyShortcut.Empty();

	m_aSlotApplet.fill(emptyShortcut);

	std::ranges::for_each(m_aSlotItem, [&](auto & items) {
		items.fill(emptyShortcut);
		});

	m_aSlotQueue.fill(emptyShortcut);

	m_nActionPoint = 0;
}

size_t CTaskbar::CountNumberOfChats() const {
	constexpr auto IsChat = [](const SHORTCUT & shortcut) {
		return shortcut.m_dwShortcut == ShortcutType::Chat;
	};

	constexpr auto NbOfChats = []<size_t N>(const std::array<SHORTCUT, N> &shortcuts) {
		return std::ranges::count_if(shortcuts, IsChat);
	};

	auto ChatsInEachItems = m_aSlotItem | std::views::transform(NbOfChats);

	return NbOfChats(m_aSlotApplet) + std::reduce(ChatsInEachItems.begin(), ChatsInEachItems.end());
}

void CTaskbar::RemoveAll(const ShortcutType type) {
	for (int nSlot = 0; nSlot < MAX_SLOT_APPLET; ++nSlot) {
		if (m_aSlotApplet[nSlot].m_dwShortcut == type) {
#ifdef __CLIENT
			g_DPlay.SendRemoveAppletTaskBar(nSlot);
#endif
			m_aSlotApplet[nSlot].Empty();
		}
	}

	for (int nSlot = 0; nSlot < MAX_SLOT_ITEM_COUNT; ++nSlot) {
		for (int nIndex = 0; nIndex < MAX_SLOT_ITEM; ++nIndex) {
			if (m_aSlotItem[nSlot][nIndex].m_dwShortcut == type) {
#ifdef __CLIENT
				g_DPlay.SendRemoveItemTaskBar(nSlot, nIndex);
#endif
				m_aSlotItem[nSlot][nIndex].Empty();
			}
		}
	}

	if (type == ShortcutType::Skill) {
		if (!m_aSlotQueue[0].IsEmpty()) {
			memset(&m_aSlotQueue, 0, sizeof(m_aSlotQueue));
#ifdef __CLIENT
			g_DPlay.SendSkillTaskBar();
#endif
		}
	}
}

CAr & operator<<(CAr & ar, const CTaskbar & self) {
	constexpr auto PushShortcut = [](CAr & ar, const SHORTCUT & shortcut, const auto ... extra) {
		if (shortcut.IsEmpty()) return false;

		((ar << extra), ...);
		ar << shortcut;

		return true;
	};

	const auto appletsSize = ar.PushBack<size_t>(0);
	for (int i = 0; i < MAX_SLOT_APPLET; ++i) {
		if (PushShortcut(ar, self.m_aSlotApplet[i], i)) {
			++(*appletsSize);
		}
	}

	const auto itemssSize = ar.PushBack<size_t>(0);
	for (int i = 0; i < MAX_SLOT_ITEM_COUNT; ++i) {
		for (int j = 0; j < MAX_SLOT_ITEM; ++j) {
			if (PushShortcut(ar, self.m_aSlotItem[i][j], i, j)) {
				++(*itemssSize);
			}
		}
	}

	const auto queueSize = ar.PushBack<size_t>(0);
	for (int i = 0; i < MAX_SLOT_QUEUE; ++i) {
		if (PushShortcut(ar, self.m_aSlotQueue[i], i)) {
			++(*queueSize);
		}
	}

	ar << self.m_nActionPoint;
	return ar;
}

CAr & operator>>(CAr & ar, CTaskbar & self) {
	ar.IsUnsafe();

	self = CTaskbar();

	size_t nbShortcut;

	ar >> nbShortcut;
	for (size_t i = 0; i != nbShortcut; ++i) {
		int nIndex; ar >> nIndex;
		ar >> self.m_aSlotApplet[nIndex];
	}

	ar >> nbShortcut;
	for (size_t i = 0; i != nbShortcut; ++i) {
		int nIndex1, nIndex2;
		ar >> nIndex1 >> nIndex2;
		ar >> self.m_aSlotItem[nIndex1][nIndex2];
	}

	ar >> nbShortcut;
	for (size_t i = 0; i != nbShortcut; ++i) {
		int nIndex; ar >> nIndex;
		ar >> self.m_aSlotQueue[nIndex];
	}

	ar >> self.m_nActionPoint;
	return ar;
}


#ifdef __WORLDSERVER

// ��ų�� ����� ������ ��������
void CTaskbar::OnEndSkillQueue(CUser * pUser) {
	pUser->m_nUsedSkillQueue = -1;
	pUser->AddHdr(GETID(pUser), SNAPSHOTTYPE_ENDSKILLQUEUE);
	((CMover *)pUser)->ClearCmd();
	TRACE("\nOnEndSkillQueue\n");
}

int	CTaskbar::SetNextSkill(CUser * pUser) {
	pUser->m_nUsedSkillQueue++;		// ť�ε��� ��������...

	int nAP = m_nActionPoint;

	if (!(pUser->IsSMMode(SM_ACTPOINT))) {
		switch (pUser->m_nUsedSkillQueue) {
			case 1:	nAP -= 6;	break;		// �׼� ����Ʈ �Ҹ�. ���������� ����Ҽ��ִ��� �̸� �˾ƺ��� ����.
			case 2:	nAP -= 8;	break;
			case 3:	nAP -= 11;	break;
			case 4:	nAP -= 30;	break;
		}
	}

	LPSHORTCUT pShortcut = &m_aSlotQueue[pUser->m_nUsedSkillQueue];
	BOOL bResult = nAP < 0;
	if ((pUser->m_nUsedSkillQueue >= MAX_SLOT_QUEUE) || pShortcut->IsEmpty() || (nAP < 0))	// ť������ ������ ���ų� || ť�� ����ų� || AP�� ���ų�.
	{
		OnEndSkillQueue(pUser);
		return 0;		// ť���� ��.
	} else {
		if (nAP < 0)	nAP = 0;
		m_nActionPoint = nAP;
		pUser->AddSetActionPoint(nAP);		// �׼�����Ʈ Ŭ�� ����.
		OBJID idTarget = pUser->m_idSetTarget;
		TRACE("������ų��� �õ�%d, ", pShortcut->m_dwId);
		if (pUser->CMD_SetUseSkill(idTarget, pShortcut->m_dwId, SUT_QUEUEING) == 0)		// ������ ����� ����. �̵� + ��ų����� ������ ���.
		{
			TRACE("������ų��� ���� %d, ", pShortcut->m_dwId);
			SetNextSkill(pUser);	// ��ų��뿡 �����ߴٸ� ���� ��ų ����ϵ��� �Ѿ.
		}
		return 1;
	}

	return 1;
}
#endif // __WORLDSERVER

