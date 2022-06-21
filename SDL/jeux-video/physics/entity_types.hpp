extern entites::Coordinator Conductor;

class Moving_day : public entites::System
{
public:
	void Init();

	void Update(float dt);
};


void Moving_day::Init()
{
}

void Moving_day::Update(float dt)
{
	for (auto const& entity : mEntities)
	{
		// auto& rigidBody = gCoordinator.GetComponent<RigidBody>(entity);
		// auto& transform = gCoordinator.GetComponent<Transform>(entity);
		// auto const& gravity = gCoordinator.GetComponent<Gravity>(entity);

		// transform.position += rigidBody.velocity * dt;

		// rigidBody.velocity += gravity.force * dt;
	}
}