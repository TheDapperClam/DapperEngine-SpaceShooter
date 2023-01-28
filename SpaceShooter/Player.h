#pragma once

#include "Ship.h"

namespace dap {
	struct CollisionData;

	class Player : public dap::Ship {
	private:
		float nextShootEnergyRechargeTime = 0.0f;
		float positionXMin = -13.0f;
		float positionXMax = 13.0f;
		float positionYMin = -7.0f;
		float positionYMax = 7.0f;
		int shootEnergy = 0;
		int shootEnergyMax = 100;
		int shootEnergyCost = 5;
		int shootEnergyRechargeAmount = 5;
		float shootEnergyRechargeDelay = 3.0f;
		float shootEnergyRechargeInterval = 0.1f;
		bool shouldShoot = false;

	protected:
		/*Event for when the player dies*/
		virtual void OnDeath () override;
		virtual void Input () override;
		virtual void Start () override;
		virtual void Tick () override;
	};
}
