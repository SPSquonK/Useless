#pragma once

#include "ar.h"
#include <array>

#ifdef __WORLDSERVER
class CUser;
#endif

class CTaskbar {
public:
  std::array<SHORTCUT, MAX_SLOT_APPLET> m_aSlotApplet;
  std::array<
    std::array<SHORTCUT, MAX_SLOT_ITEM>,
    MAX_SLOT_ITEM_COUNT
  > m_aSlotItem;
  std::array<SHORTCUT, MAX_SLOT_QUEUE> m_aSlotQueue;

  int m_nActionPoint; // �׼� ����Ʈ - �ø�������� ���.

public:
  CTaskbar();
  // As there are no dynamic allocation, we don't really need a virtual destructor

  friend CAr & operator<<(CAr & ar, const CTaskbar & self);
  friend CAr & operator>>(CAr & ar, CTaskbar & self);

  [[nodiscard]] size_t CountNumberOfChats() const;

  

#ifdef __WORLDSERVER
  void RemoveAllSkills();
  int  SetNextSkill(CUser * pUser);
  void OnEndSkillQueue(CUser * pUser);
#endif // __WORLDSERVER

};

//
//class CUserTaskBar final : public CTaskbar {
//	// TODO: do we want to have a CUserTaskBar class
//	// that only adds m_nusedKSillQueue or should this field
//	// be moved out?
//public:
//	CUserTaskBar();
//
//public:
//	int			m_nUsedSkillQueue;	// -1:���������� ���� 0~4:���� �������� ��ųť �ε���.
//
//public:
//	void	Serialize(CAr &ar);
//};
//
