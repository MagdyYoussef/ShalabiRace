#pragma once
#ifndef NOOB_H
#define NOOB_H

#include "entity3d.h"
#include "md2loader.h"
#include "Terrain.h"
#include <string>
class Noob :
	public Entity3D
{
private:
	CMD2Model m_model;
	char* m_MD2FilePath;
	char* m_textureFilePath;
    Terrain *m_terrain;
	float m_elapsedTime;
	
	float GetDistanceFromOrigin();

public:
	float m_dieTime;
	Noob();
	Noob(Terrain* p_terrain, char *p_MD2FilePath,char *p_textureFilePath);
	~Noob(void);
	Vector3* GetVertices(int& size);
	void Initialize();
    void Draw(unsigned p_elapsedTime);
    void Update(unsigned p_elapsedTime);
	void SetAnimation(int);
};
#endif