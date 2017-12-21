#include "Terrain.h"
#include "Mathlib.h"
#include <stdio.h>
#include "TextureLoader.h"

Terrain::Terrain(const char* p_texturePath, const char* p_heightMapPath, int p_numVertsPerRow, int p_numVertsPerCol, int p_cellSpacing, float p_heightScale)
{
    m_texturePath       = const_cast<char*>(p_texturePath);
    m_heightMapPath     = const_cast<char*>(p_heightMapPath);
    m_numVertsPerRow    = p_numVertsPerRow;
    m_numVertsPerCol    = p_numVertsPerCol;
    m_cellSpacing       = p_cellSpacing;
    m_heightScale       = p_heightScale;

    m_numCellsPerRow    = m_numVertsPerRow - 1;
    m_numCellsPerCol    = m_numVertsPerCol - 1;
    m_width             = m_numCellsPerRow * m_cellSpacing;
    m_depth             = m_numCellsPerCol * m_cellSpacing;
    m_numVertices       = m_numVertsPerRow * m_numVertsPerCol;
}


Terrain::~Terrain(void)
{
}

void Terrain::Initialize()
{
    LoadResources();
    GenerateVertices();
}

int Terrain::GetHeightmapEntryFromCoord(float p_xCoord, float p_zCoord)
{
    // Translate on xz-plane by the transformation that takes
	// the terrain START point to the origin.
	p_xCoord = ((float)m_width / 2.0f) + p_xCoord;
	p_zCoord = ((float)m_depth / 2.0f) + p_zCoord;

	// Scale down by the transformation that makes the 
	// cellspacing equal to one.  This is given by 
	// 1 / cellspacing since; cellspacing * 1 / cellspacing = 1.
	p_xCoord /= (float)m_cellSpacing;
	p_zCoord /= (float)m_cellSpacing;

    float col = floor(p_xCoord);
	float row = floor(p_zCoord);

    return GetHeightmapEntry(row, col);
}

int Terrain::GetHeightmapEntry(int p_row, int p_col)
{
    return m_heightMap[abs(p_row) * m_numVertsPerRow + abs(p_col)];
}

void Terrain::SetHeightmapEntry(int p_row, int p_col, int p_value)
{
	m_heightMap[p_row * m_numVertsPerRow + p_col] = p_value;
}

void Terrain::InitializeGLBuffers()
{
	glGenBuffersARB(1, &m_glVBOVertices);
    glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_glVBOVertices);
    glBufferDataARB(GL_ARRAY_BUFFER_ARB, m_vertexBufferSize * 3 * sizeof(float), m_vertexBuffer, GL_STATIC_DRAW_ARB);

    glGenBuffersARB(1, &m_glVBOTexCoord);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_glVBOTexCoord);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, m_vertexBufferSize * 2 * sizeof(float), m_texCoordBuffer, GL_STATIC_DRAW_ARB);

	delete [] m_vertexBuffer;
    m_vertexBuffer = NULL;

	delete [] m_texCoordBuffer;
	m_texCoordBuffer = NULL;
}

void Terrain::LoadResources()
{
    m_textureId = LoadTexture(m_texturePath);
    
    vector<BYTE> buffer(m_numVertices);

    FILE *heightMapFile;
	heightMapFile = fopen(m_heightMapPath, "rb");
    fread(&buffer[0], sizeof(BYTE), m_numVertices, heightMapFile);

    //fill(buffer.begin(), buffer.end(), 0);
	fclose(heightMapFile);

    m_heightMap.resize(m_numVertices);
    for(int i = 0; i < m_numVertices; i++)
        m_heightMap[i] = buffer[i] * m_heightScale;
}

void Terrain::GenerateVertices()
{
    int startX = - m_width / 2;
    int startZ = - m_depth / 2;

    // coordinates to end generating vertices at
    int endX = m_width / 2;
    int endZ = m_depth / 2;

    m_vertexBufferSize = (m_numCellsPerRow * m_numCellsPerCol * 6);
    m_vertexBuffer = new Vertex[m_vertexBufferSize];
    m_texCoordBuffer = new TexCoord[m_vertexBufferSize];

    float vX, vZ;
    int index = 0;
    for(int z = startZ; z < endZ; z += m_cellSpacing)
    {
        for(int x = startX; x < endX; x += m_cellSpacing)
        {
            // compute the correct index into the vertex buffer
            // and heightmap based on where we are in the nested
            // loop.
            for (int vertexIdx = 0; vertexIdx < 6; vertexIdx++)
            {
				vX = (float)x + ((vertexIdx == 1 || vertexIdx == 2 || vertexIdx == 5) ? m_cellSpacing : 0);
				vZ = (float)z + ((vertexIdx == 2 || vertexIdx == 4 || vertexIdx == 5) ? m_cellSpacing : 0);
				m_vertexBuffer[index].X = vX;
				m_vertexBuffer[index].Z = vZ;

                int m_mapX = ((int)vX - startX) / m_cellSpacing;
                int m_mapZ = ((int)vZ - startZ) / m_cellSpacing;

                m_vertexBuffer[index].Y = m_heightMap[m_mapZ * m_numVertsPerRow + m_mapX];

                m_texCoordBuffer[index].U = (float)m_mapX / m_numVertsPerRow;
                // Adapt to the openGl texture coordinate
                m_texCoordBuffer[index].V = 1.0f - ((float)m_mapZ / m_numVertsPerCol);

                index++;
            }
        }
    }

    InitializeGLBuffers();
}

float Terrain::GetHeight(float x, float z)
{
	// Translate on xz-plane by the transformation that takes
	// the terrain START point to the origin.
	x = ((float)m_width / 2.0f) + x;
	z = ((float)m_depth / 2.0f) + z;

	// Scale down by the transformation that makes the 
	// cellspacing equal to one.  This is given by 
	// 1 / cellspacing since; cellspacing * 1 / cellspacing = 1.
	x /= (float)m_cellSpacing;
	z /= (float)m_cellSpacing;

	// From now on, we will interpret our positive z-axis as
	// going in the 'down' direction, rather than the 'up' direction.
	// This allows to extract the row and column simply by 'flooring'
	// x and z:

    float col = floor(x);
	float row = floor(z);

	// get the heights of the quad we're in:
	// 
    //  A   B
    //  *---*
    //  | / |
    //  *---*  
    //  C   D

	float A = GetHeightmapEntry(row,   col);
	float B = GetHeightmapEntry(row,   col+1);
	float C = GetHeightmapEntry(row+1, col);
	float D = GetHeightmapEntry(row+1, col+1);

	//
	// Find the triangle we are in:
	//

	// Translate by the transformation that takes the upper-left
	// corner of the cell we are in to the origin.  Recall that our 
	// cellspacing was nomalized to 1.  Thus we have a unit square
	// at the origin of our +x -> 'right' and +z -> 'down' system.
	float dx = x - col;
	float dz = z - row;

	// Note the below compuations of u and v are unneccessary, we really
	// only need the height, but we compute the entire vector to emphasis
	// the books discussion.
	float height = 0.0f;
	if(dz < 1.0f - dx)  // upper triangle ABC
	{
		float uy = B - A; // A->B
		float vy = C - A; // A->C

		// Linearly interpolate on each vector.  The height is the vertex
		// height the vectors u and v originate from {A}, plus the heights
		// found by interpolating on each vector u and v.
        height = A + Math::Lerp(0.0f, uy, dx) + Math::Lerp(0.0f, vy, dz);
	}
	else // lower triangle DCB
	{
		float uy = C - D; // D->C
		float vy = B - D; // D->B

		// Linearly interpolate on each vector.  The height is the vertex
		// height the vectors u and v originate from {D}, plus the heights
		// found by interpolating on each vector u and v.
		height = D + Math::Lerp(0.0f, uy, 1.0f - dx) + Math::Lerp(0.0f, vy, 1.0f - dz);
	}

	return height;
}

void Terrain::Draw(unsigned p_elapsedTime)
{
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, m_textureId);

    glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_glVBOTexCoord);
	glTexCoordPointer(2, GL_FLOAT, 0, (char *) NULL);

	glEnableClientState(GL_VERTEX_ARRAY);
    glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_glVBOVertices);
	glVertexPointer(3, GL_FLOAT, 0, (char *) NULL);

    glDrawArrays(GL_TRIANGLES, 0, m_vertexBufferSize);

	glDisableClientState(GL_VERTEX_ARRAY);

	glDisable(GL_TEXTURE_2D);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

    /*glColor3f(1, 0, 0);
    glBegin(GL_LINES);
    {
        for(int i = -m_rWidth; i <= m_rWidth; i+=5)
        {
            glVertex3f(i, 0, m_rHeight);
            glVertex3f(i, 0, -m_rHeight);
        }

        for(int i = -m_rHeight; i <= m_rHeight; i+=5)
        {
            glVertex3f(m_rWidth, 0, i);
            glVertex3f(-m_rWidth, 0, i);
        }

    }
    glEnd();*/
}