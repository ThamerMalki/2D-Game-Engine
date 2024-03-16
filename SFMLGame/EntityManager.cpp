#include"EntityManager.h"
#include <algorithm>

EntityManager::EntityManager(){}

std::shared_ptr<Entity> EntityManager::addEntity(const std::string& tag)
{
	auto e = std::shared_ptr<Entity>(new Entity(m_totalEntities++, tag));
	m_toAdd.push_back(e);
	return e;
}

EntityVector& EntityManager::getEntities()
{
	return m_entities;
}

EntityVector& EntityManager::getEntities(const std::string& tag)
{
	return m_entityMap[tag];
}

void EntityManager::update()
{
	//adding entities
	for (auto e : m_toAdd)
	{
		m_entities.push_back(e);
		m_entityMap[e->tag()].push_back(e);
	}

	m_toAdd.clear();

	//removing entities from entities vector and entities map
	m_entities.erase(std::remove_if(begin(m_entities), end(m_entities), [](auto& e) { return !e->isActive(); }), end(m_entities));
	//remove entities from the entity map
	for (auto& pair: m_entityMap)
	{
		auto& vector = pair.second;
		vector.erase(std::remove_if(begin(vector), end(vector), [](auto& e) { return !e->isActive(); }), end(vector));
	}

	m_toAdd.clear();
}