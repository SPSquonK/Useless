#include "StdAfx.h"
#include "data.h"

DWORD SAI79::GetAttackStone(ePropType element) {
	switch (element) {
		case SAI79::FIRE:        return II_CHR_SYS_SCR_FIREASTONE;
		case SAI79::WATER:       return II_CHR_SYS_SCR_WATEILSTONE;
		case SAI79::WIND:        return II_CHR_SYS_SCR_WINDYOSTONE;
		case SAI79::ELECTRICITY: return II_CHR_SYS_SCR_LIGHTINESTONE;
		case SAI79::EARTH:       return II_CHR_SYS_SCR_EARTHYSTONE;
		default:                 return 0;
	}
}

DWORD SAI79::GetDefenseStone(ePropType element) {
	switch (element) {
		case SAI79::FIRE:        return II_CHR_SYS_SCR_DEFIREASTONE;
		case SAI79::WATER:       return II_CHR_SYS_SCR_DEWATEILSTONE;
		case SAI79::WIND:        return II_CHR_SYS_SCR_DEWINDYOSTONE;
		case SAI79::ELECTRICITY: return II_CHR_SYS_SCR_DELIGHTINESTONE;
		case SAI79::EARTH:       return II_CHR_SYS_SCR_DEEARTHYSTONE;
		default:                 return 0;
	}
}

std::optional<std::pair<SAI79::ePropType, SAI79::StoneType>> SAI79::StoneIdToElement(DWORD stoneID) {
	using P = std::pair<SAI79::ePropType, SAI79::StoneType>;
	switch (stoneID) {
		case II_CHR_SYS_SCR_FIREASTONE:      return P{ SAI79::FIRE       , StoneType::Attack  };
		case II_CHR_SYS_SCR_DEFIREASTONE:    return P{ SAI79::FIRE       , StoneType::Defense };
		case II_CHR_SYS_SCR_WATEILSTONE:     return P{ SAI79::WATER      , StoneType::Attack  };
		case II_CHR_SYS_SCR_DEWATEILSTONE:   return P{ SAI79::WATER      , StoneType::Defense };
		case II_CHR_SYS_SCR_WINDYOSTONE:     return P{ SAI79::WIND       , StoneType::Attack  };
		case II_CHR_SYS_SCR_DEWINDYOSTONE:   return P{ SAI79::WIND       , StoneType::Defense };
		case II_CHR_SYS_SCR_LIGHTINESTONE:   return P{ SAI79::ELECTRICITY, StoneType::Attack  };
		case II_CHR_SYS_SCR_DELIGHTINESTONE: return P{ SAI79::ELECTRICITY, StoneType::Defense };
		case II_CHR_SYS_SCR_EARTHYSTONE:     return P{ SAI79::EARTH      , StoneType::Attack  };
		case II_CHR_SYS_SCR_DEEARTHYSTONE:   return P{ SAI79::EARTH      , StoneType::Defense };
		default:                             return std::nullopt;
	}
}
