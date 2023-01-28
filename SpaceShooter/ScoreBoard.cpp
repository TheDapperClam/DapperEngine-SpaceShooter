#include "ScoreBoard.h"

#include "GameObject.h"
#include "Label.h"

dap::ScoreBoard* dap::ScoreBoard::Current = NULL;

/*Add points to the scoreboard*/
void dap::ScoreBoard::AddScore ( unsigned int amount ) {
	score += amount;
	GetOwner ()->GetComponent<dap::Label> ()->SetText ( "Score: " + std::to_string ( score ) );
}

/*Get the current scoreboard object*/
dap::ScoreBoard* const dap::ScoreBoard::GetCurrent () {
	return Current;
}

void dap::ScoreBoard::Start () {
	Current = this;
}
