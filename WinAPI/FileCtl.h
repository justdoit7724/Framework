#pragma once
class FileCtl
{
public:
	static BOOL GetAll(std::string folder, std::vector<std::string>& list);
	static BOOL GetFiles(std::string folder, std::vector<std::string>& list);
	static BOOL GetTitle(std::string path, std::string& title);
	static BOOL FindFile(std::string& file);
	static BOOL GetExt(std::string file, std::string& ext);
	static BOOL IsFile(std::string file);
};

