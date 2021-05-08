#include "stdafx.h"
#include "XReader.h"

#define TEMPLATE				"template"
#define FRAME					"Frame"
#define FRAMETRANSFORMMATRIX	"FrameTransformMatrix"
#define MESH					"Mesh"
#define MESHNORMALS				"MeshNormals"

XComponent::XComponent(BOOL bComposite, std::string name)
	:m_bComposite(bComposite),
	m_name(name)
{
}


XComponent::~XComponent()
{
}

XFrame::XFrame(std::string name)
	:XComponent(TRUE,name)
{
}

XFrame::~XFrame()
{
	for (auto data : m_components)
	{
		delete data;
	}
}

BOOL XFrame::AddComponent(XComponent* c)
{
	m_components.push_back(c);

	return TRUE;
}

int XFrame::Size()
{
	return m_components.size();
}
XComponent* XFrame::GetChild(std::string name)
{
	for (auto comp : m_components)
	{
		if (comp->m_name == name)
			return comp;
	}

	return nullptr;
}

XValue::XValue(std::string name, std::vector<float>&& data)
	:XComponent(FALSE, name),
	m_floats(data)
{
}
XValue::XValue(std::string name, std::vector<int>&& data)
	: XComponent(FALSE, name),
	m_ints(data)
{
}
XValue::XValue(std::string name, std::vector<std::string>&& data)
	: XComponent(FALSE, name),
	m_strings(data)
{
}
XValue::~XValue()
{

}

const std::vector<float>& XValue::GetFloats()
{
	return m_floats;
}

const std::vector<int>& XValue::GetInts()
{
	return m_ints;
}

const std::vector<std::string>& XValue::GetStrings()
{
	return m_strings;
}

// remove front and back space
void Pruning(std::string full, std::string& core)
{
	core = "";

	for (int i = 0; i < full.size(); ++i)
	{
		if (full[i] != ' ')
		{
			core = full.substr(i);
			break;
		}
	}
	int backSpaceCount = 0;
	for (int i = core.size() - 1; i >= 0; --i)
	{
		if (core[i] != ' ')
		{
			core = core.substr(0, core.size() - backSpaceCount);
			break;
		}
		backSpaceCount++;
	}
}
void GetChunks(std::string line, std::vector<std::string>& list)
{
	list.clear();

	Pruning(line, line);

	int startPos = 0;
	for (int i = startPos; i < line.size(); ++i)
	{
		if (line[i] == ' ' && startPos < i)
		{
			list.push_back(line.substr(startPos, i - startPos));
			startPos = i + 1;
		}
		else if (line[i] == ',' && startPos < i)
		{
			list.push_back(line.substr(startPos, i - startPos));
			startPos = i + 1;
		}
		else if (line[i] == ';' && startPos < i)
		{
			list.push_back(line.substr(startPos, i - startPos));
			startPos = i + 1;
		}
		else if (line[i] == '{' || line[i] == '}')
		{
			if (startPos < i)
			{
				list.push_back(line.substr(startPos, i - startPos));
			}
			std::string str;
			str.push_back(line[i]);
			list.push_back(str);
			startPos = i + 1;
		}
	}

	while (true)
	{
		size_t spacePos = line.find(' ');
		if (spacePos == std::string::npos)
		{
			list.push_back(line);
			break;
		}

		list.push_back(line.substr(0, spacePos));
		line = line.substr(spacePos + 1);
	}
}

std::vector<XComponent*> XReader::m_list;
BOOL XReader::Read(std::string path)
{
	for (auto comp : m_list)
		delete comp;

	std::string line;
	std::ifstream file(path);
	if (!file.is_open())
		return FALSE;

	// read id
	std::vector<std::string> chunks;
	GetChunks(line, chunks);

	// read values
	Read(file, m_list);
	
	file.close();

	return 0;
}

XComponent* XReader::Get(const std::vector<std::string>&& list)
{
	if (list.empty())
		return nullptr;

	XComponent* curComp = nullptr;
	for (auto comp : m_list)
	{
		if (comp->m_name == list.front())
		{
			curComp = comp;
			break;
		}
	}
	if (!curComp)
		return nullptr;
	
	for (int i=1; i<list.size(); ++i)
	{
		XComponent* child = curComp->GetChild(list[i]);
		if (!child)
		{
			return curComp;
		}

		curComp = child;
	}

	return curComp;
}

void XReader::Read(std::ifstream& file, std::vector<XComponent*>& comps)
{
	comps.clear();

	std::string line;
	while (getline(file, line))
	{
		std::vector<std::string> chunks;
		GetChunks(line, chunks);
		if (chunks.empty()) // jump down
		{
			continue;
		}
		else if (chunks.front() == "}") // end of recursive
		{
			break;
		}

		std::string curKey;
		std::string curName;
		if (chunks.size() == 2)// identical
		{
			curKey = chunks[0];
			curName = chunks[0];
		}
		else
		{
			curKey = chunks[0];
			curName = chunks[1];
		}
		if (curKey == TEMPLATE) // skip template
		{
			while (getline(file, line))
			{
				if (line.find('}') != std::string::npos)
				{
					break;
				}
			}
		}
		else if (curKey == FRAME)
		{
			std::vector<XComponent*> frameComps;
			Read(file, frameComps);
			if (frameComps.size())
			{
				XComponent* frameComp = new XFrame(curName);
				for (auto comp : frameComps)
				{
					frameComp->AddComponent(comp);
				}
				comps.push_back(frameComp);
			}
		}
		else if (curKey == FRAMETRANSFORMMATRIX)
		{
			std::vector<float> data;
			for (int i = 0; i < 4; ++i)
			{
				getline(file, line);
				GetChunks(line, chunks);
				for (int j = 0; j < 4; ++j)
				{
					std::string strData = chunks[j];
					data.push_back(stoi(strData));
				}
			}
			XComponent* newValue = new XValue(curName, std::move(data));
			comps.push_back(newValue);

			getline(file, line); // end
		}
		else if (curKey == MESH)
		{
			XComponent* meshComp = new XFrame(curName);
			//vertex count & vertice
			{
				getline(file, line);
				GetChunks(line, chunks);
				int vertCount = stoi(chunks[0]);
				XComponent* vertexCountComp = new XValue("nVertices", std::vector<int>{vertCount});
				meshComp->AddComponent(vertexCountComp);

				std::vector<float> vertice;
				for (int i = 0; i < vertCount; ++i)
				{
					getline(file, line);
					GetChunks(line, chunks);
					vertice.push_back(stoi(chunks[0]));//x
					vertice.push_back(stoi(chunks[1]));//y
					vertice.push_back(stoi(chunks[2]));//z
				}
				XComponent* verticeComp = new XValue("vertices", std::move(vertice));
				meshComp->AddComponent(verticeComp);
			}

			//face count & faces
			{
				getline(file, line);
				GetChunks(line, chunks);
				int faceCount = stoi(chunks[0]);
				XComponent* faceCountComp = new XValue("nFaces", std::vector<int>{faceCount});
				meshComp->AddComponent(faceCountComp);

				std::vector<int> faces;
				for (int i = 0; i < faceCount; ++i)
				{
					getline(file, line);
					GetChunks(line, chunks);
					faces.push_back(stoi(chunks[1]));//x
					faces.push_back(stoi(chunks[2]));//y
					faces.push_back(stoi(chunks[3]));//z
				}
				XComponent* facesComp = new XValue("faces", std::move(faces));
				meshComp->AddComponent(facesComp);
			}

			//rest
			std::vector<XComponent*> extraComposite;
			Read(file, extraComposite);
			for (auto extra : extraComposite)
			{
				meshComp->AddComponent(extra);
			}
		}
		else if (curKey == MESHNORMALS)
		{
			XComponent* normComp = new XFrame(curName);
			//normals
			{
				getline(file, line);
				GetChunks(line, chunks);
				int normCount = stoi(chunks[0]);
				XComponent* normCountComp = new XValue("nNormals", std::vector<int>{normCount});
				normComp->AddComponent(normCountComp);

				std::vector<int> norms;
				for (int i = 0; i < normCount; ++i)
				{
					getline(file, line);
					GetChunks(line, chunks);
					norms.push_back(stoi(chunks[1]));//x
					norms.push_back(stoi(chunks[2]));//y
					norms.push_back(stoi(chunks[3]));//z
				}
				XComponent* normComp = new XValue("normals", std::move(norms));
				normComp->AddComponent(normComp);
			}
			//normal faces
			{
				getline(file, line);
				GetChunks(line, chunks);
				int faceCount = stoi(chunks[0]);
				XComponent* faceCountComp = new XValue("nFaceNormals", std::vector<int>{faceCount});
				normComp->AddComponent(faceCountComp);

				std::vector<int> faces;
				for (int i = 0; i < faceCount; ++i)
				{
					getline(file, line);
					GetChunks(line, chunks);
					faces.push_back(stoi(chunks[1]));//x
					faces.push_back(stoi(chunks[2]));//y
					faces.push_back(stoi(chunks[3]));//z
				}
				XComponent* normComp = new XValue("faceNormals", std::move(faces));
				normComp->AddComponent(normComp);
			}
		}
	}
}


