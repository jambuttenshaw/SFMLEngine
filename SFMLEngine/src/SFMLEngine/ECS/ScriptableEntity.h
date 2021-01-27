#pragma once

namespace SFMLEngine {

	class ScriptableEntity
	{
	public:
		virtual ~ScriptableEntity() {};

		virtual void Awake() {};
		virtual void Start() {};

		virtual void Update(float timestep) {};
	};

}
