#include "StdAfx.h"
#include "ItemElem.h"

namespace ItemElemFeatures {


	// bSize�� �Ǿ�� ����� �ø� �� �ִ��� �˻��� �� TRUE���� setting �Ѵ�.
	// bSize�� TRUE�� �� ��� dwTagetItemKind3�� NULL_ID�� �Ѵ�.
	BOOL Piercing::IsPierceAble(DWORD dwTargetItemKind3, BOOL bSize) {
		if (!GetProp())
			return FALSE;

		int nPiercedSize = GetPiercingSize();
		if (bSize) // �Ǿ�� ����� �ø��� ���
			nPiercedSize++;

		if (GetProp()->dwItemKind3 == IK3_SUIT) {
			if (nPiercedSize <= MAX_PIERCING_SUIT) {
				if (dwTargetItemKind3 == NULL_ID)
					return TRUE;
				else if (dwTargetItemKind3 == IK3_SOCKETCARD)
					return TRUE;
			}
		}

		else if (GetProp()->dwItemKind3 == IK3_SHIELD
			|| GetProp()->dwItemKind2 == IK2_WEAPON_DIRECT
			|| GetProp()->dwItemKind2 == IK2_WEAPON_MAGIC
			) {
			if (nPiercedSize <= MAX_PIERCING_WEAPON) {
				if (dwTargetItemKind3 == NULL_ID)
					return TRUE;
				else if (dwTargetItemKind3 == IK3_SOCKETCARD2)
					return TRUE;
			}
		}

		else if (CItemElem::Cast(this)->IsVisPet()) {
			if (nPiercedSize <= MAX_VIS) {
				if (dwTargetItemKind3 == NULL_ID)
					return TRUE;
				else if (dwTargetItemKind3 == IK3_VIS)
					return TRUE;
			}
		}

		return FALSE;
	}


	ItemProp * Piercing::GetProp() const {
		return CItemElem::Cast(this)->GetProp();
	}

}

