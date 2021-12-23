#pragma once

struct StateBase
{
	virtual ~StateBase() {};
	// ��������
	virtual void Init() {};
	// ���s
	virtual void Exec() {};
	// State�ύX�O����
	virtual void BeforeChange() {}
	// State�ύX�㏈��
	virtual void AfterChange() {}
};

