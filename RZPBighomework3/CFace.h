#pragma once
#include "CVector.h"

class CFace
{
public:
	CFace();
	virtual ~CFace();
	void SetNum(int);
	void SetFaceNormal(CP3, CP3, CP3);//����С�淨ʸ��
public:
	int vN;         //С��Ķ�����
	int* vI;        //С��Ķ�������
	CVector fNormal;//С��ķ�ʸ��
};

