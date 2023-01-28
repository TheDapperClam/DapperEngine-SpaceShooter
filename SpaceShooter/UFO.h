#pragma once

#include "Ship.h"
#include <string>

namespace dap {
	class UFO : public dap::Ship {
	private:
		float minPosX = -14.0f;
		float maxPosX = 14.0f;
		float targetPosY = 6.0f;

	protected:
		virtual void Start () override;
		virtual void Tick () override;
	};
}
