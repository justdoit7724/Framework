//DWORD = 32 bit int or long
//FLOAT = float

template Vector{
	FLOAT x;
	FLOAT y;
	FLOAT z;
}

template MeshFace{
	DWORD nFaceVertexIndices;
	array DWORD faceVertexIndices[nFaceVertexIndices];
}

template Mesh{
	DWORD nVertices;
	array Vector vertices[nVertices];
	DWORD nFaces;
	array MeshFace faces[nFaces]
}

template Frame{
	[FrameTransformMatrix]
	[Mesh]
}

template MeshNormals{
	DWORD nNormals;
	array Vector normals[nNormals];
	DWORD nFaceNormals;
	array MeshFace faceNormals[nFaceNormals];
}