#include "Reflection/Reflection.h"
#include "RFL_Test.h"

#include <fstream>
using std::ofstream;

REFL_DEFINE_OBJECT(CSubObj)
	REFL_DEFINE_FLOAT_MEMBER(CSubObj, m_number),
	REFL_DEFINE_INT_MEMBER(CSubObj, m_value),
	REFL_DEFINE_STRING_MEMBER(CSubObj, m_string),
	REFL_DEFINE_VECTOR4_MEMBER(CSubObj, m_vector4),
	REFL_DEFINE_MATRIX4_MEMBER(CSubObj, m_matrix4),
REFL_DEFINE_END

REFL_DEFINE_OBJECT(CParent)
	REFL_DEFINE_OBJECT_MEMBER(CParent, m_Obj),
	REFL_DEFINE_STRING_MEMBER(CParent, m_string),
	REFL_DEFINE_FLOAT_MEMBER(CParent, m_number),
	REFL_DEFINE_INT_MEMBER(CParent, m_value),
	REFL_DEFINE_OBJECT_PTR_MEMBER(CParent, m_subObj),
	REFL_DEFINE_OBJECT_PTR_VECTOR_MEMBER(CParent, m_vectorObjects),
REFL_DEFINE_END

CParent g_parent;
CParent g_parentCopy;
CSubObj g_subObj;

// Writes and read an object with all intrinsic types and validates that it was read back correctly.
void RFL_Test1()
{
	g_subObj.m_number = 1234.0f;
	g_subObj.m_value = 5678;
	g_subObj.m_string = "This is the sub object.";
	float v = 0.0f;
	for (auto& value : g_subObj.m_matrix4.GetData())
	{	
		value = v;
		v+= 1.0f;
	}
	v = 100.0f;
	for (auto& value : g_subObj.m_vector4.GetData())
	{
		value = v;
		v += 1.0f;
	}
	g_subObj.m_matrix4.GetData()[0] = 123.0f;
	g_subObj.m_matrix4.GetData()[1] = 456.0f;
	g_subObj.m_matrix4.GetData()[2] = 789.0f;
	g_subObj.m_matrix4.GetData()[3] = 123.0f;
	g_parent.m_number = 555.0f;
	g_parent.m_value = 666;
	g_parent.m_Obj = g_subObj;
	g_parent.m_subObj = &g_subObj;
	g_parent.m_vectorObjects.push_back(&g_subObj);
	g_parent.m_vectorObjects.push_back(&g_subObj);
	g_parent.m_vectorObjects.push_back(&g_subObj);
	g_parent.Write("Object.json");
	g_parentCopy.Read("Object.json");
	ReflectionDebug.Assert(g_parentCopy.m_number == g_parent.m_number, "Failed serialize.");
	ReflectionDebug.Assert(g_parentCopy.m_value == g_parent.m_value, "Failed serialize.");
	ReflectionDebug.Assert(g_parentCopy.m_string == g_parent.m_string, "Failed serialize.");
	ReflectionDebug.Assert(g_parentCopy.m_Obj.m_number == g_parent.m_Obj.m_number, "Failed serialize.");
	ReflectionDebug.Assert(g_parentCopy.m_Obj.m_value == g_parent.m_Obj.m_value, "Failed serialize.");
	ReflectionDebug.Assert(g_parentCopy.m_Obj.m_string == g_parent.m_Obj.m_string, "Failed serialize.");
	int i = 0;
	for (auto& value : g_subObj.m_matrix4.GetData())
	{
		ReflectionDebug.Assert(value == g_parentCopy.m_Obj.m_matrix4.GetData()[i++], "Failed serialize.");		
	}
	i = 0;
	for (auto& value : g_subObj.m_vector4.GetData())
	{
		ReflectionDebug.Assert(value == g_parentCopy.m_Obj.m_vector4.GetData()[i++], "Failed serialize.");
	}

	ReflectionDebug.Assert(g_parentCopy.m_subObj->m_number == g_parent.m_Obj.m_number, "Failed serialize.");
	ReflectionDebug.Assert(g_parentCopy.m_subObj->m_value == g_parent.m_Obj.m_value, "Failed serialize.");
	ReflectionDebug.Assert(g_parentCopy.m_subObj->m_string == g_parent.m_Obj.m_string, "Failed serialize.");
	i = 0;
	for (auto& value : g_subObj.m_matrix4.GetData())
	{
		ReflectionDebug.Assert(value == g_parentCopy.m_subObj->m_matrix4.GetData()[i++], "Failed serialize.");
	}
	i = 0;
	for (auto& value : g_subObj.m_vector4.GetData())
	{
		ReflectionDebug.Assert(value == g_parentCopy.m_subObj->m_vector4.GetData()[i++], "Failed serialize.");
	}
	i = 0;
	for (auto sub : g_parentCopy.m_vectorObjects)
	{
		CSubObj* subCopy = dynamic_cast<CSubObj*>(sub);
		ReflectionDebug.Assert(subCopy->m_number == g_subObj.m_number, "Failed serialize.");
		ReflectionDebug.Assert(subCopy->m_value == g_subObj.m_value, "Failed serialize.");
		ReflectionDebug.Assert(subCopy->m_string == g_subObj.m_string, "Failed serialize.");
		i = 0;
		for (auto& value : subCopy->m_matrix4.GetData())
		{
			ReflectionDebug.Assert(value == g_subObj.m_matrix4.GetData()[i++], "Failed serialize.");
		}
		i = 0;
		for (auto& value : subCopy->m_vector4.GetData())
		{
			ReflectionDebug.Assert(value == g_subObj.m_vector4.GetData()[i++], "Failed serialize.");
		}
	}

}
// Writes some random text to a file and tries to read it back.
void RFL_Test2()
{

}


