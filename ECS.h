#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <bitset>
#include <array>
#include "ECS.h"

class Component;
class Entity;
class Manager;

using ComponentId = std::size_t;
using Group = std::size_t;

inline ComponentId getNewComponentTypeId()
{
	static ComponentId lastID = 0u;
	return lastID++;
}

template <typename T> inline ComponentId getComponentTypeId() noexcept
{
	static ComponentId typeID = getNewComponentTypeId();
	return typeID;
}

constexpr std::size_t maxComponents = 32; // max components an entity can hold
constexpr std::size_t maxGroups = 32;

using ComponentBitSet = std::bitset<maxComponents>; // basically an array of 32 bits, 1s or 0s
using GroupBitSet = std::bitset<maxGroups>;
using ComponentArray = std::array<Component*, maxComponents>; // array of component pointers

class Component // components are parts of an entitiy
{
public:
	Entity* entity; // pointer to its "owner"

	virtual void init() {}
	virtual void update() {}
	virtual void draw() {}
	virtual ~Component() {}
};

class Entity // keeps list of components
{
private:
	Manager &manager;
	bool active = true; // shows whether the entity is still on screen or "active"
	std::vector<std::unique_ptr<Component>> components; // unique ptr manages the vector of components

	ComponentArray componentArray;
	ComponentBitSet componentBitSet; 
	GroupBitSet groupBitSet;

public:
	Manager& getManager() { return this->manager; }

	Entity(Manager& mManager) : manager(mManager) {}
	void update()
	{
		// auto automatically determines the correct type for 'c'
		for (auto& c : components) c->update(); // for every component in vector of components, update each one
	}

	void draw() 
	{
		for (auto& c : components) c->draw();
	}

	bool isActive() const { return active; } 
	void destroy() { active = false; }

	bool hasGroup(Group mGroup)
	{
		return groupBitSet[mGroup];
	}

	void addGroup(Group mGroup);
	void deleteGroup(Group mGroup)
	{
		groupBitSet[mGroup] = false;
	}

	template <typename T> bool hasComponent() const
	{
		return componentBitSet[getComponentTypeId<T>()];
	}

	template <typename T, typename... TArgs> T& addComponent(TArgs&&... mArgs)
	{
		T* c(new T(std::forward<TArgs>(mArgs)...));
		c->entity = this;
		std::unique_ptr<Component> uPtr{ c };
		components.emplace_back(std::move(uPtr));

		componentArray[getComponentTypeId<T>()] = c;  // puts component in the array
		componentBitSet[getComponentTypeId<T>()] = true; // and updates the next bit to true, meaning there is another component

		c->init();
		return *c;
	}

	template <typename T> T& getComponent() const
	{
		auto ptr(componentArray[getComponentTypeId<T>()]); // creates pointer to position in component array
		return *static_cast<T*>(ptr); 
	}

	void setActive(bool act)
	{
		this->active = act;
	}
};

class Manager
{
private:
	std::array<std::vector<Entity*>, maxGroups> groupedEntities;

public:
	std::vector<std::unique_ptr<Entity>> entities; // vector of smart pointers to entities of the game

	void update() // updates all of the entities
	{
		for (auto &e : entities) e->update(); 
	}

	void draw()
	{
		for (auto &e : entities) e->draw(); // draws each entities
	}

	void refresh()
	{
		for (auto i(0u); i < maxGroups; i++)
		{
			auto& v(groupedEntities[i]);
			v.erase(
				std::remove_if(std::begin(v), std::end(v),
					[i](Entity* mEntity)
			{
				return !mEntity->isActive() || !mEntity->hasGroup(i);
			}),
				std::end(v));
		}

		entities.erase(std::remove_if(std::begin(entities), std::end(entities), 
			[](const std::unique_ptr<Entity> &mEntity)
		{
			return !mEntity->isActive();
		}), std::end(entities)); // removes any entities that are inactive
	}

	void AddToGroup(Entity* mEntity, Group mGroup)
	{
		groupedEntities[mGroup].emplace_back(mEntity);
	}

	std::vector<Entity*>& getGroup(Group mGroup)
	{
		return groupedEntities[mGroup];
	}

	Entity& addEntity()
	{
		Entity *e = new Entity(*this);
		std::unique_ptr<Entity> uPtr{ e };

		entities.emplace_back(std::move(uPtr)); // adds it to the vecotr list
		return *e;
	}

	Entity& addRoot()
	{
		Entity *e = new Entity(*this);
		std::unique_ptr<Entity> uPtr{ e };

		entities.emplace_back(std::move(uPtr)); // adds it to the vecotr list
		return *e;
	}

	void freezeEntities()
	{
		for (auto& e : entities)
		{
			e->destroy();
		}
	}
};