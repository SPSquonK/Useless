#pragma once

#include <array>

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
  // As there are no dynamic allocation, we don't really need a destructor

  friend CAr & operator<<(CAr & ar, const CTaskbar & self);
  friend CAr & operator>>(CAr & ar, CTaskbar & self);

  [[nodiscard]] size_t CountNumberOfChats() const;

};


