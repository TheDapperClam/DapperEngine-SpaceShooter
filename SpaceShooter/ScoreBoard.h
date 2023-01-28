#pragma once

#include "Component.h"

namespace dap {
	class ScoreBoard : public dap::Component {
	private:
		static dap::ScoreBoard* Current;

	public:
		/*Get the current scoreboard object*/
		static dap::ScoreBoard* const GetCurrent ();

	private:
		unsigned int score;

	public:
		/*Add points to the scoreboard*/
		void AddScore ( unsigned int amount );

	protected:
		virtual void Start () override;
	};
}
