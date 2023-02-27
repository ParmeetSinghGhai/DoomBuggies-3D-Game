#include <GameSettings/GameSettings.h>

std::map<std::string, std::pair<std::string, void*>> GameSettings::VariableList;

int GameSettings::PlayerCount=1;
int GameSettings::EnemyCount=4;
float GameSettings::DefaultObjectSpeed=0.1f;
float GameSettings::PlayerSpeed=0.8f;
float GameSettings::EnemySpeedMax=20.8f;
float GameSettings::EnemySpeedMin=20.8f;
int GameSettings::EnemySpeedIncreaseInterval=2000;
float GameSettings::EnemySpeedIncreaseRate=0.1f;
float GameSettings::EnemyHealthBeforeReload=30.0f;
float GameSettings::EnemyWeaponBeforeReload=30.0f;

int GameSettings::LampCount=30;
int GameSettings::LampLightOnTime=20000;
int GameSettings::LampLightOffTime=20000;
float GameSettings::LampLightMax=0.8f;
float GameSettings::LampLightMin=0.1f;
float GameSettings::LampLightChangeSpeed=0.001f;

float GameSettings::SunLightColorRed=0.4f;
float GameSettings::SunLightColorGreen=0.4f;
float GameSettings::SunLightColorBlue=0.4f;

int GameSettings::HealthAndWeaponSpawnTime=5000;
float GameSettings::PlayerWeaponDamageRate=0.05f;
float GameSettings::EnemyWeaponDamageRate=1.05f;
float GameSettings::PlayerWeaponConsumptionRate=0.1f;
float GameSettings::EnemyWeaponConsumptionRate=0.1f;
float GameSettings::PlayerWeaponRange=10.0f;
float GameSettings::EnemyWeaponRange=10.0f;
float GameSettings::PlayerWeaponRestorationRate=20.0f;
float GameSettings::EnemyWeaponRestorationRate=20.0f;
float GameSettings::PlayerHealthRestorationRate=20.0f;
float GameSettings::EnemyHealthRestorationRate=20.0f;

float GameSettings::PlayerSelectColorRed=0.0f;
float GameSettings::PlayerSelectColorGreen=0.0f;
float GameSettings::PlayerSelectColorBlue=0.8f;
float GameSettings::EnemySelectColorRed=0.8f;
float GameSettings::EnemySelectColorGreen=0.0f;
float GameSettings::EnemySelectColorBlue=0.0f;
float GameSettings::HealthSelectColorRed=0.0f;
float GameSettings::HealthSelectColorGreen=0.8f;
float GameSettings::HealthSelectColorBlue=0.0f;
float GameSettings::WeaponSelectColorRed=0.0f;
float GameSettings::WeaponSelectColorGreen=0.8f;
float GameSettings::WeaponSelectColorBlue=0.0f;
float GameSettings::LocationSelectColorRed=0.8f;
float GameSettings::LocationSelectColorGreen=0.8f;
float GameSettings::LocationSelectColorBlue=0.0f;

float GameSettings::CameraKeyBoardSensitivity=0.3f;
float GameSettings::CameraMouseSensitivity=0.2f;

int GameSettings::InanimateCount=30;

float GameSettings::ExplosionRate=0.01f;




void GameSettings::Setup()
{
	VariableList.insert(std::pair<std::string, std::pair<std::string, void*>>("PlayerCount", std::pair<std::string, void*>("int", (void*)&GameSettings::PlayerCount)));
	VariableList.insert(std::pair<std::string, std::pair<std::string, void*>>("EnemyCount", std::pair<std::string, void*>("int", (void*)&GameSettings::EnemyCount)));
	VariableList.insert(std::pair<std::string, std::pair<std::string, void*>>("DefaultObjectSpeed", std::pair<std::string, void*>("float", (void*)&GameSettings::DefaultObjectSpeed)));
	VariableList.insert(std::pair<std::string, std::pair<std::string, void*>>("PlayerSpeed", std::pair<std::string, void*>("float", (void*)&GameSettings::PlayerSpeed)));
	VariableList.insert(std::pair<std::string, std::pair<std::string, void*>>("EnemySpeedMax", std::pair<std::string, void*>("float", (void*)&GameSettings::EnemySpeedMax)));
	VariableList.insert(std::pair<std::string, std::pair<std::string, void*>>("EnemySpeedMin", std::pair<std::string, void*>("float", (void*)&GameSettings::EnemySpeedMin)));
	VariableList.insert(std::pair<std::string, std::pair<std::string, void*>>("EnemySpeedIncreaseInterval", std::pair<std::string, void*>("int", (void*)&GameSettings::EnemySpeedIncreaseInterval)));
	VariableList.insert(std::pair<std::string, std::pair<std::string, void*>>("EnemySpeedIncreaseRate", std::pair<std::string, void*>("float", (void*)&GameSettings::EnemySpeedIncreaseRate)));
	VariableList.insert(std::pair<std::string, std::pair<std::string, void*>>("EnemyHealthBeforeReload", std::pair<std::string, void*>("float", (void*)&GameSettings::EnemyHealthBeforeReload)));
	VariableList.insert(std::pair<std::string, std::pair<std::string, void*>>("EnemyWeaponBeforeReload", std::pair<std::string, void*>("float", (void*)&GameSettings::EnemyWeaponBeforeReload)));
	
	VariableList.insert(std::pair<std::string, std::pair<std::string, void*>>("LampCount", std::pair<std::string, void*>("int", (void*)&GameSettings::LampCount)));
	VariableList.insert(std::pair<std::string, std::pair<std::string, void*>>("LampLightOnTime", std::pair<std::string, void*>("int", (void*)&GameSettings::LampLightOnTime)));
	VariableList.insert(std::pair<std::string, std::pair<std::string, void*>>("LampLightOffTime", std::pair<std::string, void*>("int", (void*)&GameSettings::LampLightOffTime)));
	VariableList.insert(std::pair<std::string, std::pair<std::string, void*>>("LampLightMax", std::pair<std::string, void*>("float", (void*)&GameSettings::LampLightMax)));
	VariableList.insert(std::pair<std::string, std::pair<std::string, void*>>("LampLightMin", std::pair<std::string, void*>("float", (void*)&GameSettings::LampLightMin)));
	VariableList.insert(std::pair<std::string, std::pair<std::string, void*>>("LampLightChangeSpeed", std::pair<std::string, void*>("float", (void*)&GameSettings::LampLightChangeSpeed)));
	
	VariableList.insert(std::pair<std::string, std::pair<std::string, void*>>("SunLightColorRed", std::pair<std::string, void*>("float", (void*)&GameSettings::SunLightColorRed)));
	VariableList.insert(std::pair<std::string, std::pair<std::string, void*>>("SunLightColorGreen", std::pair<std::string, void*>("float", (void*)&GameSettings::SunLightColorGreen)));
	VariableList.insert(std::pair<std::string, std::pair<std::string, void*>>("SunLightColorBlue", std::pair<std::string, void*>("float", (void*)&GameSettings::SunLightColorBlue)));
	
	VariableList.insert(std::pair<std::string, std::pair<std::string, void*>>("HealthAndWeaponSpawnTime", std::pair<std::string, void*>("int", (void*)&GameSettings::HealthAndWeaponSpawnTime)));
	VariableList.insert(std::pair<std::string, std::pair<std::string, void*>>("PlayerWeaponDamageRate", std::pair<std::string, void*>("float", (void*)&GameSettings::PlayerWeaponDamageRate)));
	VariableList.insert(std::pair<std::string, std::pair<std::string, void*>>("EnemyWeaponDamageRate", std::pair<std::string, void*>("float", (void*)&GameSettings::EnemyWeaponDamageRate)));
	VariableList.insert(std::pair<std::string, std::pair<std::string, void*>>("PlayerWeaponConsumptionRate", std::pair<std::string, void*>("float", (void*)&GameSettings::PlayerWeaponConsumptionRate)));
	VariableList.insert(std::pair<std::string, std::pair<std::string, void*>>("EnemyWeaponConsumptionRate", std::pair<std::string, void*>("float", (void*)&GameSettings::EnemyWeaponConsumptionRate)));
	VariableList.insert(std::pair<std::string, std::pair<std::string, void*>>("PlayerWeaponRange", std::pair<std::string, void*>("float", (void*)&GameSettings::PlayerWeaponRange)));
	VariableList.insert(std::pair<std::string, std::pair<std::string, void*>>("EnemyWeaponRange", std::pair<std::string, void*>("float", (void*)&GameSettings::EnemyWeaponRange)));
	VariableList.insert(std::pair<std::string, std::pair<std::string, void*>>("PlayerWeaponRestorationRate", std::pair<std::string, void*>("float", (void*)&GameSettings::PlayerWeaponRestorationRate)));
	VariableList.insert(std::pair<std::string, std::pair<std::string, void*>>("EnemyWeaponRestorationRate", std::pair<std::string, void*>("float", (void*)&GameSettings::EnemyWeaponRestorationRate)));
	VariableList.insert(std::pair<std::string, std::pair<std::string, void*>>("PlayerHealthRestorationRate", std::pair<std::string, void*>("float", (void*)&GameSettings::PlayerHealthRestorationRate)));
	VariableList.insert(std::pair<std::string, std::pair<std::string, void*>>("EnemyHealthRestorationRate", std::pair<std::string, void*>("float", (void*)&GameSettings::EnemyHealthRestorationRate)));
	
	VariableList.insert(std::pair<std::string, std::pair<std::string, void*>>("PlayerSelectColorRed", std::pair<std::string, void*>("float", (void*)&GameSettings::PlayerSelectColorRed)));
	VariableList.insert(std::pair<std::string, std::pair<std::string, void*>>("PlayerSelectColorGreen", std::pair<std::string, void*>("float", (void*)&GameSettings::PlayerSelectColorGreen)));
	VariableList.insert(std::pair<std::string, std::pair<std::string, void*>>("PlayerSelectColorBlue", std::pair<std::string, void*>("float", (void*)&GameSettings::PlayerSelectColorBlue)));
	VariableList.insert(std::pair<std::string, std::pair<std::string, void*>>("EnemySelectColorRed", std::pair<std::string, void*>("float", (void*)&GameSettings::EnemySelectColorRed)));
	VariableList.insert(std::pair<std::string, std::pair<std::string, void*>>("EnemySelectColorGreen", std::pair<std::string, void*>("float", (void*)&GameSettings::EnemySelectColorGreen)));
	VariableList.insert(std::pair<std::string, std::pair<std::string, void*>>("EnemySelectColorBlue", std::pair<std::string, void*>("float", (void*)&GameSettings::EnemySelectColorBlue)));
	VariableList.insert(std::pair<std::string, std::pair<std::string, void*>>("HealthSelectColorRed", std::pair<std::string, void*>("float", (void*)&GameSettings::HealthSelectColorRed)));
	VariableList.insert(std::pair<std::string, std::pair<std::string, void*>>("HealthSelectColorGreen", std::pair<std::string, void*>("float", (void*)&GameSettings::HealthSelectColorGreen)));
	VariableList.insert(std::pair<std::string, std::pair<std::string, void*>>("HealthSelectColorBlue", std::pair<std::string, void*>("float", (void*)&GameSettings::HealthSelectColorBlue)));
	VariableList.insert(std::pair<std::string, std::pair<std::string, void*>>("WeaponSelectColorRed", std::pair<std::string, void*>("float", (void*)&GameSettings::WeaponSelectColorRed)));
	VariableList.insert(std::pair<std::string, std::pair<std::string, void*>>("WeaponSelectColorGreen", std::pair<std::string, void*>("float", (void*)&GameSettings::WeaponSelectColorGreen)));
	VariableList.insert(std::pair<std::string, std::pair<std::string, void*>>("WeaponSelectColorBlue", std::pair<std::string, void*>("float", (void*)&GameSettings::WeaponSelectColorBlue)));
	VariableList.insert(std::pair<std::string, std::pair<std::string, void*>>("LocationSelectColorRed", std::pair<std::string, void*>("float", (void*)&GameSettings::LocationSelectColorRed)));
	VariableList.insert(std::pair<std::string, std::pair<std::string, void*>>("LocationSelectColorGreen", std::pair<std::string, void*>("float", (void*)&GameSettings::LocationSelectColorGreen)));
	VariableList.insert(std::pair<std::string, std::pair<std::string, void*>>("LocationSelectColorBlue", std::pair<std::string, void*>("float", (void*)&GameSettings::LocationSelectColorBlue)));

	VariableList.insert(std::pair<std::string, std::pair<std::string, void*>>("CameraKeyBoardSensitivity", std::pair<std::string, void*>("float", (void*)&GameSettings::CameraKeyBoardSensitivity)));
	VariableList.insert(std::pair<std::string, std::pair<std::string, void*>>("CameraMouseSensitivity", std::pair<std::string, void*>("float", (void*)&GameSettings::CameraMouseSensitivity)));
	
	VariableList.insert(std::pair<std::string, std::pair<std::string, void*>>("InanimateCount", std::pair<std::string, void*>("int", (void*)&GameSettings::InanimateCount)));
	
	VariableList.insert(std::pair<std::string, std::pair<std::string, void*>>("ExplosionRate", std::pair<std::string, void*>("float", (void*)&GameSettings::ExplosionRate)));
}

void GameSettings::Load(std::string FilePath)
{
	try
	{
		std::cout << "LOADING GAME SETTINGS FROM FILE ::" << FilePath << "\n";
		std::ifstream SettingsFile(FilePath);
		std::string Data;
		std::string Delimiter = "=";
		std::string Var = "";
		std::string Val = "";
		std::string IntType = "int";
		std::string FloatType = "float";
		std::map<std::string, std::string> input;
		bool Found = false;
		while (std::getline(SettingsFile, Data))
		{
			int Pos = Data.find(Delimiter);
			if (Pos != std::string::npos)
			{
				Var = Data.substr(0, Pos); 
				Var.erase(std::remove(Var.begin(), Var.end(), ' '), Var.end());
				Val = Data.substr(Pos + 1); 
				Val.erase(std::remove(Val.begin(), Val.end(), ' '), Val.end());;
				Found = false;
				for (std::map<std::string, std::pair<std::string, void*>>::iterator it = VariableList.begin(); it != VariableList.end(); it++)
				{
					if (std::strcmp(it->first.c_str(), Var.c_str()) == 0)
					{
						if (it->second.first.compare(IntType) == 0)
						{
							*((int*)it->second.second) = std::stoi(Val);
							std::cout << Var << " = " << *((int*)it->second.second) << "\n";
						}
						else if (it->second.first.compare(FloatType) == 0)
						{
						    *((float*)it->second.second) = std::stof(Val);
							std::cout << Var << " = " << *((float*)it->second.second) << "\n";
						}
						Found = true;
						break;
					}
				}
				if (!Found)
					std::cout << Var << " COULD NOT BE FOUND !\n";
			}
		}
	}
	catch (std::exception e)
	{
		std::cout << "ERROR IN READING SETTINGS FILE::" << e.what() << "\n";
	}
}