#include "Noob.h"
#include <gl/glut.h>

Noob::Noob()
{
}
Noob::Noob(Terrain* p_terrain,  char *p_MD2FilePath,char *p_textureFilePath)
{
    m_terrain = p_terrain;

	m_MD2FilePath = new char[strlen(p_MD2FilePath)];
	m_textureFilePath = new char[strlen(p_textureFilePath)];

	strcpy(m_MD2FilePath, p_MD2FilePath);
	strcpy(m_textureFilePath, p_textureFilePath);

	m_elapsedTime = 0;
	m_dieTime = 0;
}

Noob::~Noob(void)
{
}

Vector3* Noob::GetVertices(int& size)
{
	size = m_model.m_numberOfVertices;
	return m_model.m_vertixList;
}

void Noob::Initialize()
{
	m_model.LoadModel(m_MD2FilePath);
	m_model.LoadSkin(m_textureFilePath);
	m_model.ScaleModel(2.0);
	m_model.SetAnim(RUN);
	//this->setPosition(0, 100, 0);
}
void Noob::Draw(unsigned p_elapsedTime)
{
	m_elapsedTime += p_elapsedTime/1000.0;
	glEnable(GL_TEXTURE_2D);
	glPushMatrix();
	{
		glMultMatrixf(&getWorldMatrix()[0][0]);
		//glTranslatef(this->getPosition().x, this->getPosition().y, this->getPosition().z);
		glRotatef(180, 0, 1, 0);
		m_model.DrawModel(m_elapsedTime);
	}
	glPopMatrix();
	Entity3D::Draw(p_elapsedTime);
}

void Noob::SetAnimation(int p_animationCode)
{
	m_model.SetAnim(p_animationCode);
}

float Noob::GetDistanceFromOrigin()
{
	float dist = sqrt(this->getPosition().x * this->getPosition().x +
		this->getPosition().y * this->getPosition().y +
		this->getPosition().z * this->getPosition().z);

	return dist;
}

void Noob::Update(unsigned p_elapsedTime)
{
	if(m_model.m_anim.type == RUN)
		setVelocity(0, 0, 160);
	else if(m_model.m_anim.type == DEATH_FALLBACK)
	{
		setVelocity(0, 0, 0);
		m_dieTime += p_elapsedTime;
	}

	if(GetDistanceFromOrigin() > 4000)
	{
		orient(180, 0, 0);
	}
    
	Entity3D::Update(p_elapsedTime);

    if(m_terrain != 0 && m_model.m_anim.type != DEATH_FALLBACK)
    {
        float height = m_terrain->GetHeightmapEntryFromCoord(m_position.x, m_position.z);
        m_position.set(m_position.x, height + 50, m_position.z);
    }

}
