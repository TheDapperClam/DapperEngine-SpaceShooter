#include "UFO.h"

#include "Collision.h"
#include "GameObject.h"
#include "Random.h"
#include "Scene.h"
#include "Transform.h"

void dap::UFO::Start () {
	dap::Ship::Start ();
	bulletOffset = -1.0f;
	bulletSpeed = -0.25f;
	canShoot = false;
	health = 3;
	maxSpeed = 0.2f;
	moveAcceleration = 1.0f;
	moveDeceleration = 1.0f;
	maxSpeed = 0.25f;
	shootInterval = 0.3f;
}

void dap::UFO::Tick () {
	dap::Ship::Tick ();

	glm::vec2 pos = GetOwner ()->GetTransform ()->GetPosition ();
	// Our UFO will spawn off screen, and then move into view at a fixed y position
	if ( pos.y > targetPosY )
		moveInput.y = -1.0f;
	else {
		moveInput.y = 0.0f;
		if ( moveInput.x == 0.0f ) {
			// Since our UFO spawns in the center of the screen we want it to move either left or right after appearing
			const float RAND_MOVE_DIR = dap::Random::Range ( -1.0f, 1.0f );
			moveInput.x = RAND_MOVE_DIR >= 0 ? 1 : -1;
		}
		// The UFO will move back and forth across the screen shooting at the player
		if ( pos.x < minPosX ) {
			moveInput.x = 1.0f;
			canShoot = true;
		} else if ( pos.x > maxPosX ) {
			moveInput.x = -1.0f;
			canShoot = true;
		}
	}

	Shoot ( dap::Collision::GetLayer ( "Attack_Enemy" ), dap::Collision::GetLayer ( "Friend" ) | dap::Collision::GetLayer ( "Default" ) );
}
