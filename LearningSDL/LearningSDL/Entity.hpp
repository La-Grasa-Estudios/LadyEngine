#pragma once
#ifndef ENTITY_HPP
#define ENTITY_HPP
#include <iostream>
#include <queue>
#include "Components.hpp"
#include <array>

using Entity = std::uint64_t;
const Entity MAX_ENTITIES = 100000;

class EntityManager
{
private:
	std::queue<Entity> mAvailableEntities;
	std::array<Signature, MAX_ENTITIES> mSignatures;
	uint64_t mLivingEntitiesCount;

public:
	EntityManager();
	~EntityManager();
	
	Entity CreateEntity();

	void DestroyEntity(Entity entity);
	void SetSignature(Entity entity, Signature signature);
	Signature GetSignature(Entity entity);


};

#endif //!ENTITY_HPP