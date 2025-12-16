#ifndef ECS_H
#define ECS_H
#include <bitset>
#include <vector>

const unsigned int MAX_COMPONENTS = 32;

typedef std::bitset<MAX_COMPONENTS> Signature;


class Entity
{
public:
	Entity(int id);
	int GetID() const;
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
	/*
		Returns the id of a component type.
	*/
	static int GetID()
	{
		const auto id = nextID++;
		return id;
	}
};

class System
{
public:

	System() = default;
	~System() = default;

	void AddEntity(Entity entity);
	void RemoveEntity(Entity entity);

	std::vector<Entity> GetEntities() const;
	const Signature& GetSignature() const;

private:

	Signature& componentSignature;
	std::vector<Entity> entities;

	/*
		Defines the component type that entities must have to be considered by the system.
	*/
	template <typename T>
	void RequireComponent();
};

class Registry
{

};

#endif // !ECS_H
