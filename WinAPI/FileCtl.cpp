#include "stdafx.h"
#include "FileCtl.h"
#include <shobjidl.h> 
#include <filesystem>

BOOL FileCtl::GetFiles(std::string folder, std::vector<std::string>& list)
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

BOOL FileCtl::GetTitle(std::string path, std::string& title)
{
    title = "";

	std::string tmpPath = path;
	int firstPos = -1;
	int lastPos = -1;
	for (int i = tmpPath.size() - 1; i >= 0; --i)
	{
		if (tmpPath[i] == '\\' || tmpPath[i] == '/')
		{
			firstPos = i + 1;
			break;
		}
		else if (tmpPath[i] == '.')
		{
			lastPos = i - 1;
		}
	}
	if (firstPos == -1 || lastPos == -1 || firstPos > lastPos)
		return FALSE;

	title = tmpPath.substr(firstPos, lastPos - firstPos + 1);
	if (title == "")
		return FALSE;

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
	ext = "";

	auto dotPos = file.find('.');
	if (dotPos == std::string::npos)
		return FALSE;
	ext = file.substr(dotPos + 1);
	if (ext.find('.') != std::string::npos)
		return FALSE;

	return ext.size();
}
