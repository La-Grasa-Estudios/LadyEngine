#pragma once
#ifndef COMPONENTARRAY_HPP
#define COMPONENTARRAY_HPP
#include <unordered_map>
#include "Entity.hpp"

class IComponentArray
{
public:
	virtual ~IComponentArray() = default;
	virtual void DestroyEntity(Entity entity) = 0;
};

template <typename T>
class ComponentArray : public IComponetArray
{
private:
	std::array<T, MAX_ENTITIES> mComponentArray;
	std::unordered_map<Entity, size_t> mEntitytoIndexMap;
	std::unordered_map<size_t, Entity> mIndextoEntityMap;
	size_t mSize;

public:
	void InsertData(Entity entity, T component)
	{
		if (!(mEntitytoIndexMap.find(entity) == mEntitytoIndexMap.end()))
		{
			std::cout << "\nTrying to add a duplicate component for the same entity.Ensure each entity has only one of this component type\n";
			return;
		}
		rsize_t newIndex = mSize;
		mEntitytoIndexMap[entity] = newIndex;
		mIndextoEntity[newIndex] = entity;
		mComponentArray[newIndex] = component;
		++mSize;
	}

	

};

#endif //!COMPONENTARRAY_HPP