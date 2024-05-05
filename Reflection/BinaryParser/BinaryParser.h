#pragma once


#include <iostream>
#include <vector>
#include <fstream>
#include "Reflection/Reflection.h"


using std::ifstream;
using std::ofstream;
using std::vector;

class CRFL_BinaryParser : public IRFL_Parser
{
public:   

    void ReadFloat(const CPropertyBase& property, CReflectedBase* obj)
    {
        if(ValidateProperties(property))
            ReadRaw(property, obj);
    }
    void ReadInt(const CPropertyBase& property, CReflectedBase* obj)
    {
        if( ValidateProperties(property) )
            ReadRaw(property, obj);
    }
    void ReadString(const CPropertyBase& property, CReflectedBase* obj)
    {
        if (ValidateProperties(property))
        {
            string* string = reinterpret_cast<std::string*>(property.GetAddress(obj));
            ReadString(*string);
        }
    }
    void ReadMatrix4(const CPropertyBase& property, CReflectedBase* obj)
    {
        if (ValidateProperties(property))
            ReadRaw(property, obj);
    }
    void ReadVector4(const CPropertyBase& property, CReflectedBase* obj)
    {
        if (ValidateProperties(property))
            ReadRaw(property, obj);
    }
    void ReadObject(const CPropertyBase& property, CReflectedBase* obj)
    {   
        if (ValidateProperties(property))
        {
            CReflectedBase* subObj = reinterpret_cast<CReflectedBase*>(property.GetAddress(obj));
            subObj->ReadMembers(*this);
        }
    }
    void ReadObjectPtr(const CPropertyBase& property, CReflectedBase* obj)
    {
        if (ValidateProperties(property))
        {
            string className;
            ReadString(className);
            CReflectedBase** subObjPtr = (CReflectedBase**)(property.GetAddress(obj));
            CReflectedBase* subObj = ClassFactory::createObject(className.c_str());
            subObj->ReadMembers(*this);
            *subObjPtr = subObj;
        }
    }
    void ReadObjectPtrArray(const CPropertyBase& property, CReflectedBase* obj)
    {
        if (ValidateProperties(property))
        {
            int arraySize;
            ReadInt(arraySize);
            std::vector<CReflectedBase*>* objVector = (std::vector<CReflectedBase*>*)(property.GetAddress(obj));
            for (int i = 0 ; i < arraySize ; i++)
            {
                string className;
                ReadString(className);
                CReflectedBase* subObj = ClassFactory::createObject(className.c_str());
                subObj->ReadMembers(*this);
                objVector->push_back(subObj);
            }
        }
    }
    void WriteFloat(const CPropertyBase& property, CReflectedBase* obj)
    {
        WriteProperties(property);
        WriteRaw(property, obj);        
    }
    void WriteInt(const CPropertyBase& property, CReflectedBase* obj)
    {
        WriteProperties(property);
        WriteRaw(property, obj);
    }
    void WriteString(const CPropertyBase& property, CReflectedBase* obj)
    {
        WriteProperties(property);
        string* string = reinterpret_cast<std::string*>(property.GetAddress(obj));
        WriteString(*string);
    }
    void WriteMatrix4(const CPropertyBase& property, CReflectedBase* obj)
    {
        WriteProperties(property);
        WriteRaw(property, obj);
    }
    void WriteVector4(const CPropertyBase& property, CReflectedBase* obj)
    {
        WriteProperties(property);
        WriteRaw(property, obj);
    }
    void WriteObject(const CPropertyBase& property, CReflectedBase* obj)
    {
        WriteProperties(property);        
        CReflectedBase* subObj = reinterpret_cast<CReflectedBase*>(property.GetAddress(obj));
        subObj->WriteMembers(*this);
    }
    void WriteObjectPtr(const CPropertyBase& property, CReflectedBase* obj)
    {
        WriteProperties(property);
        CReflectedBase** subObjPtr = (CReflectedBase**)(property.GetAddress(obj));
        CReflectedBase* subObj = *subObjPtr;
        string className = subObj->GetClassName();
        WriteString(className);
        subObj->WriteMembers(*this);
    }
    void WriteObjectPtrArray(const CPropertyBase& property, CReflectedBase* obj)
    {
        WriteProperties(property);
        std::vector<CReflectedBase*>* objVector = (std::vector<CReflectedBase*>*)(property.GetAddress(obj));
        WriteInt((int)objVector->size());
        for (auto element : *objVector)
        {
            string className = element->GetClassName();
            WriteString(className);
            element->WriteMembers(*this);
        }
    }

    bool BeginOutput(const string& fileName)
    {
        m_outFile.open(fileName, std::ios::binary);
        return m_outFile.good();
    }
    void EndOutput()
    {
        m_outFile.close();
    }
    bool BeginInput(const string& fileName)
    {
        m_inFile.open(fileName, std::ios::binary);
        return m_inFile.good();
    }
    void EndInput()
    {
        m_inFile.close();
    }
    void BeginObject(const char* className = "") {}
    void EndObject(const char* memberName) {}
    void BeginArray() {}
    void EndArray() {}


    bool ReadString(string& theString)
    {
        static char tempString[1024];
        size_t length;
        Read(&length, sizeof(size_t));
        Read(&tempString[0], length);
        theString.assign(tempString);
        return true;
    }
    void Write(const void* data, size_t size)
    {
        m_outFile.write(reinterpret_cast<const char*>(data), size);
    }
    void Read(void* data, size_t size)
    {
        m_inFile.read(reinterpret_cast<char*>(data), size);
    }

    bool WriteProperties(const CPropertyBase& property)
    {
        size_t size = property.GetSize();
        Write(&size, sizeof(size_t));
        RFL_Type type = property.GetType();
        Write(&type, sizeof(RFL_Type));
        return true;
    }

    bool ValidateProperties(const CPropertyBase& property)
    {
        size_t size;
        Read(&size, sizeof(size_t));
        RFL_Type type;
        Read(&type, sizeof(RFL_Type));
        if (size != property.GetSize())
        {
            ReflectionDebug.print("Bad Type Size\n");
            return false;
        }
        if (type != property.GetType())
        {
            return false;
        }
        return true;
    }

    bool WriteRaw(const CPropertyBase& property, CReflectedBase *obj)
    {
        //WriteProperties(property);
        char* address = reinterpret_cast<char*>(property.GetAddress(obj));
        Write(address, property.GetSize());
        return true;
    }

    bool ReadRaw(const CPropertyBase& property, CReflectedBase* obj)
    {
        //ValidateProperties(property);
        char* address = reinterpret_cast<char*>(property.GetAddress(obj));
        Read(address, property.GetSize());
        return true;
    }

    bool WriteString( const string theString )
    {        
        size_t length = theString.length()+1;
        Write(&length, sizeof(size_t));
        Write(theString.c_str(), length);
        return true;
    }
 
    void WriteInt(const int &value)
    {
        Write(&value, sizeof(int));
    }
    void ReadInt(int &value)
    {
        Read(&value, sizeof(int));
    }


private:
    ofstream m_outFile;
    ifstream m_inFile;
};