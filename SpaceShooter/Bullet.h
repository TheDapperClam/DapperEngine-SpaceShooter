#pragma once

#include "Component.h"
#include "glm/glm.hpp"

namespace dap {
	class AudioSource;
	struct CollisionData;

	class Bullet : public dap::Component {
	private:
		static const float DELETE_DELAY;
		static const std::string SOUND_IMPACT_NAME;

	private:
		glm::vec2 velocity;
		float deleteTime = 0.0f;
		unsigned int pointsOnHit = 0;
		dap::AudioSource* impactSoundSource = NULL;

	public:
		void SetPointsOnHit ( unsigned int amount );
		void SetVelocity ( glm::vec2 vel );

	public:
		virtual void Collision ( dap::CollisionData& data ) override;
		virtual void Start () override;
		virtual void Tick () override;
	};
}
