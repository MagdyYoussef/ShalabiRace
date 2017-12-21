#pragma once

#include <windows.h>
#include <gl/glew.h>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <fstream>
#include <vector>
#include <crtdbg.h>
#include "mathlib.h"
using namespace std;


// This is 2D point class.  This will be used to store the UV coordinates.
class Vector2 
{
public:
	float x, y;
};

// This is our face structure.  This is is used for indexing into the vertex 
// and texture coordinate arrays.  From this information we know which vertices
// from our vertex array go to which face, along with the correct texture coordinates.
struct tFace
{
	int vertIndex[3];			// indicies for the verts that make up this triangle
	int coordIndex[3];			// indicies for the tex coords to texture this face
};

// This holds the information for a material.  It may be a texture map of a color.
// Some of these are not used, but I left them.
struct tMaterialInfo
{
	char  strName[255];			// The texture name
	char  strFile[255];			// The texture file name (If this is set it's a texture map)
	BYTE  color[3];				// The color of the object (R, G, B)
	int   textureId;				// the texture ID
    int   lightMapId;
    int   texture2Id;
	float uTile;				// u tiling of texture  
	float vTile;				// v tiling of texture	
	float uOffset;			    // u offset of texture
	float vOffset;				// v offset of texture
} ;

// This holds all the information for our model/scene. 
// You should eventually turn into a robust class that 
// has loading/drawing/querying functions like:
// LoadModel(...); DrawObject(...); DrawModel(...); DestroyModel(...);
struct t3DObject 
{
	int  numOfVerts;			// The number of verts in the model
	int  numOfFaces;			// The number of faces in the model
	int  numTexVertex;			// The number of texture coordinates
	int  materialID;			// The texture ID to use, which is the index into our texture array
	bool bHasTexture;			// This is TRUE if there is a texture map for this object
	char strName[255];			// The name of the object
	Vector3  *pVerts;			// The object's vertices
	Vector3  *pNormals;			// The object's normals
	Vector2  *pTexVerts;		// The texture's UV coordinates
	tFace *pFaces;				// The faces information of the object

	~t3DObject() {
		//delete all dynamically allocated data
	}
};

// This holds our model information.  This should also turn into a robust class.
// We use STL's (Standard Template Library) vector class to ease our link list burdens. :)
struct t3DModel 
{
public:
	int numOfObjects;					// The number of objects in the model
	int numOfMaterials;					// The number of materials for the model
	int	*m_glCommandBuffer;			
	vector<tMaterialInfo> pMaterials;	// The list of material information (Textures and colors)
	vector<t3DObject> pObjects;			// The object list for our model

//--- Methods
	void LoadMD2Model(const char *modelFileName, const char *modelTextureFileName);
	void Free();
	void RenderFrame(int frameIndex);
};

