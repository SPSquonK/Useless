#pragma once

#include "debug.h"

const DWORD NULL_ID	= 0xffffffff;

template <class T> class CFixedArray final
{
private:
	std::vector<T> m_data;
	std::vector<size_t> m_offsets;

public:
	CFixedArray();
	
	int  GetSize() { return static_cast<int>(m_data.size()); }
	void SetAtGrow(size_t nIndex, const T * pData);
	T * GetAt(DWORD dwIndex);
	void Optimize();
	void RemoveAll() { *this = CFixedArray(); }
};

template <class T> inline CFixedArray<T>::CFixedArray() {
	m_data.reserve(100);
	m_offsets.reserve(100);
}

template <class T> inline T * CFixedArray<T>::GetAt(DWORD dwIndex) {
	if (dwIndex >= m_offsets.size()) return nullptr;

	const auto offset = m_offsets[dwIndex];
	if (offset == NULL_ID) return nullptr;

	return &m_data[offset];
}

template <class T> void CFixedArray<T>::Optimize() {
	m_data.shrink_to_fit();
	m_offsets.shrink_to_fit();
}

template <class T> void CFixedArray<T>::SetAtGrow( size_t nIndex, const T * pData )
{
	if (nIndex < m_offsets.size()) {
		if (m_offsets[nIndex] == NULL_ID) {
			const size_t offset = m_data.size();
			T & data = m_data.emplace_back();
			std::memcpy(&data, pData, sizeof(T));
			m_offsets[nIndex] = offset;
		} else {
			std::memcpy(&m_data[m_offsets[nIndex]], pData, sizeof(T));
		}
	} else {
		if (nIndex != 0) m_offsets.resize(nIndex - 1, NULL_ID);

		const size_t offset = m_data.size();
		T & data = m_data.emplace_back();
		std::memcpy(&data, pData, sizeof(T));
		m_offsets.emplace_back(offset);
	}
}

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

