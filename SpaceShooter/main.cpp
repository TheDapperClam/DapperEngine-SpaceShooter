#include "SpaceShooterGame.h"

//#include "Editor.h"
#include "SpaceShooterGame.h"

int main (int argc, char* args[]) {
	// We don't want to make any changes to the game scene, so don't set up the editor for now
	//dap::Editor editor;
	//dap::Editor::SetCurrent ( &editor );
	dap::SpaceShooterGame game;
	game.Run ( "Space Shooter", "Resources/Scenes/SpaceShooter.dsn" );
	return 0;
}