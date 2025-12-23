#ifndef ECS_H
#define ECS_H
#include <bitset>
#include <vector>
#include <unordered_map>
#include <typeindex>
#include <set>
#define FMT_UNICODE 0
#include <spdlog/spdlog.h>
#include <deque>
const unsigned int MAX_COMPONENTS = 32;

typedef std::bitset<MAX_COMPONENTS> Signature;


class Entity
{
public:
	Entity(int id);
	int GetID() const;
	bool operator<(const Entity& other) const { return ID < other.ID; };

	template<typename T, typename ...Targs>
	void AddComponent(Targs&&... args);

	template<typename T>
	void RemoveComponent();

	template<typename T>
	bool HasComponent();

	template<typename T>
	T& GetComponent();

	void Kill();

	//forward declaration
	class Registry* registry;
private:
	int ID;
};


struct IComponent
{
protected:
	static int nextID;
};

/*
	Used to assign unique id to a component type.
*/
template<typename T>
class Component : public IComponent
{
public:
	/*
		Returns the id of a component type.
	*/
	static int GetID()
	{
		static auto id = nextID++;
		return id;
	}
};

class System
{
public:

	System() = default;
	~System() = default;

	void AddEntityToSystem(Entity entity);
	void RemoveEntity(Entity entity);

	std::vector<Entity> GetSystemEntities() const;
	const Signature& GetComponentSignature() const;

	/*
		Defines the component type that entities must have to be considered by the system.
	*/
	template <typename T>
	void RequireComponent();

private:

	Signature componentSignature;
	std::vector<Entity> entities;


};

class IPool
{
public:
	virtual ~IPool() = default;
};

template<typename T>
class Pool : public IPool
{
public:
	Pool(int size = 100)
	{
		data.resize(size);
	}
	virtual ~Pool() = default;
	bool IsEmpty()
	{
		return data.empty();
	}
	bool GetSize()
	{
		return data.size();
	}
	void Resize(int size)
	{
		data.resize(size);
	}
	void Clear()
	{
		data.clear();
	}
	void Add(T object)
	{
		data.push_back(object);
	}
	void Set(int index, T object)
	{
		data[index] = object;
	}
	T& Get(int index)
	{
		return static_cast<T&>(data[index]);
	}
	T& operator [](unsigned int index)
	{
		return data[index];
	}
private:
	/*
	Contigous block of memory of component data
	*/
	std::vector<T> data;
};

class Registry
{
public:
	Registry() = default;

	void Update();

	Entity CreateEntity();
	
	/*
		Targs means multiple generic arguments.
	*/
	template<typename T, typename... Targs>
	void AddComponent(Entity entity, Targs&&... args);

	template<typename T>
	bool HasComponent(Entity entity);

	template<typename T>
	void RemoveComponent(Entity entity);

	template<typename T>
	T& GetComponent(Entity entity);

	template<typename T, typename... Targs>
	void AddSystem(Targs&&... args);

	template<typename T>
	void RemoveSystem();

	template<typename T>
	bool HasSystem();

	template<typename T>
	T& GetSystem();

	void AddEntityToSystems(Entity entity);

	void RemoveEntityFromSystems(Entity entity);

	void KillEntity(Entity entity);
private:
	int numEntitites = 0;
	/*
		Keeps pools of all the data for a certain component type based on index.
		Vector index = component type id.
		Pool index = entity id.
	*/
	std::vector<std::shared_ptr<IPool>> componentPools;

	/*
		Holds component signatures per entity, saying which component is "on" for an entity.
		vector index = entity index.
	*/
	std::vector<Signature> entityComponentSignatures;

	std::unordered_map<std::type_index, std::shared_ptr<System>> systems;

	/*Sets of entitites to be added or deleted in the next registry update in order to don't mess the systems working status.*/
	std::set<Entity> entitiesToBeAdded;
	std::set<Entity> entitiesToBeDeleted;

	/*free ids from previously deleted entitites.*/
	std::deque<int> freeIDs;

};

template<typename T>
void System::RequireComponent()
{
	const auto componentID = Component<T>::GetID();
	componentSignature.set(componentID);
}

template <typename T, typename... Targs>
void Registry::AddComponent(Entity entity, Targs&&... args)
{
	const auto componenetID = Component<T>::GetID();
	const auto entityID = entity.GetID();

	/*
		If component id is greater than the current size of the component pools, then resize the component pools.	
	*/
	if (componenetID >= componentPools.size())
	{
		componentPools.resize(componenetID + 1, nullptr);
	}

	/*
		If we still don't have a pool for that component type, then create a new pool based on that type.
	*/
	if (!componentPools[componenetID])
	{
		std::shared_ptr<Pool<T>> newComponentPool = std::make_shared<Pool<T>>();
		componentPools[componenetID] = newComponentPool;
	}

	/*
		Get the pool of component values for that component type.
	*/
	std::shared_ptr<Pool<T>> componentPool = std::static_pointer_cast<Pool<T>>(componentPools[componenetID]);


	/*
		If the entity id is bigger than the component pool's size then resize the component pool.
	*/
	if (entityID >= componentPool->GetSize())
	{
		componentPool->Resize(numEntitites);
	}
	/*
		Create a new component as generic component type and push arguments to the constructor.
	*/
	T newComponent(std::forward<Targs>(args)...);

	/*
		Go to the pool of the component type and add the new component
		based on the index of the component holder entity.
	*/
	componentPool->Set(entityID, newComponent);

	/*
		Change the component signature of the entity and set the component id on the bitset to 1.
	*/
	entityComponentSignatures[entityID].set(componenetID);

	spdlog::info("Component ID: " + std::to_string(componenetID) + " was added to entity id: " + std::to_string(entityID));
}

template<typename T>
bool Registry::HasComponent(Entity entity)
{
	const auto componentID = Component<T>::GetID();
	const auto entityID = entity.GetID();

	return entityComponentSignatures[entityID].test(componentID);
}

template<typename T>
void Registry::RemoveComponent(Entity entity)
{
	const auto componentID = Component<T>::GetID();
	const auto entityID = entity.GetID();

	entityComponentSignatures[entityID].set(componentID, false);
	spdlog::info("Component ID: " + std::to_string(componentID) + " was removed from entity id: " + std::to_string(entityID));

}

template<typename T, typename... Targs>
void Registry::AddSystem(Targs&&... args)
{
	std::shared_ptr<T> newSystem = std::make_shared<T>(std::forward<Targs>(args)...);
	systems.insert(std::make_pair(std::type_index(typeid(T)), newSystem));
}

template<typename T>
void Registry::RemoveSystem()
{
	auto system = systems.find(std::type_index(typeid(T)));
	systems.erase(system);
}

template<typename T>
bool Registry::HasSystem()
{
	return systems.find(std::type_index(typeid(T))) != systems.end();
}

template<typename T>
T& Registry::GetSystem()
{
	auto system = systems.find(std::type_index(typeid(T)));
	return *(std::static_pointer_cast<T>(system->second));
}

template<typename T>
T& Registry::GetComponent(Entity entity)
{
	const auto componentID = Component<T>::GetID();
	const auto entityID = entity.GetID();

	auto componentPool = std::static_pointer_cast<Pool<T>>(componentPools[componentID]);

	return componentPool->Get(entityID);
}

template <typename T, typename... Targs>
void Entity::AddComponent(Targs&&... args)
{
	this->registry->AddComponent<T>(*this, std::forward<Targs>(args)...);
}

template<typename T>
void Entity::RemoveComponent()
{
	this->registry->RemoveComponent<T>(*this);
}

template<typename T>
bool Entity::HasComponent()
{
	return this->registry->HasComponent<T>(*this);
}

template<typename T>
T& Entity::GetComponent()
{
	return this->registry->GetComponent<T>(*this);
}
#endif // !ECS_H
