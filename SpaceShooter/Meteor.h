#pragma once

#include "Component.h"
#include "glm/glm.hpp"

namespace dap {
	struct CollisionData;

	class Meteor : public dap::Component {
	private:
		static const float DELETE_TIME;

	private:
		float nextDeleteTime = 0.0f;
		float sizeMin = 0.5f;
		float sizeMax = 5.0f;
		float speedMin = 0.03f;
		float speedMax = 0.1f;
		float rotationSpeed = 0.0f;
		float rotationSpeedMax = 2.0f;
		glm::vec2 velocity;

	protected:
		virtual void Collision ( dap::CollisionData& data ) override;
		virtual void Start () override;
		virtual void Tick () override;
	};
}
