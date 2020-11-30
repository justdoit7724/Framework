#pragma once
#include <string>

// hide getting file name and line number 
// and just write log
#define FileLog LogMgr::SrcPos(__FILE__, __LINE__).Log

class DXLIB_DLL LogMgr
{
public:
	LogMgr();

	void Log(std::string strLogKind, int level, std::string strFmt, ...);

	static LogMgr& SrcPos(std::string strFileLoc, int nLine);

private:
	std::string m_strFileLoc;
	int m_nLine;


};

