#include "DebugChannel.h"

using namespace DebugChannels;

CDebugChannelManager &CDebugChannelManager::GetInstance()
{
	static CDebugChannelManager theManager;
	return theManager;
}

CDebugChannel::CDebugChannel(const string& name) : m_name(name)
{
	CDebugChannelManager::GetInstance().AddChannel(this);
	OpenLog();
}
CDebugChannel::~CDebugChannel()
{
	CDebugChannelManager::GetInstance().RemoveChannel(this);
	CloseLog();
}

string CDebugChannel::FormatString(const char *fmt, va_list vl)
{
	static const int bufferSize = 1024;
	static char buffer[bufferSize];
	string result;
	int nsize = vsnprintf(buffer, bufferSize, fmt, vl);
	if (bufferSize <= nsize)
	{
		nsize++;
		char* buffer = new char[nsize]; //+1 for /0
		nsize = vsnprintf(buffer, nsize, fmt, vl);
		result = buffer;
		delete[] buffer;
	}
	else
	{
		result = buffer;
	}
	return buffer;
}

void CDebugChannel::print(const string message)
{
	if (m_outputflags & OF_StdOut)
		std::cout << message;
	if (m_outputflags & OF_Log && m_outFile.is_open())
		m_outFile << message;
	
}

void CDebugChannel::printf(const char *format,...)
{	
	va_list vl;
	va_start(vl, format);
	string formated = FormatString(format, vl);	
	this->print(formated);
	va_end(vl);	
}

bool CDebugChannel::Assert(bool condition, const char* format, ...)
{
	va_list vl;
	va_start(vl, format);
	string formated = FormatString(format, vl);
	va_end(vl);
	if (condition == false)
	{
		print(formated.c_str());
		assert(condition);
	}
	return condition; 
}
