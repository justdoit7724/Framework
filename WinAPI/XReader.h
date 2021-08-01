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


#define XTEMPLATE					"template"
#define XFRAME						"Frame"
#define XASSETKIND					"AssetKind"
#define XFRAMETRANSFORMMATRIX		"FrameTransformMatrix"
#define XMESH						"Mesh"
#define XMESH_VERT_COUNT			"nVertice"
#define XMESH_VERT					"Vertice"
#define XMESH_FACE_COUNT			"nFace"
#define XMESH_FACE					"Faces"
#define XMESHNORMALS				"MeshNormals"
#define XMESHNORMALS_NORM_COUNT		"nNormal"
#define XMESHNORMALS_NORM			"Normals"
#define XMESHNORMALS_INDEX_COUNT	"nFace"
#define XMESHNORMALS_INDEX			"Faces"
#define XMESHTEXTURECOORDS			"MeshTextureCoords"
#define XANIMATIONKEY				"AnimationKey"
#define XANIMATION					"Animation"
#define XANIMATIONSET				"AnimationSet"
#define XASSET						"Asset"
#define XRESOURCE					"Resource"

class XComponent
{
public:
	XComponent(BOOL bComposite, std::string key, std::string name);
	virtual ~XComponent();

	virtual int Size() { return 0; }
	virtual std::vector< XComponent*> GetChild(std::string key) { return std::vector< XComponent*>(); }
	virtual BOOL AddComponent(XComponent* c) { return FALSE; };
	virtual const std::vector<float>& GetFloats() { return std::vector<float>(); }
	virtual const std::vector<int>& GetInts() { return std::vector<int>(); }
	virtual const std::vector<std::string>& GetStrings() { return std::vector<std::string>(); }

	const BOOL m_bComposite;
	const std::string m_key;
	const std::string m_name;
};
class XFrame : public XComponent
{
public:
	XFrame(std::string key, std::string name);
	~XFrame();

	int Size()override; 
	std::vector< XComponent*> GetChild(std::string key) override;
	BOOL AddComponent(XComponent* c)override;

private:
	friend class XReader;
	std::unordered_map<std::string, std::vector<XComponent*>> m_components;
};
class XValue : public XComponent
{
public:
	XValue(std::string key, std::vector<float>&& data);
	XValue(std::string key, std::vector<int>&& data);
	XValue(std::string key, std::vector<std::string>&& data);
	~XValue();

	const std::vector<float>& GetFloats()override;
	const std::vector<int>& GetInts()override;
	const std::vector<std::string>& GetStrings()override;

private:
	union {
		std::vector<float> m_floats;
		std::vector<int> m_ints;
		std::vector<std::string> m_strings;
	};
};

class XReader
{
public:
	static BOOL Read(std::string path, XFrame* list);

private:
	static void Read(std::ifstream& file, XComponent* parentComp);
};

