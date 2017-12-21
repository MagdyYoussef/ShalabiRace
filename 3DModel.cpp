#include "3DModel.h"
#include "Md2.h"
#include "TextureLoader.h"

void t3DModel::Free() {
	if(m_glCommandBuffer) {
		delete[] m_glCommandBuffer;
		m_glCommandBuffer = NULL;
	}
	pMaterials.clear();
	pObjects.clear(); //each object will destroy itself
	numOfMaterials = numOfObjects = 0;
}

void t3DModel::LoadMD2Model(const char *modelFileName, const char *modelTextureFileName)
{
	//Free();
	CLoadMD2 Md2Loader;

	Md2Loader.ImportMD2(this, modelFileName, modelTextureFileName);

	//load the texture of each material in the material list
	for(int i = 0; i < numOfMaterials; i++)
	{
		if(strlen(pMaterials[i].strFile) > 0)
			pMaterials[i].textureId = LoadTexture(pMaterials[i].strFile);
        pMaterials[i].texture2Id = LoadTexture("Textures\\skydome.bmp");
        //pMaterials[i].lightMapId = LoadTexture("Textures\\hill.bmp");
	}



	//flip all normals
	for(int i=0; i<numOfObjects; i++)
	{
		for(int j=0; j<pObjects[i].numOfVerts; j++)
            pObjects[i].pNormals[j].inverse();
	}
}

void t3DModel::RenderFrame(int frameIndex) {
	t3DObject *currObj = &pObjects[frameIndex];
	tMaterialInfo *pMaterial = &pMaterials[currObj->materialID];

    glActiveTexture(GL_TEXTURE0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, pMaterial->textureId);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

    glActiveTexture(GL_TEXTURE1);
	glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, pMaterial->lightMapId);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    glActiveTexture(GL_TEXTURE2);
	glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, pMaterial->texture2Id);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glBegin(GL_TRIANGLES);
	// loop faces
	for(int f = 0; f < currObj->numOfFaces; f++)
	{
		tFace &face = currObj->pFaces[f];
		//loop the vertices of the face
		for(int v = 0; v < 3; v++)
		{
			int vertIndex = face.vertIndex[v];
			int texIndex = face.coordIndex[v];

			glNormal3fv((const float *)&currObj->pNormals[vertIndex]);
			
			if(currObj->pTexVerts) 
			{
                //glTexCoord2f(currObj->pTexVerts[ texIndex ].x, currObj->pTexVerts[ texIndex ].y);
                glMultiTexCoord2f(GL_TEXTURE0, currObj->pTexVerts[ texIndex ].x, currObj->pTexVerts[ texIndex ].y);
                glMultiTexCoord2f(GL_TEXTURE1, currObj->pTexVerts[ texIndex ].x, currObj->pTexVerts[ texIndex ].y);
                glMultiTexCoord2f(GL_TEXTURE2, currObj->pTexVerts[ texIndex ].x, currObj->pTexVerts[ texIndex ].y);
			}
			glVertex3f(currObj->pVerts[ vertIndex ].x, currObj->pVerts[ vertIndex ].y, currObj->pVerts[ vertIndex ].z);
		}
	}
	glEnd();

    glActiveTexture(GL_TEXTURE2);
	glDisable(GL_TEXTURE_2D);

    glActiveTexture(GL_TEXTURE1);
	glDisable(GL_TEXTURE_2D);

    glActiveTexture(GL_TEXTURE0);
	glDisable(GL_TEXTURE_2D);

	//glDisable(GL_TEXTURE_2D);
}