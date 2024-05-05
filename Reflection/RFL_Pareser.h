#pragma once

#include <string>
#include "ReflectionMap.h"

using std::string;

class IRFL_Parser
{
public:
    virtual void WriteFloat(const CPropertyBase& property, CReflectedBase* obj) = 0;
    virtual void ReadFloat(const CPropertyBase& property, CReflectedBase* obj) = 0;
    virtual void WriteInt(const CPropertyBase& property, CReflectedBase* obj) = 0;
    virtual void ReadInt(const CPropertyBase& property, CReflectedBase* obj) = 0;
    virtual void ReadString(const CPropertyBase& property, CReflectedBase* obj) = 0;
    virtual void WriteString(const CPropertyBase& property, CReflectedBase* obj) = 0;
    virtual void WriteMatrix4(const CPropertyBase& property, CReflectedBase* obj) = 0;
    virtual void ReadMatrix4(const CPropertyBase& property, CReflectedBase* obj) = 0;
    virtual void WriteVector4(const CPropertyBase& property, CReflectedBase* obj) = 0;
    virtual void ReadVector4(const CPropertyBase& property, CReflectedBase* obj) = 0;
    virtual void WriteObject(const CPropertyBase& property, CReflectedBase* obj) = 0;
    virtual void ReadObject(const CPropertyBase& property, CReflectedBase* obj) = 0;
    virtual void WriteObjectPtr(const CPropertyBase& property, CReflectedBase* obj) = 0;
    virtual void ReadObjectPtr(const CPropertyBase& property, CReflectedBase* obj) = 0;
    virtual void WriteObjectPtrArray(const CPropertyBase& property, CReflectedBase* obj) = 0;
    virtual void ReadObjectPtrArray(const CPropertyBase& property, CReflectedBase* obj) = 0;
    virtual bool BeginOutput(const string &fileName) = 0;
    virtual void EndOutput() = 0;
    virtual bool BeginInput(const string& fileName) = 0;
    virtual void EndInput() = 0;
    virtual void BeginObject(const char* className = "") = 0;
    virtual void EndObject(const char* memberName) = 0;
    virtual bool WriteProperties(const CPropertyBase& property) = 0;
    virtual void BeginArray() = 0;
    virtual void EndArray() = 0;
};
