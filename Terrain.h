#ifndef TERRAIN_H
#define TERRAIN_H

#include "DrawableGameComponent.h"
#include <vector>
using namespace std;

#define DEFAULT_WIDTH 512
#define DEFAULT_HEIGHT 512

class Terrain : public DrawableGameComponent
{
    struct Vertex
    {
        float X, Y, Z;
    };

    struct TexCoord
    {
        float U, V;
    };

    int         m_width;
    int         m_depth;
    int         m_cellSpacing;
    int         m_numVertices;
    int         m_numVertsPerRow;
    int         m_numVertsPerCol;
    int         m_numCellsPerRow;
    int         m_numCellsPerCol;
    int         m_textureId;
    int         m_lightMapId;
    int         m_vertexBufferSize;
    unsigned    m_glVBOVertices;
	unsigned    m_glVBOTexCoord;
    float       m_heightScale;
    char        *m_texturePath;
    char        *m_heightMapPath;
    TexCoord*   m_texCoordBuffer;
    Vertex*     m_vertexBuffer;
    vector<int> m_heightMap;

public:
    Terrain(const char* p_texturePath, const char* p_heightMapPath, int p_numVertsPerRow, int p_numVertsPerCol, int p_cellSpacing, float p_heightScale);
    void Initialize();
    ~Terrain(void);

    int GetHeightmapEntry(int p_row, int p_col);
    void SetHeightmapEntry(int p_row, int p_col, int p_value);
    int GetHeightmapEntryFromCoord(float p_xCoord, float p_zCoord);

    float GetHeight(float x, float z);
    void LoadResources();
    void InitializeGLBuffers();
    void GenerateVertices();
    void ComputeTriangleIndices();
    void Draw(unsigned p_elapsedTime);
};

#endif