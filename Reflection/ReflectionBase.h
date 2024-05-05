#pragma once

#include <vector>
#include "ClassFactory/ClassFactory.h"
#include "RFL_Pareser.h"
#include "DebugChannel\DebugChannel.h"
extern DebugChannels::CDebugChannel ReflectionDebug;

using std::vector;
#define REFL_DECLARE_OBJECT(_O_,_P_)												\
    vector<CReflectionMapEntry> &GetReflectionMap() { return _O_::s_ReflectionMap; }\
    virtual const char *GetClassName() { return #_O_; }								\
    static vector<CReflectionMapEntry> s_ReflectionMap;								\
    static CReflectedBase *create() { return new _O_; }								\
																					\
	virtual void WriteMembers(IRFL_Parser& doc)										\
	{																				\
		InternalWriteMembers( GetReflectionMap(), doc);								\
		_P_::WriteMembers(doc);														\
	}																				\
	virtual void ReadMembers( IRFL_Parser& doc )									\
	{																				\
		InternalReadMembers( GetReflectionMap(), doc);								\
		_P_::ReadMembers(doc);														\
	}																				\
    static ClassFactory s_factoryRegistration; 

#define REFL_DEFINE_OBJECT(_O_)										\
    ClassFactory _O_::s_factoryRegistration(#_O_,_O_::create);		\
    vector<CReflectionMapEntry> _O_::s_ReflectionMap = { 

#define REFL_DEFINE_END };

#define REFL_DEFINE_FLOAT_MEMBER(_O_,_M_)				CReflectionMapEntry(new CFloatProperty( RT_Float, sizeof(float), offsetof(_O_, _M_), #_M_) )
#define REFL_DEFINE_STRING_MEMBER(_O_,_M_)				CReflectionMapEntry(new CStringProperty( RT_String, sizeof(string), offsetof(_O_, _M_), #_M_) )
#define REFL_DEFINE_INT_MEMBER(_O_,_M_)					CReflectionMapEntry(new CIntProperty( RT_Int, sizeof(int), offsetof(_O_, _M_), #_M_ ) )
#define REFL_DEFINE_OBJECT_MEMBER(_O_,_M_)				CReflectionMapEntry(new CSubObjectProperty( RT_Object, sizeof(void *), offsetof(_O_, _M_), #_M_ ) )
#define REFL_DEFINE_OBJECT_PTR_MEMBER(_O_,_M_)			CReflectionMapEntry(new CSubObjectPtrProperty(RT_ObjectPtr, sizeof(void *), offsetof(_O_, _M_), #_M_ ) )
#define REFL_DEFINE_OBJECT_PTR_VECTOR_MEMBER(_O_,_M_)	CReflectionMapEntry(new CObjPtrVectorProperty( RT_ObjectPtrVec, sizeof(std::vector<CPropertyBase*>), offsetof(_O_, _M_), #_M_ ) )
#define REFL_DEFINE_VECTOR4_MEMBER(_O_,_M_)				CReflectionMapEntry(new CVector4Property( RT_Vector4, sizeof(CVector4),offsetof(_O_, _M_), #_M_) )
#define REFL_DEFINE_MATRIX4_MEMBER(_O_,_M_)				CReflectionMapEntry(new CMatrix4Property( RT_Matrix4, sizeof(CMatrix4),offsetof(_O_, _M_), #_M_) )

class CReflectedBase
{
public:	
	CReflectedBase() 
	{}
	virtual ~CReflectedBase() {};
	void Read(const char *fileName);
	void Write(const char *fileName);
	virtual void ReadMembers(IRFL_Parser& doc) {}
	virtual void WriteMembers(IRFL_Parser& doc) {}
	virtual const char *GetClassName()	{ return nullptr; }
protected:

	void InternalReadMembers(vector<CReflectionMapEntry>& reflectionMap, IRFL_Parser& doc );
	void InternalWriteMembers(vector<CReflectionMapEntry>& reflectionMap, IRFL_Parser& doc );
};
