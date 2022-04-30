#include "stdafx.h"
#include "FileCtl.h"
#include <shobjidl.h> 
#include <filesystem>

BOOL FileCtl::GetAll(std::string folder, std::vector<std::string>& list)
{
	list.clear();

	for (const auto& file : std::filesystem::directory_iterator(folder))
	{
		std::wstring totalPathW = file.path().c_str();
		std::string totalPathA = std::string(totalPathW.begin(), totalPathW.end());
		std::string tmp;
		if (!GetTitle(totalPathA, tmp))
			continue;

		list.push_back(totalPathA);
	}

	return list.size();
}

BOOL FileCtl::GetFiles(std::string folder, std::vector<std::string>& list)
{
	list.clear();

	for (const auto& file : std::filesystem::directory_iterator(folder))
	{
		std::wstring totalPathW = file.path().c_str();
		std::string totalPathA = std::string(totalPathW.begin(), totalPathW.end());
		std::string tmp;
		if (!IsFile(totalPathA))
			continue;
		
		list.push_back(totalPathA);
	}

	return list.size();
}

BOOL FileCtl::GetTitle(std::string path, std::string& title)
{
	std::string path2 = path;

    title = "";

	int firstPos = -1;
	for (int i = path2.size() - 1; i >= 0; --i)
	{
		if (path2[i] == '\\' || path2[i] == '/')
		{
			firstPos = i + 1;
			break;
		}
	}
	if (firstPos >= 0)
		title = path2.substr(firstPos);
	else
		title = path2;

	return TRUE;
}

BOOL FileCtl::FindFile(std::string& file)
{
	file = "";

	HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED |
		COINIT_DISABLE_OLE1DDE);
	if (FAILED(hr))
		return FALSE;

	IFileOpenDialog* pFileOpen;

	// Create the FileOpenDialog object.
	hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL,
		IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));

	if (SUCCEEDED(hr))
	{
		hr = pFileOpen->Show(NULL);

		if (SUCCEEDED(hr))
		{
			IShellItem* pItem;
			hr = pFileOpen->GetResult(&pItem);
			if (SUCCEEDED(hr))
			{
				PWSTR pszFilePath;
				hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

				// Display the file name to the user.
				if (SUCCEEDED(hr))
				{
					std::wstring pathW = (std::wstring)pszFilePath;
					file = std::string(pathW.begin(), pathW.end());
					CoTaskMemFree(pszFilePath);
				}
				pItem->Release();
			}
		}
		pFileOpen->Release();
	}
	CoUninitialize();
	
	return TRUE;
}

BOOL FileCtl::GetExt(std::string file, std::string& ext)
{
	std::string file2 = file;

	ext = "";

	auto dotPos = file2.rfind('.');
	if (dotPos == std::string::npos)
		return FALSE;
	ext = file2.substr(dotPos + 1);
	if (ext.find('.') != std::string::npos)
		return FALSE;

	return ext.size();
}

BOOL FileCtl::IsFile(std::string file)
{
	if (!GetTitle(file, file))
		return FALSE;

	if (!GetExt(file, file))
		return FALSE;

	return TRUE;
}
