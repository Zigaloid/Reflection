#pragma once

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

#include "Reflection/Reflection.h"
#include "rapidjson/include/prettywriter.h"
#include "rapidjson/include/stringbuffer.h"
#include "rapidjson/include/Document.h"

using namespace rapidjson;
using std::ifstream;
using std::ofstream;
using std::vector;

#define JTAG_ELELMENTS  "Elements"
#define JTAG_TYPE       "Type"
#define JTAG_SIZE       "Size"
#define JTAG_VALUE      "Value"
#define JTAG_ROOT       "RootObject"

class CRFL_JSONParser : public IRFL_Parser
{
public:   
    void ReadFloat(const CPropertyBase& property, CReflectedBase* obj)
    {        
        ReflectionDebug.Assert(obj, "Null Ptr passed to function.\n");
        Value* valueMember = FindValue(property);
        if (valueMember)
        {
            float* value = reinterpret_cast<float*>(property.GetAddress(obj));
            *value = valueMember->GetFloat();
        }
    }
    void ReadInt(const CPropertyBase& property, CReflectedBase* obj)
    {     
        Value* valueMember = FindValue(property);        
        if (valueMember)
        {
            int* value = reinterpret_cast<int*>(property.GetAddress(obj));
            *value = valueMember->GetInt();
        }
    }
    void ReadString(const CPropertyBase& property, CReflectedBase* obj)
    {                
        Value* valueMember = FindValue(property);
        if (valueMember)
        {
            string* value = reinterpret_cast<string*>(property.GetAddress(obj));
            *value = valueMember->GetString();
        }
    }
    void ReadMatrix4(const CPropertyBase& property, CReflectedBase* obj)
    {        
        Value* valueMember = FindValue(property);        
        if (valueMember && valueMember->IsArray())
        {
            CMatrix4* value = reinterpret_cast<CMatrix4*>(property.GetAddress(obj));
            float* floatArray = value->GetData();
            for (uint32_t i = 0; i < valueMember->GetArray().Size(); i++)
            {
                floatArray[i] = valueMember->GetArray()[i].GetFloat();
            }
        }            
    }
    void ReadVector4(const CPropertyBase& property, CReflectedBase* obj)
    {       
        Value* valueMember = FindValue(property);        
        if (valueMember && valueMember->IsArray())
        {
            CVector4* value = reinterpret_cast<CVector4*>(property.GetAddress(obj));
            float* floatArray = value->GetData();
            for (uint32_t i = 0; i < valueMember->GetArray().Size(); i++)
            {
                floatArray[i] = valueMember->GetArray()[i].GetFloat();
            }
        }
    }
    void ReadObject(const CPropertyBase& property, CReflectedBase* obj)
    {
        Value* member = &m_currentObject->FindMember(property.GetName().c_str())->value;

        Value* valueMember = FindValue(property);
        if (valueMember)
        {
            Push(member);
            CReflectedBase* subObj = reinterpret_cast<CReflectedBase*>(property.GetAddress(obj));
            subObj->ReadMembers(*this);
            Pop();
        }
    }
    void ReadObjectPtr(const CPropertyBase& property, CReflectedBase* obj)
    {
        Value* member = &m_currentObject->FindMember(property.GetName().c_str())->value;
        Value* valueMember = FindValue(property);
        if (valueMember)
        {
            Push(member);
            CReflectedBase** subObjPtr = (CReflectedBase**)(property.GetAddress(obj));
            CReflectedBase* subObj = ClassFactory::createObject(valueMember->GetString());
            subObj->ReadMembers(*this);
            *subObjPtr = subObj;
            Pop();
        }
    }
    void ReadObjectPtrArray(const CPropertyBase& property, CReflectedBase* obj)
    {
        Value* member = &m_currentObject->FindMember(property.GetName().c_str())->value;   
        Value* valueMember = FindValue(property);
        if( valueMember )
        {
            vector<CReflectedBase*>* vectorOfObject = reinterpret_cast<vector<CReflectedBase*>*>(property.GetAddress(obj));
            Value* arrayMember = &member->FindMember(JTAG_ELELMENTS)->value;
            if (arrayMember->IsArray())
            {
                for (auto& element : arrayMember->GetArray())
                {
                    Push(&element);
                    Value* valueMember = &element.FindMember(JTAG_VALUE)->value;
                    CReflectedBase* subObj = ClassFactory::createObject(valueMember->GetString());
                    subObj->ReadMembers(*this);
                    vectorOfObject->push_back(subObj);
                    Pop();
                }
            }
        }        
    }    
    void WriteFloat(const CPropertyBase& property, CReflectedBase* obj)
    {
        BeginObject();
        float* value = reinterpret_cast<float*>(property.GetAddress(obj));
        Value name;
        name.SetString(JTAG_VALUE,m_allocator);
        m_currentObject->AddMember(name, *value, m_allocator);
        WriteProperties(property);
        EndObject(property.GetName().c_str());
    }
    void WriteInt(const CPropertyBase& property, CReflectedBase* obj)
    {
        BeginObject();
        int* value = reinterpret_cast<int*>(property.GetAddress(obj));
        Value name;
        name.SetString(JTAG_VALUE, m_allocator);
        m_currentObject->AddMember(name, *value, m_allocator);
        WriteProperties(property);
        EndObject(property.GetName().c_str());
    }
    void WriteString(const CPropertyBase& property, CReflectedBase* obj)
    {
        BeginObject();
        string* string = reinterpret_cast<std::string*>(property.GetAddress(obj));
        Value name;
        name.SetString(JTAG_VALUE, m_allocator);
        Value item;
        item.SetString(string->c_str(), m_allocator);
        m_currentObject->AddMember(name, item, m_allocator);
        WriteProperties(property);
        EndObject(property.GetName().c_str());
    }
    void WriteMatrix4(const CPropertyBase& property, CReflectedBase* obj)
    {
        BeginObject();
        CMatrix4* matrix4 = reinterpret_cast<CMatrix4*>(property.GetAddress(obj));        
        Value array;
        array.SetArray();
        for (auto &element : matrix4->GetData())
        {            
            array.PushBack(Value(element).Move(), m_allocator);
        }
        Value name;
        name.SetString(JTAG_VALUE, m_allocator);
        m_currentObject->AddMember(name, array, m_allocator);
        WriteProperties(property);
        EndObject(property.GetName().c_str());
    }
    void WriteVector4(const CPropertyBase& property, CReflectedBase* obj)
    {
        BeginObject();
        CVector4* vector4 = reinterpret_cast<CVector4*>(property.GetAddress(obj));
        Value array;
        array.SetArray();
        for (auto& element : vector4->GetData())
        {
            array.PushBack(Value(element).Move(), m_allocator);
        }
        Value name;
        name.SetString(JTAG_VALUE, m_allocator);
        m_currentObject->AddMember(name, array, m_allocator);
        WriteProperties(property);
        EndObject(property.GetName().c_str());
    }
    void WriteObject(const CPropertyBase& property, CReflectedBase* obj)
    {        
        CReflectedBase* subObj = reinterpret_cast<CReflectedBase*>(property.GetAddress(obj));
        BeginObject(subObj->GetClassName());
        subObj->WriteMembers(*this);
        WriteProperties(property);        
        EndObject(property.GetName().c_str());
    }
    void WriteObjectPtr(const CPropertyBase& property, CReflectedBase* obj)
    {        
        CReflectedBase** subObjPtr = (CReflectedBase**)(property.GetAddress(obj));
        CReflectedBase* subObj = *subObjPtr;        
        BeginObject(subObj->GetClassName());
        subObj->WriteMembers(*this);        
        WriteProperties(property);        
        EndObject(property.GetName().c_str());
    }
    void WriteObjectPtrArray(const CPropertyBase& property, CReflectedBase* obj)
    {
        std::vector<CReflectedBase*>* objVector = (std::vector<CReflectedBase*>*)(property.GetAddress(obj));        
        BeginObject(obj->GetClassName());
        BeginArray();                
        for (auto element : *objVector)
        {
            BeginObject(element->GetClassName());
            element->WriteMembers(*this);
            EndObject(property.GetName().c_str());
        }        
        EndArray();
        WriteProperties(property);
        EndObject(property.GetName().c_str());
    }

    bool WriteProperties(const CPropertyBase& property)
    {           
        Value name;
        Value memberType;
        memberType.SetInt((int)property.GetType());
        m_currentObject->AddMember(JTAG_TYPE, memberType, m_allocator);
        Value memberSize;
        memberSize.SetInt((int)property.GetSize());
        m_currentObject->AddMember(JTAG_SIZE, memberSize, m_allocator);        
        return true;
    }   

    void Push(Value *value)
    {
        m_objectStack.push_back(m_currentObject);
        m_currentObject = value;
    }
    void Pop()
    {
        m_currentObject = m_objectStack.back();
        m_objectStack.pop_back();
    }
    void BeginArray()
    {
        Value* newValue = new Value;
        newValue->SetArray();
        Push(newValue);
    }
    void EndArray()
    {
        Value name;
        name.SetString(JTAG_ELELMENTS, m_allocator);
        Value* child = m_currentObject;
        Pop();
        m_currentObject->AddMember(name, *child, m_allocator);
        delete child;
    }
    void BeginObject(const char *className = "")
    {        
        Value* newValue = new Value;
        newValue->SetObject();
        Push(newValue);
        if (string(className).size())
        {
            Value name;
            name.SetString(className, m_allocator);
            m_currentObject->AddMember(JTAG_VALUE, name, m_allocator);
        }
    }
    void EndObject(const char* memberName)
    {
        Value name;
        name.SetString(memberName, m_allocator);
        Value* child = m_currentObject;
        Pop();
        if( m_currentObject->IsArray() )
            m_currentObject->PushBack(*child, m_allocator);
        else
            m_currentObject->AddMember(name, *child, m_allocator);
        delete child;
    }
    bool BeginOutput(const string &fileName)
    {           
        m_currentObject = &m_document.SetObject();
        BeginObject();
        m_outFile.open(fileName);        
        return m_outFile.good();
    }
    void EndOutput()
    {   
        EndObject(JTAG_ROOT);
        StringBuffer buffer;
        PrettyWriter<StringBuffer> writer(buffer);        
        m_document.Accept(writer);
        m_outFile << buffer.GetString() << endl;
        m_outFile.close();        
    }
    bool BeginInput(const string& fileName)
    {        
        m_inFile.open(fileName, std::ios::binary);      
        if (m_inFile.good())
        {
            std::stringstream buffer;
            buffer << m_inFile.rdbuf();
            m_document.Parse(buffer.str().c_str());
            m_currentObject = nullptr;
            m_currentObject = &m_document.FindMember(JTAG_ROOT)->value;
            return true;
        }
        return false;
    }
    void EndInput()
    {        
        m_inFile.close();
    }
    bool ValidateProperties(const CPropertyBase& property, Value* objectValue)
    {
        if (objectValue->IsObject())
        {
            if (objectValue->HasMember(JTAG_SIZE))
            {
                Value* sizeMember = &objectValue->FindMember(JTAG_SIZE)->value;
                if (property.GetSize() == sizeMember->GetInt())
                {
                    if (objectValue->HasMember(JTAG_TYPE))
                    {
                        Value* typeMember = &objectValue->FindMember(JTAG_TYPE)->value;
                        if (property.GetType() == typeMember->GetInt())
                        {
                            return true;
                        }
                    }
                }
            }
        }
        ReflectionDebug.print("Bad size or Type found while reading object.\n");

        return false;
    }
    Value* FindValue(const CPropertyBase& property)
    {
        Value* member = &m_currentObject->FindMember(property.GetName().c_str())->value;
        if (member)
        {
            if (ValidateProperties(property, member))
            {
                Value* valueMember = &member->FindMember(JTAG_VALUE)->value;
                return valueMember;
            }
        }
        return nullptr;
    }
private:
    ofstream m_outFile;
    ifstream m_inFile;    
    Document m_document;
    vector<Value*> m_objectStack;
    Value *m_currentObject;
    Document::AllocatorType& m_allocator = m_document.GetAllocator();
    Value& m_objectArray = m_document.SetArray();
    Writer<StringBuffer>* m_writer;
    StringBuffer m_stringBuffer;
};
