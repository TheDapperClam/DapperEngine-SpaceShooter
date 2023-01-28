#include "Player.h"

#include "Collision.h"
#include "CollisionData.h"
#include "GameObject.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "Input.h"
#include "Label.h"
#include "MathF.h"
#include "RectCollider.h"
#include "Rendering.h"
#include "Scene.h"
#include "SpaceShooterGame.h"
#include "Transform.h"

/*Event for when the player dies*/
void dap::Player::OnDeath () {
	// We'll just end the game once the player has died
	dap::Scene::GetCurrent ()->GetGameObject ( "gameover_label" )->GetComponent<dap::Label> ()->SetEnabled ( true );
	dap::SpaceShooterGame::Reset ( 5.0f );
}

void dap::Player::Input () {
	float horizontal = dap::Input::Held ( GLFW_KEY_D ) - dap::Input::Held ( GLFW_KEY_A );
	float vertical = dap::Input::Held ( GLFW_KEY_W ) - dap::Input::Held ( GLFW_KEY_S );
	moveInput = glm::vec2 ( horizontal, vertical );
	shouldShoot = dap::Input::Held ( GLFW_KEY_SPACE );
}

void dap::Player::Start () {
	dap::Ship::Start ();
	shootEnergy = shootEnergyMax;
}

void dap::Player::Tick () {
	const float TIME = dap::Rendering::GetTime ();
	// We want to keep the player within a set boundary 
	glm::vec2 currentPos = GetOwner ()->GetTransform ()->GetPosition ();
	glm::vec2 clampedPos = currentPos;
	clampedPos.x = dap::MathF::Clamp ( clampedPos.x, positionXMin, positionXMax );
	clampedPos.y = dap::MathF::Clamp ( clampedPos.y, positionYMin, positionYMax );
	// Reset our velocity if we hit the game boundaries so that we don't stick when we try to move away
	if ( clampedPos.x <= positionXMin || clampedPos.x >= positionXMax ) {
		velocity.x = clampedPos.x - currentPos.x;
	}
	if ( clampedPos.y <= positionYMin || clampedPos.y >= positionYMax )
		velocity.y = clampedPos.y - currentPos.y;
	dap::Ship::Tick ();

	if ( shouldShoot ) {
		if ( shootEnergy >= shootEnergyCost ) {
			// The player wants to shoot, and they currently have enough energy to do so
			if ( Shoot ( dap::Collision::GetLayer ( "Attack_Friend" ), dap::Collision::GetLayer ( "Enemy" ) | dap::Collision::GetLayer ( "Default" ), 1 ) ) {
				nextShootEnergyRechargeTime = TIME + shootEnergyRechargeDelay;
				shootEnergy -= shootEnergyCost;
			}
		} else {
			// The player is still trying to shoot even though they are out of energy, so keep resetting the recharge timer until they stop
			nextShootEnergyRechargeTime = TIME + shootEnergyRechargeDelay;
		}
	}

	// The player isn't trying to shoot, and the recharge time has been met, so start rechargint the player's energy
	if ( !shouldShoot && TIME > nextShootEnergyRechargeTime && shootEnergy < shootEnergyMax ) {
		shootEnergy = std::min ( shootEnergy + shootEnergyRechargeAmount, shootEnergyMax );
		nextShootEnergyRechargeTime = TIME + shootEnergyRechargeInterval;
	}

	dap::Scene* currentScene = dap::Scene::GetCurrent ();
	if ( currentScene ) {
		dap::GameObject* shootEnergyLabel = currentScene->GetGameObject ( "Blaster_Label" );
		dap::GameObject* rechargingLabel = currentScene->GetGameObject ( "Recharging Label" );
		// We want to display how much energy the player has left
		if ( shootEnergyLabel )
			shootEnergyLabel->GetComponent<dap::Label> ()->SetText ( "Blaster: " + std::to_string ( shootEnergy ) );
		// We'll be showing a recharging label to show that the player's energy is recharging if not at max
		if ( rechargingLabel )
			rechargingLabel->GetComponent<dap::Label> ()->SetEnabled ( shootEnergy < shootEnergyMax );
	}
}
