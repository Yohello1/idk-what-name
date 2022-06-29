namespace entites
{
	/*
	class ecs_manager
	{
	};
	class basic_entity
	{
		// What does every entity have?

		// Health
		// Mana?
		// Location
		// Borders
		// Texture?

	protected:
		cord_2d position;
		cord_2d border[2];

	public:
	};
	*/
	// https://code.austinmorlan.com/austin/ecs/src/branch/master/LICENSE
	using Entity = std::uint32_t;
	const Entity MAX_ENTITIES = 16384; // 2^14

#include "entity_compnents.cpp"

	// Nick
	using ComponentType = std::uint8_t;
	const ComponentType MAX_COMPONENTS = 32;

	// Signatures
	using Signature = std::bitset<MAX_COMPONENTS>;

	class Entity_Manager
	{
	public:
		Entity_Manager()
		{
			for (Entity entity = 0; entity < MAX_ENTITIES; entity++)
			{
				mAvailableEntities.push(entity);
			}
		}
		Entity create_entity()
		{
			assert(mLivingEntityCount < MAX_ENTITIES && "TOO MANY ENTITIES EXIST.");

			// Get an id, first one
			Entity id = mAvailableEntities.front();
			mAvailableEntities.pop();
			mLivingEntityCount++;

			return id;
		}

		void DestroyEntity(Entity entity)
		{
			assert(entity < MAX_ENTITIES && "Entity out of range.");

			// Invalidate the destroyed entity's signature
			mSignatures[entity].reset();

			// Put the destroyed ID at the back of the queue
			mAvailableEntities.push(entity);
			--mLivingEntityCount;
		}
		void SetSignature(Entity entity, Signature signature)
		{
			assert(entity < MAX_ENTITIES && "Entity out of range.");

			// Put this entity's signature into the array
			mSignatures[entity] = signature;
		}

		Signature GetSignature(Entity entity)
		{
			assert(entity < MAX_ENTITIES && "Entity out of range.");

			// Get this entity's signature from the array
			return mSignatures[entity];
		}

	private:
		// Queue of unused entity IDs
		std::queue<Entity> mAvailableEntities{};

		// Array of signatures where the index corresponds to the entity ID
		std::array<Signature, MAX_ENTITIES> mSignatures{};

		// Total living entities - used to keep limits on how many exist
		uint32_t mLivingEntityCount{};
	};

	class IComponentArray
	{
	public:
		virtual ~IComponentArray() = default;
		virtual void EntityDestroyed(Entity entity) = 0;
	};

	template <typename T>
	class ComponentArray : public IComponentArray
	{
	public:
		void InsertData(Entity entity, T component)
		{
			assert(mEntityToIndexMap.find(entity) == mEntityToIndexMap.end() && "Component added to same entity more than once.");

			// Put new entry at end
			size_t newIndex = mSize;
			mEntityToIndexMap[entity] = newIndex;
			mIndexToEntityMap[newIndex] = entity;
			mComponentArray[newIndex] = component;
			++mSize;
		}

		void RemoveData(Entity entity)
		{
			assert(mEntityToIndexMap.find(entity) != mEntityToIndexMap.end() && "Removing non-existent component.");

			// Copy element at end into deleted element's place to maintain density
			size_t indexOfRemovedEntity = mEntityToIndexMap[entity];
			size_t indexOfLastElement = mSize - 1;
			mComponentArray[indexOfRemovedEntity] = mComponentArray[indexOfLastElement];

			// Update map to point to moved spot
			Entity entityOfLastElement = mIndexToEntityMap[indexOfLastElement];
			mEntityToIndexMap[entityOfLastElement] = indexOfRemovedEntity;
			mIndexToEntityMap[indexOfRemovedEntity] = entityOfLastElement;

			mEntityToIndexMap.erase(entity);
			mIndexToEntityMap.erase(indexOfLastElement);

			--mSize;
		}

		T &GetData(Entity entity)
		{
			assert(mEntityToIndexMap.find(entity) != mEntityToIndexMap.end() && "Retrieving non-existent component.");

			return mComponentArray[mEntityToIndexMap[entity]];
		}

		void EntityDestroyed(Entity entity) override
		{
			if (mEntityToIndexMap.find(entity) != mEntityToIndexMap.end())
			{
				RemoveData(entity);
			}
		}

	private:
		std::array<T, MAX_ENTITIES> mComponentArray{};
		std::unordered_map<Entity, size_t> mEntityToIndexMap{};
		std::unordered_map<size_t, Entity> mIndexToEntityMap{};
		size_t mSize{};
	};

	class ComponentManager
	{
	public:
		template <typename T>
		void RegisterComponent()
		{
			const char *typeName = typeid(T).name();

			assert(mComponentTypes.find(typeName) == mComponentTypes.end() && "Registering component type more than once.");

			// Add this component type to the component type map
			mComponentTypes.insert({typeName, mNextComponentType});

			// Create a ComponentArray pointer and add it to the component arrays map
			mComponentArrays.insert({typeName, std::make_shared<ComponentArray<T>>()});

			// Increment the value so that the next component registered will be different
			++mNextComponentType;
		}

		template <typename T>
		ComponentType GetComponentType()
		{
			const char *typeName = typeid(T).name();

			assert(mComponentTypes.find(typeName) != mComponentTypes.end() && "Component not registered before use.");

			// Return this component's type - used for creating signatures
			return mComponentTypes[typeName];
		}

		template <typename T>
		void AddComponent(Entity entity, T component)
		{
			// Add a component to the array for an entity
			GetComponentArray<T>()->InsertData(entity, component);
		}

		template <typename T>
		void RemoveComponent(Entity entity)
		{
			// Remove a component from the array for an entity
			GetComponentArray<T>()->RemoveData(entity);
		}

		template <typename T>
		T &GetComponent(Entity entity)
		{
			// Get a reference to a component from the array for an entity
			return GetComponentArray<T>()->GetData(entity);
		}

		void EntityDestroyed(Entity entity)
		{
			// Notify each component array that an entity has been destroyed
			// If it has a component for that entity, it will remove it
			for (auto const &pair : mComponentArrays)
			{
				auto const &component = pair.second;

				component->EntityDestroyed(entity);
			}
		}

	private:
		// Map from type string pointer to a component type
		std::unordered_map<const char *, ComponentType> mComponentTypes{};

		// Map from type string pointer to a component array
		std::unordered_map<const char *, std::shared_ptr<IComponentArray>> mComponentArrays{};

		// The component type to be assigned to the next registered component - starting at 0
		ComponentType mNextComponentType{};

		// Convenience function to get the statically casted pointer to the ComponentArray of type T.
		template <typename T>
		std::shared_ptr<ComponentArray<T>> GetComponentArray()
		{
			const char *typeName = typeid(T).name();

			assert(mComponentTypes.find(typeName) != mComponentTypes.end() && "Component not registered before use.");

			return std::static_pointer_cast<ComponentArray<T>>(mComponentArrays[typeName]);
		}
	};

	class System
	{
	public:
		std::set<Entity> mEntities;
	};

	class SystemManager
	{
	public:
		template <typename T>
		std::shared_ptr<T> RegisterSystem()
		{
			const char *typeName = typeid(T).name();

			assert(mSystems.find(typeName) == mSystems.end() && "Registering system more than once.");

			// Create a pointer to the system and return it so it can be used externally
			auto system = std::make_shared<T>();
			mSystems.insert({typeName, system});
			return system;
		}

		template <typename T>
		void SetSignature(Signature signature)
		{
			const char *typeName = typeid(T).name();

			assert(mSystems.find(typeName) != mSystems.end() && "System used before registered.");

			// Set the signature for this system
			mSignatures.insert({typeName, signature});
		}

		void EntityDestroyed(Entity entity)
		{
			// Erase a destroyed entity from all system lists
			// mEntities is a set so no check needed
			for (auto const &pair : mSystems)
			{
				auto const &system = pair.second;

				system->mEntities.erase(entity);
			}
		}

		void EntitySignatureChanged(Entity entity, Signature entitySignature)
		{
			// Notify each system that an entity's signature changed
			for (auto const &pair : mSystems)
			{
				auto const &type = pair.first;
				auto const &system = pair.second;
				auto const &systemSignature = mSignatures[type];

				// Entity signature matches system signature - insert into set
				if ((entitySignature & systemSignature) == systemSignature)
				{
					system->mEntities.insert(entity);
				}
				// Entity signature does not match system signature - erase from set
				else
				{
					system->mEntities.erase(entity);
				}
			}
		}

	private:
		// Map from system type string pointer to a signature
		std::unordered_map<const char *, Signature> mSignatures{};

		// Map from system type string pointer to a system pointer
		std::unordered_map<const char *, std::shared_ptr<System>> mSystems{};
	};

	class Coordinator
	{
	public:
		void Init()
		{
			// Create pointers to each manager
			mComponentManager = std::make_unique<ComponentManager>();
			mEntity_Manager = std::make_unique<Entity_Manager>();
			mSystemManager = std::make_unique<SystemManager>();
		}

		// Entity methods
		Entity CreateEntity()
		{
			return mEntity_Manager->create_entity();
		}

		void DestroyEntity(Entity entity)
		{
			mEntity_Manager->DestroyEntity(entity);

			mComponentManager->EntityDestroyed(entity);

			mSystemManager->EntityDestroyed(entity);
		}

		// Component methods
		template <typename T>
		void RegisterComponent()
		{
			mComponentManager->RegisterComponent<T>();
		}

		template <typename T>
		void AddComponent(Entity entity, T component)
		{
			mComponentManager->AddComponent<T>(entity, component);

			auto signature = mEntity_Manager->GetSignature(entity);
			signature.set(mComponentManager->GetComponentType<T>(), true);
			mEntity_Manager->SetSignature(entity, signature);

			mSystemManager->EntitySignatureChanged(entity, signature);
		}

		template <typename T>
		void RemoveComponent(Entity entity)
		{
			mComponentManager->RemoveComponent<T>(entity);

			auto signature = mEntity_Manager->GetSignature(entity);
			signature.set(mComponentManager->GetComponentType<T>(), false);
			mEntity_Manager->SetSignature(entity, signature);

			mSystemManager->EntitySignatureChanged(entity, signature);
		}

		template <typename T>
		T &GetComponent(Entity entity)
		{
			return mComponentManager->GetComponent<T>(entity);
		}

		template <typename T>
		ComponentType GetComponentType()
		{
			return mComponentManager->GetComponentType<T>();
		}

		// System methods
		template <typename T>
		std::shared_ptr<T> RegisterSystem()
		{
			return mSystemManager->RegisterSystem<T>();
		}

		template <typename T>
		void SetSystemSignature(Signature signature)
		{
			mSystemManager->SetSignature<T>(signature);
		}

	private:
		std::unique_ptr<ComponentManager> mComponentManager;
		std::unique_ptr<Entity_Manager> mEntity_Manager;
		std::unique_ptr<SystemManager> mSystemManager;
	};
}
