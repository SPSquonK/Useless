#ifndef __LORD_SKILL_EXECUTABLE_H__
#define	__LORD_SKILL_EXECUTABLE_H__

#include "lordskill.h"

// ���� ��ų
// Ʈ���� �������� ���
// ȿ�� ���� ó�� �ƴ�
// ���� ��� �ð��� ���� ���� ���� �Ǵ�
// �� ���� �� ����
class CLordSkillComponentODBC : public CLordSkillComponentExecutable
{
public:
	CLordSkillComponentODBC(int nId, CScript & script) : CLordSkillComponentExecutable(nId, script) {}

	void	Execute( u_long idPlayer, u_long idTarget, VOID* pParam ) override;
};

#endif	// __LORD_SKILL_EXECUTABLE_H__