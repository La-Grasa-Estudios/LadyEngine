#include "Entity.hpp"

EntityManager::EntityManager()
{
	for (Entity entity = 0; entity < MAX_ENTITIES; ++entity)
	{
		mAvailableEntities.push(entity);
	}
}

Entity EntityManager::CreateEntity()
{
	if(mLivingEntitiesCount > MAX_ENTITIES)
	{
		std::cout << "\nToo many entities while creating an entity\n";
		return -1;
	}
	else
	{
		Entity id = mAvailableEntities.front();
		mAvailableEntities.pop();
		++mLivingEntitiesCount;
		return id;
	}

}

void EntityManager::DestroyEntity(Entity entity)
{
	if(entity < MAX_ENTITIES)
	{
		std::cout << "\nEntity out of range while to destroy an entity\n";
		return;
	}
	else
	{
		mSignatures[entity].reset();
		mAvailableEntities.push(entity);
		--mLivingEntitiesCount;
	}
}

void EntityManager::SetSignature(Entity entity, Signature signature)
{
	if(entity > MAX_ENTITIES)
	{
		std::cout << "\nEntity out of range while trying to set signature\n";
		return;
	}
	else
	{
		mSignatures[entity] = signature;
	}
}

Signature EntityManager::GetSignature(Entity entity)
{
	 if(entity > MAX_ENTITIES)
	 {
		 std::cout << "\nEntity out of range while triying to get signature\n";
		 return -1;
	}
	 else
	 {
		 return mSignatures[entity];
	 }
}



