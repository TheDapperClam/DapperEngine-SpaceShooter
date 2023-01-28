#pragma once

#include "Component.h"
#include "glm/glm.hpp"
#include <string>

namespace dap {
	class AudioSource;
	struct CollisionData;

	class Ship : public dap::Component {
	protected:
		static const std::string BULLET_PREFAB;
		static const std::string DEATH_SOUND;

	protected:
		float bulletOffset = 1.0f;
		float bulletSpeed = 0.5f;
		bool canShoot = true;
		int health = 1;
		float maxSpeed = 0.2f;
		float moveAcceleration = 0.02f;
		float moveDeceleration = 0.075f;
		glm::vec2 moveInput;
		float shootInterval = 0.2f;
		float nextShootTime = 0.0f;
		glm::vec2 velocity;
		dap::AudioSource* deathSoundSource = NULL;

	private:
		bool dead = false;

	public:
		/*Has this ship been destroyed*/
		bool IsDead ();

	protected:
		/*Function for making our ship shoot*/
		bool Shoot ( int layer, int mask, unsigned int pointsOnHit = 0 );
		/*Event for when this ship is destroyed*/
		virtual void OnDeath () {};

	protected:
		virtual void Collision ( dap::CollisionData& data ) override;
		virtual void Start () override;
		virtual void Tick () override;
	};
}
