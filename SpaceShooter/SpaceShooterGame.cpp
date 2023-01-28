#include "SpaceShooterGame.h"

#include "GameObject.h"
#include "GLFW/glfw3.h"
#include "Input.h"
#include "JSONObject.h"
#include "Label.h"
#include "Random.h"
#include "Rendering.h"
#include "Scene.h"
#include "Transform.h"

#include "Bullet.h"
#include "Meteor.h"
#include "Player.h"
#include "ScoreBoard.h"
#include "UFO.h"

bool dap::SpaceShooterGame::IsResetting = false;
const std::string dap::SpaceShooterGame::METEOR_PREFAB = "Resources/Prefabs/Meteor.dpf";
const std::string dap::SpaceShooterGame::UFO_PREFAB = "Resources/Prefabs/UFO.dpf";
float dap::SpaceShooterGame::NextEnemySpawnTime = 0.0f;
bool dap::SpaceShooterGame::UFOIsSpawned = false;
float dap::SpaceShooterGame::UFOSpawnBucket = 0.0f;
bool dap::SpaceShooterGame::Running = false;
float dap::SpaceShooterGame::ResetTime = 0.0f;

void dap::SpaceShooterGame::Input () {
	if ( dap::Input::Pressed ( GLFW_KEY_ESCAPE ) )
		Stop ();

	if ( dap::Input::Pressed ( GLFW_KEY_ENTER ) && !Running ) {
		// We'll hide the list of controls once we've started the game
		Running = true;
		dap::Scene::GetCurrent ()->GetGameObject ( "controls_label" )->GetComponent<dap::Label> ()->SetEnabled ( false );
	}
}

void dap::SpaceShooterGame::RegisterFactories () {
	// These are our components specific to this game that we want to be able to add to our game objects
	dap::JSONObject::RegisterFactory<dap::Bullet> ( new dap::FactoryMetaData ( "Bullet", true ) );
	dap::JSONObject::RegisterFactory<dap::Meteor> ( new dap::FactoryMetaData ( "Meteor", true ) );
	dap::JSONObject::RegisterFactory<dap::Player> ( new dap::FactoryMetaData ( "Player", true ) );
	dap::JSONObject::RegisterFactory<dap::ScoreBoard> ( new dap::FactoryMetaData ( "ScoreBoard", true ) );
	dap::JSONObject::RegisterFactory<dap::UFO> ( new dap::FactoryMetaData ( "UFO", true ) );
}

/*Set our game to be reset*/
void dap::SpaceShooterGame::Reset ( float delay ) {
	// We'll set how long we want to wait before resetting the game, which is done in the game's tick function
	IsResetting = true;
	ResetTime = dap::Rendering::GetTime () + delay;
}

void dap::SpaceShooterGame::Start () {
	// We want to reset when starting in order to initialize the game
	Reset ();
}

void dap::SpaceShooterGame::Tick () {
	if ( IsResetting && dap::Rendering::GetTime () > ResetTime ) {
		// We're resetting the game, so reset our game variables, and reload the scene at the end of the frame
		dap::Scene::LoadLate ( dap::Scene::GetCurrent ()->GetFilePath () );
		IsResetting = false;
		Running = false;
		NextEnemySpawnTime = 0.0f;
		UFOIsSpawned = false;
		UFOSpawnBucket = 0.0f;
		return;
	}
	if ( !Running )
		return;
	const float TIME = dap::Rendering::GetTime ();
	if ( TIME > NextEnemySpawnTime ) {
		// We don't want enemies to spawn at a fixed rate, so add a random delay to the spawn timer
		NextEnemySpawnTime = TIME + dap::Random::Range ( enemySpawnIntervalMin, enemySpawnIntervalMax );
		// UFOs will have a low chance of spawning at the start of the game, or if one was just destroyed,
		// and they will continue to have an increased chance of spawning after each failed chance
		const float UFO_SPAWN_CHANCE = dap::Random::Range ( 1.0f, ufoSpawnBucketMax );
		const bool SPAWN_UFO = !UFOIsSpawned && UFOSpawnBucket >= UFO_SPAWN_CHANCE;

		dap::GameObject* obj = dap::Scene::GetCurrent ()->SpawnPrefab ( SPAWN_UFO ? UFO_PREFAB : METEOR_PREFAB );
		// We don't want objects to just pop in, so spawn them off screen, and if they're a UFO, always at the center of the screen
		glm::vec3 spawnPos = SPAWN_UFO ? glm::vec3 ( 0.0f, enemySpawnY, 0.0f ) : 
										 glm::vec3 ( dap::Random::Range ( meteorSpawnXMin, meteorSpawnXMax ), enemySpawnY, 0.0f );
		obj->GetTransform ()->SetPosition ( spawnPos );

		if ( SPAWN_UFO ) {
			// We only want one UFO to spawn at a time, and we want them to spawn at a lower rate after succesfully spawning one
			UFOIsSpawned = true;
			currentUFOName = obj->GetName ();
			UFOSpawnBucket = 0.0f;
		} else if ( !UFOIsSpawned || !dap::Scene::GetCurrent ()->GetGameObject ( currentUFOName ) ) {
			UFOIsSpawned = false;
			// A UFO hasn't spawned so increase its chance of spawning for next time
			UFOSpawnBucket = std::min ( UFOSpawnBucket + ufoSpawnBucketIncrement, ufoSpawnBucketMax );
		}
	}
}
