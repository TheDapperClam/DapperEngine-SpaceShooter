#include "Ship.h"

#include <AudioSource.h>
#include "Bullet.h"
#include "CollisionData.h"
#include "GameObject.h"
#include "RectCollider.h"
#include "Rendering.h"
#include "Scene.h"
#include <SoundClip.h>
#include "Transform.h"

const std::string dap::Ship::BULLET_PREFAB = "Resources/Prefabs/Bullet.dpf";
const std::string dap::Ship::DEATH_SOUND = "explosion";

void dap::Ship::Collision ( dap::CollisionData& data ) {
	health--;
	if ( health <= 0 ) {
		dead = true;
		deathSoundSource->Play ();
		OnDeath ();
		GetOwner ()->Destroy ();
	}
}

/*Has this ship been destroyed*/
bool dap::Ship::IsDead () {
	return dead;
}

/*Function for making our ship shoot*/
bool dap::Ship::Shoot ( int layer, int mask, unsigned int pointsOnHit ) {
	if ( !canShoot )
		return false;
	const float TIME = dap::Rendering::GetTime ();
	if ( TIME < nextShootTime )
		return false;
	dap::GameObject* bullet = dap::Scene::GetCurrent ()->SpawnPrefab ( BULLET_PREFAB );
	// We set the new bullet's layer and mask to determine what it will be colliding with
	bullet->GetComponent<RectCollider> ()->SetCollisionLayer ( layer );
	bullet->GetComponent<RectCollider> ()->SetCollisionMask ( mask );
	glm::vec2 spawnPos = GetOwner ()->GetTransform ()->GetPosition ();
	spawnPos.y += bulletOffset;
	bullet->GetTransform ()->SetPosition ( spawnPos );
	bullet->GetComponent<dap::Bullet> ()->SetVelocity ( glm::vec2 ( 0.0f, bulletSpeed ) );
	bullet->GetComponent<dap::Bullet> ()->SetPointsOnHit ( pointsOnHit );
	// We might want to wait a bit before we can shoot again
	nextShootTime = TIME + shootInterval;
	return true;
}

void dap::Ship::Start () {
	dap::Component::Start ();
	dead = false;
	deathSoundSource = owner->AddComponent<dap::AudioSource> ();
	deathSoundSource->SetPlayOnStart ( false );
	deathSoundSource->SetPersistent ( true );
	deathSoundSource->SetSound ( dap::SoundClip::GetSoundClip ( DEATH_SOUND ) );
}

void dap::Ship::Tick () {
	dap::Component::Tick ();
	const bool TRYING_TO_MOVE = glm::length ( moveInput );
	// We want to accelerate and decelerate our ship's movement instead of just snapping to full speed or complete stop
	if ( TRYING_TO_MOVE )
		velocity += moveInput * moveAcceleration;
	else
		velocity -= velocity * moveDeceleration;
	// We don't want our ship to keep accelerating, so we cap its max speed
	if ( glm::length ( velocity ) >= maxSpeed )
		velocity = glm::normalize ( velocity ) * maxSpeed;

	GetOwner ()->GetTransform ()->Translate ( velocity );
}
