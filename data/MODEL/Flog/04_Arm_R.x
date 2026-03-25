xof 0302txt 0064
template Header {
 <3D82AB43-62DA-11cf-AB39-0020AF71E433>
 WORD major;
 WORD minor;
 DWORD flags;
}

template Vector {
 <3D82AB5E-62DA-11cf-AB39-0020AF71E433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template Coords2d {
 <F6F23F44-7686-11cf-8F52-0040333594A3>
 FLOAT u;
 FLOAT v;
}

template Matrix4x4 {
 <F6F23F45-7686-11cf-8F52-0040333594A3>
 array FLOAT matrix[16];
}

template ColorRGBA {
 <35FF44E0-6C7C-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template ColorRGB {
 <D3E16E81-7835-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template IndexedColor {
 <1630B820-7842-11cf-8F52-0040333594A3>
 DWORD index;
 ColorRGBA indexColor;
}

template Boolean {
 <4885AE61-78E8-11cf-8F52-0040333594A3>
 WORD truefalse;
}

template Boolean2d {
 <4885AE63-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template MaterialWrap {
 <4885AE60-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template TextureFilename {
 <A42790E1-7810-11cf-8F52-0040333594A3>
 STRING filename;
}

template Material {
 <3D82AB4D-62DA-11cf-AB39-0020AF71E433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template MeshFace {
 <3D82AB5F-62DA-11cf-AB39-0020AF71E433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template MeshFaceWraps {
 <4885AE62-78E8-11cf-8F52-0040333594A3>
 DWORD nFaceWrapValues;
 Boolean2d faceWrapValues;
}

template MeshTextureCoords {
 <F6F23F40-7686-11cf-8F52-0040333594A3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}

template MeshMaterialList {
 <F6F23F42-7686-11cf-8F52-0040333594A3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material]
}

template MeshNormals {
 <F6F23F43-7686-11cf-8F52-0040333594A3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}

template MeshVertexColors {
 <1630B821-7842-11cf-8F52-0040333594A3>
 DWORD nVertexColors;
 array IndexedColor vertexColors[nVertexColors];
}

template Mesh {
 <3D82AB44-62DA-11cf-AB39-0020AF71E433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

Header{
1;
0;
1;
}

Mesh {
 20;
 -27.80779;0.00000;4.60690;,
 -27.80779;-3.84369;3.20791;,
 -0.19282;-3.84369;3.20790;,
 -0.19282;0.00000;4.60689;,
 -27.80779;-5.88887;-0.33445;,
 -0.19282;-5.88887;-0.33446;,
 -27.80779;-5.17859;-4.36268;,
 -0.19282;-5.17859;-4.36269;,
 -27.80779;-2.04518;-6.99192;,
 -0.19282;-2.04518;-6.99193;,
 -27.80779;2.04518;-6.99192;,
 -0.19282;2.04518;-6.99193;,
 -27.80779;5.17859;-4.36268;,
 -0.19282;5.17859;-4.36269;,
 -27.80779;5.88887;-0.33446;,
 -0.19282;5.88887;-0.33447;,
 -27.80779;3.84369;3.20791;,
 -0.19282;3.84369;3.20790;,
 -27.80779;0.00000;4.60690;,
 -0.19282;0.00000;4.60689;;
 
 9;
 4;0,1,2,3;,
 4;1,4,5,2;,
 4;4,6,7,5;,
 4;6,8,9,7;,
 4;8,10,11,9;,
 4;10,12,13,11;,
 4;12,14,15,13;,
 4;14,16,17,15;,
 4;16,18,19,17;;
 
 MeshMaterialList {
  3;
  9;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0;;
  Material {
   0.204000;0.552000;0.188000;1.000000;;
   0.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.540000;0.552000;0.125600;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.172800;0.172800;0.172800;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  9;
  0.000000;0.000000;1.000000;,
  0.000000;-0.642788;0.766044;,
  0.000000;-0.984808;0.173649;,
  -0.000000;-0.866026;-0.500000;,
  -0.000000;-0.342020;-0.939693;,
  -0.000000;0.342020;-0.939693;,
  -0.000000;0.866026;-0.500000;,
  0.000000;0.984808;0.173648;,
  0.000000;0.642788;0.766044;;
  9;
  4;0,1,1,0;,
  4;1,2,2,1;,
  4;2,3,3,2;,
  4;3,4,4,3;,
  4;4,5,5,4;,
  4;5,6,6,5;,
  4;6,7,7,6;,
  4;7,8,8,7;,
  4;8,0,0,8;;
 }
 MeshTextureCoords {
  20;
  0.000000;0.000000;,
  0.111110;0.000000;,
  0.111110;1.000000;,
  0.000000;1.000000;,
  0.222220;0.000000;,
  0.222220;1.000000;,
  0.333330;0.000000;,
  0.333330;1.000000;,
  0.444440;0.000000;,
  0.444440;1.000000;,
  0.555560;0.000000;,
  0.555560;1.000000;,
  0.666670;0.000000;,
  0.666670;1.000000;,
  0.777780;0.000000;,
  0.777780;1.000000;,
  0.888890;0.000000;,
  0.888890;1.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;;
 }
}
