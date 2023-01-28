#include "Bullet.h"

#include "AudioSource.h"
#include "CollisionData.h"
#include "GameObject.h"
#include "MathF.h"
#include "Rendering.h"
#include "ScoreBoard.h"
#include "SpaceShooterGame.h"
#include <SoundClip.h>
#include "Transform.h"

const float dap::Bullet::DELETE_DELAY = 10.0f;
const std::string dap::Bullet::SOUND_IMPACT_NAME = "impact";

void dap::Bullet::Collision ( dap::CollisionData& data ) {
	// We want to reward the player with some points for every enemy that they successfully land a hit on
	dap::ScoreBoard* sb = dap::ScoreBoard::GetCurrent ();
	if ( sb )
		sb->AddScore ( pointsOnHit );
	// We're done here, so just play the hit sound and destroy the bullet object
	impactSoundSource->Play ();
	owner->Destroy ();
}

void dap::Bullet::SetPointsOnHit ( unsigned int amount ) {
	pointsOnHit = amount;
}

void dap::Bullet::SetVelocity ( glm::vec2 vel ) {
	velocity = vel;
	// We want our bullet to face the direction that it is currently traveling in to better visualize its velocity
	GetOwner ()->GetTransform ()->SetRotation ( dap::MathF::Rad2Deg ( std::atan2f ( vel.y, vel.x ) ) - 90.0f );
}

void dap::Bullet::Start () {
	dap::Component::Start ();
	impactSoundSource = owner->AddComponent<dap::AudioSource> ();
	impactSoundSource->SetPlayOnStart ( false );
	impactSoundSource->SetPersistent ( true );
	impactSoundSource->SetSound ( dap::SoundClip::GetSoundClip ( SOUND_IMPACT_NAME ) );
	// We'll want to delete our bullet after a dely in case it doesn't hit anything
	deleteTime = dap::Rendering::GetTime () + DELETE_DELAY;
}

void dap::Bullet::Tick () {
	dap::Component::Tick ();
	GetOwner ()->GetTransform ()->Translate ( velocity );
	
	// Our bullet has survived for too long without hitting anything, so delete it
	if ( dap::Rendering::GetTime () > deleteTime )
		GetOwner ()->Destroy ();
}
