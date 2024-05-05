// Reflection.cpp : Defines the entry point for the console application.
//
#include <vector>
#include "Reflection\Reflection.h"
#include "ClassFactory\ClassFactory.h"
#include "Reflection\BinaryParser\BinaryParser.h"
#include "Reflection\JSONParser\JSONParser.h"
#include "Reflection\BinaryParser\BinaryParser.h"

DebugChannels::CDebugChannel ReflectionDebug("Reflection");

void CReflectedBase::Read(const char * fileName)
{	
	ReflectionDebug.Assert(fileName, "Read called with a null string pointer.\n");
	CRFL_JSONParser doc;
	doc.BeginInput(fileName);	
	ReadMembers(doc);
	doc.EndInput();
}

void CReflectedBase::Write(const char * fileName)
{
	ReflectionDebug.Assert(fileName, "Write called with a null string pointer.\n");
	CRFL_JSONParser doc;	
	doc.BeginOutput(fileName);
	WriteMembers(doc);	
	doc.EndOutput();
}

void CReflectedBase::InternalReadMembers(vector<CReflectionMapEntry> &reflectionMap, IRFL_Parser& doc)
{	
	for( auto &property : reflectionMap)
	{
		property.GetProperty()->Read(&doc, this);
	}	
}

void CReflectedBase::InternalWriteMembers(vector<CReflectionMapEntry>& reflectionMap, IRFL_Parser& doc )
{
	for (auto& property : reflectionMap)
	{
		property.GetProperty()->Write(&doc, this);
	}
}

