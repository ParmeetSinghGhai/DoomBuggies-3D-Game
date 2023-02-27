#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <algorithm>
class GameSettings
{
public:
	static int PlayerCount;
	static int EnemyCount;
	static float DefaultObjectSpeed;
	static float PlayerSpeed;
	static float EnemySpeedMax;
	static float EnemySpeedMin;
	static int EnemySpeedIncreaseInterval;
	static float EnemySpeedIncreaseRate;
	static float EnemyHealthBeforeReload;
	static float EnemyWeaponBeforeReload;

	static int LampCount;
	static int LampLightOnTime;
	static int LampLightOffTime;
	static float LampLightMax;
	static float LampLightMin;
	static float LampLightChangeSpeed;

	static float SunLightColorRed;
	static float SunLightColorGreen;
	static float SunLightColorBlue;
	
	static int HealthAndWeaponSpawnTime;
	static float PlayerWeaponDamageRate;
	static float EnemyWeaponDamageRate;
	static float PlayerWeaponConsumptionRate;
	static float EnemyWeaponConsumptionRate;
	static float PlayerWeaponRange;
	static float EnemyWeaponRange;
	static float PlayerWeaponRestorationRate;
	static float EnemyWeaponRestorationRate;
	static float PlayerHealthRestorationRate;
	static float EnemyHealthRestorationRate;
	
	static float PlayerSelectColorRed;
	static float PlayerSelectColorGreen;
	static float PlayerSelectColorBlue;
	static float EnemySelectColorRed;
	static float EnemySelectColorGreen;
	static float EnemySelectColorBlue;
	static float HealthSelectColorRed;
	static float HealthSelectColorGreen;
	static float HealthSelectColorBlue;
	static float WeaponSelectColorRed;
	static float WeaponSelectColorGreen;
	static float WeaponSelectColorBlue;
	static float LocationSelectColorRed;
	static float LocationSelectColorGreen;
	static float LocationSelectColorBlue;

	static float CameraKeyBoardSensitivity;
	static float CameraMouseSensitivity;

	static int InanimateCount;

	static float ExplosionRate;
	
	

	static std::map<std::string, std::pair<std::string,void*>> VariableList;
	static void Setup();
	static void Load(std::string FilePath);
};