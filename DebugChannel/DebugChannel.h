#pragma once
#include <string>
#include <memory>
#include <map>
#include <vector>
#include <iostream>
#include <fstream>
#include <assert.h>
#include <cstdarg>

using std::string;
using std::ofstream;
using std::vector;
using std::map;

namespace DebugChannels
{

enum
{
	OF_Null = 0x0000,
	OF_StdOut = 0x0001,
	OF_Log = 0x0001,
};

class CDebugChannel
{
public:
	CDebugChannel(const string& name);
	~CDebugChannel();

	void OpenLog()
	{
		string fileName = m_name + ".log";
		m_outFile.open(fileName, std::ios::binary);
	};	
	void CloseLog()
	{
		if( m_outFile.is_open())
			m_outFile.close();
	};			
	void printf(const char* format, ...);
	void print(const string message);
	void SetOutputFlags(int flags)
	{
		m_outputflags = flags;
	}
	bool Assert(bool condition, const char *message, ...);
	const string& GetName() const { return m_name; }
private:	
	string FormatString(const char* fmt, va_list vl);
	const string m_name;
	int m_outputflags = OF_StdOut | OF_Log;
	ofstream m_outFile;
};

class CDebugChannelManager
{
public:
	static CDebugChannelManager& GetInstance();
	void AddChannel(CDebugChannel *channel)
	{
		m_channels[channel->GetName()] = channel;
	}
	void RemoveChannel(CDebugChannel* channel)
	{		
		m_channels.erase(channel->GetName());
	}

private:
	map<const string, CDebugChannel *> m_channels;
};

} // namespace DebugChannels