#include "stdafx.h"
#include "XReader.h"


XComponent::XComponent(BOOL bComposite, std::string key, std::string name)
	:m_bComposite(bComposite),
	m_key(key),
	m_name(name)
{
}


XComponent::~XComponent()
{
}

XFrame::XFrame(std::string key, std::string name)
	:XComponent(TRUE, key,name)
{
}

XFrame::~XFrame()
{
	for (auto components : m_components)
	{
		for (auto comp : components.second)
		{
			delete comp;
		}
	}
	m_components.clear();
}

BOOL XFrame::AddComponent(XComponent* c)
{
	m_components[c->m_key].push_back(c);

	return TRUE;
}

int XFrame::Size()
{
	return m_components.size();
}
std::vector<XComponent*> XFrame::GetChild(std::string key)
{
	if (m_components.find(key) != m_components.end())
		return m_components[key];

	return std::vector<XComponent*>();
}
XValue::XValue(std::string key, std::vector<float>&& data)
	:XComponent(FALSE, key, "Value"),
	m_floats(data)
{
}
XValue::XValue(std::string key, std::vector<int>&& data)
	: XComponent(FALSE, key, "Value"),
	m_ints(data)
{
}
XValue::XValue(std::string key, std::vector<std::string>&& data)
	: XComponent(FALSE, key, "Value"),
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
BOOL GetLineChunks(std::ifstream& file, std::queue<std::string>& chunks)
{
	while (chunks.size())
		chunks.pop();

	std::string line;
	if (!getline(file, line))
		return FALSE;

	Pruning(line, line);

	std::string tmp = "";
	for (int i = 0; i < line.size(); ++i)
	{
		if (line[i] == ' ' ||
			line[i] == ',' ||
			line[i] == ';')
		{
			if (tmp.size())
				chunks.push(tmp);
			tmp.clear();
			continue;
		}
		else if (
			line[i] == '{' ||
			line[i] == '}')
		{
			if (tmp.size())
				chunks.push(tmp);
			tmp = line[i];
			chunks.push(tmp);
			tmp.clear();
			continue;
		}

		tmp.push_back(line[i]);
	}

	if (tmp.size())
		chunks.push(tmp);

	return TRUE;

}

BOOL XReader::Read(std::string path, XFrame* list)
{
	std::string line;
	std::ifstream file(path);
	if (!file.is_open() || !getline(file, line))
		return FALSE;

	Read(file, list);
	
	file.close();

	return TRUE;
}

void XReader::Read(std::ifstream& file, XComponent* parentComp)
{
	std::queue<std::string> chunks;
	while (GetLineChunks(file, chunks))
	{
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
			curKey = chunks.front(); 
			curName = chunks.front(); chunks.pop();
		}
		else
		{
			curKey = chunks.front(); chunks.pop();
			curName = chunks.front(); chunks.pop();
		}
		if (curKey == XTEMPLATE) // skip template
		{
			while (GetLineChunks(file, chunks))
			{
				while (chunks.size())
				{
					if (chunks.front() == "}" || chunks.front() == "{")
						break;
					chunks.pop();
				}
			}
		}
		else if (curKey == XFRAME)
		{
			XComponent* frameComp = new XFrame(curKey, curName);
			Read(file, frameComp);
			parentComp->AddComponent(frameComp);
		}
		else if (curKey == XASSETKIND)
		{
			GetLineChunks(file, chunks);

			std::vector<std::string> data;
			data.push_back(chunks.front());
			XComponent* newValue = new XValue(curKey, std::move(data));
			parentComp->AddComponent(newValue);

			GetLineChunks(file, chunks); // end
		}
		else if (curKey == XFRAMETRANSFORMMATRIX)
		{
			std::vector<float> data;
			for (int i = 0; i < 4; ++i)
			{
				GetLineChunks(file, chunks);
				for (int j = 0; j < 4; ++j)
				{
					std::string strData = chunks.front(); chunks.pop();
					data.push_back(stoi(strData));
				}
			}
			XComponent* newValue = new XValue(curName, std::move(data));
			parentComp->AddComponent(newValue);

			GetLineChunks(file, chunks); // end
		}
		else if (curKey == XMESH)
		{
			XComponent* meshComp = new XFrame(curKey, curName);
			//vertex count & vertice
			{
				GetLineChunks(file, chunks);
				int vertCount = stoi(chunks.front()); chunks.pop();
				XComponent* vertexCountComp = new XValue(XMESH_VERT_COUNT, std::vector<int>{vertCount});
				meshComp->AddComponent(vertexCountComp);

				std::vector<float> vertice;
				for (int i = 0; i < vertCount; ++i)
				{
					GetLineChunks(file, chunks);
					vertice.push_back(stof(chunks.front())); chunks.pop();//x
					vertice.push_back(stof(chunks.front())); chunks.pop();//y
					vertice.push_back(stof(chunks.front())); chunks.pop();//z
				}
				XComponent* verticeComp = new XValue(XMESH_VERT, std::move(vertice));
				meshComp->AddComponent(verticeComp);
			}

			//face count & faces
			{
				GetLineChunks(file, chunks);
				int faceCount = stoi(chunks.front()); chunks.pop();
				XComponent* faceCountComp = new XValue(XMESH_FACE_COUNT, std::vector<int>{faceCount});
				meshComp->AddComponent(faceCountComp);

				std::vector<int> faces;
				for (int i = 0; i < faceCount; ++i)
				{
					GetLineChunks(file, chunks);
					chunks.pop();
					faces.push_back(stoi(chunks.front()));chunks.pop();//x
					faces.push_back(stoi(chunks.front()));chunks.pop();//y
					faces.push_back(stoi(chunks.front()));chunks.pop();//z
				}
				XComponent* facesComp = new XValue(XMESH_FACE, std::move(faces));
				meshComp->AddComponent(facesComp);
			}

			//rest
			Read(file, meshComp);
			parentComp->AddComponent(meshComp);
		}
		else if (curKey == XMESHNORMALS)
		{
			XComponent* normComp = new XFrame(curKey, curName);
			//normals
			{
				GetLineChunks(file, chunks);
				int normCount = stoi(chunks.front()); chunks.pop();
				XComponent* normCountValue = new XValue(XMESHNORMALS_NORM_COUNT, std::vector<int>{normCount});
				normComp->AddComponent(normCountValue);

				std::vector<float> norms;
				for (int i = 0; i < normCount; ++i)
				{
					GetLineChunks(file, chunks);
					norms.push_back(stof(chunks.front())); chunks.pop();//x
					norms.push_back(stof(chunks.front())); chunks.pop();//y
					norms.push_back(stof(chunks.front())); chunks.pop();//z
				}
				XComponent* normValue = new XValue(XMESHNORMALS_NORM, std::move(norms));
				normComp->AddComponent(normValue);
			}
			//normal faces
			{
				GetLineChunks(file, chunks);
				int faceCount = stoi(chunks.front()); chunks.pop();
				XComponent* faceCountValue = new XValue(XMESHNORMALS_INDEX_COUNT, std::vector<int>{faceCount});
				normComp->AddComponent(faceCountValue);

				std::vector<int> faces;
				for (int i = 0; i < faceCount; ++i)
				{
					GetLineChunks(file, chunks);
					faces.push_back(stoi(chunks.front()));chunks.pop();//x
					faces.push_back(stoi(chunks.front()));chunks.pop();//y
					faces.push_back(stoi(chunks.front()));chunks.pop();//z
				}
				XComponent* faceNormValue = new XValue(XMESHNORMALS_INDEX, std::move(faces));
				normComp->AddComponent(faceNormValue);
			}
			parentComp->AddComponent(normComp);

			GetLineChunks(file, chunks); // end
		}
		else if (curKey == XMESHTEXTURECOORDS)
		{
			XComponent* texCoordComp = new XFrame(curKey, curName);
			
			GetLineChunks(file, chunks);
			int coordCount = stoi(chunks.front()); chunks.pop();
			XComponent* coordCountComp = new XValue("TexCoordCount", std::vector<int>{coordCount});
			texCoordComp->AddComponent(coordCountComp);

			std::vector<int> coords;
			for (int i = 0; i < coordCount; ++i)
			{
				GetLineChunks(file, chunks);
				coords.push_back(stof(chunks.front())); chunks.pop();//x
				coords.push_back(stof(chunks.front())); chunks.pop();//y
			}
			XComponent* verticeComp = new XValue("TexCoords", std::move(coords));
			texCoordComp->AddComponent(verticeComp);
			
			parentComp->AddComponent(texCoordComp);
			GetLineChunks(file, chunks); // end
		}
		else if (curKey == XANIMATIONKEY)
		{
			XComponent* animKeyComp = new XFrame(curKey, curName);

			// skip KeyType (only take matrix)
			GetLineChunks(file, chunks);
			//key count
			GetLineChunks(file, chunks);
			int keyCount = stoi(chunks.front()); chunks.pop();
			XComponent* keyCountValue = new XValue("KeyCount", std::vector<int>{keyCount});
			animKeyComp->AddComponent(keyCountValue);

			for (int i = 0; i < keyCount; ++i)
			{
				XComponent* subKeyComp = new XFrame("AnimationKey","AnimationKey");
				GetLineChunks(file, chunks);
				float time = stof(chunks.front()); chunks.pop();
				chunks.pop(); // skip matrix element count
				subKeyComp->AddComponent(new XValue("Time", std::vector<float>{time}));
				std::vector<float> matrix;
				matrix.push_back(stof(chunks.front()));	chunks.pop();
				matrix.push_back(stof(chunks.front()));	chunks.pop();
				matrix.push_back(stof(chunks.front()));	chunks.pop();
				matrix.push_back(stof(chunks.front()));	chunks.pop();
				GetLineChunks(file, chunks);
				matrix.push_back(stof(chunks.front()));	chunks.pop();
				matrix.push_back(stof(chunks.front()));	chunks.pop();
				matrix.push_back(stof(chunks.front()));	chunks.pop();
				matrix.push_back(stof(chunks.front()));	chunks.pop();
				GetLineChunks(file, chunks);
				matrix.push_back(stof(chunks.front()));	chunks.pop();
				matrix.push_back(stof(chunks.front()));	chunks.pop();
				matrix.push_back(stof(chunks.front()));	chunks.pop();
				matrix.push_back(stof(chunks.front()));	chunks.pop();
				GetLineChunks(file, chunks);
				matrix.push_back(stof(chunks.front()));	chunks.pop();
				matrix.push_back(stof(chunks.front()));	chunks.pop();
				matrix.push_back(stof(chunks.front()));	chunks.pop();
				matrix.push_back(stof(chunks.front()));	chunks.pop();
				subKeyComp->AddComponent(new XValue("Transform", std::move(matrix)));
				animKeyComp->AddComponent(subKeyComp);
			}

			parentComp->AddComponent(animKeyComp);
			GetLineChunks(file, chunks); // end
		}
		else if (curKey == XANIMATION)
		{
			XComponent* animComp = new XFrame(curKey, curName);
			GetLineChunks(file, chunks);
			//bone name
			chunks.pop();
			std::string boneName = chunks.front();
			XComponent* boneNameValue = new XValue("BoneName", std::vector<std::string>{boneName});
			animComp->AddComponent(boneNameValue);

			Read(file, animComp);

			parentComp->AddComponent(animComp);
		}
		else if (curKey == XANIMATIONSET)
		{
			XComponent* animSetComp = new XFrame(curKey, curName);

			Read(file, animSetComp);

			parentComp->AddComponent(animSetComp);
		}
		else if (curKey == XASSET)
		{
			GetLineChunks(file, chunks);

			std::vector<std::string> data;
			data.push_back(chunks.front());
			XComponent* newValue = new XValue(curName, std::move(data));
			parentComp->AddComponent(newValue);

			GetLineChunks(file, chunks); // end
		}
		else if (curKey == XRESOURCE)
		{
			GetLineChunks(file, chunks);

			std::vector<std::string> data;
			data.push_back(chunks.front());
			XComponent* newValue = new XValue(curKey, std::move(data));
			parentComp->AddComponent(newValue);

			GetLineChunks(file, chunks); // end
		}
		else
		{
			while (GetLineChunks(file, chunks))
			{
				BOOL bOut = FALSE;
				while (chunks.size())
				{
					if (chunks.front() == "}")
					{
						bOut = TRUE;
						break;
					}
					chunks.pop();
				}
				if (bOut)
					break;
			}
		}
	}
}


