#include "Scene.h"

Scene::Scene(void)
{
}

Scene::~Scene()
{
    vector<DrawableGameComponent*>::iterator itr;
    for(itr = m_components.begin();
        itr != m_components.end();
        itr++)
    {
        delete *itr;
    }
}

void Scene::Initialize()
{
    vector<DrawableGameComponent*>::iterator itr;
    for(itr = m_components.begin();
        itr != m_components.end();
        itr++)
    {
        (*itr)->Initialize();
    }
}

void Scene::LoadContent()
{
    vector<DrawableGameComponent*>::iterator itr;
    for(itr = m_components.begin();
        itr != m_components.end();
        itr++)
    {
        (*itr)->LoadContent();
    }
}

void Scene::Update(unsigned p_elapsedTime)
{
    vector<DrawableGameComponent*>::iterator itr;
    for(itr = m_components.begin();
        itr != m_components.end();
        itr++)
    {
        if((*itr)->Enabled())
        {
            (*itr)->Update(p_elapsedTime);
        }
    }
}

void Scene::Draw(unsigned p_elapsedTime)
{
    vector<DrawableGameComponent*>::iterator itr;
    for(itr = m_components.begin();
        itr != m_components.end();
        itr++)
    {
        if((*itr)->Visible())
        {
            (*itr)->Draw(p_elapsedTime);
        }
    }
}
