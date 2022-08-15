#pragma once

#include "debug.h"

const DWORD NULL_ID	= 0xffffffff;

#include "Container_FixedArray.hpp"

typedef	__int64	EXPINTEGER;
typedef	double	EXPFLOAT;

#if defined(__WORLDSERVER ) || defined(__CLIENT)
struct NaviPoint //	�׺���̼ǿ� Ŭ������ ��ġ�� ǥ���ϴ� ����� ���� ����ü
{
	D3DXVECTOR3 Pos = D3DXVECTOR3(0.f, 0.f, 0.f);		//	�� ��ġ�� ���̰����ͻ��� ��ġ�� �ƴ� ������� ��ġ���� �����Ѵ�.
	short		Time = 0;			//	����Ʈ�� �����ִ� �ð�
	DWORD		objid = 0xffffffff;
	std::string      Name = "";
}; 

typedef std::vector<NaviPoint> V_NaviPoint;

#endif // defined(__WORLDSERVER ) || defined(__CLIENT)

namespace SAI79
{
	// ************************************
	// * Ÿ�� ����.	: 0
	// * ��   �Ӽ�	: 1
	// * ��   �Ӽ�	: 2
	// * ���� �Ӽ�	: 3
	// * �ٶ� �Ӽ�	: 4
	// * ��   �Ӽ�	: 5
	// * ��   �Ӽ� Ÿ���� ePropType�� ����
	// ************************************
	enum	ePropType 	{ NO_PROP = 0, FIRE, WATER, ELECTRICITY, WIND, EARTH, END_PROP };
} // namespace SAI79

