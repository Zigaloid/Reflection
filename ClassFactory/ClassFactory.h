#ifndef CLASSFACTORY_H
#define CLASSFACTORY_H

#include <map>
#include <string>
using std::map;
using std::string;

class CReflectedBase;

typedef CReflectedBase* (*createFunctionPtr)();
typedef map<string, createFunctionPtr> tClassMap;

class ClassFactory
{
public:
    ClassFactory( string name, createFunctionPtr creatFunction );
    static CReflectedBase *createObject(const char *);
private:
    static tClassMap *s_classFactoryMap;    
};

#endif