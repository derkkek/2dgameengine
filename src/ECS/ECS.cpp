#include "ECS.h"


void System::AddEntity(Entity entity)
{
	entities.push_back(entity);
}
void System::RemoveEntity(Entity entity)
{
	entities.erase(std::remove_if(entities.begin(), entities.end(), [&entity](Entity other){
			return entity.GetID() == other.GetID();})
		, entities.end());
}

std::vector<Entity> System::GetEntities() const
{
	return entities;
}
const Signature& System::GetSignature() const
{
	return componentSignature;
}


template<typename T>
void System::RequireComponent()
{
	const auto componentID = Component<T>::GetID();
	componentSignature.set(componentID);
}