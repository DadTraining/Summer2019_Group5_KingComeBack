#include "Item.h"

Item::Item(Scene * scene, int ID)
{
	this->Init(ID); //nhap id vao func init()
	scene->addChild(m_sprite);
}

Item::~Item()
{
}

void Item::Init(int ID)
{
	switch (ID)
	{
	case 0:
		m_itemAttribute = "strength";
		m_dame = 100;
		break;
	case 1:
		m_itemAttribute = "agiliti";
		m_dame = 98;
		break;
	case 2:
		m_itemAttribute = "intelligence";
		m_dame = 120;
		break;
		

	default:
		break;
	}
	this->m_sprite = ResourceManager::GetInstance()->GetSpriteById(ID);
	Size screensize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	this->m_sprite->removeFromParent();
	
}

void Item::Died()
{
}

std::string Item::SetState(std::string state)
{
	m_state = state;
	return m_state;
}

void Item::SetPosition()
{
	
}
