#include "ECS.h"

int IComponent::nextID = 0;

Entity::Entity(int id)
	:ID(id)
{

}
int Entity::GetID() const
{
	return ID;
}

void Entity::Kill()
{
	registry->KillEntity(*this);
}

void System::AddEntityToSystem(Entity entity)
{
	entities.push_back(entity);
}
void System::RemoveEntity(Entity entity)
{
	entities.erase(std::remove_if(entities.begin(), entities.end(), [&entity](Entity other){
			return entity.GetID() == other.GetID();})
		, entities.end());
}

std::vector<Entity> System::GetSystemEntities() const
{
	return entities;
}
const Signature& System::GetComponentSignature() const
{
	return componentSignature;
}


void Registry::Update()
{
	for (auto& entity : entitiesToBeAdded)
	{
		AddEntityToSystems(entity);
	}
	entitiesToBeAdded.clear();

	for (auto& entity : entitiesToBeDeleted)
	{
		RemoveEntityFromSystems(entity);
		entityComponentSignatures[entity.GetID()].reset();
		freeIDs.push_back(entity.GetID());
	}
	entitiesToBeDeleted.clear();



}

Entity Registry::CreateEntity()
{
	int entityId;
	if (freeIDs.empty())
	{
		entityId = numEntitites++;
		if (entityId >= entityComponentSignatures.size())
		{
			entityComponentSignatures.resize(entityId + 1);
		}
	}
	else
	{
		entityId = freeIDs.front();
		freeIDs.pop_front();
	}

	Entity entity(entityId);
	entity.registry = this;
	entitiesToBeAdded.insert(entity);

	spdlog::info("An entity has created with id " + std::to_string(entityId));

	return entity;
}

void Registry::AddEntityToSystems(Entity entity)
{
	const auto entityID = entity.GetID();
	const auto& entityComponentSignature = entityComponentSignatures[entityID];

	for (auto& system : systems)
	{
		const auto& systemComponentSignature = system.second->GetComponentSignature();

		bool isInterested = ((entityComponentSignature & systemComponentSignature) == systemComponentSignature);

		if (isInterested)
		{
			system.second->AddEntityToSystem(entity);
		}
	}
}

void Registry::RemoveEntityFromSystems(Entity entity)
{
	for (auto system : systems)
	{
		system.second->RemoveEntity(entity);
	}
}

void Registry::KillEntity(Entity entity)
{
	entitiesToBeDeleted.insert(entity);
}
