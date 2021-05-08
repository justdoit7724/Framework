#pragma once

/*
DESCRIPTION
-read datas from x files
-form data structures with composite pattern(XComponent class)
-return XComponent class when datas is needed
FEATURE
-data is readonly except in XReader class
-Composite pattern used
*/

class XComponent
{
public:
	virtual int Size() { return 0; }
	const BOOL m_bComposite;
	const std::string m_name;
protected:
	friend class XFrame;
	friend class XReader;
	XComponent(BOOL bComposite, std::string name);
	virtual ~XComponent();
	virtual XComponent* GetChild(std::string name) { return nullptr; }
	virtual BOOL AddComponent(XComponent* c) { return FALSE; };
};
class XFrame : public XComponent
{
public:
	int Size()override; 

private:
	friend class XReader;
	XFrame(std::string name);
	~XFrame();
	XComponent* GetChild(std::string name) override;
	BOOL AddComponent(XComponent* c)override;
	std::vector<XComponent*> m_components;
};
class XValue : public XComponent
{
public:
	const std::vector<float>& GetFloats();
	const std::vector<int>& GetInts();
	const std::vector<std::string>& GetStrings();

private:
	friend class XReader;
	XValue(std::string name, std::vector<float>&& data);
	XValue(std::string name, std::vector<int>&& data);
	XValue(std::string name, std::vector<std::string>&& data);
	~XValue();
	union {
		std::vector<float> m_floats;
		std::vector<int> m_ints;
		std::vector<std::string> m_strings;
	};
};

class XReader
{
public:
	static BOOL Read(std::string path);
	static XComponent* Get(const std::vector<std::string>&& list);

private:
	static std::vector<XComponent*> m_list;
	static void Read(std::ifstream& file, std::vector<XComponent*>& comps);
};

