#pragma once

#include "Game.h"
#include <string>

namespace dap {
	class GameObject;
	class Scene;

	class SpaceShooterGame : public dap::Game {
	private:
		static const std::string METEOR_PREFAB;
		static const std::string UFO_PREFAB;

	private:
		static bool IsResetting;
		static float NextEnemySpawnTime;
		static bool UFOIsSpawned;
		static float UFOSpawnBucket;
		static bool Running;
		static float ResetTime;

	public:
		/*Set our game to be reset*/
		static void Reset ( float delay = 0.0f );

	private:
		std::string currentUFOName = "";
		float enemySpawnIntervalMin = 0.5f;
		float enemySpawnIntervalMax = 1.0f;
		float enemySpawnY = 12.0f;
		float meteorSpawnXMin = -14.0f;
		float meteorSpawnXMax = 14.0f;
		float ufoSpawnBucketIncrement = 1.0f;
		float ufoSpawnBucketMax = 100.0f;

	public:
		virtual void Input () override;
		virtual void RegisterFactories () override;
		virtual void Start () override;
		virtual void Tick () override;
	};
}
