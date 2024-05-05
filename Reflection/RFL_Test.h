#pragma once

#include "Reflection.h"
class CSubObj : public CReflectedBase
{
public:
	REFL_DECLARE_OBJECT(CSubObj, CReflectedBase);
	int m_value;
	float m_number;
	string m_string;
	CVector4 m_vector4;
	CMatrix4 m_matrix4;
};

class CParent : public CReflectedBase
{
public:
	REFL_DECLARE_OBJECT(CParent, CReflectedBase);
	int m_value;
	float m_number;
	string m_string;	
	CSubObj* m_subObj;
	CSubObj m_Obj;
	vector<CReflectedBase*> m_vectorObjects;
};

void RFL_Test1();