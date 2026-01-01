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

void Entity::Tag(const std::string& tag)
{
	registry->TagEntity(*this, tag);
}

bool Entity::HasTag(const std::string& tag)
{
	return registry->EntityHasTag(*this,tag);
}

void Entity::Group(const std::string& group)
{
	registry->GroupEntity(*this, group);
}

bool Entity::BelongsToGroup(const std::string& group)
{
	return registry->EntityBelongsToGroup(*this, group);
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

void Registry::TagEntity(Entity entity, const std::string& tag)
{
	entityPerTag.emplace(tag, entity);
	tagPerEntity.emplace(entity.GetID(), tag);
}

bool Registry::EntityHasTag(Entity entity, const std::string& tag)
{
	if (tagPerEntity.find(entity.GetID()) == tagPerEntity.end())
	{
		return false;
	}
	return entityPerTag.find(tag)->second.GetID() == entity.GetID();
}

Entity Registry::GetEntityByTag(const std::string& tag)
{
	if (entityPerTag.find(tag) == entityPerTag.end()) {
		spdlog::warn("Trying to get entity by tag, but tag '" + tag + "' does not exist.");
		// Return a default/invalid entity or handle appropriately
		return Entity(0); // or throw a more descriptive exception
	}
	return entityPerTag.at(tag);
}

void Registry::RemoveEntityTag(Entity entity)
{
	auto taggedEntity = tagPerEntity.find(entity.GetID());
	if (taggedEntity != tagPerEntity.end())
	{
		auto tag = taggedEntity->second;
		entityPerTag.erase(tag);
		tagPerEntity.erase(taggedEntity);
	}
}

void Registry::GroupEntity(Entity entity, const std::string& group)
{
	entitiesPerGroup.emplace(group, std::set<Entity>());
	entitiesPerGroup[group].emplace(entity);
	groupPerEntity.emplace(entity.GetID(), group);
}

bool Registry::EntityBelongsToGroup(Entity entity, const std::string& group)
{
	if (entitiesPerGroup.find(group) == entitiesPerGroup.end()) {
		return false; // Group doesn't exist, so entity can't belong to it
	}
	auto groupEntities = entitiesPerGroup.at(group);
	return groupEntities.find(entity.GetID()) != groupEntities.end();
}

std::vector<Entity> Registry::GetEntitiesByGroup(const std::string& group)
{
	if (entitiesPerGroup.find(group) == entitiesPerGroup.end()) {
		spdlog::warn("Trying to get entities by group, but group '" + group + "' does not exist.");
		return std::vector<Entity>(); // Return empty vector
	}
	auto& setOfEntities = entitiesPerGroup.at(group);
	return std::vector<Entity>(setOfEntities.begin(), setOfEntities.end());
}

void Registry::RemoveEntityGroup(Entity entity) 
{
	// if in group, remove entity from group management
	auto groupedEntity = groupPerEntity.find(entity.GetID());
	if (groupedEntity != groupPerEntity.end()) {
		auto group = entitiesPerGroup.find(groupedEntity->second);
		if (group != entitiesPerGroup.end()) {
			auto entityInGroup = group->second.find(entity);
			if (entityInGroup != group->second.end()) {
				group->second.erase(entityInGroup);
			}
		}
		groupPerEntity.erase(groupedEntity);
	}
}
