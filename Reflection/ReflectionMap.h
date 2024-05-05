#pragma once

#include <vector>
#include <string>
using std::string;

enum RFL_Type
{
	RT_Int = 1,
	RT_Float = 2,
	RT_String = 3,
	RT_Object = 4,
	RT_ObjectPtr = 5,
	RT_ObjectPtrVec = 6,
	RT_Vector4 = 7,
	RT_Matrix4 = 8,
};

class CReflectedBase;
class IRFL_Parser;
class CPropertyBase
{
public:		
	CPropertyBase(RFL_Type type, size_t size, size_t memberOffset, const char *name ) : m_type(type), m_size(size), m_memberOffset(memberOffset), m_name(name) {};
	void *GetAddress( CReflectedBase *obj ) const { return (void*)((char*)(obj)+GetOffset()); }		
	size_t GetOffset() const { return m_memberOffset; }
	size_t GetSize() const { return m_size; }
	const string& GetName() const { return m_name; }
	void SetName(const string &name) { m_name = name; }

	RFL_Type GetType() const { return m_type; }

	virtual void Read(IRFL_Parser* doc,  CReflectedBase *obj ) = 0;
    virtual void Write(IRFL_Parser* doc, CReflectedBase *obj ) = 0;
	virtual const char* GetTypeAsString() = 0;	
private:	
    size_t m_memberOffset;    
	size_t m_size;
    string m_name;	
	RFL_Type m_type;

};

class CReflectionMapEntry
{
public:
    CReflectionMapEntry( CPropertyBase *prop ) : m_property(prop){}
    ~CReflectionMapEntry(){}
    CPropertyBase *GetProperty() { return m_property; }
private:
    CPropertyBase *m_property;
};
