#include "Meteor.h"

#include "CollisionData.h"
#include "GameObject.h"
#include "Random.h"
#include "Rendering.h"
#include "Transform.h"

const float dap::Meteor::DELETE_TIME = 10.0f;

void dap::Meteor::Collision ( dap::CollisionData& data ) {
	// We'll just get rid of our meteor whenever it hits something
	GetOwner ()->Destroy ();
}

void dap::Meteor::Start () {
	// Give our meteor a random scale and rotational velocity for some added challenge
	float scale = dap::Random::Range ( sizeMin, sizeMax );
	rotationSpeed = dap::Random::Range ( -rotationSpeedMax, rotationSpeedMax );
	// We want to delete our meteor if it survives for too long
	nextDeleteTime = dap::Rendering::GetTime () + DELETE_TIME;
	velocity = glm::vec2 ( 0.0f, -1.0f ) * dap::Random::Range ( speedMin, speedMax );
	GetOwner ()->GetTransform ()->SetScale ( glm::vec3 ( scale, scale, 1.0f ) );
}

void dap::Meteor::Tick () {
	const float TIME = dap::Rendering::GetTime ();
	// Our meteor has been around for too long, so delete it
	if ( TIME > nextDeleteTime )
		GetOwner ()->Destroy ();
	GetOwner ()->GetTransform ()->Translate ( velocity );
	GetOwner ()->GetTransform ()->Rotate ( rotationSpeed );
}
