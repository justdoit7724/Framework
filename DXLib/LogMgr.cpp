#include "pch.h"
#include "LogMgr.h"
#include <Windows.h>
#include <stdarg.h> 
#include <fileapi.h>
#include <stdio.h>
#include <varargs.h>

#define PATH_LOG "C:\\FWLog"

LogMgr::LogMgr()
{
}

void LogMgr::Log(std::string strLogKind, int level, std::string strFmt, ...)
{
	char rgcMsgBuf[512];
	char rgcMsgBuf_sub[512];

	std::string strLogDirectory;
	SYSTEMTIME systemTime;

	GetLocalTime(&systemTime);

	const std::string middle = "\\";


	// 抗寇贸府
	strLogDirectory = PATH_LOG;
	CreateDirectoryA(strLogDirectory.c_str(), NULL);
	strLogDirectory += middle + std::to_string(systemTime.wYear);
	CreateDirectoryA(strLogDirectory.c_str(), NULL);
	strLogDirectory += middle + std::to_string(systemTime.wMonth) + "_" + std::to_string(systemTime.wDay);
	CreateDirectoryA(strLogDirectory.c_str(), NULL);

	// 抗寇贸府
	for (int i = m_strFileLoc.size() - 1; i >= 0; i--)
	{
		if (m_strFileLoc[i] == '\\')
		{
			m_strFileLoc.substr((size_t)i + 1);
		}
	}

	va_list arg;
	va_start(arg, strFmt);
	vsprintf_s(rgcMsgBuf, ARRAYSIZE(rgcMsgBuf), strFmt.c_str(), arg);
	va_end(arg);

	strcpy_s(rgcMsgBuf_sub, rgcMsgBuf);

	sprintf_s(rgcMsgBuf, "%s <<%d:%d:%d>> [dbg]Lv:%d, %s(%d)", rgcMsgBuf_sub, systemTime.wHour, systemTime.wMinute, systemTime.wSecond, level, m_strFileLoc.c_str(), m_nLine);


	FILE* stream;
	std::string strFinalDir = strLogDirectory + middle + strLogKind;
	if ((stream = _fsopen(strFinalDir.c_str(), "at", _SH_DENYNO)) != NULL)
	{
		fprintf(stream, "%s\n", rgcMsgBuf);
		fclose(stream);
	}
}


LogMgr& LogMgr::SrcPos(std::string strFileLoc, int nLine)
{
	LogMgr* log = new LogMgr();
	log->m_strFileLoc = strFileLoc;
	log->m_nLine = nLine;
	return *log;
}
