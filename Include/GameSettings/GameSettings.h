#pragma once
class GameSettings
{
public:
	static float PlayerSpeed;
	static float EnemySpeedMax;
	static float EnemySpeedMin;
	static int EnemySpeedIncreaseInterval;
	static float EnemySpeedIncreaseRate;
	static float EnemyHealthBeforeReload;
	static float EnemyWeaponBeforeReload;

	static int LampLightOnTime;
	static int LampLightOffTime;
	static float LampLightMax;
	static float LampLightMin;
	static float LampLightChangeSpeed;

	static float SunLightColorRed;
	static float SunLightColorGreen;
	static float SunLightColorBlue;

	static int HealthAndWeaponSpawnTime;
	static float MovementSpeed;
	static float WeaponDamageRate;
	static float WeaponConsumptionRate;
	static float WeaponRange;
	static float WeaponRestorationRate;
	static float HealthRestorationRate;

	static float PlayerSelectColorRed;
	static float PlayerSelectColorGreen;
	static float PlayerSelectColorBlue;
	static float EnemySelectColorRed;
	static float EnemySelectColorGreen;
	static float EnemySelectColorBlue;
	static float HealthWeaponSelectColorRed;
	static float HealthWeaponSelectColorGreen;
	static float HealthWeaponSelectColorBlue;

	static float CameraKeyBoardSensitivity;
	static float CameraMouseSensitivity;
	
	static void Setup()
	{
		GameSettings::PlayerSpeed = 0.1f;
		GameSettings::EnemySpeedMax = 0.8f;
		GameSettings::EnemySpeedMin = 0.01f;
		GameSettings::EnemySpeedIncreaseInterval = 20000;
		GameSettings::EnemySpeedIncreaseRate = 0.01f;
		GameSettings::EnemyHealthBeforeReload = 30.0f;
		GameSettings::EnemyWeaponBeforeReload = 30.0f;

		GameSettings::LampLightOnTime = 20000;
		GameSettings::LampLightOffTime = 20000;
		GameSettings::LampLightMax = 0.9f;
		GameSettings::LampLightMin = 0.08f;
		GameSettings::LampLightChangeSpeed = 0.001f;

		GameSettings::SunLightColorRed=0.08f;
		GameSettings::SunLightColorGreen=0.08f;
		GameSettings::SunLightColorBlue=0.0f;

		GameSettings::HealthAndWeaponSpawnTime = 40000;
		GameSettings::MovementSpeed = 0.8f;
		GameSettings::WeaponDamageRate = 0.05f;
		GameSettings::WeaponConsumptionRate = 0.1f;
		GameSettings::WeaponRange = 10.0f;
		GameSettings::WeaponRestorationRate=20.0f;
		GameSettings::HealthRestorationRate=20.0f;

		GameSettings::PlayerSelectColorRed=0.0f;
		GameSettings::PlayerSelectColorGreen=0.0f;
		GameSettings::PlayerSelectColorBlue=0.8f;
		GameSettings::EnemySelectColorRed=0.8f;
		GameSettings::EnemySelectColorGreen=0.0f;
		GameSettings::EnemySelectColorBlue=0.0f;
		GameSettings::HealthWeaponSelectColorRed=0.0f;
		GameSettings::HealthWeaponSelectColorGreen = 0.8f;
		GameSettings::HealthWeaponSelectColorBlue=0.0f;

		GameSettings::CameraKeyBoardSensitivity = 0.3f;
		GameSettings::CameraMouseSensitivity = 0.2f;
	}

};