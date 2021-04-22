xof 0303txt 0032

template Frame {
  <3d82ab46-62da-11cf-ab39-0020af71e433>
  [...]
}

template Matrix4x4 {
  <f6f23f45-7686-11cf-8f52-0040333594a3>
  array FLOAT matrix[16];
}

template FrameTransformMatrix {
  <f6f23f41-7686-11cf-8f52-0040333594a3>
  Matrix4x4 frameMatrix;
}

template Vector {
  <3d82ab5e-62da-11cf-ab39-0020af71e433>
  FLOAT x;
  FLOAT y;
  FLOAT z;
}

template MeshFace {
  <3d82ab5f-62da-11cf-ab39-0020af71e433>
  DWORD nFaceVertexIndices;
  array DWORD faceVertexIndices[nFaceVertexIndices];
}

template Mesh {
  <3d82ab44-62da-11cf-ab39-0020af71e433>
  DWORD nVertices;
  array Vector vertices[nVertices];
  DWORD nFaces;
  array MeshFace faces[nFaces];
  [...]
}

template MeshNormals {
  <f6f23f43-7686-11cf-8f52-0040333594a3>
  DWORD nNormals;
  array Vector normals[nNormals];
  DWORD nFaceNormals;
  array MeshFace faceNormals[nFaceNormals];
}

template Coords2d {
  <f6f23f44-7686-11cf-8f52-0040333594a3>
  FLOAT u;
  FLOAT v;
}

template MeshTextureCoords {
  <f6f23f40-7686-11cf-8f52-0040333594a3>
  DWORD nTextureCoords;
  array Coords2d textureCoords[nTextureCoords];
}

template ColorRGBA {
  <35ff44e0-6c7c-11cf-8f52-0040333594a3>
  FLOAT red;
  FLOAT green;
  FLOAT blue;
  FLOAT alpha;
}

template IndexedColor {
  <1630b820-7842-11cf-8f52-0040333594a3>
  DWORD index;
  ColorRGBA indexColor;
}

template MeshVertexColors {
  <1630b821-7842-11cf-8f52-0040333594a3>
  DWORD nVertexColors;
  array IndexedColor vertexColors[nVertexColors];
}

template VertexElement {
  <f752461c-1e23-48f6-b9f8-8350850f336f>
  DWORD Type;
  DWORD Method;
  DWORD Usage;
  DWORD UsageIndex;
}

template DeclData {
  <bf22e553-292c-4781-9fea-62bd554bdd93>
  DWORD nElements;
  array VertexElement Elements[nElements];
  DWORD nDWords;
  array DWORD data[nDWords];
}

Frame Scene {
  FrameTransformMatrix {
    1.0, 0.0, 0.0, 0.0,
    0.0, 1.0, 0.0, 0.0,
    0.0, 0.0, 1.0, 0.0,
    0.0, 0.0, 0.0, 1.0;;
  }
  Frame Root {
    FrameTransformMatrix {
      1.0, 0.0, 0.0, 0.0,
      0.0, 1.0, 0.0, 0.0,
      0.0, 0.0, 1.0, 0.0,
      0.0, 0.0, 0.0, 1.0;;
    }
    Mesh Mesh {
      24;
      -0.5; -0.5; -0.5;,
      -0.5; 0.5; -0.5;,
      0.5; 0.5; -0.5;,
      0.5; -0.5; -0.5;,
      0.5; -0.5; -0.5;,
      0.5; 0.5; -0.5;,
      0.5; 0.5; 0.5;,
      0.5; -0.5; 0.5;,
      0.5; -0.5; 0.5;,
      0.5; 0.5; 0.5;,
      -0.5; 0.5; 0.5;,
      -0.5; -0.5; 0.5;,
      -0.5; -0.5; 0.5;,
      -0.5; 0.5; 0.5;,
      -0.5; 0.5; -0.5;,
      -0.5; -0.5; -0.5;,
      -0.5; 0.5; -0.5;,
      -0.5; 0.5; 0.5;,
      0.5; 0.5; 0.5;,
      0.5; 0.5; -0.5;,
      -0.5; -0.5; 0.5;,
      -0.5; -0.5; -0.5;,
      0.5; -0.5; -0.5;,
      0.5; -0.5; 0.5;;
      12;
      3;0,1,2;,
      3;0,2,3;,
      3;4,5,6;,
      3;4,6,7;,
      3;8,9,10;,
      3;8,10,11;,
      3;12,13,14;,
      3;12,14,15;,
      3;16,17,18;,
      3;16,18,19;,
      3;20,21,22;,
      3;20,22,23;;

      MeshNormals {
        24;
        0;0;-1;,
        0;0;-1;,
        0;0;-1;,
        0;0;-1;,
        1;0;0;,
        1;0;0;,
        1;0;0;,
        1;0;0;,
        0;0;1;,
        0;0;1;,
        0;0;1;,
        0;0;1;,
        -1;0;0;,
        -1;0;0;,
        -1;0;0;,
        -1;0;0;,
        0;1;0;,
        0;1;0;,
        0;1;0;,
        0;1;0;,
        0;-1;0;,
        0;-1;0;,
        0;-1;0;,
        0;-1;0;;
        12;
        3;0,1,2;,
        3;0,2,3;,
        3;4,5,6;,
        3;4,6,7;,
        3;8,9,10;,
        3;8,10,11;,
        3;12,13,14;,
        3;12,14,15;,
        3;16,17,18;,
        3;16,18,19;,
        3;20,21,22;,
        3;20,22,23;;
      }

      MeshTextureCoords {
        24;
        0; 1;,
        0; 0;,
        1; 0;,
        1; 1;,
        0; 1;,
        0; 0;,
        1; 0;,
        1; 1;,
        0; 1;,
        0; 0;,
        1; 0;,
        1; 1;,
        0; 1;,
        0; 0;,
        1; 0;,
        1; 1;,
        0; 1;,
        0; 0;,
        1; 0;,
        1; 1;,
        0; 1;,
        0; 0;,
        1; 0;,
        1; 1;;
      }
    }
  }
}

